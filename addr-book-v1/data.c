#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include "data.h"
#include "main.h"
#include "ui.h"

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

void defragmentData(const char* filename)
{
	USERDATA userData;
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

void AddNewNode(bool bNew, int key, const void* dataCache, unsigned int sizeOfData, unsigned int offset)
{
	// 중복된 key 확인
	NODE* currentNode = g_HeadNode.Next;
	while (currentNode != &g_TailNode)
	{
		if (currentNode->key == key)  // key 비교
		{
			puts("중복된 key를 발견하여 Cahce에 추가하지 않습니다.");
			_getch();
			return;
		}
		currentNode = currentNode->Next;
	}

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
		//printf("%d\t%s\t%s\n", userData.phone, userData.name, userData.address);
		//printf("전화번호: %-15d || 이름: %s \t|| 주소: %-30s\n\n", userData.phone, userData.name, userData.address);
		
		// "deleted"인 경우 출력하지 않음
		if (strcmp(userData.name, "deleted") != 0 && strcmp(userData.address, "deleted") != 0)
		{
			printUserData(&userData, 1);
		}
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
			/*printf("전화번호: %-15d || 이름: %s \t|| 주소: %-30s || ", userData->phone, userData->name, userData->address);*/
			printUserData(userData, 3);
			if (current->offset != -1)
			{
				if (current->bNew == true)
					printf("Commit: % -10s || key: % -10d || offset: % -10d || 생성된 데이터!\n\n", "Yes", current->key, current->offset);
				else
					printf("Commit: % -10s || key: % -10d || offset: % -10d || 검색된 데이터!\n\n", "Yes", current->key, current->offset);
			}
			else
			{
				if (current->bNew == true)
					printf("Commit: % -10s || key: %- 10d || offset: % -10d || 생성된 데이터!!\n\n", "Not yet", current->key, current->offset);
				else
					printf("Commit: % -10s || key: %- 10d || offset: % -10d || 검색된 데이터!!\n\n", "Not yet", current->key, current->offset);
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

			puts("Commit! 파일에 저장을 시작합니다.");

			fwrite(userData, sizeof(USERDATA), 1, fp);

			phoneIndex[userData->phone] = g_endOffset;
			current->offset = g_endOffset;
			g_endOffset++;
		}
		current = current->Next;
	}
	fclose(fp);
	puts("Commit! 완료되었습니다.");
	_getch();
}

// 프로그램 종료 시 메모리 해제 하는 함수

void deleteList(void)
{
	NODE* current = g_HeadNode.Next;
	while (current != &g_TailNode)
	{
		if (current->offset == -1 && current->dataCache != NULL)
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
	// puts("커밋 취소가 완료되었습니다.");
	//_getch();
}

// 커밋 취소하는 함수
void commitCancel(void)
{
	NODE* current = g_HeadNode.Next;
	while (current != &g_TailNode)
	{
		if (current->offset == -1 && current->dataCache != NULL)
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

// 전화번호로 삭제하는 함수
void deleteByPhone()
{
	int phone;
	printf("삭제할 전화번호를 010과 -를 제외하고 입력하세요: ");
	if (scanf_s("%d%*c", &phone) != 1)
	{
		puts("잘못된 입력입니다.");
		_getch();
		return;
	}

	// phoneIndex에서 전화번호를 통해 오프셋을 찾음
	int offset = phoneIndex[phone];

	// 오프셋이 유효한지 확인 (초기화되지 않았을 경우는 -1 사용)
	if (offset == -1)
	{
		puts("일치하는 전화번호가 없습니다.");
		_getch();
		return;
	}

	// 파일에서 오프셋을 기반으로 USERDATA 읽기
	FILE* fp = fopen(FILENAME, "rb+");
	if (fp == NULL)
	{
		puts("파일 개방 실패");
		return;
	}

	// 해당 오프셋의 위치로 이동
	if (fseek(fp, offset * sizeof(USERDATA), SEEK_SET) != 0)
	{
		puts("파일 위치 이동 실패!");
		fclose(fp);
		return;
	}

	// 데이터 읽기
	USERDATA userData;
	if (fread(&userData, sizeof(USERDATA), 1, fp) == 1)
	{
		// 데이터 출력
		printf("전화번호: %-15d || 이름: %s \t|| 주소: %-30s\n", userData.phone, userData.name, userData.address);

		// 수정 여부 확인
		printf("데이터를 삭제하시겠습니까? [1] 예 [2] 아니오: ");
		int modifyInput;
		if (scanf_s("%d%*c", &modifyInput) != 1 || modifyInput != 1)
		{
			puts("삭제를 취소했습니다.");
			fclose(fp);
			return;
		}

		strcpy(userData.name, "deleted");
		strcpy(userData.address, "deleted");
		phoneIndex[phone] = -1;

		// 파일 오프셋으로 이동
		if (fseek(fp, offset * sizeof(USERDATA), SEEK_SET) != 0)
		{
			puts("파일 위치 이동 실패!");
			fclose(fp);
			_getch();
			return;
		}

		// 수정된 데이터 덮어쓰기
		if (fwrite(&userData, sizeof(USERDATA), 1, fp) != 1)
		{
			puts("데이터 삭제 실패!");
		}
		else
		{
			puts("데이터가 성공적으로 삭제되었습니다.");
		}
	}
	else
	{
		puts("데이터 읽기 실패!");
	}
	fclose(fp);
	_getch();
}

// 전화번호로 검색하는 함수
void searchByPhone()
{
	int phone;
	printf("검색할 전화번호를 010과 -를 제외하고 입력하세요: ");
	if (scanf_s("%d%*c", &phone) != 1)
	{
		puts("잘못된 입력입니다.");
		_getch();
		return;
	}

	// phoneIndex에서 전화번호를 통해 오프셋을 찾음
	int offset = phoneIndex[phone];

	// 오프셋이 유효한지 확인 (초기화되지 않았을 경우는 -1 사용)
	if (offset == -1)
	{
		puts("일치하는 전화번호가 없습니다.");
		_getch();
		return;
	}

	// 파일에서 오프셋을 기반으로 USERDATA 읽기
	FILE* fp = fopen(FILENAME, "rb+");
	if (fp == NULL)
	{
		puts("파일 개방 실패");
		return;
	}

	// 해당 오프셋의 위치로 이동
	if (fseek(fp, offset * sizeof(USERDATA), SEEK_SET) != 0)
	{
		puts("파일 위치 이동 실패!");
		fclose(fp);
		return;
	}

	// 데이터 읽기
	USERDATA userData;
	if (fread(&userData, sizeof(USERDATA), 1, fp) == 1)
	{
		// 데이터 출력
		printf("전화번호: %-15d || 이름: %s \t|| 주소: %-30s\n", userData.phone, userData.name, userData.address);

		// 수정 여부 확인
		printf("데이터를 수정하시겠습니까? [1] 예 [2] 아니오: ");
		int modifyInput;
		if (scanf_s("%d%*c", &modifyInput) != 1 || modifyInput != 1)
		{
			puts("수정을 취소했습니다.");
			fclose(fp);
			return;
		}

		// 이름 및 주소 수정
		printf("새로운 이름을 입력하세요: ");
		fgets(userData.name, sizeof(userData.name), stdin);
		userData.name[strcspn(userData.name, "\n")] = '\0';

		printf("새로운 주소를 입력하세요: ");
		fgets(userData.address, sizeof(userData.address), stdin);
		userData.address[strcspn(userData.address, "\n")] = '\0';

		// 파일 오프셋으로 이동
		if (fseek(fp, offset * sizeof(USERDATA), SEEK_SET) != 0)
		{
			puts("파일 위치 이동 실패!");
			fclose(fp);
			_getch();
			return;
		}

		// 수정된 데이터 덮어쓰기
		if (fwrite(&userData, sizeof(USERDATA), 1, fp) != 1)
		{
			puts("데이터 수정 실패!");
		}
		else
		{
			// 캐시된 리스트 순회하며 수정 반영
			NODE* currentNode = g_HeadNode.Next;
			while (currentNode != &g_TailNode)
			{
				USERDATA* userNode = (USERDATA*)currentNode->dataCache;
				if (userNode->phone == phone)
				{
					strcpy(userNode->address, userData.address);
					strcpy(userNode->name, userData.name);
				}
				currentNode = currentNode->Next;
			}

			puts("데이터가 성공적으로 수정되었습니다.");
		}
	}
	else
	{
		puts("데이터 읽기 실패!");
	}
	fclose(fp);
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
			/*printf("전화번호: %-15d || 이름: %s \t|| 주소: %-30s || ", userData.phone, userData.name, userData.address);
			puts("파일에서 읽은 데이터입니다.\n");*/
			printUserData(&userData, 2);

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

void searchByAddr(void)
{
	char addr[100];
	printf("검색할 주소를 입력하세요: ");
	fgets(addr, sizeof(addr), stdin);
	addr[strcspn(addr, "\n")] = '\0';

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
		if (strcmp(userData.address, addr) == 0)
		{
			/*printf("전화번호: %-15d || 이름: %s \t|| 주소: %-30s || ", userData.phone, userData.name, userData.address);
			puts("파일에서 읽은 데이터입니다.\n");*/
			printUserData(&userData, 2);

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

// 이름으로 캐시에서 검색하는 함수
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
				/*printf("전화번호: %-15d || 이름: %s \t|| 주소: %-30s\n\n", userData->phone, userData->name, userData->address);*/
				printUserData(userData, 1);
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

// 주소로 캐시에서 검색하는 함수
int searchByAddrFromCache()
{
	char addr[100];
	printf("캐시에서 검색할 주소를 입력하세요: ");
	fgets(addr, sizeof(addr), stdin);
	addr[strcspn(addr, "\n")] = '\0';

	NODE* current = g_HeadNode.Next;
	int foundFlag = 0;

	while (current != &g_TailNode)
	{
		if (current->dataCache != NULL)
		{
			USERDATA* userData = (USERDATA*)current->dataCache;

			if (strcmp(userData->address, addr) == 0)
			{
				/*printf("전화번호: %-15d || 이름: %s \t|| 주소: %-30s\n\n", userData->phone, userData->name, userData->address);*/
				printUserData(userData, 1);
				foundFlag = 1;
			}
		}
		current = current->Next;
	}

	if (!foundFlag)
	{
		puts("일치하는 주소가 캐시에 없습니다.");
	}
	_getch();
	return foundFlag;
}

// SQL문으로 검색하는 함수
void searchBySQL(void)
{
	char query[200];
	char query1[100], query2[100];
	printf("쿼리 예: name=홍길동 and addr=서울시, phone=12301230 or addr=대전시\n\n");
	printf("쿼리를 입력하세요: ");
	fgets(query, sizeof(query), stdin);
	query[strcspn(query, "\n")] = '\0';

	// 문자열 내 서브 문자열의 수를 카운트
	int and_count = count_and_or(query, " and ");
	int or_count = count_and_or(query, " or ");

	// 쿼리에 and나 or가 없을 경우
	if (and_count == 0 && or_count == 0)
	{
		char field[50], value[200];
		// = 을 기준으로 문자열을 분리
		sscanf_s(query, "%[^=]=%s", field, (unsigned int)sizeof(field), value, (unsigned int)sizeof(value));


		FILE* fp = fopen(FILENAME, "rb");
		if (fp == NULL)
		{
			puts("파일 개방 실패!");
			_getch();
			return;
		}

		USERDATA userData;
		int foundFlag = 0;
		int matchFlag = 0;
		int offset = 0;

		while (fread(&userData, sizeof(USERDATA), 1, fp))
		{
			foundFlag = 0;
			if (strcmp(field, "name") == 0 && strcmp(userData.name, value) == 0)
			{
				foundFlag = 1;
				matchFlag = 1;
			}
			else if (strcmp(field, "phone") == 0 && userData.phone == atoi(value))
			{
				foundFlag = 1;
				matchFlag = 1;
			}
			else if (strcmp(field, "addr") == 0 && strcmp(userData.address, value) == 0)
			{
				foundFlag = 1;
				matchFlag = 1;
			}

			if (foundFlag) {
				/*printf("전화번호: %-15d || 이름: %s \t|| 주소: %-30s || ", userData.phone, userData.name, userData.address);
				puts("파일에서 읽은 데이터입니다.\n");*/
				printUserData(&userData, 2);

				// 일치하는 데이터는 캐시에 추가
				AddNewNode(false, userData.phone, &userData, sizeof(USERDATA), offset);
			}
			offset++;
		}

		if (matchFlag == 0)
		{
			puts("일치하는 항목을 찾지 못했습니다.");
		}
		fclose(fp);
		_getch();
	}

	// 쿼리에 and가 1개 있는 경우
	else if (and_count == 1 && or_count == 0)
	{
		parse_query(query, query1, query2, " and ");
		printf("Query1: %s\n", query1);
		printf("Query2: %s\n\n", query2);

		char field[50], value[200];
		char field2[50], value2[200];

		sscanf_s(query1, "%[^=]=%s", field, (unsigned)sizeof(field), value, (unsigned)sizeof(value));
		sscanf_s(query2, "%[^=]=%s", field2, (unsigned)sizeof(field2), value2, (unsigned)sizeof(value2));

		FILE* fp = fopen(FILENAME, "rb");
		if (fp == NULL)
		{
			puts("파일 개방 실패!");
			_getch();
			return;
		}

		USERDATA userData;
		int foundFlag = 0;
		int offset = 0;

		while (fread(&userData, sizeof(USERDATA), 1, fp))
		{
			int firstCondition = 0;
			int secondCondition = 0;

			// 첫 번째 조건 확인
			if (strcmp(field, "name") == 0 && strcmp(userData.name, value) == 0)
				firstCondition = 1;
			else if (strcmp(field, "phone") == 0 && userData.phone == atoi(value))
				firstCondition = 1;
			else if (strcmp(field, "addr") == 0 && strcmp(userData.address, value) == 0)
				firstCondition = 1;

			// 두 번째 조건 확인
			if (strcmp(field2, "name") == 0 && strcmp(userData.name, value2) == 0)
				secondCondition = 1;
			else if (strcmp(field2, "phone") == 0 && userData.phone == atoi(value2))
				secondCondition = 1;
			else if (strcmp(field2, "addr") == 0 && strcmp(userData.address, value2) == 0)
				secondCondition = 1;

			// 두 조건 모두 만족할 경우 출력
			if (firstCondition && secondCondition)
			{
				/*printf("전화번호: %-15d || 이름: %s \t|| 주소: %-30s || ", userData.phone, userData.name, userData.address);
				puts("파일에서 읽은 데이터입니다.\n");*/
				printUserData(&userData, 2);
				foundFlag = 1; // 일치하는 항목을 찾았음

				// 일치하는 데이터는 캐시에 추가
				AddNewNode(false, userData.phone, &userData, sizeof(USERDATA), offset);
			}
			offset++;
		}

		if (foundFlag == 0)
			puts("일치하는 항목을 찾지 못했습니다.");

		fclose(fp);
		_getch();
	}


	// 쿼리에 or가 1개 있는 경우
	else if (and_count == 0 && or_count == 1)
	{
		parse_query(query, query1, query2, " or ");
		printf("Query1: %s\n", query1);
		printf("Query2: %s\n\n", query2);

		char field[50], value[200];
		char field2[50], value2[200];

		sscanf_s(query1, "%[^=]=%s", field, (unsigned)sizeof(field), value, (unsigned)sizeof(value));
		sscanf_s(query2, "%[^=]=%s", field2, (unsigned)sizeof(field2), value2, (unsigned)sizeof(value2));

		FILE* fp = fopen(FILENAME, "rb");
		if (fp == NULL)
		{
			puts("파일 개방 실패!");
			_getch();
			return;
		}

		USERDATA userData;
		int foundFlag = 0;
		int matchFlag = 0;
		int offset = 0;

		while (fread(&userData, sizeof(USERDATA), 1, fp))
		{
			foundFlag = 0;

			// 첫 번째 조건 확인
			if (strcmp(field, "name") == 0 && strcmp(userData.name, value) == 0)
			{
				foundFlag = 1;
				matchFlag = 1;
			}
			else if (strcmp(field, "phone") == 0 && userData.phone == atoi(value))
			{
				foundFlag = 1;
				matchFlag = 1;
			}
			else if (strcmp(field, "addr") == 0 && strcmp(userData.address, value) == 0)
			{
				foundFlag = 1;
				matchFlag = 1;
			}

			// 첫 번째 조건이 맞으면 데이터 출력
			if (foundFlag)
			{
				/*printf("전화번호: %-15d || 이름: %s \t|| 주소: %-30s || ", userData.phone, userData.name, userData.address);
				puts("파일에서 읽은 데이터입니다.\n");*/
				printUserData(&userData, 2);

				// 일치하는 데이터는 캐시에 추가
				AddNewNode(false, userData.phone, &userData, sizeof(USERDATA), offset);
			}

			foundFlag = 0;  // 두 번째 조건을 위해 다시 초기화

			// 두 번째 조건 확인
			if (strcmp(field2, "name") == 0 && strcmp(userData.name, value2) == 0)
			{
				foundFlag = 1;
				matchFlag = 1;
			}
			else if (strcmp(field2, "phone") == 0 && userData.phone == atoi(value2))
			{
				foundFlag = 1;
				matchFlag = 1;
			}
			else if (strcmp(field2, "addr") == 0 && strcmp(userData.address, value2) == 0)
			{
				foundFlag = 1;
				matchFlag = 1;
			}

			// 두 번째 조건이 맞으면 데이터 출력
			if (foundFlag)
			{
				/*printf("전화번호: %-15d || 이름: %s \t|| 주소: %-30s || ", userData.phone, userData.name, userData.address);
				puts("파일에서 읽은 데이터입니다.\n");*/
				printUserData(&userData, 2);

				// 일치하는 데이터는 캐시에 추가
				AddNewNode(false, userData.phone, &userData, sizeof(USERDATA), offset);
			}
			offset++;
		}

		if (matchFlag == 0)
			puts("일치하는 항목을 찾지 못했습니다.");

		fclose(fp);
		_getch();
	}

	// 쿼리에 and나 or가 1개 이상일 경우
	else
	{
		puts("아직 지원하지 않는 명령어 입니다.");
		_getch();
	}

}

// 쿼리 파싱 함수
void parse_query(const char* query, char* query1, char* query2, char* and_or)
{
	int pos = find_substring_position(query, and_or);
	if (pos != -1)
	{
		strncpy(query1, query, pos);
		query1[pos] = '\0';
		strcpy(query2, query + pos + strlen(and_or));
	}
}

// 서브 문자열의 위치를 검색하는 함수
int find_substring_position(const char* str, const char* sub)
{
	const char* pos = strstr(str, sub);

	if (pos != NULL)
		return (int)(pos - str);
	else
		return -1;
}

// 문자열 내 서브 문자열의 수를 카운트하는 함수
int count_and_or(const char* str, const char* word)
{
	int count = 0;
	const char* pos = str;

	while ((pos = strstr(pos, word)) != NULL)
	{
		count++;
		pos += strlen(word);
	}
	return count;
}