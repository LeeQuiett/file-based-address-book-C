#pragma once
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "data.h"
#include "main.h"

//unsigned int g_offset = 0;
unsigned int g_endOffset = 0;
unsigned int g_listCount = 0;

NODE g_HeadNode = { 0 };
NODE g_TailNode = { 0 };

void initializeList() 
{
	g_HeadNode.Next = &g_TailNode;
	g_TailNode.Prev = &g_HeadNode;
}

int* allocateMemoryForPhone(size_t size)
{
	int* ptr = malloc(size);
	if (ptr != NULL)
	{
		memset(ptr, -1, size);
	}

	if (ptr == NULL)
	{
		puts("메모리 할당 실패\n");
		exit(1);
	}

	return ptr;
}

void initializeIndex(const char* filename)
{
	printf("인덱스를 초기화 중입니다.\n");
	USERDATA userData;

	FILE* fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		puts("파일 개방 실패!");
		return;
	}
	while (fread(&userData, sizeof(userData), 1, fp))
	{
		phoneIndex[userData.phone] = g_endOffset;
		g_endOffset++;
	}
	fclose(fp);
}

void addData(const char* filename, USERDATA* userData)
{
	printf("[1] Commit [2] 나중에: ");
	int input;
	if (scanf_s("%d%*c", &input) != 1)
	{
		puts("잘못된 입력입니다.");
		return;
	}

	if (input == 1)
	{
		AddNewNode(true, userData->phone, userData, sizeof(USERDATA), g_endOffset);
		puts("Commit! 파일에 저장합니다.");
		FILE* fp = fopen(filename, "ab");

		if (fp == NULL)
		{
			puts("addData() 에서 파일 개방 실패!");
			return;
		}
		fwrite(userData, sizeof(USERDATA), 1, fp);
		phoneIndex[userData->phone] = g_endOffset;
		g_endOffset++;
		fclose(fp);
	}
	else if (input == 2)
	{
		puts("파일에 저장하지 않았습니다. 메인메모리에만 저장되어있습니다.");
		AddNewNode(true, userData->phone, userData, sizeof(USERDATA), -1);
		//g_endOffset++;
	}
	else
	{
		puts("잘못된 입력입니다.");
	}
}

void AddNewNode(bool bNew, int key,	const void* dataCache, unsigned int sizeOfData,	unsigned int offset)
{	
	NODE* newNode = (NODE*)calloc(1, sizeof(NODE));
	if (newNode == NULL)
	{
		puts("메모리 할당 실패!");
		return;
	}

	// NODE 인스턴스의 필드 key에 값 할당
	//int lenOfKey = (int)strlen(key);
	//newNode->key = malloc(lenOfKey + 1);
	//strcpy_s(newNode->key, lenOfKey + 1, key);
	newNode->key = key;


	// NODE 인스턴스의 필드 dataCache
	// sizeOfData에 값 할당
	if (dataCache != NULL && sizeOfData > 0)
	{
		void* newData = malloc(sizeOfData);
		memcpy(newData, dataCache, sizeOfData);
		newNode->dataCache = newData;
		newNode->sizeOfData = sizeOfData;
	}

	newNode->bNew = bNew;
	newNode->offset = offset;

	NODE* prevNode = g_TailNode.Prev;
	prevNode->Next = newNode;
	newNode->Prev = prevNode;
	newNode->Next = &g_TailNode;
	g_TailNode.Prev = newNode;

	++g_listCount;
}

void printData(const char* filename)
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
		printf("%d, %s, %s\n", userData.phone, userData.name, userData.address);
	}
	fclose(fp);
}

void printCache()
{
	NODE* current = g_HeadNode.Next;

	while (current != &g_TailNode)
	{
		if (current->dataCache != NULL)
		{
			USERDATA* userData = (USERDATA*)current->dataCache;
			printf("전화번호: %-15d || 이름: %-4s || 주소: %-30s || ", userData->phone, userData->name, userData->address);
			if (current->offset != -1)
			{
				printf("Commit: %-10s || key: %-10d || offset: %-10d\n\n", "Yes", current->key, current->offset);
			}
			else
			{
				printf("Commit: %-10s || key: %-10d || offset: %-10d\n\n", "Not yet", current->key, current->offset);
			}
		}
		current = current->Next;
	}
}

// 커밋하지 않은 데이터 저장하는 함수
void commitData(void)
{
	NODE* current = g_HeadNode.Next;
	FILE* fp = fopen(FILENAME, "ab");

	if (fp == NULL)
	{
		puts("파일 개방 실패!");
		return;
	}

	while (current != &g_TailNode)
	{
		if (current->offset == -1 && current->dataCache != NULL)
		{
			USERDATA* userData = (USERDATA*)current->dataCache;

			puts("Commit! 파일에 저장합니다.");
			
			fwrite(userData, sizeof(USERDATA), 1, fp);
			
			phoneIndex[userData->phone] = g_endOffset;
			current->offset = g_endOffset;
			g_endOffset++;			
		}
		current = current->Next;
	}
	fclose(fp);
	puts("커밋이 완료되었습니다.");
	_getch();
}

// 커밋 취소하는 함수
void commitCancle(void)
{
	NODE* current = g_HeadNode.Next;
	while (current != &g_TailNode)
	{
		if (current->offset = -1 && current->dataCache != NULL)
		{
			NODE* Prev = current->Prev;
			NODE* Next = current->Next;

			Prev->Next = current->Next;
			Next->Prev = current->Prev;

			free(current->dataCache);
			free(current);

			--g_listCount;

			current = Next;
		}
		else
		{
			current = current->Next;
		}
	}
	puts("커밋 취소가 완료되었습니다.");
	_getch();
}

// 이름으로 검색하는 함수
void searchByName(void)
{
	char name[12];
	printf("검색할 이름을 입력하세요: ");
	fgets(name, sizeof(name), stdin);
	name[strcspn(name, "\n")] = '\0';

	FILE* fp = fopen(FILENAME, "rb");
	if (fp == NULL)
	{
		puts("파일 개방 실패!");
		return;
	}

	USERDATA userData;
	int foundFlag = 0;
	int offset = 0;
	while (fread(&userData, sizeof(USERDATA), 1, fp))
	{
		if (strcmp(userData.name, name) == 0)
		{
			printf("전화번호: %-15d || 이름: %-4s || 주소: %-30s || ", userData.phone, userData.name, userData.address);
			puts("파일에서 읽은 데이터입니다.");

			// 일치하는 데이터는 캐시에 추가
			AddNewNode(false, userData.phone, &userData, sizeof(USERDATA), offset);
			foundFlag = 1;
		}
		offset++;
	}
	fclose(fp);

	if (!foundFlag)
	{
		puts("일치하는 이름이 없습니다.");
	}
	_getch();
}

int searchByNameFromCache()
{
	char name[12];
	printf("캐시에서 검색할 이름을 입력하세요: ");
	fgets(name, sizeof(name), stdin);
	name[strcspn(name, "\n")] = '\0';

	NODE* current = g_HeadNode.Next;
	int foundFlag = 0;

	while (current != &g_TailNode)
	{
		if (current->dataCache != NULL)
		{
			USERDATA* userData = (USERDATA*)current->dataCache;

			if (strcmp(userData->name, name) == 0)
			{
				printf("전화번호: %-15d || 이름: %-4s || 주소: %-30s || ", userData->phone, userData->name, userData->address);
				foundFlag = 1;
			}
		}
		current = current->Next;
	}

	if (!foundFlag)
	{
		puts("일치하는 이름이 캐시에 없습니다.");
	}
	_getch();
	return foundFlag;
}