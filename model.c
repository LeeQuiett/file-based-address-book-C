#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

//Node* head = NULL;

/*add_addr_controller*/
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
/*이름으로 검색*/
void search_by_name(const char* filename, int mode) {
	if (mode == 1) { //검색 모드
		char name[30];
		print_message("Enter the name to search: ");
		fgets(name, sizeof(name), stdin);
		name[strcspn(name, "\n")] = '\0';

		FILE* fp = fopen(filename, "rb");
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
	else { //삭제 모드
		char name[30];
		print_message("Enter the name to Delete: ");
		fgets(name, sizeof(name), stdin);
		name[strcspn(name, "\n")] = '\0';

		FILE* fp = fopen(filename, "rb");
		FILE* temp_fp = fopen(TEMP_FILENAME, "wb");
		if (fp == NULL || temp_fp == NULL) {
			perror("Failed to open file for reading");
			return;
		}

		Addr addr;
		int found = 0;
		while (fread(&addr, sizeof(Addr), 1, fp)) {		
			if (strcmp(addr.name, name) == 0) {
				print_message("Contact found:\n");
				printf("Name: %s\nTel: %s\nAddr: %s\n\n", addr.name, addr.tel, addr.addr);
				printf("Start to delete...\n");
				found = 1;
				continue; // 삭제할 구조체는 건너뛰기
			}
			fwrite(&addr, sizeof(Addr), 1, temp_fp);
		}

		if (!found) {
			print_message("Contact not found.\n");
		}

		fclose(fp);
		fclose(temp_fp);

		remove(filename);
		rename(TEMP_FILENAME, filename);
	}	
}
/*전화번호로 검색*/
void search_by_tel(const char* filename, int mode) {
	if (mode == 1) { //검색 모드
		char tel[30];
		print_message("Enter the tel to search: ");
		fgets(tel, sizeof(tel), stdin);
		tel[strcspn(tel, "\n")] = '\0';

		FILE* fp = fopen(filename, "rb");
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
	else { //삭제 모드
		char tel[30];
		print_message("Enter the tel to Delete: ");
		fgets(tel, sizeof(tel), stdin);
		tel[strcspn(tel, "\n")] = '\0';

		FILE* fp = fopen(filename, "rb");
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
				printf("Start to Delete...\n");
				strcpy(addr.name, "Deleted Name");
				strcpy(addr.tel, "Deleted tel");
				strcpy(addr.addr, "Deleted addr");
				fwrite(&addr, sizeof(Addr), 1, fp);
				printf("Name: %s\nTel: %s\nAddr: %s\n\n", addr.name, addr.tel, addr.addr);
				found = 1;
			}
		}
		if (!found) {
			print_message("Contact not found.\n");
		}
		fclose(fp);
	}
}
/*주소로 검색*/
void search_by_addr(const char* filename) {
	char addr[100];
	print_message("Enter the address to search: ");
	fgets(addr, sizeof(addr), stdin);
	addr[strcspn(addr, "\n")] = '\0';

	FILE* fp = fopen(filename, "rb");
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
/*문자열 내 서브 문자열의 수를 카운트*/
int count_and_or(const char* str, const char* word) {
	int count = 0;
	const char* pos = str;

	while ((pos = strstr(pos, word)) != NULL) {
		count++;
		pos += strlen(word);
	}
	return count;
}
/*서브 문자열의 위치를 검색*/
int find_substring_position(const char* str, const char* sub) {
	const char* pos = strstr(str, sub);

	if (pos != NULL) {
		return (int)(pos - str);
	}
	else {
		return -1;
	}
}
/*쿼리 파싱*/
void parse_query(const char* query, char* query1, char* query2, char* and_or) {
	int pos = find_substring_position(query, and_or);
	if (pos != -1) {
		strncpy(query1, query, pos);
		query1[pos] = '\0';
		strcpy(query2, query + pos + strlen(and_or));
	}
}
/*SQL 스타일 검색*/
void search_by_SQL(const char* filename) {
	char query[256];
	char query1[256], query2[256];
	printf("쿼리 예: name=홍길동 and addr=서울시, tel=02-123-123 or addr=대전시\n\n");
	print_message("Enter your search query: ");
	fgets(query, sizeof(query), stdin);
	query[strcspn(query, "\n")] = '\0';

	int and_count = count_and_or(query, " and ");
	int or_count = count_and_or(query, " or ");
	/*쿼리에 and나 or가 없을 경우*/
	if (and_count == 0 && or_count == 0) {
		char field[50], value[200];
		sscanf(query, "%[^=]=%s", field, value);

		FILE* fp = fopen(filename, "rb");
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
	/*쿼리에 and가 1개 있는 경우*/
	else if (and_count == 1 && or_count == 0) {
		
		parse_query(query, query1, query2, " and ");
		printf("Query1: %s\n", query1);
		printf("Query2: %s\n\n", query2);

		char field[50], value[200];
		char field2[50], value2[200];

		sscanf(query1, "%[^=]=%s", field, value);
		sscanf(query2, "%[^=]=%s", field2, value2);

		FILE* fp = fopen(filename, "rb");
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
				int match = 0;
				if (strcmp(field2, "name") == 0 && strcmp(addr.name, value2) == 0) {
					match = 1;
				}
				else if (strcmp(field2, "tel") == 0 && strcmp(addr.tel, value2) == 0) {
					match = 1;
				}
				else if (strcmp(field2, "addr") == 0 && strcmp(addr.addr, value2) == 0) {
					match = 1;
				}

				if (match) {
					print_message("Contact found:\n");
					printf("Name: %s\nTel: %s\nAddr: %s\n\n", addr.name, addr.tel, addr.addr);
					found = 1;
				}				
			}
		}

		if (!found) {
			print_message("Contact not found.\n");
		}

		fclose(fp);

	}
	/*쿼리에 or가 1개 있는 경우*/
	else if (and_count == 0 && or_count == 1) {
		parse_query(query, query1, query2, " or ");
		printf("Query1: %s\n", query1);
		printf("Query2: %s\n\n", query2);

		char field[50], value[200];
		char field2[50], value2[200];

		sscanf(query1, "%[^=]=%s", field, value);
		sscanf(query2, "%[^=]=%s", field2, value2);

		FILE* fp = fopen(filename, "rb");
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
			match = 0;
			if (strcmp(field2, "name") == 0 && strcmp(addr.name, value2) == 0) {
				match = 1;
			}
			else if (strcmp(field2, "tel") == 0 && strcmp(addr.tel, value2) == 0) {
				match = 1;
			}
			else if (strcmp(field2, "addr") == 0 && strcmp(addr.addr, value2) == 0) {
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
	/*쿼리에 and나 or가 1개 이상일 경우*/
	else {
		puts("Unsupported operation! Try again!");
	}
}

/*print_addr_controller*/
void print_addr(const char* filename) {
	Addr addr;

	FILE* fp = fopen(filename, "rb");
	if (fp == NULL) {
		perror("Failed to open file for reading");
		return;
	}
	while (fread(&addr, sizeof(Addr), 1, fp)) {
		printf("Name: %s\nTel: %s\nAddr: %s\n\n", addr.name, addr.tel, addr.addr);
	}

}
/* //메인메모리에 로드 X
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