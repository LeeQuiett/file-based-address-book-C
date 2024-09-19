#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

int g_offset = 0;

typedef struct USERDATA
{
	int phone;
	char name[12];
	char address[100];
	//char phone[12];
} USERDATA;

typedef enum MENU
{
	EXIT, NEW, SEARCH, PRINT, REMOVE
} MENU;

MENU printMenu(void)
{
	MENU input = 0;

	system("cls");
	printf("[1] 추가\t[2] 검색\t[3] 출력\t[4] 제거\t[0] 나가기\n");
	scanf_s("%d%*c", &input);

	return input;
}

int* allocateMemoryForPhone(size_t size)
{
	int* ptr = malloc(size);

	if (ptr == NULL)
	{
		puts("메모리 할당 실패\n");
		exit(1);
	}

	return ptr;
}

void eventLoopRun(void)
{
	MENU menu = 0;

	while ((menu = printMenu()) != 0)
	{
		switch (menu)
		{
		case NEW:
			
			break;

		case SEARCH:
			
			break;

		case PRINT:

			break;

		case REMOVE:
			break;

		default:
			puts("잘못된 입력입니다. 다시 입력해 주세요.");
			_getch();
			break;
		}
	}

	puts("big shout-out to metsakuur! By2~!");
}

void nodeDataCopy(USERDATA* left, USERDATA* right)
{
	//left->age = right->age;
	//strcpy_s(left->phone, sizeof(left->phone), right->phone);
	strcpy_s(left->name, sizeof(left->name), right->name);
	strcpy_s(left->address, sizeof(left->address), right->address);
	left->phone = right->phone;
}

void swapNode(USERDATA* left, USERDATA* right)
{
	USERDATA tmp = *left;
	nodeDataCopy(left, right);
	nodeDataCopy(right, &tmp);
}

int main(void)
{
	// 전화번호를 인덱스로 해당 번지에 파일 offset을 저장할 배열, 일종의 look-up 테이블?
	int* phone = allocateMemoryForPhone(sizeof(int) * 100000000);

	eventLoopRun();

	free(phone);
}