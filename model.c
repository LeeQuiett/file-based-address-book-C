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
		perror("Fail to file open!");
		return;
	}
	fwrite(addr, sizeof(Addr), 1, fp);
	fclose(fp);
}

void delete_addr_from_name(const char* filename, const char* name) {

}

Node* load_data_from_file(const char* filename) {

}