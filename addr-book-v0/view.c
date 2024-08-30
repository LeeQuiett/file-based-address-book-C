#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "header.h"

void print_message(const char* message) {
	printf("%s", message);
}

void output_format(Addr addr) {
	printf("Name:\t%s\nTel:\t%s\nAddr:\t%s\n\n", addr.name, addr.tel, addr.addr);
}