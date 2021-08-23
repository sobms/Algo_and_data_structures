#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <Windows.h>
#include <ctype.h>
#include <conio.h>
#include <string.h>
#include <string>
#include <iostream>

using namespace std;
char* file_name;
struct Node {
	int key;
	char* info;
	int len;
	struct Node* parent;
	struct Node* left;
	struct Node* right;
	struct Node* thread;
};
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
struct Node* Find_next_more(struct Node* ptr) {
	if (ptr->right) {
		ptr = ptr->right;
		while (ptr->left)
			ptr = ptr->left;
		return ptr;
	} 
	else {
		 while (ptr->parent && (ptr->parent->right == ptr)) {
				ptr = ptr->parent;
		 }
		 if (ptr->parent && (ptr->parent->left == ptr)) {
			 return ptr->parent;
		 }
		 else return NULL;
	}
}
struct Node* Find_next_less(struct Node* ptr) {
	if (ptr->left) {
		ptr = ptr->left;
		while (ptr->right)
			ptr = ptr->right;
		return ptr;
	}
	else {
		while (ptr->parent && (ptr->parent->left == ptr)) {
			ptr = ptr->parent;
		}
		if (ptr->parent && (ptr->parent->right == ptr)) {
			return ptr->parent;
		}
		else return NULL;
	}
}
struct Node* Insert_elem(struct Node* root, FILE* file) {
	struct Node* elem = (struct Node*)malloc(sizeof(struct Node));
	scanf("%d", &elem->key);
	elem->info = getstr();
	elem->left = NULL;
	elem->right = NULL;
	elem->len = strlen(elem->info) + 2;
	void record_in_file(struct Node* ptr, FILE* file);

	if (!root) {
		root = elem;
		elem->parent = NULL;
		elem->thread = NULL;
		record_in_file(elem, file);
		return root;
	}
	struct Node* ptr = root;
	struct Node* parent = NULL;
	while (ptr) {
		parent = ptr;
		if (elem->key < ptr->key) {
			ptr = ptr->left;
		}
		else if (elem->key > ptr->key) {
			ptr = ptr->right;
		}
		else if (elem->key == ptr->key) {
			printf("Key already exist!");
			return root;
		}
	}
	if (parent->key > elem->key) {
		elem->parent = parent;
		parent->left = elem;
		elem->thread = Find_next_more(elem);
		ptr = Find_next_less(elem);
		if (ptr)
			ptr->thread = elem;
	}
	else {
		elem->parent = parent;
		parent->right = elem;
		elem->thread = Find_next_more(elem);
		ptr = Find_next_less(elem);
		if (ptr)
			ptr->thread = elem;
	}
	record_in_file(elem, file);
	return root;
}
struct Node* Search_elem(struct Node* root, int key) {
	struct Node* ptr = root;
	if (!ptr) {
		return NULL;
	}
	while (ptr) {
		if (ptr->key < key) {
			ptr = ptr->right;
		}
		else if (ptr->key > key) {
			ptr = ptr->left;
		}
		else if (ptr->key == key) {
			return ptr;
		}
	}
	return NULL;
}
struct Node* Deletion (struct Node* ptr, struct Node* root) {
	struct Node *y,*p;
	struct Node* par;
	struct Node* subtr;
	void refresh_file(struct Node* ptr);
	if (!ptr->left || !ptr->right) {
		y = ptr;
	}
	else {
		y = Find_next_more(ptr);
	}
	if (y->left) {
		subtr = y->left;
	}
	else {
		subtr = y->right;
	}
	par = y->parent;
	p = Find_next_less(y);
	if (subtr)              // replace link from subtree to parent node
		subtr->parent = par;
	if (!par) {              // delete root
		if (p)
			p->thread = y->thread;
		free(y);
		root = subtr;
		refresh_file(root);
		return root;
	}
	else {
		if (par->left == y)
			par->left = subtr;
		else
			par->right = subtr;
	}
	ptr->info = y->info;
	ptr->key = y->key;
	if (p)
		p->thread = y->thread;
	free(y);
	refresh_file(root);
	return root;
}
void Print_list(struct Node* root) {
	struct Node* ptr = root;
	if (!ptr) {
		printf("No elements\n");
		return;
	}
	while (ptr->left)
		ptr = ptr->left;
	printf("*-------*-----------------*\n");
	while (ptr) {
		printf("| %5d | %15s |\n", ptr->key, ptr->info);
		printf("*-------*-----------------*\n");
		ptr = ptr->thread;
	}

}
void Color_str(int c_background, int c_text) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((c_background << 4) | c_text));
}
void print_main_menu(string text[], int i) {
	for (int j = 0; j < i; j++)
		cout << text[j];
	Color_str(6, 0);
	cout << text[i];
	Color_str(0, 15);
	for (int j = i + 1; j < 6; j++)
		cout << text[j];
}
void SearchWeb(string word)
{
	string base_URL = "http://chart.apis.google.com/chart?cht=gv&chl=";
	string search_URL = "dummy";
	search_URL = base_URL + word;

	cout << "Searching for: \"" << word << "\"\n";

	ShellExecuteA(NULL, "open", search_URL.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
string bypass(struct Node* ptr, string str, struct Node* par) {
	if (!ptr) {
		return str + "null" + "--" + to_string(par->key) + ";";
	}
	str = bypass(ptr->left,str, ptr);
	str = bypass(ptr->right,str, ptr);
	if (ptr->parent)
		return str + to_string(ptr->key) + "--" + to_string(ptr->parent->key) + ";";
	else return str;
}
string Create_request(struct Node* root) {
	struct Node* ptr = root;
	string str = "graph graphname {";
	if (!ptr) {
		printf("No elements\n");
		return "";
	}
	while (ptr->left)
		ptr = ptr->left;
	while (ptr) { // create list of points
		str = str + to_string(ptr->key) + ";";
		ptr = ptr->thread;
	}
	ptr = root;
	str = bypass(root, str, root->parent);
	if (str != "")
		return str + "}";
	else return "";

}
void record_in_file(struct Node* ptr, FILE* file) {
    file = fopen(file_name, "ab");
	fseek(file, 0, SEEK_END);
	fwrite(&ptr->len, sizeof(int), 1, file);
	fwrite(&ptr->key, sizeof(int), 1, file);
	fwrite(ptr->info, sizeof(char), strlen(ptr->info), file);
	fclose(file);
}
void bypass_for_refresh(struct Node* ptr, FILE* file) {
	if (!ptr) {
		return;
	}
	record_in_file(ptr, file);
	bypass_for_refresh(ptr->left, file);
	bypass_for_refresh(ptr->right, file);
}
FILE* Open_file(struct Node** root) {
	FILE* iofile = NULL;
	printf("%s", "Enter path to your file\n");
	file_name = getstr();
	//
	iofile = fopen(file_name, "rb");
	if (iofile == NULL) {
		iofile = fopen(file_name, "wb");
		if (iofile == NULL) {
			printf("Error opening file");
			_getch();
			exit(1);
		}
	}
	else {
		// чтение данных в оперативную память
		struct Node* read_from_file(struct Node* root, FILE * file);
		*root = read_from_file(*root, iofile);
	}

	system("cls");
	return iofile;
}
void refresh_file(struct Node* ptr) {
	FILE* file = fopen(file_name, "wb");
	bypass_for_refresh(ptr,file);
	fclose(file);
}
struct Node* read_from_file(struct Node* root, FILE* file) {

	int len;

	fread(&len, sizeof(int), 1, file);

	while (len) {
		struct Node* elem = (struct Node*)malloc(sizeof(struct Node));
		elem->len = len;
		fread(&elem->key, sizeof(int), 1, file);
		elem->info = (char*)malloc(sizeof(char) * (len - 2));
		fread(elem->info, sizeof(char), len - 2, file);
		elem->info[(len - 2)] = '\0';
		elem->left = NULL;
		elem->right = NULL;

		if (!root) {
			root = elem;
			elem->parent = NULL;
			elem->thread = NULL;
		}
		else
		{
			struct Node* ptr = root;
			struct Node* parent = NULL;
			while (ptr) {
				parent = ptr;
				if (elem->key < ptr->key) {
					ptr = ptr->left;
				}
				else if (elem->key > ptr->key) {
					ptr = ptr->right;
				}
			}
			if (parent->key > elem->key) {
				elem->parent = parent;
				parent->left = elem;
				elem->thread = Find_next_more(elem);
				ptr = Find_next_less(elem);
				if (ptr)
					ptr->thread = elem;
			}
			else {
				elem->parent = parent;
				parent->right = elem;
				elem->thread = Find_next_more(elem);
				ptr = Find_next_less(elem);
				if (ptr)
					ptr->thread = elem;
			}
		}
		fread(&len, sizeof(int), 1, file);
		if (feof(file))
			break;
	}
	fclose(file);
	return root;
}
void Main_Menu(struct Node* root) {
	int i = 0;
	int key;
	FILE* file = NULL;
	struct Node* felem;
	char console_input;
	file = Open_file(&root);
	string text[] = { "ADD_ELEMENT\n", "FIND_ELEMENT\n", "DELETE_ELEMENT\n", "PRINT_TABLE\n", "PRINT_AS_TREE\n", "TIMING\n" };
	print_main_menu(text, 0);
	while ((console_input = _getch()) != '\x1b') {
		if (console_input == 's') {
			i++;
			if (i >= 6) {
				i = 5;
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
				printf("Enter new element\n");
				root = Insert_elem(root, file);
			}
			if (i == 1) {
				system("cls");
				printf("Enter key of element to search it\n");
				scanf("%d", &key);
				felem = Search_elem(root, key);
				if (!felem) {
					printf("Not found");
				} 
				else {
					printf("| %5d | %15s |\n", felem->key, felem->info);
					_getch();
				}
			}
			if (i == 2) {
				printf("Enter key of element to delete it\n");
				scanf("%d", &key);
				felem = Search_elem(root, key);
				if (!felem) {
					printf("Not exist");
				} 
				else {
					root = Deletion(felem, root);
				}
			}
			if (i == 3) {
				Print_list(root);
				_getch();
			}
			if (i == 4) {
				string str = Create_request(root);
				if (str == "")
					return;
				SearchWeb(str);
			}
			if (i == 5) {
				printf("timing...\n");
				_getch();
			}
			system("cls");
			print_main_menu(text, i);
		}
	}
}
int main()
{
	struct Node* root = NULL;
	struct Node* felem;
	int key;
	Main_Menu(root);
}