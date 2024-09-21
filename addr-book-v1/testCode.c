#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "conio.h"
#include "data.h"
#include "testCode.h"

// 인덱스를 출력
void testPrintIndex()
{
	for (int i = 0; i < 99999999; i++)
	{
		if(phoneIndex[i] != -1)
			printf("%d\n", phoneIndex[i]);
	}
	_getch();
}

// 파일에서 데이터 읽어오기 함수
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

// 데이터 출력 함수
void print_records(const Addr* records, int num_records) 
{
    for (int i = 0; i < num_records; i++) {
        printf("전화번호: %d, 이름: %s, 주소: %s\n",
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

    // 데이터 파일에서 읽어오기
    load_data_from_file(records, NUM_RECORDS, "test_3_dummy.dat");

    // 데이터 출력
    print_records(records, NUM_RECORDS);

    free(records);
    _getch();
    return 0;
}

// deleted 마킹한 데이터까지 출력 함수
void printAllDataWithDeletedData(const char* filename)
{
	USERDATA userData;

	FILE* fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		puts("파일 개방 실패!");
		return;
	}

	while (fread(&userData, sizeof(USERDATA), 1, fp))
	{
			printUserData(&userData, 1);			
	}
		fclose(fp);
		_getch();
}