//
// Created by Dawid Zalewski on 2/17/2023.
//

#ifndef WEEK_02_ASCII_FONTS_H
#define WEEK_02_ASCII_FONTS_H

#include <fstream>
#include <iostream>
#include <filesystem>
#include "tty.h"
namespace ascii_art {
    /**
     * Stores a single ASCII-art glyph.
     */
    struct glyph {
        const static int max_height{24};
        const static int max_width{64};

        char data[max_height][max_width];
        int width{};
        int height{};
        
        int row_offset{};
        char symbol{};

        /**
         * Loads a glyph from a file.
         * @param input Input stream containing the glyph.
         * @returns True ig glyph was loaded, false otherwise.
         */
        bool load(std::istream& input);

        /**
         * Renders this glyph in the terminal.
         * @param col The x position in terminal.
         * @param row The y position in terminal.
         * @param conio The console object to use for rendering.
         */
        void render(int col, int row, tty::console const& conio={});
    };

    /**
     * A glyph collection.
     */
    struct glyph_collection {
        const static int MAX_GLYPHS{64};
        glyph glyphs[MAX_GLYPHS];
        int count_glyphs{0};
        std::string name{};

        /**
         * Finds a glyph in this collection.
         * @param symbol The symbol of the glyph to be found.
         * @return The index of the glyph in this collection if found. Otherwise MAX_GLYPHS is returned.
         */
        [[nodiscard]]
        int find_glyph_idx(char symbol) const;

        /**
         * Loads all the glyphs from a file.
         * @param font_file_name File with the ascii glyphs.
         */
        bool load_glyphs(std::string font_file_name);
    };

    /**
     * Prints text using ASCII-art glyphs to the terminal.
     */
    struct text_printer {
        tty::foreground_color color{};
        tty::color_variant variant;

        glyph_collection font{};

        /**
         * Sets the color of text.
         * @param col The color to be set.
         * @param var The tint of the color.
         */
        void set_color(tty::foreground_color col, tty::color_variant var);

        /**
         * Renders text to the terminal using ASCII art.
         * @param col Horizontal position on the screen.
         * @param row Vertical position on the screen.
         * @param str Text to render.
         */
        void render_text(int col, int row, std::string str);
    };

}


#endif //WEEK_02_ASCII_FONTS_H
