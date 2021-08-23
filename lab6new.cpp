#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <Windows.h>
#include <ctype.h>
#include <conio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <time.h>
#include <ctime>
#include <limits.h>
#include <math.h>
#include <gvc.h>
#include <cgraph.h>
using namespace std;
char* file_name;

struct Ribs {
	float weight;
	struct Top* top;
	struct Ribs* next;
};
struct Top {
	char* name;
	int x;
	int y;
	//for Dijikstra
	float dist;
	struct Top* prev;
	// for Dijkstra
	struct Ribs* ribs;
};
struct Graph {
	struct Top* top;
	struct Graph* next;
};
clock_t time1, time2;
int Output_graph(struct Graph* graph) {
	GVC_t* gvc;
	gvc = gvContext();

	Agraph_t* G = agopen((char*)"graph", Agdirected,0);
	struct Graph* ptr = graph;
	while (ptr) {
		Agnode_t* node = agnode(G, ptr->top->name, 1);
		agsafeset(node, (char*)"color", (char*)"red", (char*)"");
		agsafeset(node, (char*)"pos", (char*)"(0,0)", (char*)"");
		if (ptr->top->ribs) {
			struct Ribs* rib = ptr->top->ribs;
			while (rib) {
				Agedge_t* edge = agedge(G, node, agnode(G, rib->top->name, 1), &to_string(rib->weight)[0], 1);
				agsafeset(edge, (char*)"color", (char*)"red", (char*)"");
				agsafeset(edge, (char*)"label", &(to_string((int)rib->weight)[0]), (char*)"");
				rib = rib->next;
			}
		}
		ptr = ptr->next;
	}
	gvLayout(gvc, G, "dot");
	gvRenderFilename(gvc, G, "png", "out.png");
	gvFreeLayout(gvc, G);
	agclose(G);
	return (gvFreeContext(gvc));
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
struct Graph* Add_top(struct Graph* graph, struct Top* top, FILE* file = NULL) {
	struct Graph* new_top = (struct Graph*)malloc(sizeof(struct Graph));
	void record_top_in_file(FILE * file, struct Top* top);
	new_top->top = top;
	new_top->next = NULL;
	if (!graph) { // если граф пустой;
		graph = new_top;
		if (file)
			record_top_in_file(file, top);
	}
	else {
		struct Graph* ptr = graph;
		while (ptr->next) {
			ptr = ptr->next;
		}
		ptr->next = new_top;
		if (file)
			record_top_in_file(file, top);
	}

	return graph;
}
struct Top* Find_top(struct Graph* graph, char* name) {
	if (!graph) { // если граф пустой;
		//printf("No tops exist\n");
		return NULL;
	}
	else {
		struct Graph* ptr = graph;
		while (ptr->next) {
			if (strcmp(ptr->top->name, name) == 0) {
				return ptr->top;
			}
			ptr = ptr->next;
		}
		if (strcmp(ptr->top->name, name) == 0) {
			return ptr->top;
		}
	}
	return NULL;
}
struct Graph* Add_rib(struct Graph* graph, struct Top* top_from, struct Top* top_to, FILE* file = NULL) {
	struct Ribs* rib = (struct Ribs*)malloc(sizeof(struct Ribs));
	void record_rib_in_file(FILE * file, char* top_name_from, char* top_name_to);
	rib->top = top_to;
	rib->weight = sqrt(pow((top_from->x - top_to->x), 2) + pow((top_from->y - top_to->y), 2));
	if (!top_from->ribs) {
		top_from->ribs = rib;
		rib->next = NULL;
	}
	else {
		struct Ribs* ptr = top_from->ribs;
		while (ptr->next) {
			if (ptr->top->name == rib->top->name) {
				return NULL;
			}
			ptr = ptr->next;
		}
		if (ptr->top->name == rib->top->name) {
			return NULL;
		}
		rib->next = top_from->ribs;
		top_from->ribs = rib;
	}
	if (file)
		record_rib_in_file(file, top_from->name, top_to->name);
	return graph;
}
void Clear_ribs_list(struct Ribs* ptr) {
	struct Ribs* prev = ptr;
	if (!ptr)
		return;
	while (ptr->next) {
		prev = ptr;
		ptr = ptr->next;
		free(prev);
	}
	free(ptr);
}
struct Graph* delete_top(struct Graph* graph, struct Top* del_top, FILE* file = NULL) {
	struct Graph* ptr2 = graph;
	struct Graph* prev2 = NULL;
	int found = 0;
	while (ptr2) {

		if (ptr2->top->ribs) {
			struct Ribs* ptr = ptr2->top->ribs;
			struct Ribs* prev = NULL;
			while (ptr->next) {
				if (ptr->top == del_top) {
					if (prev)
						prev->next = ptr->next;
					else if (!prev)
					{
						ptr2->top->ribs = ptr->next;
					}
				}

				prev = ptr;
				ptr = ptr->next;
			}
			if (ptr->top == del_top) {
				if (!prev) {
					ptr2->top->ribs = ptr->next;
				}
				else {
					if (prev)
						prev->next = ptr->next;
				}
			}
		}
		if (ptr2->top == del_top) {
			found = 1;
			ptr2 = ptr2->next;
		}
		else {
			if (!found)
				prev2 = ptr2;
			ptr2 = ptr2->next;
		}
	}
	if (!prev2) {
		ptr2 = graph;
		graph = graph->next;
		Clear_ribs_list(ptr2->top->ribs);
		free(ptr2->top);
		free(ptr2);
		ptr2 = graph;
	}
	else {
		ptr2 = prev2->next;
		prev2->next = ptr2->next;
		prev2 = ptr2;
		ptr2 = ptr2->next;
		Clear_ribs_list(prev2->top->ribs);
		free(prev2->top);
		free(prev2);
	}
	void refresh_file(struct Graph* graph);
	if (file)
		refresh_file(graph);
	return graph;
}
void Color_str(int c_background, int c_text) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((c_background << 4) | c_text));
}
void print_main_menu(string text[], int i) {
	for (int j = 0; j < i; j++)
		cout << text[j];
	Color_str(5, 0);
	cout << text[i];
	Color_str(0, 15);
	for (int j = i + 1; j < 7; j++)
		cout << text[j];
}
struct Graph* graph_generation(struct Graph* graph, int num_tops, int num_ribs, FILE* file = NULL) {
	srand(time(NULL) + clock() + rand());
	for (int i = 0; i < num_tops;) {
		struct Top* top = (struct Top*)malloc(sizeof(struct Top));
		char* num = (char*)malloc(sizeof(char) * 6);
		top->x = rand() % 100;
		top->y = rand() % 100;
		top->ribs = NULL;
		top->name = _itoa(i, num, 10);
		/*if (Find_top(graph, top->name)) {
			free(num);
			free(top);
			continue;
		}
		else {*/
		graph = Add_top(graph, top, file);
		i++;
		//}
	}
	char* top_name_from = (char*)malloc(sizeof(char) * 6);
	char* top_name_to = (char*)malloc(sizeof(char) * 6);
	for (int i = 0; i < num_ribs;) {
		_itoa(rand() * rand() % (num_tops), top_name_from, 10);
		_itoa(rand() * rand() % (num_tops), top_name_to, 10);
		struct Top* top_from = Find_top(graph, top_name_from);
		struct Top* top_to = Find_top(graph, top_name_to);
		/*if (!top_from || !top_to) {
			continue;
		}
		else {*/
		struct Graph* new_graph = Add_rib(graph, top_from, top_to, file);
		if (new_graph) {
			graph = new_graph;
			i++;
		}
		else {
			srand(time(NULL) + clock() + rand());
			continue;

		}

		//}

	}
	return graph;
}

int graph_len(struct Graph* graph) {
	int len = 0;
	struct Graph* ptr = graph;
	while (ptr) {
		ptr = ptr->next;
		len++;
	}
	return len;
}
int queue_len(struct Top** mas, int graph_len) {
	int queue_len = 0;
	for (int i = 0; i < graph_len; i++) {
		if (mas[i])
			queue_len++;
	}
	return queue_len;
}

void swap(void* a, void* b) {
	void* c = a;
	a = b;
	b = c;
}

struct Top* Extract_Min(struct Graph* graph, struct Top** mas) {
	struct Top* min_top = NULL;
	int min_ind;
	for (int i = 0; i < graph_len(graph); i++) {
		if (!mas[i]) {
			continue;
		}
		if ((!min_top) || (mas[i]->dist < min_top->dist)) {
			min_top = mas[i];
			min_ind = i;

		}
	}
	mas[min_ind] = NULL;
	return min_top;
}
int Is_in_used(struct Graph* graph, struct Top* used, struct Top* neib_top) {
	for (int i = 0; i < graph_len(graph); i++) {
		if (used[i].name == neib_top->name) {
			return true;
		}
	}
	return false;
}
void print_way(struct Top* u) {
	if (u->prev == u) {
		printf("%s", u->name);
	}
	else if (u->prev == NULL) {
		return;
	}
	else {
		print_way(u->prev);
		printf("%s ", u->name);
	}
}
int Dijkstra(struct Graph* graph, struct Top* s, struct Top* end, int test = FALSE) {
	struct Top** mas = (struct Top**)malloc(sizeof(struct Top*) * graph_len(graph));
	struct Top* used = (struct Top*)malloc(sizeof(struct Top) * graph_len(graph));
	struct Graph* ptr = graph;
	int i = 0;
	time1 = clock();
	while (ptr) {
		// доинициализация вершин
		ptr->top->dist = INFINITY;
		ptr->top->prev = NULL;
		// инициализация s
		if (ptr->top->name == s->name) {
			ptr->top->dist = 0;
		}
		// заполнение mas 
		mas[i] = ptr->top;
		i++;
		ptr = ptr->next;
	}

	i = 0;
	int queue_length = queue_len(mas, graph_len(graph));
	while (queue_length) {
		struct Top* new_top = Extract_Min(graph, mas); // extract new top
		queue_length--;
		used[i] = *new_top; //add top in used
		i++;
		struct Ribs* rib = new_top->ribs;//choose rib to neibour top
		while (rib) {     // renew distances
			struct Top* neib_top = rib->top;
			if (!Is_in_used(graph, used, neib_top)) {
				if (neib_top->dist > new_top->dist + rib->weight) {
					neib_top->dist = new_top->dist + rib->weight;
					neib_top->prev = new_top;
				}
			}
			rib = rib->next;
		}
	}
	time2 = clock();
	// output
	if (!test) {
		printf("The shortes way from %s to %s is:\n", s->name, end->name);
		print_way(end);
		printf("\nits length = %f", end->dist);
	}
	else if (test) {
		return true;
	}
	// clear	
	free(used);
	for (int i = 0; i < graph_len(graph); i++) {
		free(mas[i]);
	}
}
FILE* Open_file(struct Graph** graph) {
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
		struct Graph* read_from_file(struct Graph* graph, FILE * file);
		*graph = read_from_file(*graph, iofile);
	}

	system("cls");
	return iofile;
}
void record_rib_in_file(FILE* file, char* top_name_from, char* top_name_to) {
	file = fopen(file_name, "ab");
	int len1 = strlen(top_name_from);
	int len2 = strlen(top_name_to);
	int type = 2; // type - rib;
	fseek(file, 0, SEEK_END);
	fwrite(&type, sizeof(int), 1, file);
	fwrite(&len1, sizeof(int), 1, file);
	fwrite(&len2, sizeof(int), 1, file);
	fwrite(top_name_from, sizeof(char), len1, file);
	fwrite(top_name_to, sizeof(char), len2, file);
	fclose(file);
}
void record_top_in_file(FILE* file, struct Top* top) {
	file = fopen(file_name, "ab");
	int type = 1; // type - top
	fseek(file, 0, SEEK_END);
	int len = strlen(top->name);
	fwrite(&type, sizeof(int), 1, file);
	fwrite(&top->x, sizeof(int), 1, file);
	fwrite(&top->y, sizeof(int), 1, file);
	fwrite(&len, sizeof(int), 1, file);
	fwrite(top->name, sizeof(char), len, file);
	fclose(file);
}
void refresh_file(struct Graph* graph) {
	FILE* file = fopen(file_name, "wb");
	struct Graph* ptr = graph;
	while (ptr) {
		record_top_in_file(file, ptr->top);
		ptr = ptr->next;
	}
	ptr = graph;
	while (ptr) {
		if (ptr->top->ribs) {
			struct Ribs* rib = ptr->top->ribs;
			while (rib) {
				record_rib_in_file(file, ptr->top->name, rib->top->name);
				rib = rib->next;
			}
		}
		ptr = ptr->next;
	}
	fclose(file);
}
struct Graph* read_from_file(struct Graph* graph, FILE* file) {
	int len1, len2, len;
	int type;
	fread(&type, sizeof(int), 1, file);
	while (type) {
		if (type == 1) {
			struct Top* top = (struct Top*)malloc(sizeof(struct Top));
			fread(&top->x, sizeof(int), 1, file);
			fread(&top->y, sizeof(int), 1, file);
			fread(&len, sizeof(int), 1, file);
			top->name = (char*)malloc(sizeof(char) * len);
			fread(top->name, sizeof(char), len, file);
			top->name[len] = '\0';
			top->ribs = NULL;
			graph = Add_top(graph, top);
		}
		else if (type == 2) {
			fread(&len1, sizeof(int), 1, file);
			fread(&len2, sizeof(int), 1, file);

			char* top_name_from = (char*)malloc(sizeof(char) * len1);
			fread(top_name_from, sizeof(char), len1, file);
			top_name_from[len1] = '\0';

			char* top_name_to = (char*)malloc(sizeof(char) * len2);
			fread(top_name_to, sizeof(char), len2, file);
			top_name_to[len2] = '\0';

			struct Top* top_from = Find_top(graph, top_name_from);
			struct Top* top_to = Find_top(graph, top_name_to);
			if ((top_from) && (top_to))
				graph = Add_rib(graph, top_from, top_to);
		}
		fread(&type, sizeof(int), 1, file);
		if (feof(file))
			break;
	}
	fclose(file);
	return graph;
}
void Timing() {
	struct Graph* graph2 = NULL;
	clock_t start_t, finish_t;
	struct Top s[50];
	struct Top end[50];
	struct Graph* ptr;
	srand(time(NULL));
	int n = 10, num_tops = 50, num_ribs = 50;
	while (n-- > 0) {
		srand(time(NULL) + clock() + rand());
		graph2 = graph_generation(graph2, num_tops, num_ribs);

		for (int i = 0; i < 50; ) {
			char* s_name = (char*)malloc(sizeof(char) * 6);
			_itoa(rand() % (num_tops + num_tops), s_name, 10);
			ptr = graph2;
			while (ptr->next) {
				if (strcmp(ptr->top->name, s_name) == 0) {
					s[i] = *ptr->top;
					i++;
					break;
				}
				ptr = ptr->next;
			}
		}
		for (int i = 0; i < 50;) {
			char* end_name = (char*)malloc(sizeof(char) * 6);
			_itoa(rand() % (num_tops + num_tops), end_name, 10);
			ptr = graph2;
			while (ptr->next) {
				if (strcmp(ptr->top->name, end_name) == 0) {
					end[i] = *ptr->top;
					i++;
					break;
				}
				ptr = ptr->next;
			}
		}
		start_t = clock();
		for (int i = 0; i < 50; i++) {
			if (Dijkstra(graph2, s, end, TRUE));
		}
		finish_t = clock();
		printf("test #%d, number of tops = %d, number of ribs = %d, time = %d, part_time = %d\n", 10 - n, (10 -
			n) * num_tops, (10 - n) * num_ribs, finish_t - start_t, time2 - time1);
	}
	void Deletion(struct Graph* graph, FILE * file = NULL);
	Deletion(graph2);
	return;
}
void Deletion(struct Graph* graph, FILE* file = NULL) {
	struct Graph* ptr = graph;
	struct Graph* prev;
	while (ptr) {
		prev = ptr;
		ptr = ptr->next;
		graph = delete_top(graph, prev->top);
		
	}
	refresh_file(graph);

}
void Main_Menu(struct Graph* graph) {
	int i = 0;
	char console_input;
	FILE* file = NULL;
	file = Open_file(&graph);
	string text[] = { "ADD_TOP\n", "ADD_RIB\n", "DELETE_TOP\n", "GRAPH GENERATION\n", "PRINT_GRAPH\n", "DIJKSTRA\n","TIMING\n" };
	print_main_menu(text, 0);
	while ((console_input = _getch()) != '\x1b') {
		if (console_input == 's') {
			i++;
			if (i >= 7) {
				i = 6;
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
				printf("Enter position (x,y) and name of top\n");
				struct Top* top = (struct Top*)malloc(sizeof(struct Top));
				scanf("%d %d", &top->x, &top->y);
				top->name = getstr();
				top->ribs = NULL;
				if (Find_top(graph, top->name)) {
					printf("Top already exist\n");
					_getch();
					system("cls");
					print_main_menu(text, i);
					continue;
				}
				graph = Add_top(graph, top, file);
			}
			if (i == 1) {
				char* top_name_from;
				char* top_name_to;
				printf("Enter names of tops to create rib\n");
				top_name_from = getstr();
				top_name_to = getstr();
				struct Top* top_from = Find_top(graph, top_name_from);
				struct Top* top_to = Find_top(graph, top_name_to);

				if ((!top_from) || (!top_to)) {
					printf("Top(s) not exist\n");
					_getch();
					system("cls");
					print_main_menu(text, i);
					continue;
				}

				struct Graph* new_graph = Add_rib(graph, top_from, top_to, file);
				if (new_graph) {
					graph = new_graph;
				}
				else {
					printf("This rib already exist\n");
					_getch();
					system("cls");
					print_main_menu(text, i);
					continue;
				}
			}
			if (i == 2) {
				char* del_top_name;
				printf("Enter name of top to deletion\n");
				del_top_name = getstr();
				struct Top* del_top = Find_top(graph, del_top_name);
				if (!del_top) {
					printf("Top not found\n");
					_getch();
					system("cls");
					print_main_menu(text, i);
					continue;
				}
				graph = delete_top(graph, del_top, file);
			}
			if (i == 3) {
				Deletion(graph, file);
				graph = NULL;
				printf("Enter number of tops and number of ribs\n");
				int num_tops, num_ribs;
				scanf("%d %d", &num_tops, &num_ribs);
				graph = graph_generation(graph, num_tops, num_ribs, file);

			}
			if (i == 4) {
				Output_graph(graph);
			}
			if (i == 5) {
				char* top1_name;
				char* top2_name;
				printf("Enter name of tops to calculate distance\n");
				top1_name = getstr();
				top2_name = getstr();
				struct Top* top1 = Find_top(graph, top1_name);
				struct Top* top2 = Find_top(graph, top2_name);
				if (!top1 || !top2) {
					printf("Top not found\n");
					_getch();
					system("cls");
					print_main_menu(text, i);
					continue;
				}
				Dijkstra(graph, top1, top2);
				_getch();
			}
			if (i == 6) {
				printf("timing...\n");
				Timing();
				_getch();
			}
			system("cls");
			print_main_menu(text, i);
		}
	}
	//Deletion(graph);
}
int main()
{
	struct Graph* graph = NULL;
	Main_Menu(graph);
}