#pragma once
#define FILENAME "addr_book_10M.dat"
#define TEMP_FILENAME "temp.dat"

/*데이터 구조체 정의*/
typedef struct {
    char name[100];
    char tel[15];
    char addr[200];
} Addr;


/*모델 관련 함수 선언*/
void add_addr(const char* filename, Addr* addr);
//mode == 1 검색, mode == 2 삭제
void search_by_name(const char* filename, int mode); 
void search_by_tel(const char* filename, int mode);
void search_by_addr(const char* filename, int mode);
void search_by_SQL(const char* filename);
void print_addr(const char* filename);

/*개발자 모드! 첫 번째와 마지막 구조체를 출력하는 함수 첫 화면에서 6을 입력하세요!*/
void print_first_and_last(const char* filename);
/*개발자 모드! 코드 실행 시간 측정! 첫 화면에서 7을 입력하세요!*/
void measure_execution_time(int first_or_last, char* str);

/*뷰 관련 함수 선언*/
void print_message(char* message);
void output_format(Addr addr);

/*컨트롤러 관련 함수 선언*/
void add_addr_controller(void);
void delete_addr_controller(void);
void search_addr_controller(void);
void print_addr_controller(void);
void exit_addr_controller(void);

/* //메모리에 로드 X
typedef struct Node {
    Addr data;
    struct Node* next;
} Node;

Node* load_data_from_file(const char* filename);
void save_to_file(Node* head, const char* filename);
*/