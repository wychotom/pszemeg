#ifndef SERVER_LOG_H
#define SERVER_LOG_H

#include <string>
#include <map>

enum Colors
{
    Yellow,
    Red,
    Green,
    Magenta,
    Cyan,
    Blue,
    Default
};

class Log
{
    public:
        static void info(const std::string &channel, const std::string &message);
        static void error(const std::string &message);
        static void general(const std::string &message);
        static std::map<Colors, std::string> colors;

    private:
        static std::string get_time();
};


#endif //SERVER_LOG_H
