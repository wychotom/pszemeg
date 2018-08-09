#include "header.h"

int main(int argc, char ** argv)
{
	#ifndef DEBUG
	print_cell();
	#endif
	
	handletraffic();
	return 0;
}
