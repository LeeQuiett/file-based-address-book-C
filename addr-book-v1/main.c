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

	//testPrintIndex(); //�׽�Ʈ �ڵ�

	initializeList();

	//testPrintList(); //�׽�Ʈ �ڵ�
	
	//printAllDataWithDeletedData(FILENAME); //�׽�Ʈ �ڵ�

	eventLoopRun();

	deleteList();

	return 0;
}
