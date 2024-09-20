#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "ui.h"
#include "data.h"
#include "main.h"

MENU printMenu(void)
{
	MENU input = 0;

	system("cls");
	printf("[1] 추가\t[2] 검색\t[3] 출력\t[4] 제거\t[5] 커밋\t[0] 나가기\n");
	scanf_s("%d%*c", &input);
	//while (getchar() != '\n');

	return input;
}

void eventLoopRun(void)
{
	MENU menu = 0;
	
	while ((menu = printMenu()) != 0)
	{
		switch (menu)
		{
		case NEW:
			addDataUi();
			break;
		case SEARCH:
			searchDataUi();
			break;
		case PRINT:
			printDataUi();
			break;
		case REMOVE:
			deleteDataUi();
			break;
		case COMMIT:
			commitDataUi();
			break;
		default:
			puts("잘못된 입력입니다. 다시 입력해 주세요.");
			_getch();
			break;
		}
	}
	puts("프로그램을 종료합니다.");
}



void deleteDataUi()
{
	puts("삭제하려면 PK값인 전화번호를 입력해야합니다!");
	printf("[1] 들어가기 [2] 돌아가기: ");

	int input;
	if (scanf_s("%d%*c", &input) != 1)
	{
		puts("잘못된 입력입니다.");
		_getch();
		return;
	}

	if (input == 1)
	{
		deleteByPhone();
	}
	else
	{
		return;
	}
}

void searchDataUi(void)
{
	puts("전화번호로 검색 시 O(1)의 시간복잡도로 검색이 가능합니다!");
	printf("[1] 이름으로 검색 [2] 전화번호로 검색 [3] 주소로 검색 [4] SQL로 검색 [5] 돌아가기: ");
	int input;
	if (scanf_s("%d%*c", &input) != 1)
	{
		puts("잘못된 입력입니다.");
		_getch();
		return;
	}

	if (input == 1)
	{
		// 캐시에서 검색 시도
		if (searchByNameFromCache() == 1)
		{
			return;
		}
		else
		{
			// 캐시에서 검색 실패 시 파일에서 검색 여부 묻기
			printf("캐시에서 찾지 못했습니다. 파일에서 검색하시겠습니까? [1] 예 [2] 아니오: ");
			int fileSearchInput;
			if (scanf_s("%d%*c", &fileSearchInput) != 1 || fileSearchInput != 1)
			{
				puts("파일에서 검색을 취소했습니다.");
				_getch();
				return;
			}

			// 파일에서 검색
			searchByName();
		}
		return;
	}
	else if (input == 2)
	{
		puts("O(1)의 시간복잡도를 갖는 전화번호 검색을 선택하셨군요^~^");
		searchByPhone();
	}

	else if (input == 3)
	{
		// 캐시에서 검색 시도
		if (searchByAddrFromCache() == 1)
		{
			return;
		}
		else
		{
			// 캐시에서 검색 실패 시 파일에서 검색 여부 묻기
			printf("캐시에서 찾지 못했습니다. 파일에서 검색하시겠습니까? [1] 예 [2] 아니오: ");
			int fileSearchInput;
			if (scanf_s("%d%*c", &fileSearchInput) != 1 || fileSearchInput != 1)
			{
				puts("파일에서 검색을 취소했습니다.");
				_getch();
				return;
			}

			// 파일에서 검색
			searchByAddr();
		}
	}
	else if (input == 4)
	{
		searchBySQL();		
	}

	else
	{
		return;
	}
}

void printUserData(USERDATA* userData, int formatSelect)
{
	switch (formatSelect)
	{
	case 1:
		printf("전화번호: %-15d || 이름: %s \t|| 주소: %-30s\n\n", userData->phone, userData->name, userData->address);
		break;
	case 2:
		printf("전화번호: %-15d || 이름: %s \t|| 주소: %-30s || ", userData->phone, userData->name, userData->address);
		puts("파일에서 읽은 데이터입니다.\n");
		break;
	case 3:
		printf("전화번호: %-15d || 이름: %s \t|| 주소: %-30s || ", userData->phone, userData->name, userData->address);
		break;
	default:
		break;
	}
}

void commitDataUi(void)
{
	printf("[1] 커밋하기 [2] 커밋하지 않은 내용 취소하기 [3] 돌아가기: ");
	int input;
	if (scanf_s("%d%*c", &input) != 1)
	{
		puts("잘못된 입력입니다.");
		_getch();
		return;
	}

	if (input == 1)
	{
		commitData();
	}
	else if (input == 2)
	{
		commitCancel();
	}
	else if (input == 3)
	{
		return;
	}
	else
	{
		puts("잘못된 입력입니다.");
		_getch();
		return;
	}
}

void addDataUi()
{
	printf("[1] 추가하기 [2] 돌아가기: ");
	int input;
	scanf_s("%d%*c", &input);
	//while (getchar() != '\n');
	
	if (input == 1)
	{
		USERDATA userData;

		// 이름 입력
		printf("이름을 입력하세요: ");
		fgets(userData.name, sizeof(userData.name), stdin);
		userData.name[strcspn(userData.name, "\n")] = '\0';

		// 전화번호 입력
		while (1)
		{
			printf("\"010\"과 \"-\"를 제외하고 전화번호를 입력하세요: ");

			if (scanf_s("%d%*c", &userData.phone) != 1)
			{
				// 예외 처리
				//while (getchar() != '\n');
				puts("잘못된 입력입니다. 다시 입력해주세요.");
				_getch();
				continue;
			}

			if (userData.phone < 0 || userData.phone > 99999999)
			{				
				puts("전화번호 범위를 초과했습니다. 다시 입력해주세요.");
				_getch();
				continue;
			}

			if (phoneIndex[userData.phone] != -1)
			{
				puts("중복된 전화번호가 있습니다. 다시 입력해 주세요.");
				continue;
			}

			break;
		}

		// 주소 입력
		printf("주소를 입력하세요: ");
		fgets(userData.address, sizeof(userData.address), stdin);
		userData.address[strcspn(userData.address, "\n")] = '\0';

		// 아래에 파일에 쓰기 작성
		addData(FILENAME, &userData);
		//puts("성공적으로 저장되었습니다.");
		_getch();
	}
	else if (input == 2)
	{
		system("cls"); return;
	}
	else
	{
		//system("cls"); 
		puts("잘못된 입력입니다.");
		_getch(); return;
	}
		
}

void printDataUi()
{
	int input;
	printf("[1] 전체 리스트 출력 [2] Cache된 리스트 출력 [3] 돌아가기: ");
	scanf_s("%d%*c", &input);

	switch (input)
	{
	case 1:
		puts("전체 리스트를 출력합니다.");

		_getch();

		printData(FILENAME);

		puts("출력이 완료되었습니다.");
		
		_getch();

		break;
	case 2:
		puts("Cache된 리스트를 출력합니다.");

		_getch();
		
		printCache();
		
		puts("Cache된 리스트 출력 완료.");
		
		_getch();

		break;
	case 3:
		break;
	default:
		puts("잘못된 입력입니다.");
		
		_getch();

		break;
	}
}