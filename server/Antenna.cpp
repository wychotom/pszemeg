#include "Antenna.h"
#include "Log.h"

#include <iostream>
#include <ctime>
#include <queue>

clock_t Antenna::transmission_start_time;
std::queue<Colors> Antenna::transmission_queue;

Antenna::Antenna()
{
}

void Antenna::render_antenna()
{
    clock_t now = clock();
    double elapsed_secs = double(now - transmission_start_time) / CLOCKS_PER_SEC;
    double duration = map(transmission_queue.size(), 0, 3, 0.3, 0.08);

    if(elapsed_secs > duration)
    {
        antenna_color = Log::colors[Colors::Default];

        if(transmission_queue.size() > 0)
        {
            transmission_start_time = clock();
            antenna_color = Log::colors[transmission_queue.front()];
            transmission_queue.pop();
        }
    }

    std::cout << "\033[35A\r";
    std::cout << antenna_color << "                   ,                          .                   " << std::endl;
    std::cout << antenna_color << "                 :;                            ;;                 " << std::endl;
    std::cout << antenna_color << "                ;,  ;:                      :;  .;                " << std::endl;
    std::cout << antenna_color << "               ,;  ;,  ;                   ;  .;  :,               " << std::endl;
    std::cout << antenna_color << "               '  .;  ;.  .     " << Log::colors[Colors::Red] << ",,." << Log::colors[Colors::Default] << "" << antenna_color << "     .  .;  ;.  '               " << std::endl;
    std::cout << antenna_color << "               '  ;.  '  '    " << Log::colors[Colors::Red] << "'''''''" << Log::colors[Colors::Default] << "" << antenna_color << "    '  '  `;  '`              " << std::endl;
    std::cout << antenna_color << "               '  ;.  '  '`  " << Log::colors[Colors::Red] << "'''''''''" << Log::colors[Colors::Default] << "" << antenna_color << "  `'  '  `;  '               " << std::endl;
    std::cout << antenna_color << "               '  `'  :,     " << Log::colors[Colors::Red] << "'''''''''" << Log::colors[Colors::Default] << "" << antenna_color << "    ,   ,;  '`                 " << std::endl;
    std::cout << antenna_color << "               .;  ::  ::     " << Log::colors[Colors::Default] << "," << Log::colors[Colors::Red] << "'''''" << Log::colors[Colors::Default] << "," << antenna_color << "    ::  :;  ;.               " << std::endl;
    std::cout << antenna_color << "                ::  ,;        " << Log::colors[Colors::Default] << "''' '''" << antenna_color << "       ;,  ::                " << Log::colors[Colors::Default] << std::endl;
    std::cout << antenna_color << "                 ,;          " << Log::colors[Colors::Default] << ":'': ,'';" << antenna_color << "         ;,                 " << Log::colors[Colors::Default] << std::endl;
    std::cout << "                             ''' `''''                            " << std::endl;
    std::cout << "                            ;'':'''''''                           " << std::endl;
    std::cout << "                           `''''''  ;''`                          " << std::endl;
    std::cout << "                           '''';     '''                          " << std::endl;
    std::cout << "                          ,'''';     ;'':                         " << std::endl;
    std::cout << "                          '''.'''',   '''                         " << std::endl;
    std::cout << "                         :'':   ;'''; ,'';                        " << std::endl;
    std::cout << "                         '''      :'''''''                        " << std::endl;
    std::cout << "                        ;''.         ''''''                       " << std::endl;
    std::cout << "                       `'''            ;'''.                      " << std::endl;
    std::cout << "                       '''           ,''''''                      " << std::endl;
    std::cout << "                      ,'';         :'''' ;'':                     " << std::endl;
    std::cout << "                      '''        ;''':    '''                     " << std::endl;
    std::cout << "                     ;'',     `'''':      ,'';                    " << std::endl;
    std::cout << "                     '''    `''''.         '''                    " << std::endl;
    std::cout << "                    '''.  ,''''            `'''                   " << std::endl;
    std::cout << "                   `''' ;''''               ;''.                  " << std::endl;
    std::cout << "                   ''';''':                  '''                  " << std::endl;
    std::cout << "                  ,''''',                    :'':                 " << std::endl;
    std::cout << "                  ''''.                       '''                 " << std::endl;
    std::cout << "                 ;'',                         .'';                " << std::endl;
    std::cout << "                 '''                           '''`               " << std::endl;
    std::cout << "                 `:`                           `:`                " << std::endl << std::flush;
}

void Antenna::set_transmitting(Colors color)
{
    transmission_queue.push(color);
}

double Antenna::map(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


