#pragma once
#include <stdio.h>
#include "ui.h"
#include "data.h"
#include "main.h"
#include "testCode.h"

int main(void)
{
	phoneIndex = allocateMemoryForPhone(sizeof(int) * 100000000);

	initializeIndex(FILENAME);

	//testPrintIndex(); //테스트 코드

	initializeList();

	//testPrintList(); //테스트 코드
	
	//printAllDataWithDeletedData(FILENAME); //테스트 코드

	eventLoopRun();

	deleteList();

	return 0;
}
