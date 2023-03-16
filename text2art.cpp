//
// Created by dza02 on 2/17/2023.
//

#include <iostream>
#include "tty.h"
#include "ascii_fonts.h"

tty::foreground_color text_to_color(std::string text, std::string color){
    ascii_art::text_printer obj{};
    tty::console conio{};
    conio.clear();
    obj.font.load_glyphs("./fonts/doh.ascii");

    tty::foreground_color foregroundColor;
    tty::color_variant colorVariant = tty::color_variant::DIM;

    switch (color[0]) {
        case 'B':
            foregroundColor = tty::foreground_color::BLACK;
            break;
        case 'R':
            foregroundColor = tty::foreground_color::RED;
            break;
        case 'G':
            foregroundColor = tty::foreground_color::GREEN;
            break;
        case 'Y':
            foregroundColor = tty::foreground_color::YELLOW;
            break;
        case 'M':
            foregroundColor = tty::foreground_color::MAGNETA;
            break;
        case 'C':
            foregroundColor = tty::foreground_color::CYAN;
            break;
        case 'W':
            foregroundColor = tty::foreground_color::WHITE;
            break;
        default:
            throw std::invalid_argument("Invalid input. Color or text argument is not passed");
    }

    obj.set_color(foregroundColor, colorVariant);
    obj.render_text(7,5,text);
    return {};
}

int main(int argc, char * argv[]){
    std::cout << "Enter the text and color name. Color should be written using caps lock, i.e. 'WHITE' or 'W': " << "\n";
    std::string name;
    std::string color;
    std::cin >> name >> color;
    text_to_color(name, color);
}