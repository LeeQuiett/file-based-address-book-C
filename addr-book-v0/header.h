#pragma once
#define FILENAME "addr_book_10M.dat"
#define TEMP_FILENAME "temp.dat"

/*������ ����ü ����*/
typedef struct {
    char name[100];
    char tel[15];
    char addr[200];
} Addr;


/*�� ���� �Լ� ����*/
void add_addr(const char* filename, Addr* addr);
//mode == 1 �˻�, mode == 2 ����
void search_by_name(const char* filename, int mode); 
void search_by_tel(const char* filename, int mode);
void search_by_addr(const char* filename, int mode);
void search_by_SQL(const char* filename);
void print_addr(const char* filename);

/*������ ���! ù ��°�� ������ ����ü�� ����ϴ� �Լ� ù ȭ�鿡�� 6�� �Է��ϼ���!*/
void print_first_and_last(const char* filename);
/*������ ���! �ڵ� ���� �ð� ����! ù ȭ�鿡�� 7�� �Է��ϼ���!*/
void measure_execution_time(int first_or_last, char* str);

/*�� ���� �Լ� ����*/
void print_message(char* message);
void output_format(Addr addr);

/*��Ʈ�ѷ� ���� �Լ� ����*/
void add_addr_controller(void);
void delete_addr_controller(void);
void search_addr_controller(void);
void print_addr_controller(void);
void exit_addr_controller(void);

/* //�޸𸮿� �ε� X
typedef struct Node {
    Addr data;
    struct Node* next;
} Node;

Node* load_data_from_file(const char* filename);
void save_to_file(Node* head, const char* filename);
*/