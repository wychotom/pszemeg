#ifndef SERVER_ANTENNA_H
#define SERVER_ANTENNA_H

#include "Log.h"

#include <ctime>
#include <string>
#include <queue>

class Antenna
{
    public:
        Antenna();
        void render_antenna(int clients_count);
        void set_transmitting(Colors color);

    private:
        double map(double x, double in_min, double in_max, double out_min, double out_max);
        static clock_t transmission_start_time;
        static std::queue<Colors> transmission_queue;
        void show_antenna(int clients_count);
        std::string antenna_color;
};

#endif //SERVER_ANTENNA_H
