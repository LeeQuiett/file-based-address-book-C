#pragma once
#define NUM_RECORDS 4
#define NAME_LENGTH 12
#define ADDR_LENGTH 100

typedef struct
{
    int tel;
    char name[NAME_LENGTH];
    char addr[ADDR_LENGTH];
} Addr;

void testPrintIndex();
void testPrintList();
void load_data_from_file(Addr* records, int num_records, const char* filename);
void print_records(const Addr* records, int num_records);
void testPrintList();