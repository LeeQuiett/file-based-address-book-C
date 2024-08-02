#include <stdio.h>
#include <string.h>
#include "header.h"

#define FILENAME "addr_book.dat"

void add_addr_controller() {
	Addr addr;
	print_message("Enter name: ");
	fgets(addr.name, sizeof(addr.name), stdin);
	addr.name[strcpn(addr.name, '\n')] = '\0';
	putchar('\n');

	print_message("Enter tel: ");
	fgets(addr.tel, sizeof(addr.tel), stdin);
	addr.tel[strcpn(addr.tel, '\n')] == '\0';
	putchar('\n');

	print_message("Enter addr: ");
	fgets(addr.addr, sizeof(addr.addr), stdin);
	addr.addr[strcpn(addr.tel, '\n')] == '\0';
	putchar('\n');

	add_addr(FILENAME, &addr);
}

void delete_addr_controller() {

}

void search_addr_controller() {

}

void print_addr_controller() {

}

void exit_addr_controller() {

}