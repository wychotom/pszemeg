#include "header.h"
#include <wchar.h>

#define BLACK_BG "\e[37m\e[40m"
#define GREEN_BG "\e[37m\e[42m"
#define NORMAL_BG "\e[0m"

void print_cell(struct UE_INFO state)//im just goofing pls no atacc
{
	const wchar_t white_arrow_up = '⬆';
	const wchar_t white_arrow_down = '⬇';
	const wchar_t black_arrow_up = '⇧';
	const wchar_t black_arrow_down = '⇩';

	const wchar_t connected = 'C';
	const wchar_t not_connected = 'D';

	const int downward_arrow_pos = 38;
	const int upward_arrow_pos = 39;

	const int connection_sign = 41;

	const int hundreds = 45;//23 24 25
	const int decimals = 44;//23 24 25
	const int ones = 43;//23 24 25

	const char cellphoneup[] = //I HOPE ITS BEAUTIFUL
	"	                  "BLACK_BG".--."NORMAL_BG"\n"
	"                          "BLACK_BG"|  |"NORMAL_BG"\n"
	"                          "BLACK_BG"|  |"NORMAL_BG"\n"
	"                          "BLACK_BG"|  |"NORMAL_BG"\n"
	"                          "BLACK_BG"|  |"NORMAL_BG"\n"
	"         "BLACK_BG"_.-----------._  |  |"NORMAL_BG"\n"
	"      "BLACK_BG".-'      __       `-.  |"NORMAL_BG"\n"
	"    "BLACK_BG".'       .'  `.        `.|"NORMAL_BG"\n"
	"   "BLACK_BG";         :    :          ;"NORMAL_BG"\n"
	"   "BLACK_BG"|         `.__.'          |"NORMAL_BG"\n"
	"   "BLACK_BG"|   ___                   |"NORMAL_BG"\n"
	"   "BLACK_BG"|  (_E_) E R I C S S O N  |"NORMAL_BG"\n"
	"   "BLACK_BG"| "GREEN_BG".---------------------."BLACK_BG" |"NORMAL_BG"\n";




	char cellphonedata[] = "   " BLACK_BG "| " GREEN_BG "|                  99%|"BLACK_BG" |"NORMAL_BG"\n";
	cellphonedata[hundreds] = 'X';
	cellphonedata[decimals] = 'Y';
	cellphonedata[ones] = 'Z';

	cellphonedata[downward_arrow_pos] = '^';
	cellphonedata[upward_arrow_pos] = 'v';

	cellphonedata[connection_sign] = connected;

	//	"   | |                     | |\n" //▲△✈ ⬇⇩ ⇧⬆

	const char cellphonedown[] = 
	"   "BLACK_BG"| "GREEN_BG"|                     |"BLACK_BG" |"NORMAL_BG"\n"
	"   "BLACK_BG"| "GREEN_BG"|                     |"BLACK_BG" |"NORMAL_BG"\n"
	"   "BLACK_BG"| "GREEN_BG"|                     |"BLACK_BG" |"NORMAL_BG"\n"
	"   "BLACK_BG"| "GREEN_BG"|                     |"BLACK_BG" |"NORMAL_BG"\n"
	"   "BLACK_BG"| "GREEN_BG"|                     |"BLACK_BG" |"NORMAL_BG"\n"
	"   "BLACK_BG"| "GREEN_BG"|                     |"BLACK_BG" |"NORMAL_BG"\n"
	"   "BLACK_BG"| "GREEN_BG"`---------------------'"BLACK_BG" |"NORMAL_BG"\n"
	"   "BLACK_BG"|                         |"NORMAL_BG"\n"
	"   "BLACK_BG"|                __       |"NORMAL_BG"\n"
	"   "BLACK_BG"|  ________  .-~~__~~-.   |"NORMAL_BG"\n"
	"   "BLACK_BG"| |___C___/ /  .'  `.  \\  |"NORMAL_BG"\n"
	"   "BLACK_BG"|  ______  ;   : OK :   ; |"NORMAL_BG"\n"
	"   "BLACK_BG"| |__A___| |  _`.__.'_  | |"NORMAL_BG"\n"
	"   "BLACK_BG"|  _______ ; \\< |  | >/ ; |"NORMAL_BG"\n";

	printf("\033[28A\r%s%s%s", cellphoneup, cellphonedata, cellphonedown);
	
}