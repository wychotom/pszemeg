#ifndef SERVER_ANTENNA_H
#define SERVER_ANTENNA_H

#include "Log.h"

#include <ctime>
#include <string>

class Antenna
{
    public:
        Antenna();
        void render_antenna();
        void set_transmitting();
        void set_color(Colors color);

    private:
        static clock_t transmission_start_time;
        static std::string antenna_color;
};

#endif //SERVER_ANTENNA_H
