#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "conio.h"
#include "data.h"
#include "testCode.h"

// �ε����� ���
void testPrintIndex()
{
	for (int i = 0; i < 99999999; i++)
	{
		if(phoneIndex[i] != -1)
			printf("%d\n", phoneIndex[i]);
	}
	_getch();
}

// ���Ͽ��� ������ �о���� �Լ�
void load_data_from_file(Addr* records, int num_records, const char* filename) 
{
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Failed to open file for reading");
        exit(EXIT_FAILURE);
    }
    fread(records, sizeof(Addr), num_records, file);
    fclose(file);
}

// ������ ��� �Լ�
void print_records(const Addr* records, int num_records) 
{
    for (int i = 0; i < num_records; i++) {
        printf("��ȭ��ȣ: %d, �̸�: %s, �ּ�: %s\n",
            records[i].tel, records[i].name, records[i].addr);
    }
}

void testPrintList()
{
    Addr* records = (Addr*)malloc(NUM_RECORDS * sizeof(Addr));
    if (records == NULL) {
        perror("Failed to allocate memory for records");
        return EXIT_FAILURE;
    }

    // ������ ���Ͽ��� �о����
    load_data_from_file(records, NUM_RECORDS, "test_3_dummy.dat");

    // ������ ���
    print_records(records, NUM_RECORDS);

    free(records);
    _getch();
    return 0;
}

// deleted ��ŷ�� �����ͱ��� ��� �Լ�
void printAllDataWithDeletedData(const char* filename)
{
	USERDATA userData;

	FILE* fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		puts("���� ���� ����!");
		return;
	}

	while (fread(&userData, sizeof(USERDATA), 1, fp))
	{
			printUserData(&userData, 1);			
	}
		fclose(fp);
		_getch();
}