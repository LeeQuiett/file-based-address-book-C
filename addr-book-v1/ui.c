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
	printf("[1] �߰�\t[2] �˻�\t[3] ���\t[4] ����\t[5] Ŀ��\t[0] ������\n");
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
			puts("�߸��� �Է��Դϴ�. �ٽ� �Է��� �ּ���.");
			_getch();
			break;
		}
	}
	puts("���α׷��� �����մϴ�.");
}



void deleteDataUi()
{
	puts("�����Ϸ��� PK���� ��ȭ��ȣ�� �Է��ؾ��մϴ�!");
	printf("[1] ���� [2] ���ư���: ");

	int input;
	if (scanf_s("%d%*c", &input) != 1)
	{
		puts("�߸��� �Է��Դϴ�.");
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
	puts("��ȭ��ȣ�� �˻� �� O(1)�� �ð����⵵�� �˻��� �����մϴ�!");
	printf("[1] �̸����� �˻� [2] ��ȭ��ȣ�� �˻� [3] �ּҷ� �˻� [4] SQL�� �˻� [5] ���ư���: ");
	int input;
	if (scanf_s("%d%*c", &input) != 1)
	{
		puts("�߸��� �Է��Դϴ�.");
		_getch();
		return;
	}

	if (input == 1)
	{
		// ĳ�ÿ��� �˻� �õ�
		if (searchByNameFromCache() == 1)
		{
			return;
		}
		else
		{
			// ĳ�ÿ��� �˻� ���� �� ���Ͽ��� �˻� ���� ����
			printf("ĳ�ÿ��� ã�� ���߽��ϴ�. ���Ͽ��� �˻��Ͻðڽ��ϱ�? [1] �� [2] �ƴϿ�: ");
			int fileSearchInput;
			if (scanf_s("%d%*c", &fileSearchInput) != 1 || fileSearchInput != 1)
			{
				puts("���Ͽ��� �˻��� ����߽��ϴ�.");
				_getch();
				return;
			}

			// ���Ͽ��� �˻�
			searchByName();
		}
		return;
	}
	else if (input == 2)
	{
		puts("O(1)�� �ð����⵵�� ���� ��ȭ��ȣ �˻��� �����ϼ̱���^~^");
		searchByPhone();
	}

	else if (input == 3)
	{
		// ĳ�ÿ��� �˻� �õ�
		if (searchByAddrFromCache() == 1)
		{
			return;
		}
		else
		{
			// ĳ�ÿ��� �˻� ���� �� ���Ͽ��� �˻� ���� ����
			printf("ĳ�ÿ��� ã�� ���߽��ϴ�. ���Ͽ��� �˻��Ͻðڽ��ϱ�? [1] �� [2] �ƴϿ�: ");
			int fileSearchInput;
			if (scanf_s("%d%*c", &fileSearchInput) != 1 || fileSearchInput != 1)
			{
				puts("���Ͽ��� �˻��� ����߽��ϴ�.");
				_getch();
				return;
			}

			// ���Ͽ��� �˻�
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
		printf("��ȭ��ȣ: %-15d || �̸�: %s \t|| �ּ�: %-30s\n\n", userData->phone, userData->name, userData->address);
		break;
	case 2:
		printf("��ȭ��ȣ: %-15d || �̸�: %s \t|| �ּ�: %-30s || ", userData->phone, userData->name, userData->address);
		puts("���Ͽ��� ���� �������Դϴ�.\n");
		break;
	case 3:
		printf("��ȭ��ȣ: %-15d || �̸�: %s \t|| �ּ�: %-30s || ", userData->phone, userData->name, userData->address);
		break;
	default:
		break;
	}
}

void commitDataUi(void)
{
	printf("[1] Ŀ���ϱ� [2] Ŀ������ ���� ���� ����ϱ� [3] ���ư���: ");
	int input;
	if (scanf_s("%d%*c", &input) != 1)
	{
		puts("�߸��� �Է��Դϴ�.");
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
		puts("�߸��� �Է��Դϴ�.");
		_getch();
		return;
	}
}

void addDataUi()
{
	printf("[1] �߰��ϱ� [2] ���ư���: ");
	int input;
	scanf_s("%d%*c", &input);
	//while (getchar() != '\n');
	
	if (input == 1)
	{
		USERDATA userData;

		// �̸� �Է�
		printf("�̸��� �Է��ϼ���: ");
		fgets(userData.name, sizeof(userData.name), stdin);
		userData.name[strcspn(userData.name, "\n")] = '\0';

		// ��ȭ��ȣ �Է�
		while (1)
		{
			printf("\"010\"�� \"-\"�� �����ϰ� ��ȭ��ȣ�� �Է��ϼ���: ");

			if (scanf_s("%d%*c", &userData.phone) != 1)
			{
				// ���� ó��
				//while (getchar() != '\n');
				puts("�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���.");
				_getch();
				continue;
			}

			if (userData.phone < 0 || userData.phone > 99999999)
			{				
				puts("��ȭ��ȣ ������ �ʰ��߽��ϴ�. �ٽ� �Է����ּ���.");
				_getch();
				continue;
			}

			if (phoneIndex[userData.phone] != -1)
			{
				puts("�ߺ��� ��ȭ��ȣ�� �ֽ��ϴ�. �ٽ� �Է��� �ּ���.");
				continue;
			}

			break;
		}

		// �ּ� �Է�
		printf("�ּҸ� �Է��ϼ���: ");
		fgets(userData.address, sizeof(userData.address), stdin);
		userData.address[strcspn(userData.address, "\n")] = '\0';

		// �Ʒ��� ���Ͽ� ���� �ۼ�
		addData(FILENAME, &userData);
		//puts("���������� ����Ǿ����ϴ�.");
		_getch();
	}
	else if (input == 2)
	{
		system("cls"); return;
	}
	else
	{
		//system("cls"); 
		puts("�߸��� �Է��Դϴ�.");
		_getch(); return;
	}
		
}

void printDataUi()
{
	int input;
	printf("[1] ��ü ����Ʈ ��� [2] Cache�� ����Ʈ ��� [3] ���ư���: ");
	scanf_s("%d%*c", &input);

	switch (input)
	{
	case 1:
		puts("��ü ����Ʈ�� ����մϴ�.");

		_getch();

		printData(FILENAME);

		puts("����� �Ϸ�Ǿ����ϴ�.");
		
		_getch();

		break;
	case 2:
		puts("Cache�� ����Ʈ�� ����մϴ�.");

		_getch();
		
		printCache();
		
		puts("Cache�� ����Ʈ ��� �Ϸ�.");
		
		_getch();

		break;
	case 3:
		break;
	default:
		puts("�߸��� �Է��Դϴ�.");
		
		_getch();

		break;
	}
}