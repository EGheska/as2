//
// Created by Dawid Zalewski on 2/17/2023.
//

#ifndef WEEK_02_POMODORO_H
#define WEEK_02_POMODORO_H

#include "tty.h"
#include "ascii_fonts.h"

namespace pomodoro{
    /**
    * Pomodoro timer.
    */
    struct pomodoro {
        // TODO define the pomodoro structure
        // VARIABLES FIRST:
//
        // You'll need those to talk to the terminal
        tty::console conio{};
        ascii_art::text_printer prn{};

        // Work time in seconds.
        long work_time{};
        // Break time in seconds.
        long break_time{};

        // Indicates whether the timer is in the work or the break interval.
        bool is_working{true};

        // FUNCTIONS:

        // initializes member variables and loads fonts in prn
        void initialize(long time_work, long time_break);

        // function that contains most of the logic
        // it has a while loop and uses the `render` function
        void run();

        // renders time left using the correct color, used by `run`
        void render(long time_left, ascii_art::text_printer& obj);

        // sets the font color depending on the current interval
        // used by `render`
        void set_terminal_colors(ascii_art::text_printer& obj);
    };
}

#endif //WEEK_02_POMODORO_H
