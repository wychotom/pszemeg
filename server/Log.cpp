#include "Log.h"
#include <iostream>
#include <string>
#include <sys/time.h>

void Log::info(const std::string &channel, const std::string &message)
{
    int max_channel_length = 5;

    std::string log_content;
    log_content += get_time() + " ";
    log_content += std::string(max_channel_length - channel.size(), ' ');
    log_content += "\033[1;33m[" + channel + "] \033[0m";
    log_content += message;

    std::cout << log_content << std::endl;
}

void Log::error(const std::string &message)
{
    std::string log_content;
    log_content += get_time() + " ";
    log_content += "\033[1;31m" + message + "\033[0m";

    std::cout << log_content << std::endl;
}

std::string Log::get_time()
{
    char buffer[20];
    timeval tp;
    gettimeofday(&tp, 0);
    time_t curtime = tp.tv_sec;
    tm *t = localtime(&curtime);

    strftime(buffer,20,"%X",t);

    std::string time_str(buffer);
    time_str += ".";
    time_str += tp.tv_usec/1000 < 100 ? "0" : "";
    time_str += tp.tv_usec/1000 < 10 ? "0" : "";
    time_str += std::to_string(tp.tv_usec/1000);

    return time_str;
}
