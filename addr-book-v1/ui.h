#pragma once
#include "data.h"

typedef enum MENU
{
	EXIT, NEW, SEARCH, PRINT, REMOVE, COMMIT, DEFRAGMENT
} MENU;

MENU printMenu(void);
void eventLoopRun(void);
void addDataUi(void);
void printDataUi(void);
void commitDataUi(void);
void searchDataUi(void);
void printUserData(USERDATA* userData, int formatSelect);
void deleteDataUi(void);
