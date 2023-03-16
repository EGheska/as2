
#include <filesystem>
#include <thread>
#include <ctime>
#include <string>
#include <sstream>
#include <chrono>
#include "pomodoro.h"


// TODO: implement the functions of the pomodoro struct
void pomodoro::pomodoro::initialize(long time_work, long time_break) {
    work_time = time_work;
    break_time = time_break;
}

void pomodoro::pomodoro::run() {
    long seconds = work_time % 60;
    long minutes = work_time / 60;
    auto duration = std::chrono::minutes(minutes) + std::chrono::seconds(seconds);
    auto start_time = std::chrono::system_clock::now();
    auto end_time = start_time + duration;
    while(true){
        while (std::chrono::system_clock::now() < end_time) {
            auto remaining_time = std::chrono::duration_cast<std::chrono::seconds>(
                    end_time - std::chrono::system_clock::now());
            long left_time = remaining_time.count();
            render(left_time);
            std::cout << minutes << ":" << seconds << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            conio.clear();
        }
        std::this_thread::sleep_for(std::chrono::seconds(break_time));
        seconds = work_time % 60;
        minutes = work_time / 60;
        duration = std::chrono::minutes(minutes) + std::chrono::seconds(seconds);
    }
}

void pomodoro::pomodoro::render(long time_left) {
    std::stringstream time;
    ascii_art::text_printer obj{};
    obj.font.load_glyphs("./fonts/ansi_shadow.ascii");
    time << time_left/60 << ":" <<time_left%60;
    std::string new_time = time.str();
    set_terminal_colors();
    obj.render_text(7,5, new_time);
}

void pomodoro::pomodoro::set_terminal_colors() {
        ascii_art::text_printer obj{};
        tty::foreground_color foregroundColor;
        tty::color_variant colorVariant = tty::color_variant::DIM;
        foregroundColor = tty::foreground_color::BLUE;
        obj.set_color(foregroundColor, colorVariant);
}