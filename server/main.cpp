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
    std::cout << "\033[1;33m                   ,                          .                   " << std::endl;
    std::cout << "\033[1;33m                 :;                            ;;                 " << std::endl;
    std::cout << "\033[1;33m                ;,  ;:                      :;  .;                " << std::endl;
    std::cout << "\033[1;33m               ,;  ;,  ;                   ;  .;  :,               " << std::endl;
    std::cout << "\033[1;33m               '  .;  ;.  .     \033[1;31m,,.\033[0m\033[1;33m     .  .;  ;.  '               " << std::endl;
    std::cout << "\033[1;33m               '  ;.  '  '    \033[1;31m'''''''\033[0m\033[1;33m    '  '  `;  '`              " << std::endl;
    std::cout << "\033[1;33m               '  ;.  '  '`  \033[1;31m'''''''''\033[0m\033[1;33m  `'  '  `;  '               " << std::endl;
    std::cout << "\033[1;33m               '  `'  :,     \033[1;31m'''''''''\033[0m\033[1;33m    ,   ,;  '`                 " << std::endl;
    std::cout << "\033[1;33m               .;  ::  ::     \033[0m,\033[1;31m'''''\033[0m,\033[1;33m    ::  :;  ;.               " << std::endl;
    std::cout << "\033[1;33m                ::  ,;        \033[0m''' '''\033[1;33m       ;,  ::                \033[0m" << std::endl;
    std::cout << "\033[1;33m                 ,;          \033[0m:'': ,'';\033[1;33m         ;,                 \033[0m" << std::endl;
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
    render_antenna();

    try
    {
        eNodeB enb;
        std::cout << "\n\n\033[1;31mPROGRAMMET FUNGERAR\033[0m\n\n";
        enb.start();
    }
    catch(std::string e)
    {
        Log::error(e);
    }

    std::cout << "\n\n\033[1;31mMAIN HALT\033[0m\n\n" << std::endl;
}