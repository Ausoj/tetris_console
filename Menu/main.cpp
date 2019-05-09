#include "menu.h"
#include "tetris.h"
#include <iostream>

int main() {
	Menu menu;
	
	while (true) {
		if (menu.bMenu())
			game();
		// game wont stop after lose condition is met
	}


	return 0;
}