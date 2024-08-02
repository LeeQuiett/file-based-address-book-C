#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "header.h"

void add_addr_controller() {
    Addr addr;
    print_message("Enter name: ");
    fgets(addr.name, sizeof(addr.name), stdin);
    addr.name[strcspn(addr.name, "\n")] = '\0';
    putchar('\n');

    print_message("Enter tel: ");
    fgets(addr.tel, sizeof(addr.tel), stdin);
    addr.tel[strcspn(addr.tel, "\n")] = '\0';
    putchar('\n');

    print_message("Enter addr: ");
    fgets(addr.addr, sizeof(addr.addr), stdin);
    addr.addr[strcspn(addr.addr, "\n")] = '\0';
    putchar('\n');

    add_addr(FILENAME, &addr);
}


void delete_addr_controller() {

}

void search_addr_controller() {
    while (1) {
        print_message("[1] Search by name, [2] Search by tel, [3] Search by addr, [4] Search by SQL: ");
        int input;
        scanf("%d%*c", &input);
        switch (input) {
        case 1: search_by_name(); return;
        case 2: search_by_tel(); return;
        case 3: search_by_addr(); return;
        case 4: search_by_SQL(); return;
        default: puts("Wrong input! Try again!"); break;
        }
    }
}

void print_addr_controller() {

}

void exit_addr_controller() {

}