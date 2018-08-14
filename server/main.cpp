#include <iostream>
#include "eNodeB.h"
#include "Log.h"
#include <string>
#include <csignal>

void stop_handler(int sig)
{
    if (sig == SIGINT)
    {
        eNodeB::is_running = false;
    }
}

void render_antenna()
{
    std::cout << Log::colors[Colors::Yellow] << "                   ,                          .                   " << std::endl;
    std::cout << Log::colors[Colors::Yellow] << "                 :;                            ;;                 " << std::endl;
    std::cout << Log::colors[Colors::Yellow] << "                ;,  ;:                      :;  .;                " << std::endl;
    std::cout << Log::colors[Colors::Yellow] << "               ,;  ;,  ;                   ;  .;  :,               " << std::endl;
    std::cout << Log::colors[Colors::Yellow] << "               '  .;  ;.  .     " << Log::colors[Colors::Red] << ",,." << Log::colors[Colors::Default] << "" << Log::colors[Colors::Yellow] << "     .  .;  ;.  '               " << std::endl;
    std::cout << Log::colors[Colors::Yellow] << "               '  ;.  '  '    " << Log::colors[Colors::Red] << "'''''''" << Log::colors[Colors::Default] << "" << Log::colors[Colors::Yellow] << "    '  '  `;  '`              " << std::endl;
    std::cout << Log::colors[Colors::Yellow] << "               '  ;.  '  '`  " << Log::colors[Colors::Red] << "'''''''''" << Log::colors[Colors::Default] << "" << Log::colors[Colors::Yellow] << "  `'  '  `;  '               " << std::endl;
    std::cout << Log::colors[Colors::Yellow] << "               '  `'  :,     " << Log::colors[Colors::Red] << "'''''''''" << Log::colors[Colors::Default] << "" << Log::colors[Colors::Yellow] << "    ,   ,;  '`                 " << std::endl;
    std::cout << Log::colors[Colors::Yellow] << "               .;  ::  ::     " << Log::colors[Colors::Default] << "," << Log::colors[Colors::Red] << "'''''" << Log::colors[Colors::Default] << "," << Log::colors[Colors::Yellow] << "    ::  :;  ;.               " << std::endl;
    std::cout << Log::colors[Colors::Yellow] << "                ::  ,;        " << Log::colors[Colors::Default] << "''' '''" << Log::colors[Colors::Yellow] << "       ;,  ::                " << Log::colors[Colors::Default] << std::endl;
    std::cout << Log::colors[Colors::Yellow] << "                 ,;          " << Log::colors[Colors::Default] << ":'': ,'';" << Log::colors[Colors::Yellow] << "         ;,                 " << Log::colors[Colors::Default] << std::endl;
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

int main()
{
    signal(SIGINT, stop_handler);

    #ifndef DEBUG
        render_antenna();
    #endif

    try
    {
        eNodeB enb;
        Log::general("\n\nPROGRAMMET FUNGERAR\n\n");
        enb.start();
    }
    catch(std::string e)
    {
        Log::error(e);
    }

    Log::general("\n\nMAIN HALT\n\n");
}