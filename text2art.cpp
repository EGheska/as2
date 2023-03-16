//
// Created by dza02 on 2/17/2023.
//

#include <iostream>
#include "tty.h"
#include "ascii_fonts.h"

tty::foreground_color text_to_color(std::string text, std::string color){
    ascii_art::text_printer obj{};
    if (color == "BLACK"){
        obj.font.load_glyphs("./fonts/doh.ascii");
        obj.set_color(tty::foreground_color::BLACK, tty::color_variant::DIM);
        obj.render_text(7,5 ,text);
    } else if (color == "RED"){
        obj.font.load_glyphs("./fonts/doh.ascii");
        obj.set_color(tty::foreground_color::RED, tty::color_variant::DIM);
        obj.render_text(7,5,text);
    } else if (color == "GREEN"){
        obj.font.load_glyphs("./fonts/doh.ascii");
        obj.set_color(tty::foreground_color::GREEN, tty::color_variant::DIM);
        obj.render_text(7,5 ,text);
    } else if (color == "YELLOW"){
        obj.font.load_glyphs("./fonts/doh.ascii");
        obj.set_color(tty::foreground_color::YELLOW, tty::color_variant::DIM);
        obj.render_text(7,5 ,text);
    } else if (color == "BLUE"){
        obj.font.load_glyphs("./fonts/doh.ascii");
        obj.set_color(tty::foreground_color::BLUE, tty::color_variant::DIM);
        obj.render_text(7,5 ,text);
    } else if (color == "MAGNETA"){
        obj.font.load_glyphs("./fonts/doh.ascii");
        obj.set_color(tty::foreground_color::MAGNETA, tty::color_variant::DIM);
        obj.render_text(7,5 ,text);
    } else if (color == "CYAN") {
        obj.font.load_glyphs("./fonts/doh.ascii");
        obj.set_color(tty::foreground_color::CYAN, tty::color_variant::DIM);
        obj.render_text(7,5 ,text);
    } else if (color == "WHITE") {
        obj.font.load_glyphs("./fonts/doh.ascii");
        obj.set_color(tty::foreground_color::WHITE, tty::color_variant::DIM);
        obj.render_text(7,5 ,text);
    }
//    switch(i){
//        case tty::foreground_color::BLACK:
//            obj.set_fg_color(tty::foreground_color::BLACK, tty::color_variant::DIM);
//            break;
//        case tty::foreground_color::RED:
//            obj.set_fg_color(tty::foreground_color::RED, tty::color_variant::DIM);
//            break;
//        case tty::foreground_color::GREEN:
//            obj.set_fg_color(tty::foreground_color::GREEN, tty::color_variant::DIM);
//            break;
//        case tty::foreground_color::YELLOW:
//            obj.set_fg_color(tty::foreground_color::YELLOW, tty::color_variant::DIM);
//            break;
//        case tty::foreground_color::BLUE:
//            obj.set_fg_color(tty::foreground_color::BLUE, tty::color_variant::DIM);
//            break;
//        case tty::foreground_color::MAGNETA:
//            obj.set_fg_color(tty::foreground_color::MAGNETA, tty::color_variant::DIM);
//            break;
//        case tty::foreground_color::CYAN:
//            obj.set_fg_color(tty::foreground_color::CYAN, tty::color_variant::DIM);
//            break;
//        case tty::foreground_color::WHITE:
//            obj.set_fg_color(tty::foreground_color::WHITE, tty::color_variant::DIM);
//            break;
//    }
    return {};
}

int main(int argc, char * argv[]){
//    tty::console conio{};
//    conio.clear();
//    conio.move_xy(7,5);
//    conio.set_fg_color(tty::foreground_color::RED, tty::color_variant::DIM);
//    std::cout << "Hello, green world!\n";
//
//    ascii_art::text_printer prn{};
//    prn.font.load_glyphs("./fonts/doh.ascii");
//
//    prn.set_color(tty::foreground_color::BLUE, tty::color_variant::NORMAL);
//    prn.render_text(2, 2, "VOID");
//    return EXIT_SUCCESS;
    std::string name;
    std::string color;
    std::cin >> name >> color;
    text_to_color(name, color);
}