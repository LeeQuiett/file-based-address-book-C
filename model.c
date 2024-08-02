#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

//Node* head = NULL; 메모리에 로드X

void add_addr(const char* filename, Addr* addr) {
	FILE* fp = fopen(filename, "ab");
	if (fp == NULL) {
		perror("Fail to open file for add");
		return;
	}
	fwrite(addr, sizeof(Addr), 1, fp);
	fclose(fp);
}


/*search_addr_controller*/
void search_by_name(void) {
	char name[30];
	print_message("Enter the name to search: ");
	fgets(name, sizeof(name), stdin);
	name[strcspn(name, "\n")] = '\0';

	FILE* fp = fopen(FILENAME, "rb");
	if (fp == NULL) {
		perror("Failed to open file for reading");
		return;
	}

	Addr addr;
	int found = 0;
	while (fread(&addr, sizeof(Addr), 1, fp)) {
		if (strcmp(addr.name, name) == 0) {
			print_message("Contact found:\n");
			printf("Name: %s\nTel: %s\nAddr: %s\n\n", addr.name, addr.tel, addr.addr);
			found = 1;			
		}
	}

	if (!found) {
		print_message("Contact not found.\n");
	}

	fclose(fp);
}
void search_by_tel(void) {
	char tel[30];
	print_message("Enter the tel to search: ");
	fgets(tel, sizeof(tel), stdin);
	tel[strcspn(tel, "\n")] = '\0';

	FILE* fp = fopen(FILENAME, "rb");
	if (fp == NULL) {
		perror("Failed to open file for reading");
		return;
	}

	Addr addr;
	int found = 0;
	while (fread(&addr, sizeof(Addr), 1, fp)) {
		if (strcmp(addr.tel, tel) == 0) {
			print_message("Contact found:\n");
			printf("Name: %s\nTel: %s\nAddr: %s\n\n", addr.name, addr.tel, addr.addr);
			found = 1;			
		}
	}
	if (!found) {
		print_message("Contact not found.\n");
	}
	fclose(fp);

}
void search_by_addr(void) {
	char addr[100];
	print_message("Enter the address to search: ");
	fgets(addr, sizeof(addr), stdin);
	addr[strcspn(addr, "\n")] = '\0';  // Remove newline character

	FILE* fp = fopen(FILENAME, "rb");
	if (fp == NULL) {
		perror("Failed to open file for reading");
		return;
	}

	Addr a;
	int found = 0;
	while (fread(&a, sizeof(Addr), 1, fp)) {
		if (strcmp(a.addr, addr) == 0) {
			print_message("Contact found:\n");
			printf("Name: %s\nTel: %s\nAddr: %s\n", a.name, a.tel, a.addr);
			found = 1;
		}
	}

	if (!found) {
		print_message("Contact not found.\n");
	}

	fclose(fp);
}
void search_by_SQL(void) {
	char query[256];
	print_message("Enter your search query: ");
	fgets(query, sizeof(query), stdin);
	query[strcspn(query, "\n")] = '\0';

	//쿼리가 'field=value' 형태라고 가정
	char field[50], value[200];
	sscanf(query, "%[^=]=%s", field, value);

	FILE* fp = fopen(FILENAME, "rb");
	if (fp == NULL) {
		perror("Failed to open file for reading");
		return;
	}

	Addr addr;
	int found = 0;
	while (fread(&addr, sizeof(Addr), 1, fp)) {
		int match = 0;
		if (strcmp(field, "name") == 0 && strcmp(addr.name, value) == 0) {
			match = 1;
		}
		else if (strcmp(field, "tel") == 0 && strcmp(addr.tel, value) == 0) {
			match = 1;
		}
		else if (strcmp(field, "addr") == 0 && strcmp(addr.addr, value) == 0) {
			match = 1;
		}

		if (match) {
			print_message("Contact found:\n");
			printf("Name: %s\nTel: %s\nAddr: %s\n\n", addr.name, addr.tel, addr.addr);
			found = 1;
		}
	}

	if (!found) {
		print_message("Contact not found.\n");
	}

	fclose(fp);
}
//메모리에 로드X
/*
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


void save_to_file(Node* head, const char* filename) {
	FILE* fp = fopen(filename, "wb");
	if (fp == NULL) {
		perror("Fail to open file to save");
		return;
	}
	Node* current = head;
	while (current) {
		fwrite(&current->data, sizeof(Addr), 1, fp);
		current = current->next;
	}
	fclose(fp);
}
*/