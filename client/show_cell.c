#include "header.h"
#include <wchar.h>

#define BLACK_BG "\e[37m\e[40m"
#define BLACK_TEXT_GREEN_BG "\e[30m\e[42m"
#define DEFAULT "\e[0m"
#define CELL_BEGINNING_POSITION "\e[28A\r"

void print_cell(struct UE_INFO *state)
{
	const char connected = 'C';
	const char not_connected = 'D';

	const char sending = '^';
	const char receiving = 'v';
	

	const int downward_arrow_pos = 38;
	const int upward_arrow_pos = 39;
	const int connection_sign = 41;
	const int ones_pos = 45;
	const int decimals_pos = 44;
	const int hundreds_pos = 43;

	char battery_str[3];
	extract_battery(state->battery_life, battery_str);

	const char cellphoneup[] =
	"	                  "BLACK_BG".--."DEFAULT"\n"
	"                          "BLACK_BG"|  |"DEFAULT"\n"
	"                          "BLACK_BG"|  |"DEFAULT"\n"
	"                          "BLACK_BG"|  |"DEFAULT"\n"
	"                          "BLACK_BG"|  |"DEFAULT"\n"
	"         "BLACK_BG"_.-----------._  |  |"DEFAULT"\n"
	"      "BLACK_BG".-'      __       `-.  |"DEFAULT"\n"
	"    "BLACK_BG".'       .'  `.        `.|"DEFAULT"\n"
	"   "BLACK_BG";         :    :          ;"DEFAULT"\n"
	"   "BLACK_BG"|         `.__.'          |"DEFAULT"\n"
	"   "BLACK_BG"|   ___                   |"DEFAULT"\n"
	"   "BLACK_BG"|  (_E_) E R I C S S O N  |"DEFAULT"\n"
	"   "BLACK_BG"| "BLACK_TEXT_GREEN_BG".---------------------."BLACK_BG" |"DEFAULT"\n";




	char cellphonedata[] = "   " BLACK_BG "| " BLACK_TEXT_GREEN_BG "|                    %|"BLACK_BG" |"DEFAULT"\n";
	cellphonedata[hundreds_pos] = battery_str[0];
	cellphonedata[decimals_pos] = battery_str[1];
	cellphonedata[ones_pos] = battery_str[2];

	if(state->UE_state < 5)
	 	cellphonedata[connection_sign] = not_connected;
	else
	 	cellphonedata[connection_sign] = connected;

	cellphonedata[upward_arrow_pos] = ' ';
	cellphonedata[downward_arrow_pos] = ' ';
	
	char * text_to_show = NULL;

	

	const char cellphonemiddle[] = 
	"   "BLACK_BG"| "BLACK_TEXT_GREEN_BG"|                     |"BLACK_BG" |"DEFAULT"\n"
	"   "BLACK_BG"| "BLACK_TEXT_GREEN_BG"|                     |"BLACK_BG" |"DEFAULT"\n";

	char cellphonetext_blank[] = "   "BLACK_BG"| "BLACK_TEXT_GREEN_BG"|                     |"BLACK_BG" |"DEFAULT"\n";
	char cellphonetext_send[] = "   "BLACK_BG"| "BLACK_TEXT_GREEN_BG"|  DATA IS BEING SENT |"BLACK_BG" |"DEFAULT"\n";
	char cellphonetext_recv[] = "   "BLACK_BG"| "BLACK_TEXT_GREEN_BG"|  DATA IS BEING RECV |"BLACK_BG" |"DEFAULT"\n";


	const char cellphonedown[] = "   "BLACK_BG"| "BLACK_TEXT_GREEN_BG"|                     |"BLACK_BG" |"DEFAULT"\n"
	"   "BLACK_BG"| "BLACK_TEXT_GREEN_BG"|                     |"BLACK_BG" |"DEFAULT"\n"
	"   "BLACK_BG"| "BLACK_TEXT_GREEN_BG"|                     |"BLACK_BG" |"DEFAULT"\n"
	"   "BLACK_BG"| "BLACK_TEXT_GREEN_BG"`---------------------'"BLACK_BG" |"DEFAULT"\n"
	"   "BLACK_BG"|                         |"DEFAULT"\n"
	"   "BLACK_BG"|                __       |"DEFAULT"\n"
	"   "BLACK_BG"|  ________  .-~~__~~-.   |"DEFAULT"\n"
	"   "BLACK_BG"| |___C___/ /  .'  `.  \\  |"DEFAULT"\n"
	"   "BLACK_BG"|  ______  ;   : OK :   ; |"DEFAULT"\n"
	"   "BLACK_BG"| |__A___| |  _`.__.'_  | |"DEFAULT"\n"
	"   "BLACK_BG"|  _______ ; \\< |  | >/ ; |"DEFAULT"\n";

	//text_to_show = cellphonetext_blank;
	text_to_show = cellphonetext_send;

	switch(state->UE_state)
	{
		case INIT_BROADCAST:
			cellphonedata[downward_arrow_pos] = receiving;
			break;
		case RANDOM_ACCESS_PREAMBLE:
			cellphonedata[upward_arrow_pos] = sending;
			cellphonedata[downward_arrow_pos] = receiving;
			break;
		case RANDOM_ACCESS_RESPONSE:
			cellphonedata[upward_arrow_pos] = sending;
			break;
		case RRC_REQUEST:
			cellphonedata[upward_arrow_pos] = sending;
			cellphonedata[downward_arrow_pos] = receiving;
			break;
		case RRC_SETUP:
			cellphonedata[downward_arrow_pos] = receiving;
			break;
		case RRC_SETUP_COMPLETE:
			cellphonedata[upward_arrow_pos] = sending;
			break;
		case CONNECTED:
			text_to_show = cellphonetext_blank;
			break;
		case CONN_SENDING:
			cellphonedata[downward_arrow_pos] = sending;
			text_to_show = cellphonetext_send;
			state->UE_state = CONNECTED;
			break;
		case CONN_RECEIVE:
			cellphonedata[downward_arrow_pos] = receiving;
			text_to_show = cellphonetext_recv;
			state->UE_state = CONNECTED;
			break;
	}

	if(state->battery_life == 0)
	{
		text_to_show = cellphonetext_blank;
		cellphonedata[upward_arrow_pos] = ' ';
		cellphonedata[downward_arrow_pos] = ' ';
		cellphonedata[connection_sign] = ' ';
		cellphonedata[hundreds_pos] = ' ';
		cellphonedata[decimals_pos] = ' ';
		cellphonedata[ones_pos] = ' ';
		cellphonedata[ones_pos + 1] = ' ';

	}

	printf(CELL_BEGINNING_POSITION"%s%s%s%s%s", cellphoneup, cellphonedata, cellphonemiddle, text_to_show, cellphonedown);
	
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
	for(int i = 0; i < max_digits - 1; i++)
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