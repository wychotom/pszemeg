#include "header.h"
#include <wchar.h>

#define BLACK_BG "\e[37m\e[40m"
#define GREEN_BG "\e[37m\e[42m"
#define NORMAL_BG "\e[0m"

void print_cell(struct UE_INFO state)//im just goofing pls no atacc
{
	const char connected = 'C';
	const char not_connected = 'D';

	// const int downward_arrow_pos = 38;
	// const int upward_arrow_pos = 39;

	const int connection_sign = 41;

	const int ones = 45;//23 24 25
	const int decimals = 44;//23 24 25
	const int hundreds = 43;//23 24 25

	char battery_str[3];
	extract_battery(state.battery_life, &battery_str);

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




	char cellphonedata[] = "   " BLACK_BG "| " GREEN_BG "|                    %|"BLACK_BG" |"NORMAL_BG"\n";
	cellphonedata[hundreds] = battery_str[0];
	cellphonedata[decimals] = battery_str[1];
	cellphonedata[ones] = battery_str[2];

	if(state.UE_state < 5)
		cellphonedata[connection_sign] = not_connected;
	else
		cellphonedata[connection_sign] = connected;

	// cellphonedata[downward_arrow_pos] = '^';
	// cellphonedata[upward_arrow_pos] = 'v';

	// cellphonedata[connection_sign] = connected;

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

void extract_battery(int number, char *result)
{
	const int max_digits = 3;
	const int ASCII_OFFSET = 48;

	int currval;
	for(int i = max_digits - 1; i >= 0; i--)
	{
		currval = number%10;
        result[i] = (char)(currval + ASCII_OFFSET);
        number=(number - currval)/10;
	}
	for(int i = 0; i < max_digits; i++)
	{
		if(result[i] != '0')
			break;
		result[i] = ' ';
	}
}

inline void print_initial_offset()
{
	const int cellphone_height = 29;
	for(int i = 0; i < cellphone_height; i++)
		printf("\n");
}