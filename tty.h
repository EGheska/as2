/*
 * MIT License
 *
 * Copyright (c) $YEAR Dawid Zalewski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LIBTTY_TTY_H
#define LIBTTY_TTY_H

#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <cstdarg>
#include <cstdio>
#include <iostream>

namespace tty {

    /**
     * Represents the size of a terminal window.
     */
    struct size2d {
        int width;
        int height;
    };


    namespace detail{
        enum struct ctrl;
        void print_fmt(const char *, ...)  __attribute__ ((format (printf, 1, 2)));
        void print_fmt_seq(ctrl fmt, ...);

        template <typename...Args>
        void print_ctrl_seq(Args&&...args);

        constexpr auto TTY_ESC{"\033"};

        enum struct ctrl : int {
            RESET = 0,
            MOVE_HOME,
            CLEAR_BELOW,
            CLEAR_ABOVE,
            CLEAR_ALL,

            ERASE_END_OF_LINE,
            ERASE_START_OF_LINE,
            ERASE_LINE,

            MOVE_XY,
            MOVE_UP_BY,
            MOVE_DOWN_BY,
            MOVE_RIGHT_BY,
            MOVE_LEFT_BY,

            ATTRIBUTE_2_SET
        };

        static const char * seqs[] = {
            "s",
            "[H",
            "[J",
            "[1J",
            "[2J",

            "[K",
            "[1K",
            "[2K",

            "[%d;%dH",
            "[%dA",
            "[%dB",
            "[%dC",
            "[%dD",
            "[%d;%dm"
        };

        inline void print_fmt(const char * fmt, ...)  {
            va_list args;
            va_start(args, fmt);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
            std::fprintf(stdout, TTY_ESC);
            std::vfprintf(stdout, fmt, args);
#pragma clang diagnostic pop
        }

        inline void print_fmt_seq(ctrl fmt, ...)  {
            va_list args;
            va_start(args, fmt);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
#pragma clang diagnostic ignored "-Wformat-nonliteral"
            std::fprintf(stdout, TTY_ESC);
            std::vfprintf(stdout, seqs[static_cast<int>(fmt)], args);
#pragma clang diagnostic pop
        }

        template <typename...Args>
        void print_ctrl_seq(Args&&...args){

            (std::fprintf(stdout, "\033%s", seqs[static_cast<int>(std::forward<Args>(args))]),...);
        }
    }

    /**
     * Color constants for terminal font colors.
     */
    enum struct foreground_color : int {
        BLACK   = 30,
        RED     = 31,
        GREEN   = 32,
        YELLOW  = 33,
        BLUE    = 34,
        MAGNETA = 35,
        CYAN    = 36,
        WHITE   = 37
    };

    /**
     * Color tint constants for terminal font colors.
     */
    enum struct color_variant : int {
        NORMAL  = 0,
        BRIGHT  = 1,
        DIM     = 2
    };

    /**
     * Simple Linux terminal manipulation utilities.
     */
    struct console {
        /**
         * Makes a beep sound in the terminal.
         */
        void beep() const {
            fprintf(stdout, "%c", '\07');
        }

        /**
         * Resets the console.
         */
        void reset() const {
            using namespace detail;
            detail::print_ctrl_seq(ctrl::RESET);
        }

        /**
         * Disables input echo in the terminal.
         * @return Old console settings
         */
        struct termios disable_input_echo() noexcept {
            if (!tio_changed_) {
                std::cout << std::unitbuf;

                tcgetattr(fileno(stdin), &cached_tio_);

                auto tio = cached_tio_;
                tio.c_lflag &= static_cast<decltype(tio.c_lflag)>(~ICANON & ~ECHO);

                tcsetattr(fileno(stdin), TCSANOW, &tio);
                std::setbuf(stdin, nullptr);
                tio_changed_ = true;
            }
            return cached_tio_;
        }

        /**
         * Enables input echo in the terminal.
         */
        void enable_input_echo() noexcept {
            if(tio_changed_){
                cached_tio_.c_lflag |= static_cast<decltype(cached_tio_.c_lflag)>(ICANON | ECHO);

                tcsetattr(fileno(stdin), TCSANOW, &cached_tio_);
                tio_changed_ = false;
            }
        }

        /**
         * Reads a single character from unbuffered stdin.
         * @return Character read or EOF if there was nothing to read.
         * @warning Don't use this function if you haven't called disable_input_echo first.
         */
        char getc() const {

            int nbytes;
            ioctl(fileno(stdin), FIONREAD, &nbytes);
            if (nbytes) {
                char letter{};

                auto result{read(fileno(stdin), &letter, 1)};
                if (result == 1)
                    return letter;
                else   // nothing available (0) or error (-1)
                    return EOF;
            }
            return EOF;
        }

        /**
         * Gets the size of the terminal window in [cols x rows]
         * @return The size of the terminal window.
         */
        size2d size() const noexcept {
            if (isatty(fileno(stdout))) {
                struct winsize ws;
                ioctl(fileno(stdout), TIOCGWINSZ, &ws);
                return {ws.ws_col, ws.ws_row};
            } else if (isatty(fileno(stdin))) {
                struct winsize ws;
                ioctl(fileno(stdin), TIOCGWINSZ, &ws);
                return {ws.ws_col, ws.ws_row};
            } else if (isatty(fileno(stderr))) {
                struct winsize ws;
                ioctl(fileno(stderr), TIOCGWINSZ, &ws);
                return {ws.ws_col, ws.ws_row};
            }
            return {};
        }

        /**
         * Moves the cursor in the terminal window.
         * @param col The target column position.
         * @param row The target row position.
         */
        void move_xy(int col, int row) const {
            using namespace detail;
            print_fmt_seq(ctrl::MOVE_XY, row, col);
        }

        /**
         * Moves the cursor lines rows up.
         * @param lines Number of lines to move.
         */
        void move_up_by(int lines) const {
            using namespace detail;
            print_fmt_seq(ctrl::MOVE_UP_BY, lines);
        }

        /**
         * Moves the cursor lines rows down.
         * @param lines Number of lines to move.
         */
        void move_down_by(int lines) const {
            using namespace detail;
            print_fmt_seq(ctrl::MOVE_DOWN_BY, lines);
        }

        /**
         * Moves the cursor chars columns to the right.
         * @param lines Number of rows to move.
         */
        void move_right_by(int chars) const {
            using namespace detail;
            print_fmt_seq(ctrl::MOVE_RIGHT_BY, chars);
        }

        /**
         * Moves the cursor chars columns to the left.
         * @param lines Number of rows to move.
         */
        void move_left_by(int chars) const {
            using namespace detail;
            print_fmt_seq(ctrl::MOVE_LEFT_BY, chars);
        }

        /**
         * Clears the terminal screen.
         */
        void clear() const {
            using namespace detail;
            print_ctrl_seq(ctrl::MOVE_HOME, ctrl::CLEAR_BELOW);
//            print_ctrl_seq(ctrl::CLEAR_ALL);
        }

        /**
         * Moves the cursor to the home position (0, 0)
         */
        void move_home() const{
            using namespace detail;
            print_ctrl_seq(ctrl::MOVE_HOME);
        }

        /**
         * Clears all lines below the current row.
         */
        void clear_below() const {
            using namespace detail;
            print_ctrl_seq(ctrl::CLEAR_BELOW);
        }

        /**
         * Clears all lines above the current row.
         */
        void clear_above() const {
            using namespace detail;
            print_ctrl_seq(ctrl::CLEAR_ABOVE);
        }

        /**
         * Clears the current line from the cursor to the end.
         */
        void erase_end_of_line() const {
            using namespace detail;
            print_ctrl_seq(ctrl::ERASE_END_OF_LINE);
        }

        /**
        * Clears the current line from the cursor to the start.
        */
        void erase_start_of_line() const {
            using namespace detail;
            print_ctrl_seq(ctrl::ERASE_START_OF_LINE);
        }

        /**
         * Clears the current line.
         */

        void erase_current_line() const {
            using namespace detail;
            print_ctrl_seq(ctrl::ERASE_LINE);
        }

        /**
         * Sets the font color.
         * @param color The base color.
         * @param var The tint of the color.
         */
        void set_fg_color(foreground_color color, color_variant var = color_variant::NORMAL) const {
            using namespace detail;
            print_fmt_seq(ctrl::ATTRIBUTE_2_SET, static_cast<int>(var), static_cast<int>(color));
        }

        ~console() noexcept {
            enable_input_echo();
        }
    private:
        struct termios cached_tio_{};
        bool tio_changed_{false};
    };

}


#endif //LIBTTY_TTY_H
