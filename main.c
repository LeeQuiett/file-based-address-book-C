#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "header.h"

#define MENU_COUNT 5

typedef enum { ADD = 1, DELETE, SEARCH, PRINT, EXIT } MY_SELECT;
typedef void (*pFunction)(void);


int main() {
	MY_SELECT my_select;
	pFunction menuFunctions[MENU_COUNT] = { 
		add_addr_controller,
		delete_addr_controller,
		search_addr_controller,
		print_addr_controller,
		exit_addr_controller
	};

	while (1) {
		printf("[1] ADD [2] DELETE [3] SEARCH [4] PRINT [5] EXIT: ");
		scanf("%d%*c", &my_select);

		if (my_select == 6) {
			print_first_and_last(FILENAME);
			continue;
		}

		if (my_select >= ADD && my_select <= EXIT) {
			if (my_select == EXIT) {
				menuFunctions[my_select - 1]();
				break;
			}
			else {
				menuFunctions[my_select - 1]();
			}

		}
		else {
			puts("Wrong input! Try again!");
		}
	}
	return 0;
}