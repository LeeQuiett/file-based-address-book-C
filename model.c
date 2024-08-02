#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

typedef struct Node {
	Addr data;
	struct Node* next;
} Node;

Node* head = NULL;

void add_addr(const char* filename, Addr* addr) {
	FILE* fp = fopen(filename, "ab");
	if (fp == NULL) {
		perror("Fail to open file for add");
		return;
	}
	fwrite(addr, sizeof(Addr), 1, fp);
	fclose(fp);
}

Node* load_data_from_file(const char* filename) {
	FILE* fp = fopen(filename, "rb");
	if (fp == NULL) {
		perror("Fail to open file for load");
		return NULL;
	}
	Node* head = NULL;
	Node* tail = NULL;
	Addr addr;
	while (fread(&addr, sizeof(Addr), 1, fp)) {
		Node* newNode = (Node*)malloc(sizeof(Node));
		if (!newNode) {
			perror("Memory alloc failed");
			fclose(fp);
			return NULL;
		}
		newNode->data = addr;
		newNode->next = NULL;

		if (head == NULL) {
			head = newNode;
			tail = head;
		} 
		else {
			tail->next = newNode;
			tail = newNode;
		}
	}
	fclose(fp);
	return head;
}