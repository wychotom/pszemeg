#include "Antenna.h"
#include "Log.h"

#include <iostream>
#include <ctime>

clock_t Antenna::transmission_start_time = 0;
std::string Antenna::antenna_color = "";

Antenna::Antenna()
{
}

void Antenna::render_antenna()
{
    clock_t now = clock();
    double elapsed_secs = double(now - Antenna::transmission_start_time) / CLOCKS_PER_SEC;

    if(elapsed_secs > 0.1)
    {
        antenna_color = Log::colors[Colors::Default];
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

void Antenna::set_transmitting()
{
    Antenna::transmission_start_time = clock();
}

void Antenna::set_color(Colors color)
{
    antenna_color = Log::colors[color];
}


