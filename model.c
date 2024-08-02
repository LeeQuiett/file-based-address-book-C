#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char name[30];
	char tel[15];
	char addr[100];
} Addr;

void add_addr(const char* filename, Addr* addr) {
	FILE* fp = fopen(filename, "ab");
	if (fp == NULL) {
		perror("Fail to file open!");
		return;
	}
	fwrite(addr, sizeof(Addr), 1, fp);
	fclose(fp);
}