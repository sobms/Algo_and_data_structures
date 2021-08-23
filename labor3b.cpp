// labor3b.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#define MAXLINES 100
#define MAXLENGTH 100
#define MAX_SIZE_INCR 100
#define MAX_BUF_LENGTH 10

void Strcat(char* a, char* b) {
	int i = strlen(a);
	while (*b != '\0') {
		a[i++] = *(b++);
	}
	a[i] = '\0';
}
void Strcat_symb(char* a, char c) {
	unsigned int len = strlen(a);
	a[len] = c;
	a[len + 1] = '\0';
}

char* getstr() {
	char buf[MAX_BUF_LENGTH] = "\0";
	int str_l = 0;
	char* new_str;
	int res;
	int length = 1;
	char c = ' ';
	new_str = (char*)malloc(MAX_BUF_LENGTH * sizeof(char));
	*new_str = '\0';
	do {
		res = scanf("%9[^\n]", buf);
		if (res == EOF) {
			new_str = NULL;
			continue;
		}
		scanf("%1c", &c);
		if (c == '\n') {
			Strcat(new_str, buf);
			Strcat_symb(new_str, c);
			res = 0;
		}
		else {
			Strcat(new_str, buf);
			Strcat_symb(new_str, c);
			str_l = strlen(new_str);
			length++;
			new_str = (char*)realloc(new_str, length * MAX_BUF_LENGTH * sizeof(char));
			new_str[str_l] = '\0';
		}

	} while (res > 0);
	return new_str;
}
struct List {
	char data;
	struct List* next;
	struct List* prev;
};
struct List* find_tail(struct List* lst) {
	if (lst == NULL) return lst;
	if (lst->prev != NULL) {
		while (lst->prev) {
			lst = lst->prev;
		}
	}
	return lst;
}
struct List* init(char c) {
	struct List* head = (struct List*)malloc(sizeof(struct List));
	head->data = c;
	head->next = NULL;
	head->prev = NULL;
	return head;
}
struct List* addElem(struct List* lst, char Data) {
	struct List* newelem = (struct List*)malloc(sizeof(struct List));
	if (lst->next != NULL) {
		while (lst->next) {
			lst = lst->next;
		}
	}
	lst->next = newelem;
	newelem->next = NULL;
	newelem->prev = lst;
	newelem->data = Data;
	return newelem;
}
void free_prev_prev(struct List* head, struct List* last) {
	if ((head->prev->prev != last) && (head->prev->prev != NULL)) {
		if ((head->prev->prev->data == ' ')&&(head->prev->prev->prev!=NULL)){
			if ((head->prev->prev->prev->data != ' ') && (head->prev->prev->prev->data != '\n') && (head->prev->prev->prev->data != '\t') && (head->prev->prev->prev->data != '\0')) {
				return;
			}
		}
		free(head->prev->prev);
	}
}
struct List* findElem(struct List* head, char FData) {
	struct List* lst = head;
	while (lst->next) {
		if (lst->data == FData) {
			return lst;
		}
		lst = lst->next;
	} return NULL;
}
/*char str_input_list(struct List *str) {
	char c;
	while ((c = getchar()) != '\0') {
		if ((c == '\n') || (c == '\x1a')) {
			addElem(str, c);
			addElem(str, '\0');
			return c;
		}
		else
			addElem(str, c);
	}
	return 0;
}*/
struct List* create_list(char* s) {
	struct List* new_str;
	if (*s != '\0') {
		new_str = init(*s);
		s++;
	}
	else {
		return init(*s);
	}
	while (*s != '\0') {
		new_str = addElem(new_str, *s);
		s++;
	}
	new_str = find_tail(new_str);
	return new_str;
}
void print_list(struct List* lst) {
	struct List* head = find_tail(lst);
	if (head == NULL) return head;
	while (head->next) {
		printf("%c", head->data);
		head = head->next;
	}
	printf("%c", head->data);
}
struct List* change_list(struct List* lst) {
	struct List* head = find_tail(lst);
	struct List* first_elem = NULL;
	struct List* last = NULL;
	while (head->next) {
		if ((head->next->data == ' ') || (head->next->data == '\n') || (head->next->data == '\t')) {
			if ((head->data != ' ') && (head->data != '\n') && (head->data != '\t') && (head->data != '\0')) {
				first_elem = head;
				if (last != NULL) {
					last->next = NULL;
					last = addElem(last, ' ');
					last->next = first_elem;
					free(first_elem->prev);
					first_elem->prev = last;
					last = first_elem;

				}
				else {
					free(first_elem->prev);
					first_elem->prev = last;
					last = first_elem;
				}
			}
		}
		head = head->next;
		free_prev_prev(head, last); 
	}
	if (first_elem != NULL) {
		first_elem->next = NULL;
	}
	else return first_elem;
	first_elem = find_tail(first_elem);
	return first_elem;
}
void Clear(struct List* lst) {
	struct List* that = find_tail(lst);
	struct List* last;
	if (that == NULL) return that;
	while (that->next) {
		last = that;
		that = that->next;
		free(last);
	}
	free(that);
}
int main()
{
	//struct List* str[MAXLINES];
	char* s = '\0';
	int i = 0;
	struct List* one_str = NULL;

	while ((s = getstr()) != NULL) {
		one_str = create_list(s);
		one_str = change_list(one_str);
		print_list(one_str);
		printf("!\n");
		Clear(one_str);
		i++;
	}
	
	return 0;
}
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
