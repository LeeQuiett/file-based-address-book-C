#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "header.h"

void add_addr_controller() {
    print_message("[1] Enter [2] Back: ");
    int input;
    scanf("%d%*c", &input);
    if (input == 1) {
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
    else if(input == 2) {
        system("cls"); return;
    }
    else {
        system("cls"); puts("Wrong input! Try again!"); return;
    }
}


void delete_addr_controller() {
    while (1) {
        print_message("[1] Search for Del by name [2] Search for Del by tel [3] Search for Del by addr [4] Back: ");
        int input;
        scanf("%d%*c", &input);
        switch (input) {
        case 1: search_by_name(FILENAME, 2); return;
        case 2: search_by_tel(FILENAME, 2); return;
        case 3: search_by_addr(FILENAME, 2); return;
        //case 4: search_by_SQL(FILENAME, 2); return;
        case 4: system("cls"); return;
        default: system("cls"); puts("Wrong input! Try again!"); break;
        }
    }
}

void search_addr_controller() {
    while (1) {
        print_message("[1] Search by name [2] Search by tel [3] Search by addr [4] Search by SQL [5] Back: ");
        int input;
        scanf("%d%*c", &input);
        switch (input) {
        case 1: search_by_name(FILENAME, 1); return;
        case 2: search_by_tel(FILENAME, 1); return;
        case 3: search_by_addr(FILENAME, 1); return;
        case 4: search_by_SQL(FILENAME, 1); return;
        case 5: system("cls"); return;
        default: system("cls"); puts("Wrong input! Try again!"); break;
        }
    }
}

void print_addr_controller() {
    print_message("Print the entire address book\n\n");
    print_addr(FILENAME);
}

void exit_addr_controller() {
    print_message("Exit the program...Bye!\n");
    return;
}