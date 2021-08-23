#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <Windows.h>
#include <conio.h>
#include <ctype.h>
#define HASH_MAS_SIZE 10
#define MAX_SIZE 100
struct Item {                // элемент таблицы
	int key1;
	int key2;
	char* info;
};
struct Hash_key {     // структура под массив производных ключей
	struct key_space_2* head;
};
struct key_space_2 {          // структура элементов пространства2
	struct key_space_2* next;
	struct Item* elem;
};
struct key_space_1 {           // структура элементов пространства1
	struct Item* elem;
};
int Hash(int key) {
	return key % 10;
}

void Strcat_symb(char* a, char c) {
	unsigned int len = strlen(a);
	a[len] = c;
	a[len + 1] = '\0';
}
char* getstr() {
	char buf[10] = "\0";
	int str_l = 0;
	char* new_str;
	int res;
	int length = 1;
	char c = ' ';
	new_str = (char*)malloc(10 * sizeof(char));
	*new_str = '\0';
	do {
		res = scanf("%9[^\n]", buf);
		if (res == EOF) {
			new_str = NULL;
			continue;
		}
		scanf("%1c", &c);
		if (c == '\n') {
			strcat(new_str, buf);
			res = 0;
		}
		else {
			strcat(new_str, buf);
			Strcat_symb(new_str, c);
			str_l = strlen(new_str);
			length++;
			new_str = (char*)realloc(new_str, length * 10 * sizeof(char));
			new_str[str_l] = '\0';
		}
	} while (res > 0);
	return new_str;
}

int len_mas1 = 0;
struct Item* Find(struct key_space_1 key_mas1[], struct Hash_key hash_mas[]) {
	printf("Enter '1' to choose key type 1 and '2' to choose key type 2\n");
	char key_type;
	int key;
	struct key_space_2* head;
	key_type = _getch();
	if (key_type == '\x1b')
		return NULL;
	printf("Enter key\n");
	//_getch();
	scanf("%d", &key);
	if (key_type == '1') {
		for (int i = 0; (i < len_mas1); i++) {

			if (key_mas1[i].elem->key1 == key) {
				return key_mas1[i].elem;
			}
		}
		return NULL;
	}
	else if (key_type == '2') {
		head = hash_mas[Hash(key)].head;
		if (head) {
			while (head) {
				if (head->elem->key2 == key) {
					return head->elem;
				}
				head = head->next;
			}
		}
		return NULL;
	}
	else {
		printf("Incorrect data");
		return NULL;
	}
}
int Is_elem_in_table(struct key_space_1 key_mas1[], int key1,int key2) {
	for (int i = 0; (i < len_mas1); i++) {

		if ((key_mas1[i].elem->key1 == key1) && (key_mas1[i].elem->key2 == key2)) {
			return true;
		}
	}
	return false;
}
void Add_in_key_space_1(struct key_space_1 key_mas1[], struct Item* item, struct Hash_key hash_mas[]) {
	int i = len_mas1 - 1;
	while ((i >= 0) && ((key_mas1)[i].elem->key1 > item->key1)) {
		key_mas1[i + 1] = key_mas1[i];
		i--;
	}
	(key_mas1)[i + 1].elem = item;
	len_mas1++;
}
void Add_in_key_space_2(struct key_space_1 key_mas1[], struct Hash_key hash_mas[], struct Item* item) {
	if (hash_mas[Hash(item->key2)].head == NULL) {
		struct key_space_2* node = (struct key_space_2*)malloc(sizeof(struct key_space_2));
		node->elem = item;
		node->next = NULL;
		hash_mas[Hash(item->key2)].head = node;
	} else {
		struct key_space_2* node = (struct key_space_2*)malloc(sizeof(struct key_space_2));
		node->next = hash_mas[Hash(item->key2)].head;
		node->elem = item;
		hash_mas[Hash(item->key2)].head = node;
			}
}
void Add_new_elem(struct key_space_1* key_mas1, struct Hash_key hash_mas[]) {
	struct Item* item = (struct Item*)malloc(sizeof(struct Item));
	scanf("%d %d", &item->key1, &item->key2);
	item->info = getstr();
	if (Is_elem_in_table(key_mas1, item->key1, item->key2)) {
		printf("Key is already in table\n");
		free(item);
		return;
	}
	Add_in_key_space_1(key_mas1, item,hash_mas);
	Add_in_key_space_2(key_mas1, hash_mas, item);
}
int Binary_Search(struct key_space_1* key_mas1, int key1) {
	int l = 0;
	int r = len_mas1 - 1;
	int middle;
	while (l <= r) {
		middle = (l + r) / 2;
		if (key_mas1[middle].elem->key1 == key1)
			return middle;
		else if (key_mas1[middle].elem->key1 < key1) {
			l = middle + 1;
		}
		else if (key_mas1[middle].elem->key1 > key1) {
			r = middle - 1;
		}
	}
	return -1;
}
int Deletion_from_space1(struct key_space_1* key_mas1, int key1,int flag) {
	int ind = Binary_Search(key_mas1, key1);
	int key2 = -1;
	if (ind=='-1') {
		return key2;
	}
	key2 = key_mas1[ind].elem->key2;
	len_mas1--;
	int i;
	for (i = ind; i < len_mas1; i++) {
		key_mas1[i] = key_mas1[i + 1];
	}
	key_mas1[i].elem = NULL;
	//free(key_mas1 + i-1);
	return key2;
}
int Deletion_from_space2(struct Hash_key hash_mas[], int key2, int flag) {
	struct key_space_2* head = hash_mas[Hash(key2)].head;
	struct key_space_2* prev = head;
	int key1 = -1;
	while (head) {
		if (head->elem->key2 == key2) {
			int key1 = head->elem->key1;
			prev->next = head->next;
			if (head == hash_mas[Hash(key2)].head) {
				if (flag == true)
					free(head->elem);
				hash_mas[Hash(key2)].head = NULL;
				return key1;
			}
			else {
				if (flag==true)
					free(head->elem);
				prev->next = head->next;
				head = NULL;
				return key1;
			}
		}
		prev = head;
		head = head->next;
	}
	return key1;
}
void Deletion(struct key_space_1* key_mas1, struct Hash_key hash_mas[]) {
	char key_type;
	int key1, key2;
	int key = 0;
	printf("Enter '1' to choose key type 1 and '2' to choose key type 2\n");
	key_type = _getch();
	printf("Enter key\n");
	scanf("%d", &key);
	if (key_type == '1'){
		key2 = Deletion_from_space1(key_mas1, key,false);
		if (key2 == -1) {
			return;
		}
		Deletion_from_space2(hash_mas, key2,true);
	}
	if (key_type == '2') {
		key1 = Deletion_from_space2(hash_mas, key,false);
		if (key1 == -1) {
			return;
		}
		Deletion_from_space1(key_mas1, key1,true);
	}
}
Print_table1(struct key_space_1* key_mas1){
	printf("*-------*-------*-----------------*\n");
	printf("|  key1 |  key2 |            info |\n");
	printf("*-------*-------*-----------------*\n");
	for (int i = 0; i < len_mas1; i++) {
		printf("| %5d | %5d | %15s |\n", key_mas1[i].elem->key1, key_mas1[i].elem->key2, key_mas1[i].elem->info);
		printf("*-------*-------*-----------------*\n");
	}
}

void Color_str(int c_background, int c_text) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((c_background << 4) | c_text));
}

void print_main_menu(char** text, int i) {
	for (int j = 0; j < i; j++)
		printf(*(text + j));
	Color_str(3, 0);
	printf(*(text + i));
	Color_str(0, 15);
	for (int j = i + 1; j < 4; j++)
		printf(*(text + j));
}
void Main_Menu(struct key_space_1* key_mas1, struct Hash_key hash_mas[]) {
	struct Item* f_elem;

	int i = 0;
	char console_input;
	const char* text[] = { "ADD_ELEMENT\n", "FIND_ELEMENT\n", "DELETE_ELEMENT\n", "PRINT_TABLE\n" };
	print_main_menu(text, 0);
	while ((console_input = _getch()) != '\x1b') {
		if (console_input == 's') {
			i++;
			if (i >= 4) {
				i = 3;
			}
			system("cls");
			print_main_menu(text, i);
		}
		if (console_input == 'w') {
			i--;
			if (i <= 0) {
				i = 0;
			}
			system("cls");
			print_main_menu(text, i);
		}
		if (console_input == '\r') {
			system("cls");
			if (i == 0) {
				Add_new_elem(key_mas1, hash_mas);
			}
			if (i == 1) {
				f_elem = Find(key_mas1, hash_mas);
				printf("| %5d | %5d | %15s |\n", f_elem->key1, f_elem->key2, f_elem->info);
				_getch();
			}
			if (i == 2) {
				Deletion(key_mas1, hash_mas);
			}
			if (i == 3) {
				Print_table1(key_mas1);
				_getch();
			}
			system("cls");
			print_main_menu(text, i);
		}
	}
}
int main()
{
	struct Item* it;
	struct key_space_1 key_mas1[MAX_SIZE];
	for (int i = 0; i < MAX_SIZE; i++) {
		key_mas1[i].elem = NULL;
	}
	struct Hash_key hash_mas[HASH_MAS_SIZE];
	for (int i = 0; i < HASH_MAS_SIZE; i++) {
		hash_mas[i].head = NULL;
	}
	/*for (int i = 0; i < 3; i++) {
		Add_new_elem(key_mas1, hash_mas);
		printf("%d", key_mas1[0].elem->key1);
		printf("\n");
	}*/

	/*it = Find(key_mas1, hash_mas);
	if (it) {
		printf("%d", it->key1);
		printf("\n");
	}
	Deletion(key_mas1, hash_mas);*/
	Main_Menu(key_mas1, hash_mas);
	return 0;
}
