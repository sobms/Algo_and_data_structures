// llabor3b.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINES 100
#define MAXLENGTH 100
#define MAX_SIZE_INCR 100
#define MAX_BUF_LENGTH 10
void Strcpy(char* a, char* b) {
	for (; (*(a++) = *(b++)) != '\0';);
}
void Strcat_symb(char* a, char c) {
	unsigned int len = strlen(a);
	a[len] = c;
	a[len + 1] = '\0';
}

void Strncat(char* a, char* b, int n) {
	int i = strlen(a);
	while ((*b != '\0') && (n > 0)) {
		*(a + i) = *b;
		b++;
		i++;
		n--;
	}
	*(a + i) = '\0';
}
void Strcat(char* a, char* b) {
	int i = strlen(a);
	while (*b != '\0') {
		a[i++] = *(b++);
	}
	a[i] = '\0';
}

void change_str(char* str) {
	char str2[MAXLENGTH] = "\0";
	char* str_save = str;
	int i = 0;
	int firs_word = 1;
	while ((i = strcspn(str, "\n \t")) != strlen(str)) {
		if ((*(str + i - 1) != ' ') && (*(str + i - 1) != '\n') && (*(str + i - 1) != '\t') && (*(str + i - 1) != '\0') && (i != 0))
		{
			if (firs_word == 0) {
				strcat(str2, " ");
			}
			else { firs_word = 0; }
			strncat(str2, str + i - 1, 1);
			str = (str + i);
		}
		str++;
	}
	str = str_save;
	strcpy(str, str2);
}

char* getstr() {
	char buf[MAX_BUF_LENGTH] = "\0";
	int str_l = 0;
	char* new_str;
	int res;
	int length = 1;
	char c = ' ';
	new_str = (char*)malloc((MAX_BUF_LENGTH+5) * sizeof(char));
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
			Strcat_symb(new_str, c);
			res = 0;
		}
		else {
			strcat(new_str, buf);
			Strcat_symb(new_str, c);
			str_l = strlen(new_str);
			length++;
			new_str = (char*)realloc(new_str, length * (MAX_BUF_LENGTH+5) * sizeof(char));
			new_str[str_l] = '\0';
		}
	} while (res > 0);
	return new_str;
}
int  main()
{
	char c = 'r';
	int i = 0;
	char* str;
	while ((str = getstr()) != NULL) {
		change_str(str);
		printf("%s!\n", str);
		free(str);
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
