#include "Log.h"
#include <iostream>
#include <string>
#include <sys/time.h>
#include <map>

std::map<Colors, std::string> Log::colors = {
        {Colors::Default, "\033[0m"},
        {Colors::Yellow, "\033[1;33m"},
        {Colors::Red, "\033[1;31m"},
        {Colors::Magenta, "\033[1;32m"},
        {Colors::Green, "\033[1;35m"},
        {Colors::Cyan, "\033[1;36m"},
        {Colors::Blue, "\033[1;34m"}
};

void Log::info(const std::string &channel, const std::string &message)
{
    #ifdef DEBUG
        int max_channel_length = 5;

        std::string log_content;
        log_content += get_time() + " ";
        log_content += std::string(max_channel_length - channel.size(), ' ');
        log_content += colors[Colors::Yellow] + "[" + channel + "] " + colors[Colors::Default];
        log_content += message;

        std::cout << log_content << std::endl;
    #endif
}

void Log::error(const std::string &message)
{
    #ifdef DEBUG
        std::string log_content;
        log_content += get_time() + " ";
        log_content += colors[Colors::Red] + message + colors[Colors::Default];

        std::cout << log_content << std::endl;
    #endif
}

void Log::general(const std::string &message)
{
    #ifdef DEBUG
        std::cout << colors[Colors::Red] << message << colors[Colors::Default] << std::endl;
    #endif
}

std::string Log::get_time()
{
    char buffer[20];
    timeval time_of_day = {};
    gettimeofday(&time_of_day, nullptr);
    time_t current_time = time_of_day.tv_sec;
    tm *local_time = localtime(&current_time);

    strftime(buffer, 20, "%X", local_time);

    std::string time_str(buffer);
    time_str += ".";
    time_str += time_of_day.tv_usec/1000 < 100 ? "0" : "";
    time_str += time_of_day.tv_usec/1000 < 10 ? "0" : "";
    time_str += std::to_string(time_of_day.tv_usec/1000);

    return time_str;
}
