#pragma once

typedef enum MENU
{
	EXIT, NEW, SEARCH, PRINT, REMOVE, COMMIT
} MENU;

MENU printMenu(void);
void eventLoopRun(void);
void addDataUi(void);
void printDataUi(void);
void commitDataUi(void);
void searchDataUi(void);