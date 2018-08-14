#ifndef SERVER_LOG_H
#define SERVER_LOG_H

#include <string>

class Log
{
    public:
        static void info(const std::string &channel, const std::string &message);
        static void error(const std::string &message);

    private:
        static std::string get_time();
};


#endif //SERVER_LOG_H
