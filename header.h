#pragma once
#define FILENAME "addr_book.dat"

// 데이터 구조체 정의
typedef struct {
    char name[30];
    char tel[15];
    char addr[100];
} Addr;


// 모델 관련 함수 선언
void add_addr(const char* filename, Addr* addr);

// 뷰 관련 함수 선언
void print_message(char* message);

// 컨트롤러 관련 함수 선언
void add_addr_controller(void);
void delete_addr_controller(void);
void search_addr_controller(void);
void print_addr_controller(void);
void exit_addr_controller(void);

//메모리에 로드X
/*
typedef struct Node {
    Addr data;
    struct Node* next;
} Node;
Node* load_data_from_file(const char* filename);
void save_to_file(Node* head, const char* filename);
*/