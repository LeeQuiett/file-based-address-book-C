#pragma once
#include <stdbool.h>
#include <stdint.h>
//#define FILENAME "addr_book_10M.dat"
//#define FILENAME "Tree_test_addr_book.dat"
#define FILENAME "test_3_dummy.dat"
#define TEMP_FILENAME "temp.dat"

typedef struct USERDATA
{
	int phone;
	char name[12];
	char address[100];
	//char phone[12];
} USERDATA;

typedef struct NODE
{
	bool bNew;
	int key;
	void* dataCache;
	unsigned int sizeOfData;
	unsigned int offset;

	struct NODE* Next;
	struct NODE* Prev;
} NODE;

NODE g_HeadNode;
NODE g_TailNode;

void addData(const char* filename, USERDATA* userData);
void AddNewNode(bool bNew, int key,	const void* dataCache, unsigned int sizeOfData,	unsigned int offset);
void initializeList(void);
int* allocateMemoryForPhone(size_t size);
void initializeIndex(const char* filename);
void printData(const char* filename);
void printCache(void);
void commitData(void);
void commitCancel(void);
void searchByName(void);
void searchByPhone(void);
void searchByAddr(void);
void searchBySQL(void);
int searchByNameFromCache();
int searchByAddrFromCache();
int count_and_or(const char* str, const char* word);
void parse_query(const char* query, char* query1, char* query2, char* and_or);
int find_substring_position(const char* str, const char* sub);
void deleteList(void);
void deleteByPhone(void);
void defragmentData();