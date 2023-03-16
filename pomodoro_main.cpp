#include <iostream>
#include "pomodoro.h"
#include <thread>


int main(int argc, char * argv[]){
    // TODO implement argument parsing
    long work_time, break_time;
    std::cout << "Enter a work time and a break time for the timer";
    std::cin >> work_time, break_time;
    pomodoro::pomodoro timer{};
    timer.initialize(work_time, break_time);
    timer.run();

}