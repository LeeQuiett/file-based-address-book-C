# 프로젝트 개요

이름, 주소, 전화번호를 관리할 수 있는 주소록 프로그램

<details>
<summary>ver 0: 초기 버전 README.md </summary>
<div markdown="1">

# 요구사항 분석 및 정의

- 이름, 주소 전화번호를 관리 할 수 있는 주소록 프로그램 작성
- 메뉴 출력 및 화면 제어 로직은 반드시 Event loop구조를 적용하고 함수 포인터 테이블을 사용해 호출
- 주소록 데이터 정보는 모두 파일로 저장
- 1 만개 이상의 더미 데이터를 넣어서 운영
    - 이름 중복 허용
    - 첫 번째 데이터와 1만 번째 데이터 간 검색속도 차이 확인
    - [더미 데이터는 이렇게 생성하였습니다.](https://github.com/LeeQuiett/address-book-dummy-data-generator)
- 성능 극대화 방안 제시

---

# 프로그램 실행 구조

- **시작 시 메뉴 표시**: 사용자에게 메뉴를 표시하고 선택을 입력받습니다.
- **입력 처리**: 사용자의 입력에 따라 적절한 함수를 함수 포인터를 이용해 호출합니다.
- **특수 기능 처리**: 특별한 입력 (`6` 또는 `7`)에 대한 추가 기능을 수행합니다.
    - 6입력: 첫 번째 데이터와 마지막 데이터를 출력합니다.
    - 7입력: 첫 번째 데이터와 마지막 데이터를 검색하는데 걸린 시간을 출력합니다.
- **종료**: 사용자가 `EXIT`를 선택하면 프로그램 종료합니다.
- **실행 예**
    
    ```c
    [1] ADD [2] DELETE [3] SEARCH [4] PRINT [5] EXIT: 1
    [1] Enter [2] Back: 1
    Enter name: lobonabeat
    
    Enter tel: 010-1234-1234

    Enter addr: LA
    
    [1] ADD [2] DELETE [3] SEARCH [4] PRINT [5] EXIT: 3
    [1] Search by name [2] Search by tel [3] Search by addr [4] Search by SQL [5] Back: 1
    Enter the name to search: lobonabeat
    Contact found:
    
    Name:   lobonabeat
    Tel:    010-1234-1234
    Addr:   LA
    
    [1] ADD [2] DELETE [3] SEARCH [4] PRINT [5] EXIT: 5
    Exit the program...Bye!
    ```
    
- **특수 기능**: 첫 화면에서 6 입력
    
    ```c
    [1] ADD [2] DELETE [3] SEARCH [4] PRINT [5] EXIT: 6
    First Record:
    Name:   박은하
    Tel:    010-1448-1926
    Addr:   경남 마포구 14동 67호
    
    Last Record:
    Name:   윤현현
    Tel:    010-5924-2000
    Addr:   세종 금천구 2동 34호
    ```
    
- **특수 기능**: 첫 화면에서 7 입력
    
    ```c
    [1] ADD [2] DELETE [3] SEARCH [4] PRINT [5] EXIT: 7
    ===========================
    First Addr!
    Start measuring execution time
    
    Contact found:
    
    Name:   박은하
    Tel:    010-1448-1926
    Addr:   경남 마포구 14동 67호
    
    Execution time: 0.035987 seconds
    ===========================
    ===========================
    Last Addr!
    Start measuring execution time
    
    Contact found:
    
    Name:   윤현현
    Tel:    010-5924-2000
    Addr:   세종 금천구 2동 34호
    
    Execution time: 0.040347 seconds
    ===========================
    ```
    

---

# 코드 상세 설명

## 자료구조

- **“추가, 제거, 검색, 출력, 나가기”중 하나를 입력받을 열거형**
    
    ```c
    typedef enum { ADD = 1, DELETE, SEARCH, PRINT, EXIT } MY_SELECT;
    ```
    
- **열거형 변수에 받은 입력과 일치하는 인덱스의 함수를 호출할 함수 포인터**
    
    ```c
    pFunction menuFunctions[MENU_COUNT] = { 
    	add_addr_controller,
    	delete_addr_controller,
    	search_addr_controller,
    	print_addr_controller,
    	exit_addr_controller
    };
    ```
    
- **I/O를 수행할 이진 파일의 경로를 전처리기로 정의**
    
    ```c
    #define FILENAME "addr_book_10M.dat" //10만개의 구조체를 담은 파일 경로
    #define TEMP_FILENAME "temp.dat" //데이터 삭제 시에 임시로 사용할 파일 경로
    ```
    
- **주소록 데이터 구조체 정의**
    
    ```c
    typedef struct {
        char name[100];
        char tel[15];
        char addr[200];
    } Addr;
    ```
    

---

## 메뉴 출력 및 화면 제어 로직에 Event loop 구조를 사용합니다.

```c
while (1) {
	printf("[1] ADD [2] DELETE [3] SEARCH [4] PRINT [5] EXIT: ");
	scanf("%d%*c", &my_select);

	if (my_select == 6) {
		print_first_and_last(FILENAME);
		continue;
	}
	else if (my_select == 7) {
		measure_execution_time(1, "First");
		measure_execution_time(2, "Last");
		continue;
	}

	if (my_select >= ADD && my_select <= EXIT) {
		if (my_select == EXIT) {
			menuFunctions[my_select - 1]();
			break;
		}
		else {
			menuFunctions[my_select - 1]();
		}

	}
	else {
		puts("Wrong input! Try again!");
	}
}
```

---

## 입력을 바탕으로 Controller에서 적절한 처리 후 Model과 View의 함수를 호출합니다.

```c
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
        case 4: search_by_SQL(FILENAME); return;
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
```

---

## 주소록에 데이터를 추가합니다.

- **append 모드로 파일의 끝에 데이터를 추가합니다.**

```c
void add_addr(const char* filename, Addr* addr) {
	FILE* fp = fopen(filename, "ab");
	if (fp == NULL) {
		perror("Fail to open file for add");
		return;
	}
	fwrite(addr, sizeof(Addr), 1, fp);
	fclose(fp);
}
```

---

## 이름으로 주소록을 검색합니다. 전화번호와 주소를 이용한 검색도 동일한 로직을 사용합니다.

- **검색 모드를 확인합니다.**
    - **검색 모드**
        - 사용자로부터 검색할 이름을 입력 받습니다.
        - 파일에서 구조체를 읽으며 읽은 데이터의 `name`이 사용자 입력과 일치하면 해당 주소를 출력합니다.
    - **삭제 모드**
        - 사용자로부터 검색할 이름을 입력 받습니다.
        - 파일에서 구조체를 읽으며 읽은 데이터의 `name`이 사용자 입력과 일치하면 해당 주소를 건너뛰고 나머지 주소를 임시 파일에 씁니다.
        - 원본 파일을 삭제한 뒤, 임시 파일을 원본 파일 이름으로 변경합니다.

```c
void search_by_name(const char* filename, int mode) {
	if (mode == 1) { // 검색 모드
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
				print_message("Contact found:\n\n");
				output_format(addr);
				found = 1;
			}
		}

		if (!found) {
			print_message("Contact not found.\n\n");
		}

		fclose(fp);
	}
	else { // 삭제 모드
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
				print_message("Contact found:\n\n");
				printf("Start to delete this\n");
				output_format(addr);
				found = 1;
				continue; // 삭제할 구조체는 건너뛰기
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
```

---

## SQL 스타일의 검색을 지원합니다.

- **쿼리 예: name=홍길동 and addr=서울시, tel=02-123-123 or addr=대전시**

### 1. `count_and_or`

- 문자열 내에서 특정 서브 문자열(단어)의 개수를 세는 함수입니다.

```c
int count_and_or(const char* str, const char* word) {
    int count = 0;
    const char* pos = str;

    while ((pos = strstr(pos, word)) != NULL) {
        count++;
        pos += strlen(word);
    }
    return count;
}
```

- `str`은 검색할 전체 문자열입니다.
- `word`는 찾고자 하는 서브 문자열입니다.
- `strstr(pos, word)`는 `pos` 이후의 문자열에서 `word`를 찾습니다.
- `pos`가 `NULL`이 아닌 경우(즉, `word`를 찾은 경우) `count`를 증가시키고, `pos`를 `word`의 끝으로 이동시킵니다.
- 이 과정을 반복하여 `word`의 모든 발생을 세고, 최종적으로 개수를 반환합니다.

### 2. `find_substring_position`

- 문자열 내에서 서브 문자열의 위치를 찾는 함수입니다.

```c
int find_substring_position(const char* str, const char* sub) {
    const char* pos = strstr(str, sub);

    if (pos != NULL) {
        return (int)(pos - str);
    } else {
        return -1;
    }
}
```

- `str`은 검색할 전체 문자열입니다.
- `sub`는 찾고자 하는 서브 문자열입니다.
- `strstr(str, sub)`는 `str` 내에서 `sub`를 찾습니다.
- `pos`가 `NULL`이 아니면, `sub`의 위치를 반환합니다. 위치는 `pos - str`으로 계산합니다.
- `pos`가 `NULL`인 경우(즉, `sub`를 찾지 못한 경우), `-1`을 반환합니다.

### 3. `parse_query`

- 쿼리 문자열을 `and` 또는 `or` 연산자를 기준으로 분리하는 함수입니다.

```c
void parse_query(const char* query, char* query1, char* query2, char* and_or) {
    int pos = find_substring_position(query, and_or);
    if (pos != -1) {
        strncpy(query1, query, pos);
        query1[pos] = '\0';
        strcpy(query2, query + pos + strlen(and_or));
    }
}
```

- `query`는 분석할 쿼리 문자열입니다.
- `query1`과 `query2`는 `and_or`를 기준으로 분리된 두 쿼리 부분을 저장할 버퍼입니다.
- `and_or`는 쿼리를 나누는 기준 문자열입니다(예: " and " 또는 " or ").
- `find_substring_position(query, and_or)`를 호출하여 `and_or`의 위치를 찾습니다.
- `strncpy(query1, query, pos)`는 `query`의 시작부터 `and_or`의 위치까지를 `query1`에 복사합니다.
- `query1[pos] = '\0'`로 `query1`을 문자열 종료 문자로 끝냅니다.
- `strcpy(query2, query + pos + strlen(and_or))`는 `and_or` 뒤의 나머지 문자열을 `query2`에 복사합니다.

### **4. 쿼리에 “and”나 “or”가 있는 경우**

- `and`의 경우 입력받은 조건이 모두 일치하면 출력합니다.
- `or`의 경우 입력받은 조건 중 하나만 일치해도 출력합니다.

```c
/*쿼리에 and가 1개 있는 경우*/
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
				print_message("Contact found:\n\n");
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
/*쿼리에 or가 1개 있는 경우*/
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
			print_message("Contact found:\n\n");
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
			print_message("Contact found:\n\n");
			output_format(addr);
			found = 1;
		}
	}
```

---

## 첫 번째와 마지막 데이터를 검색하는 시간을 측정합니다.

### 1. `print_first_and_last`

- 첫 번째와 마지막 구조체를 읽어 출력합니다.

```c
void print_first_and_last(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("Failed to open file for reading");
        return;
    }

    // 파일 크기 계산
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    size_t struct_size = sizeof(Addr);

    // 구조체 수 계산
    int num_records = file_size / struct_size;

    // 첫 번째 구조체 읽기
    Addr first_addr;
    if (fread(&first_addr, struct_size, 1, fp) == 1) {
        printf("First Record:\n");
        output_format(first_addr);
    }
    else {
        perror("Failed to read the first record");
    }

    // 마지막 구조체 읽기
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
```

- `fseek(fp, 0, SEEK_END)`와 `ftell(fp)`로 파일의 총 크기를 구합니다.
- `sizeof(Addr)`로 구조체의 크기를 구하고, `file_size / struct_size`로 전체 구조체 수를 계산합니다.
- **첫 번째 구조체 읽기**:
    - 파일의 시작 부분에서 구조체를 읽어 `first_addr`에 저장합니다.
    - `output_format(first_addr)`을 호출하여 출력합니다.
- **마지막 구조체 읽기**:
    - 파일의 끝에서 구조체 크기만큼 이동하여 마지막 구조체를 읽습니다.
    - `output_format(last_addr)`을 호출하여 출력합니다.

### 2. `measure_execution_time`

- `first_or_last` 값에 따라 첫 번째 또는 마지막 구조체를 검색하는데 걸린 시간을 측정합니다.

```c
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

    // 카운터의 주파수를 가져옵니다!
    QueryPerformanceFrequency(&frequency);

    // 시작 시간 기록
    QueryPerformanceCounter(&start);

    // 측정할 함수 호출
    Addr addr;
    int found = 0;
    while (fread(&addr, sizeof(Addr), 1, fp)) {
        if (strcmp(addr.tel, name) == 0) {
            print_message("Contact found:\n\n");
            output_format(addr);
            found = 1;
        }
    }

    if (!found) {
        print_message("Contact not found.\n\n");
    }

    fclose(fp);

    // 종료 시간 기록
    QueryPerformanceCounter(&end);

    // 실행 시간 계산
    double elapsed_time = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

    printf("Execution time: %f seconds\n===========================\n", elapsed_time);
}
```

- `first_or_last` 값에 따라 `name`을 설정합니다.
- **시간 측정 준비**:
    - `QueryPerformanceFrequency(&frequency)`로 카운터의 주파수를 얻습니다.
    - `QueryPerformanceCounter(&start)`로 시작 시간을 기록합니다.
- **검색 수행**:
    - 파일을 읽으면서 일치하는 경우를 찾습니다.
    - 찾은 경우 `print_message`와 `output_format`으로 결과를 출력합니다.
- **시간 측정 완료**:
    - `QueryPerformanceCounter(&end)`로 종료 시간을 기록합니다.
    - 경과 시간을 계산하여 출력합니다.

---

# 성능 극대화 방안

## 1. out of memory 기반 B 트리 자료구조 이용

현재 코드는 순차적으로 이진 데이터를 읽고 있습니다. 따라서 탐색 시간은 $O(n)$의 시간 복잡도를 가질 것입니다. 

인덱스를 추가하고 out of memory 기반 B 트리 자료구조를 사용한다면, 최악의 경우에도 $O(logn)$의 시간 복잡도를 가지게 개선할 수 있을 것입니다.

## 2. 플래시 메모리 상에서 지연 갱신을 이용한 B 트리의 더 효율적인 구현

플래시 메모리는 하드디스크와 다르게 읽기, 쓰기 연산 이외에 소거 연산이 필요하고, 읽기-쓰기 연산은 페이지 단위(2KB)로 각각 25μs와 200μs의 시간으로 수행됩니다. 그러나 소거 연산은 블록 단위(128KB)로 세 가지 연산중 가장 느린 1.5ms의 시간으로 수행됩니다. 또한 특정 물리 주소에 데이터가 기록되었을 때 해당 주소에 대해 제자리 갱신이 불가능하여, 해당 블록을 소거 한 후, 해당 페이지에 쓰기 연산을 수행하는 erase-before-write 동작이 선생되어야 합니다.

플래시 메모리는 소거 연산이 필요하기 때문에 FTL(Flash Translation Layer)이 필요합니다. FTL은 호스트의 쓰기 요청을 받아 플래시 메모리의 비어있는 페이지를 매핑시켜줍니다.

그러나 FTL이 아무리 효율적인 매핑 알고리즘을 제공하여도 **디스크 기반의 B 트리는 제자리 갱신이 빈번하게 발생하기 때문에 성능 저하가 발생**합니다.

이러한 **B 트리의 단점을 개선한 μ-트리와 LSB-트리**는 모든 삽입 연산을  한 번의 쓰기 연산으로 처리하여 **인덱스의 성능을 향상**시켰으나, **μ-트리는 페이지 구조상 노드 분할이 빈번하기 발생하여 쓰기 및 검색 성능이 저하되는 문제점을 가집니다.** 또한 **LSB-트리는 임시 노드에 대한 검색과 트리 병합을 위한 추가 연산이 필요하다는 문제점을 가집니다.**

따라서 지연 갱신을 이용한 B 트리(LUB 트리)를 사용할 수 있습니다. LUB 트리는 루트 노드를 메모리에 항상 상주시켜 **검색 성능을 향상시키고, 삽입 및 갱신 연산이 발생하는 노드를 메모리에 두어 플래시 메모리의 쓰기 연산을 지연시킴으로써 쓰기 연산을 향상**시킬 수 있습니다. 또한 **순차 쓰기에 대한 노드 분할을 사용하지 않음으로써 페이지 효율을 증가**시키고 추가적인 쓰기 연산을 줄일 수 있습니다.

> 참고문헌
> 
> 
> Kim, Bo-Kyung, Min-Hee Yoo, and Dong-Ho Lee. 2011. "An Efficient Implementation of B-Tree Using Lazy Update on Flash Memory." *Proceedings of the Korean Institute of Information Scientists and Engineers* 38, no. 1 (2011): 69-72.
---
# 추가 개선 방향

- ADT 적용 (자료구조와 관리 대상자료 분리)
- 인덱스 사용
- 데이터 캐싱
- Commit과 그에 대응하는 롤백 구현 및 멀티스레딩 환경 지원
- Client/Server 구조 적용
    - System, Socket 프로그래밍 학습 필요
 
---

</div>
</details>

---

## 구현 사항

- UI는 **Event Loop** 구조를 적용하여 구현했습니다.
- **ADT**, 자료구조와 데이터를 분리했습니다.
- **모든 주소록 데이터는 파일**로 저장했습니다.
- **1000만 개 이상**의 더미 **데이터를 운영**합니다.
    - **PK**: 전화번호
    - 나머지 필드는 중복을 허용합니다.
    - [더미 데이터는 이렇게 생성하였습니다.](https://github.com/LeeQuiett/address-book-dummy-data-generator)
- **메인 메모리에 PK 인덱스 운영(O(1)의 시간복잡도 제공)**
    - 프로그램 시작 시 파일을 읽어 인덱스를 초기화합니다.
    - 인덱스는 주소록 **파일의 offset을** 가지고 있습니다. 이를 통해 주소록 **파일에 즉시 접근**합니다.
    - **O(1)의 시간 복잡도 제공: 전화번호를 배열의 인덱스**로 사용하여 일종의 Look-up Array를 구현했습니다.
    - 데이터가 **1억 개**일 경우 크기: 약 `400MB`
    - 데이터가 1**000만 개**일 경우 크기: 약 `40MB`
    - 데이터가 **100만 개**일 경우 크기: 약 `4MB`
- ~~**나머지 필드의 인덱스**는 중복 허용으로 인해 **리스트를 가진 트리 구조로 관리하거나**, 파일에서 선형적으로 읽어오는 방식을 시간 및 공간 복잡도를 고려하여 선택할 예정입니다.~~
- 1억 개의 데이터를 가진 테이블에서 **모든 필드를 인덱싱할 경우 약 14.8GB의 메모리가 필요**합니다. 메모리 효율성을 고려해 속도보다는 메모리를 우선하여, Primary Key만 인덱스로 운영하고 나머지 필드에 대해서는 선형 검색 방식을 선택했습니다. **Primary Key를 알고 있으면 O(1)의 속도로 접근**할 수 있기 때문에 이 방법을 채택했습니다.
    - 트리 구조로 관리할 경우 필요한 메모리 계산, `name`을 키값으로 갖는다면
        
        ```c
        typedef struct TreeNode {
            char name[12];               // 12 bytes
            struct TreeNode* left; // 8 bytes
            struct TreeNode* right;// 8 bytes
        } TreeNode;
        ```
        
    - 각 노드의 크기: `12 bytes (name) + 8 bytes (left) + 8 byte (right) = 28 bytes`
    - 노드가 **1억 개**일 경우 크기 : 약 `2,800MB`
    - 노드가 **1000만 개**일 경우 크기 : 약 `280MB`
    - 노드가 **100만 개**일 경우 크기: 약 `28MB`
    - **노드가 1억 개일 경우 모든 필드에 대해 인덱스를 운영한다면**
        - 400MB(전화번호) + 2,800MB(이름) + 11.6GB(주소) = 약 `14.8GB`
- 파일에서 구조체를 **선형적으로 검색하여 마지막 구조체를 읽어들이는데 걸리는 대략적인 시간**을 다음과 같이 계산합니다.
    
    ```c
    구조체 크기 계산
    
    typedef struct USERDATA {
        int phone;       // 4 bytes
        char name[12];   // 12 bytes
        char address[100]; // 100 bytes
    } USERDATA;
    
    // 총 116 bytes
    ```
    
    - 데이터가 **1억 개일 경우 크기**
        - `11.6 GB = 116 bytes * 100,000,000 = 11,600,000,000 bytes`
    - 디스크 I/O 속도를 가정하여 선형 검색 시간을 계산합니다.
        - **HDD**: 100 MB/s (평균적인 성능)
        - **SSD**: 500 MB/s (평균적인 성능, 고성능 SSD는 더 빠를 수 있음)
            
            ```c
            1억 개일 경우
            
            // HDD의 경우
            읽기 시간 (HDD) = 전체 파일 크기 / 디스크 속도
                            = 11,600 MB / 100 MB/s
                            = 116 seconds
            
            // SSD의 경우
            읽기 시간 (SSD) = 전체 파일 크기 / 디스크 속도
                            = 11,600 MB / 500 MB/s
                            = 23.2 seconds
            ```
            
- **요약**
    - **1억 개**:
        - 파일 크기: 11.6 GB
        - HDD: 116 초
        - SSD: 23.2 초
    - **5천만 개**:
        - 파일 크기: 5.8 GB
        - HDD: 58 초
        - SSD: 11.6 초
    - **1천만 개**:
        - 파일 크기: 1.16 GB
        - HDD: 11.6 초
        - SSD: 2.32 초
    - **백만 개**:
        - 파일 크기: 0.116 GB
        - HDD: 1.16 초
        - SSD: 0.232 초
- **데이터 캐싱 및 커밋/롤백 구현**
    - 생성, 수정, 검색된 데이터는 **메모리에 연결 리스트 형식으로 유지**하여 처리 속도를 개선했습니다.
    - 검색 시 **캐시된 리스트를 먼저 검색**합니다.
    - 향후 LRU 알고리즘과 같은 캐시 정책을 적용하여 메모리 사용량을 최적화할 **예정**입니다.
    - 캐시된 데이터를 **즉시 파일에 저장하지 않고 커밋 여부를 확인**합니다. 커밋하지 않고 **변경을 취소**할 수도 있습니다.
    - 캐시 추가 시, 중복된 key 검증을 수행합니다.
- **간단한 SQL 질의를 지원합니다.**
    - 사용자 입력을 받아 SQL 형식의 쿼리로 데이터 검색을 지원합니다.
    - and, or 연산을 지원합니다.
- **데이터 수정을 지원합니다.**
    - key값과 일치하는 데이터의 수정을 지원합니다.
    - 수정 시 캐시된 리스트 데이터도 함께 수정합니다.
    - 수정 후 인덱스를 업데이트 합니다.
- **클라이언트/서버 아키텍처**
    - 시스템 및 소켓 프로그래밍을 활용하여 클라이언트/서버 아키텍처를 구현할 예정입니다.
- **멀티스레딩 환경 지원**
    - 멀티 유저 지원을 위해 멀티스레딩 환경을 지원할 예정입니다.
- **데이터 삭제를 지원합니다.**
    - 삭제한 데이터는 실제로 지우지 않고 마킹합니다.
    - 마킹된 데이터는 출력 시 건너뜁니다.
- **디스크 조각모음을 지원합니다.**
    - 삭제된 레코드의 위치를 기록하여 파일의 끝의 데이터를 읽어와 덮어씁니다.
    - 파일 끝에 있는 레코드가 삭제된 상태일 경우 파일 크기를 조정합니다.
    - 삭제된 레코드 처리 후 파일의 크기를 조정하여 불필요한 데이터를 제거합니다.

**🌟 기여 방법**

오류를 발견하거나 개선 사항이 있을 경우, 이슈를 열거나 풀 리퀘스트를 제출해 주세요. 모든 기여는 환영합니다!

**📧 연락처**

질문이나 문의 사항이 있으시면 [vkdnjpo1512@naver.com](mailto:vkdnjpo1512@naver.com)으로 연락 주세요.

**🗝️LICENSE**

이 프로젝트는 MIT 라이선스에 따라 배포됩니다.
