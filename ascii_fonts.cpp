//
// Created by Dawid Zalewski on 2/17/2023.
//

#include <sstream>
#include <cstring>
#include "ascii_fonts.h"



bool ascii_art::glyph::load(std::istream& input) {

    if (input) {
        std::string line;
        if (std::getline(input, line)) {
            std::istringstream iss{line};
            if (!(iss >> line >> symbol) || line != "GLYPH"){
                std::cerr << "Error loading glyph, symbol not decoded.\n";
                return false;
            }
        }

        if (std::getline(input, line)){
            std::istringstream iss{line};
            if (!(iss >> line >> row_offset) || line != "OFFSET"){
                std::cerr << "Error loading glyph, row offset not decoded.\n";
                return false;
            }
        }

        auto row{0};
        width = 0;

        while (std::getline(input, line)) {

            if (row >= max_height){
                std::cerr << "Error loading glyph from stream, the height of the glyphs is too big.\n";
                return false;
            }

            if (line.length() >= max_width){
                std::cerr << "Error loading glyph from stream, the width of the glyphs is too big.\n";
                return false;
            }

            std::memcpy(&(data[row][0]), line.c_str(), line.length() + 1);

            // counts the real length of an utf-8 encoded string
            auto str = line.c_str();
            auto length{0};
            while (*str) {
                length += (*str++ & 0xc0) != 0x80;
            }

            width = std::max(width, length);
            ++row;
        }
        height = row;
    }

    return true;
}

void ascii_art::glyph::render(int col, int row, const tty::console& conio) {
    row += row_offset;
    conio.move_xy(col, row);
    for (auto i{0}; i < height; ++i) {
        std::cout << &(data[i][0]);
        conio.move_xy(col, ++row);
    }
}

int ascii_art::glyph_collection::find_glyph_idx(char symbol) const {
    for (auto i{0}; i < count_glyphs; ++i) {
        if (glyphs[i].symbol == symbol)
            return i;
    }
    return MAX_GLYPHS;
}



bool ascii_art::glyph_collection::load_glyphs(std::string font_file_name)  {
    
    if (auto input{ std::ifstream{font_file_name}}){
        std::string str{};
        if (!(input >> str) || !std::getline(input, name) || str != "FONT"){
            std::cerr << "Error loading font, font name not decoded.\n";
            return false;
        }
        int count;
        if (!std::getline(input, str) || !str.starts_with("COUNT")){
            std::cerr << "Error loading font, count not decoded.\n";
            return false;
        }

        std::istringstream iss{str};

        if (!(iss >> str >> count) || str != "COUNT"){
            std::cerr << "Error loading font, count not decoded.\n";
            return false;
        }

        count_glyphs = 0;
        bool first{true};

        std::ostringstream oss{};

        while(std::getline(input, str)){
            if (str.starts_with("GLYPH")){
                if (!first){
                    // full glyph is already read, process
                    iss.clear();
                    iss.str(oss.str());
                    glyphs[count_glyphs++].load(iss);
                }
                oss.clear();
                oss.str(str);
                oss.seekp(0, std::ios_base::end);
                oss << '\n';
            }
            else{
                oss << str << '\n';
            }
            first = false;
        }

        // full glyph is read, process
        iss.clear();
        iss.str(oss.str());
        glyphs[count_glyphs++].load(iss);

        if (count_glyphs != count){
            std::cerr << "Error loading font, read " << count_glyphs << " glyphs, expected " << count << '\n';
            return false;
        }
    }
    else{
        std::cerr << "Error loading font, file '" << font_file_name << "' was not found.\n";
        return false;
    }

    return true;
}

void ascii_art::text_printer::set_color(tty::foreground_color col, tty::color_variant var)  {
    color = col;
    variant = var;
}

void ascii_art::text_printer::render_text(int col, int row, std::string str) {
    tty::console conio{};
    conio.set_fg_color(color, variant);
    for (auto i{0u}; i < str.length(); ++i) {
        if (auto index{font.find_glyph_idx(str[i])}; index != glyph_collection::MAX_GLYPHS) {
            font.glyphs[index].render(col, row, conio);
            col += font.glyphs[index].width + 2;
        }
    }
}