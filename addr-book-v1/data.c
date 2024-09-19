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
		puts("�޸� �Ҵ� ����\n");
		exit(1);
	}

	return ptr;
}

void initializeIndex(const char* filename)
{
	printf("�ε����� �ʱ�ȭ ���Դϴ�.\n");
	USERDATA userData;

	FILE* fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		puts("���� ���� ����!");
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
	printf("[1] Commit [2] ���߿�: ");
	int input;
	if (scanf_s("%d%*c", &input) != 1)
	{
		puts("�߸��� �Է��Դϴ�.");
		return;
	}

	if (input == 1)
	{
		AddNewNode(true, userData->phone, userData, sizeof(USERDATA), g_endOffset);
		puts("Commit! ���Ͽ� �����մϴ�.");
		FILE* fp = fopen(filename, "ab");

		if (fp == NULL)
		{
			puts("addData() ���� ���� ���� ����!");
			return;
		}
		fwrite(userData, sizeof(USERDATA), 1, fp);
		phoneIndex[userData->phone] = g_endOffset;
		g_endOffset++;
		fclose(fp);
	}
	else if (input == 2)
	{
		puts("���Ͽ� �������� �ʾҽ��ϴ�. ���θ޸𸮿��� ����Ǿ��ֽ��ϴ�.");
		AddNewNode(true, userData->phone, userData, sizeof(USERDATA), -1);
		//g_endOffset++;
	}
	else
	{
		puts("�߸��� �Է��Դϴ�.");
	}
}

void AddNewNode(bool bNew, int key,	const void* dataCache, unsigned int sizeOfData,	unsigned int offset)
{	
	NODE* newNode = (NODE*)calloc(1, sizeof(NODE));
	if (newNode == NULL)
	{
		puts("�޸� �Ҵ� ����!");
		return;
	}

	// NODE �ν��Ͻ��� �ʵ� key�� �� �Ҵ�
	//int lenOfKey = (int)strlen(key);
	//newNode->key = malloc(lenOfKey + 1);
	//strcpy_s(newNode->key, lenOfKey + 1, key);
	newNode->key = key;


	// NODE �ν��Ͻ��� �ʵ� dataCache
	// sizeOfData�� �� �Ҵ�
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
		puts("���� ���� ����!");
		return;
	}

	while (fread(&userData, sizeof(USERDATA), 1, fp))
	{
		//printf("%d\t%s\t%s\n", userData.phone, userData.name, userData.address);
		printf("��ȭ��ȣ: %-15d || �̸�: %s \t|| �ּ�: %-30s\n\n", userData.phone, userData.name, userData.address);
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
			printf("��ȭ��ȣ: %-15d || �̸�: %s \t|| �ּ�: %-30s || ", userData->phone, userData->name, userData->address);
			if (current->offset != -1)
			{
				if (current->bNew == true)
					printf("Commit: % -10s || key: % -10d || offset: % -10d || ������ ������!\n\n", "Yes", current->key, current->offset);
				else
					printf("Commit: % -10s || key: % -10d || offset: % -10d || �˻��� ������!\n\n", "Yes", current->key, current->offset);
			}
			else
			{
				if (current->bNew == true)
					printf("Commit: % -10s || key: %- 10d || offset: % -10d || ������ ������!!\n\n", "Not yet", current->key, current->offset);
				else
					printf("Commit: % -10s || key: %- 10d || offset: % -10d || �˻��� ������!!\n\n", "Not yet", current->key, current->offset);
			}
		}
		current = current->Next;
	}
}

// Ŀ������ ���� ������ �����ϴ� �Լ�
void commitData(void)
{
	NODE* current = g_HeadNode.Next;
	FILE* fp = fopen(FILENAME, "ab");

	if (fp == NULL)
	{
		puts("���� ���� ����!");
		return;
	}

	while (current != &g_TailNode)
	{
		if (current->offset == -1 && current->dataCache != NULL)
		{
			USERDATA* userData = (USERDATA*)current->dataCache;

			puts("Commit! ���Ͽ� ������ �����մϴ�.");
			
			fwrite(userData, sizeof(USERDATA), 1, fp);
			
			phoneIndex[userData->phone] = g_endOffset;
			current->offset = g_endOffset;
			g_endOffset++;			
		}
		current = current->Next;
	}
	fclose(fp);
	puts("Commit! �Ϸ�Ǿ����ϴ�.");
	_getch();
}

// Ŀ�� ����ϴ� �Լ�
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
	puts("Ŀ�� ��Ұ� �Ϸ�Ǿ����ϴ�.");
	_getch();
}

// ��ȭ��ȣ�� �˻��ϴ� �Լ�
void searchByPhone() 
{
	int phone;
	printf("�˻��� ��ȭ��ȣ�� 010�� -�� �����ϰ� �Է��ϼ���: ");
	if (scanf_s("%d%*c", &phone) != 1)
	{
		puts("�߸��� �Է��Դϴ�.");
		_getch();
		return;
	}

	// phoneIndex���� ��ȭ��ȣ�� ���� �������� ã��
	int offset = phoneIndex[phone];

	// �������� ��ȿ���� Ȯ�� (�ʱ�ȭ���� �ʾ��� ���� -1 ���)
	if (offset == -1)
	{
		puts("��ġ�ϴ� ��ȭ��ȣ�� �����ϴ�.");
		_getch();
		return;
	}

	// ���Ͽ��� �������� ������� USERDATA �б�
	FILE* fp = fopen(FILENAME, "rb+");
	if (fp == NULL)
	{
		puts("���� ���� ����");
		return;
	}

	// �ش� �������� ��ġ�� �̵�
	if (fseek(fp, offset * sizeof(USERDATA), SEEK_SET) != 0)
	{
		puts("���� ��ġ �̵� ����!");
		fclose(fp);
		return;
	}

	// ������ �б�
	USERDATA userData;
	if (fread(&userData, sizeof(USERDATA), 1, fp) == 1)
	{
		// ������ ���
		printf("��ȭ��ȣ: %-15d || �̸�: %s \t|| �ּ�: %-30s\n", userData.phone, userData.name, userData.address);

		// ���� ���� Ȯ��
		printf("�����͸� �����Ͻðڽ��ϱ�? [1] �� [2] �ƴϿ�: ");
		int modifyInput;
		if (scanf_s("%d%*c", &modifyInput) != 1 || modifyInput != 1)
		{
			puts("������ ����߽��ϴ�.");
			fclose(fp);
			return;
		}

		// �̸� �� �ּ� ����
		printf("���ο� �̸��� �Է��ϼ���: ");
		fgets(userData.name, sizeof(userData.name), stdin);
		userData.name[strcspn(userData.name, "\n")] = '\0';

		printf("���ο� �ּҸ� �Է��ϼ���: ");
		fgets(userData.address, sizeof(userData.address), stdin);
		userData.address[strcspn(userData.address, "\n")] = '\0';

		// ���� ���������� �̵�
		if (fseek(fp, offset * sizeof(USERDATA), SEEK_SET) != 0)
		{
			puts("���� ��ġ �̵� ����!");
			fclose(fp);
			_getch();
			return;
		}

		// ������ ������ �����
		if (fwrite(&userData, sizeof(USERDATA), 1, fp) != 1)
		{
			puts("������ ���� ����!");
		}
		else
		{
			puts("�����Ͱ� ���������� �����Ǿ����ϴ�.");
		}
	}
	else
	{
		puts("������ �б� ����!");
	}
	fclose(fp);
	_getch();
}

// �̸����� �˻��ϴ� �Լ�
void searchByName(void)
{
	char name[12];
	printf("�˻��� �̸��� �Է��ϼ���: ");
	fgets(name, sizeof(name), stdin);
	name[strcspn(name, "\n")] = '\0';

	FILE* fp = fopen(FILENAME, "rb");
	if (fp == NULL)
	{
		puts("���� ���� ����!");
		return;
	}

	USERDATA userData;
	int foundFlag = 0;
	int offset = 0;
	while (fread(&userData, sizeof(USERDATA), 1, fp))
	{
		if (strcmp(userData.name, name) == 0)
		{
			printf("��ȭ��ȣ: %-15d || �̸�: %s \t|| �ּ�: %-30s || ", userData.phone, userData.name, userData.address);
			puts("���Ͽ��� ���� �������Դϴ�.\n");

			// ��ġ�ϴ� �����ʹ� ĳ�ÿ� �߰�
			AddNewNode(false, userData.phone, &userData, sizeof(USERDATA), offset);
			foundFlag = 1;
		}
		offset++;
	}
	fclose(fp);

	if (!foundFlag)
	{
		puts("��ġ�ϴ� �̸��� �����ϴ�.");
	}
	_getch();
}

void searchByAddr(void)
{
	char addr[100];
	printf("�˻��� �̸��� �Է��ϼ���: ");
	fgets(addr, sizeof(addr), stdin);
	addr[strcspn(addr, "\n")] = '\0';

	FILE* fp = fopen(FILENAME, "rb");
	if (fp == NULL)
	{
		puts("���� ���� ����!");
		return;
	}

	USERDATA userData;
	int foundFlag = 0;
	int offset = 0;
	while (fread(&userData, sizeof(USERDATA), 1, fp))
	{
		if (strcmp(userData.address, addr) == 0)
		{
			printf("��ȭ��ȣ: %-15d || �̸�: %s \t|| �ּ�: %-30s || ", userData.phone, userData.name, userData.address);
			puts("���Ͽ��� ���� �������Դϴ�.\n");

			// ��ġ�ϴ� �����ʹ� ĳ�ÿ� �߰�
			AddNewNode(false, userData.phone, &userData, sizeof(USERDATA), offset);
			foundFlag = 1;
		}
		offset++;
	}
	fclose(fp);

	if (!foundFlag)
	{
		puts("��ġ�ϴ� �̸��� �����ϴ�.");
	}
	_getch();
}

// �̸����� ĳ�ÿ��� �˻��ϴ� �Լ�
int searchByNameFromCache()
{
	char name[12];
	printf("ĳ�ÿ��� �˻��� �̸��� �Է��ϼ���: ");
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
				printf("��ȭ��ȣ: %-15d || �̸�: %s \t|| �ּ�: %-30s\n\n", userData->phone, userData->name, userData->address);
				foundFlag = 1;
			}
		}
		current = current->Next;
	}

	if (!foundFlag)
	{
		puts("��ġ�ϴ� �̸��� ĳ�ÿ� �����ϴ�.");
	}
	_getch();
	return foundFlag;
}

// �ּҷ� ĳ�ÿ��� �˻��ϴ� �Լ�
int searchByAddrFromCache()
{
	char addr[100];
	printf("ĳ�ÿ��� �˻��� �ּҸ� �Է��ϼ���: ");
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
				printf("��ȭ��ȣ: %-15d || �̸�: %s \t|| �ּ�: %-30s\n\n", userData->phone, userData->name, userData->address);
				foundFlag = 1;
			}
		}
		current = current->Next;
	}

	if (!foundFlag)
	{
		puts("��ġ�ϴ� �ּҰ� ĳ�ÿ� �����ϴ�.");
	}
	_getch();
	return foundFlag;
}

// SQL������ �˻��ϴ� �Լ�
void searchBySQL(void)
{
	char query[200];
	char query1[100], query2[100];
	printf("���� ��: name=ȫ�浿 and addr=�����, phone=1230-1230 or addr=������\n\n");
	printf("������ �Է��ϼ���: ");
	fgets(query, sizeof(query), stdin);
	query[strcspn(query, "\n")] = '\0';

	// ���ڿ� �� ���� ���ڿ��� ���� ī��Ʈ
	int and_count = count_and_or(query, " and ");
	int or_count = count_and_or(query, " or ");

	// ������ and�� or�� ���� ���
	if (and_count == 0 && or_count == 0)
	{
		char field[50], value[200];
		// = �� �������� ���ڿ��� �и�
		sscanf(query, "%[^=]=%s", field, value);

		FILE* fp = fopen(FILENAME, "rb");
		if (fp == NULL)
		{
			puts("���� ���� ����!");
			_getch();
			return;
		}

		USERDATA userData;
		int foundFlag = 0;

		while (fread(&userData, sizeof(USERDATA), 1, fp))
		{
			if (strcmp(field, "name") == 0 && strcmp(userData.name, value) == 0)
				foundFlag = 1;
			else if (strcmp(field, "phone") == 0 && userData.phone == atoi(value))
				foundFlag = 1;
			else if (strcmp(field, "addr") == 0 && strcmp(userData.address, value) == 0)
				foundFlag = 1;

			if (foundFlag) {
				printf("��ȭ��ȣ: %-15d || �̸�: %s \t|| �ּ�: %-30s || ", userData.phone, userData.name, userData.address);
				puts("���Ͽ��� ���� �������Դϴ�.\n");
			}
		}

		if (foundFlag == 0)
		{
			puts("��ġ�ϴ� �׸��� ã�� ���߽��ϴ�.");
		}
		fclose(fp);
		_getch();
	}
}

// ���ڿ� �� ���� ���ڿ��� ���� ī��Ʈ
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