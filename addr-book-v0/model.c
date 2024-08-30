#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "header.h"

//Node* head = NULL; //���� �޸𸮿� �ε�X

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
/*�̸����� �˻�*/
void search_by_name(const char* filename, int mode) {
	if (mode == 1) { // �˻� ���
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
				output_format(addr);
				found = 1;
			}
		}

		if (!found) {
			print_message("Contact not found.\n\n");
		}

		fclose(fp);
	}
	else { // ���� ���
		char name[30];
		print_message("Enter the name to Delete: ");
		fgets(name, sizeof(name), stdin);
		name[strcspn(name, "\n")] = '\0';

		FILE* fp = fopen(filename, "rb");
		FILE* temp_fp = fopen(TEMP_FILENAME, "wb");
		if (fp == NULL || temp_fp == NULL) {
			perror("Failed to open file for reading/writing");
			if (fp) fclose(fp);
			if (temp_fp) fclose(temp_fp);
			return;
		}

		Addr addr;
		int found = 0;
		while (fread(&addr, sizeof(Addr), 1, fp)) {
			if (strcmp(addr.name, name) == 0) {
				print_message("Contact found:\n");
				printf("Start to delete this\n");
				output_format(addr);
				found = 1;
				continue; // ������ ����ü�� �ǳʶٱ�
			}
			fwrite(&addr, sizeof(Addr), 1, temp_fp);
		}

		if (!found) {
			print_message("Contact not found.\n\n");
		}

		fclose(fp);
		fclose(temp_fp);

		if (remove(filename) != 0) {
			perror("Failed to remove original file");
		}
		if (rename(TEMP_FILENAME, filename) != 0) {
			perror("Failed to rename temp file");
		}
	}
}
/*��ȭ��ȣ�� �˻�*/
void search_by_tel(const char* filename, int mode) {
	if (mode == 1) { // �˻� ���
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
				output_format(addr);
				found = 1;
			}
		}
		if (!found) {
			print_message("Contact not found.\n\n");
		}

		fclose(fp);
	}
	else { // ���� ���
		char tel[30];
		print_message("Enter the tel to Delete: ");
		fgets(tel, sizeof(tel), stdin);
		tel[strcspn(tel, "\n")] = '\0';

		FILE* fp = fopen(filename, "rb");
		FILE* temp_fp = fopen(TEMP_FILENAME, "wb");
		if (fp == NULL || temp_fp == NULL) {
			perror("Failed to open file for reading/writing");
			if (fp) fclose(fp);
			if (temp_fp) fclose(temp_fp);
			return;
		}

		Addr addr;
		int found = 0;
		while (fread(&addr, sizeof(Addr), 1, fp)) {
			if (strcmp(addr.tel, tel) == 0) {
				print_message("Contact found:\n");
				printf("Start to Delete this\n");
				output_format(addr);
				found = 1;
				continue;
			}
			fwrite(&addr, sizeof(Addr), 1, temp_fp);
		}
		if (!found) {
			print_message("Contact not found.\n\n");
		}

		fclose(fp);
		fclose(temp_fp);

		if (remove(filename) != 0) {
			perror("Failed to remove original file");
		}
		if (rename(TEMP_FILENAME, filename) != 0) {
			perror("Failed to rename temp file");
		}
	}
}
/*�ּҷ� �˻�*/
void search_by_addr(const char* filename, int mode) {
	if (mode == 1) { // �˻� ���
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
				output_format(a);
				found = 1;
			}
		}

		if (!found) {
			print_message("Contact not found.\n\n");
		}

		fclose(fp);
	}
	else { // ���� ���
		char addr[100];
		print_message("Enter the address to Delete: ");
		fgets(addr, sizeof(addr), stdin);
		addr[strcspn(addr, "\n")] = '\0';

		FILE* fp = fopen(filename, "rb");
		FILE* temp_fp = fopen(TEMP_FILENAME, "wb");
		if (fp == NULL || temp_fp == NULL) {
			perror("Failed to open file for reading/writing");
			if (fp) fclose(fp);
			if (temp_fp) fclose(temp_fp);
			return;
		}

		Addr a;
		int found = 0;
		while (fread(&a, sizeof(Addr), 1, fp)) {
			if (strcmp(a.addr, addr) == 0) {
				print_message("Contact found:\n");
				printf("Start to Delete this\n");
				output_format(a);
				found = 1;
				continue;
			}
			fwrite(&a, sizeof(Addr), 1, temp_fp);
		}

		if (!found) {
			print_message("Contact not found.\n\n");
		}

		fclose(fp);
		fclose(temp_fp);

		if (remove(filename) != 0) {
			perror("Failed to remove original file");
		}
		if (rename(TEMP_FILENAME, filename) != 0) {
			perror("Failed to rename temp file");
		}
	}
}

/*���ڿ� �� ���� ���ڿ��� ���� ī��Ʈ*/
int count_and_or(const char* str, const char* word) {
	int count = 0;
	const char* pos = str;

	while ((pos = strstr(pos, word)) != NULL) {
		count++;
		pos += strlen(word);
	}
	return count;
}
/*���� ���ڿ��� ��ġ�� �˻�*/
int find_substring_position(const char* str, const char* sub) {
	const char* pos = strstr(str, sub);

	if (pos != NULL) {
		return (int)(pos - str);
	}
	else {
		return -1;
	}
}
/*���� �Ľ�*/
void parse_query(const char* query, char* query1, char* query2, char* and_or) {
	int pos = find_substring_position(query, and_or);
	if (pos != -1) {
		strncpy(query1, query, pos);
		query1[pos] = '\0';
		strcpy(query2, query + pos + strlen(and_or));
	}
}
/*SQL ��Ÿ�� �˻�*/
void search_by_SQL(const char* filename) {
	char query[256];
	char query1[256], query2[256];
	printf("���� ��: name=ȫ�浿 and addr=�����, tel=02-123-123 or addr=������\n\n");
	print_message("Enter your search query: ");
	fgets(query, sizeof(query), stdin);
	query[strcspn(query, "\n")] = '\0';

	int and_count = count_and_or(query, " and ");
	int or_count = count_and_or(query, " or ");
	/*������ and�� or�� ���� ���*/
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
				output_format(addr);
				found = 1;
			}
		}

		if (!found) {
			print_message("Contact not found.\n\n");
		}

		fclose(fp);
	}
	/*������ and�� 1�� �ִ� ���*/
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
					output_format(addr);
					found = 1;
				}
			}
		}

		if (!found) {
			print_message("Contact not found.\n\n");
		}

		fclose(fp);

	}
	/*������ or�� 1�� �ִ� ���*/
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
				output_format(addr);
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
				output_format(addr);
				found = 1;
			}
		}

		if (!found) {
			print_message("Contact not found.\n\n");
		}

		fclose(fp);
	}
	/*������ and�� or�� 1�� �̻��� ���*/
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
		output_format(addr);
	}
	fclose(fp);
}

/*������ ���! ù ��°�� ������ ����ü�� ����ϴ� �Լ� ù ȭ�鿡�� 6�� �Է��ϼ���!*/
void print_first_and_last(const char* filename) {
	FILE* fp = fopen(filename, "rb");
	if (fp == NULL) {
		perror("Failed to open file for reading");
		return;
	}

	// ���� ũ�� ���
	fseek(fp, 0, SEEK_END);
	long file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	size_t struct_size = sizeof(Addr);

	// ����ü �� ���
	int num_records = file_size / struct_size;

	// ù ��° ����ü �б�
	Addr first_addr;
	if (fread(&first_addr, struct_size, 1, fp) == 1) {
		printf("First Record:\n");
		output_format(first_addr);
	}
	else {
		perror("Failed to read the first record");
	}

	// ������ ����ü �б�
	fseek(fp, -(int)struct_size, SEEK_END);
	Addr last_addr;
	if (fread(&last_addr, struct_size, 1, fp) == 1) {
		printf("Last Record:\n");
		output_format(last_addr);
	}
	else {
		perror("Failed to read the last record");
	}
	fclose(fp);
}

/*������ ���! ù ��° Ȥ�� ������ �������� �˻� �ð� ����! ù ȭ�鿡�� 7�� �Է��ϼ���!*/
void measure_execution_time(int first_or_last, char* str) {
	printf("===========================\n%s Addr!\nStart measuring execution time\n\n", str);
	char name[30];
	if (first_or_last == 1) {
		strcpy(name, "010-1448-1926");
	}
	else {
		strcpy(name, "010-5924-2000");
	}

	FILE* fp = fopen(FILENAME, "rb");
	if (fp == NULL) {
		perror("Failed to open file for reading");
		return;
	}
		
	LARGE_INTEGER frequency, start, end;

	// ī������ ���ļ��� �����ɴϴ�!
	QueryPerformanceFrequency(&frequency);

	// ���� �ð� ���
	QueryPerformanceCounter(&start);

	// ������ �Լ� ȣ��
	Addr addr;
	int found = 0;
	while (fread(&addr, sizeof(Addr), 1, fp)) {
		if (strcmp(addr.tel, name) == 0) {
			print_message("Contact found:\n");
			output_format(addr);
			found = 1;
		}
	}

	if (!found) {
		print_message("Contact not found.\n\n");
	}

	fclose(fp);

	// ���� �ð� ���
	QueryPerformanceCounter(&end);

	// ���� �ð� ���
	double elapsed_time = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

	printf("Execution time: %f seconds\n===========================\n", elapsed_time);
}

/* // ���θ޸𸮿� �ε� X
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