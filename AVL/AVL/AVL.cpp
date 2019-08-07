//+Fix: после загрузки в дерево записей становится меньше (error)
//+Fix: в Spisok попадают все записи отдела (IsEarlier func logic error)
//+Fix: появление пустой записи zap (Load func logic error)
//+Fix: IsGreater: Изменить сравнение dr на савнение IsEarlier()
//Change: IsEarlier func to int (-1 0 1)

//TODO Почистить от комментариев и отладки, в т.ч. вывод в файл
//TODO исправить ввод-вывод на с++ cin/cout вместо printf


// AVL.cpp: определяет точку входа для консольного приложения.
#include "stdafx.h"
#include <windows.h>
#include <conio.h>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <iostream>	//отладка
#include <fstream>	//отладка
using namespace std;
//-----
const int n = 4000; 
//ZapDB *base[n];

struct ZapDB{	//Структура записи БД
	char fio[32];	//символьный массив фио
	unsigned short otdel;	//16-pазpядное положительное число - № отдела
	char dolzhn[22];	//символьный массив должность
	char dr[8];	//символьный массив ДР дд-мм-гг
};
struct node {	//Структура узла дерева
	ZapDB* key;
	char height;
	node* left;
	node* right;
	node(ZapDB* k) { key = k, height = 1; left = right = 0; }
	//} *Tree = NULL;	//добавить указатель в main, внести его как параметр при вызове функций
};
struct list {
	ZapDB* key;
	list* prev;
	list* next;
};
//node Tree=NULL;		//Указатель на вершину дерева

//Объявления функций
void PrintZapDB(ZapDB* zap);
void PrintTree(node* p);	//, int level)
node* Load(char* file);
char Height(node* p);
int BFactor(node* p);
void HeightReload(node* p);
node* Rotateleft(node* q);
node* Rotateright(node* p);
node* Balance(node* p);
node* Insert(node* p, ZapDB* z);
bool IsGreater(ZapDB* a, ZapDB* b);
void Search(node* Tree, char str[], int otd, list** Spisok, list** head, list** tail);
void FindDR(node* Tree, char str[], int otd, char dr[]);
char* GetFamily(char str[]);
int IsEarlier(char* tree_dr, char* dr);
void InsertToList(ZapDB* key, list** Spsk, list** head, list** tail);
void PrintList(list* Spisok, list* head, list* tail);
void SearchRecursion(node* Tree, int otd, char* dr, list** Spisok, list** head, list** tail);
void FindDRRecursion(node * Tree, int otd, char * str, char* res, bool* found);
void PrintTreeInFile(node* p, string f);	//отладка
void PrintZapDBInFile(ZapDB* zap, string f);	//отладка

//-----MAIN---------------
int _tmain(int argc, _TCHAR* argv[]){
	system("CLS");
	printf("   AVL-tree\n");
	node* Tree = NULL;
	list* Spisok = nullptr;
	list* head = nullptr;
	list* tail = nullptr;
	char search_str[32];
	int search_otd;
	Tree = Load("BASE2_lat.DAT");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	PrintTreeInFile(Tree, "file_PrintTree");	//, 0)
	bool search=true;
	while (search) {
		cout << "\nВведите ФИО сотрудника для поиска: " << endl;
		cin >> search_str;
		cout << "и номер отдела: " << endl;
		cin >> search_otd;
		cout << "\nИщем сотрудников из отдела № " << search_otd << " моложе сотрудника " << search_str << endl;
		Search(Tree, search_str, search_otd, &Spisok, &head, &tail);
		if (Spisok == nullptr) {
			cout << "\nЗаписей не найдено! \n\nДля выхода нажмите ESC, \nдля нового поиска - любую клавишу..." << endl;
			int key = _getch();
			if (key == 224 || key == 0) key = _getch();
			switch (key){
			case 27: 
				search = false;
				break;
//			case 'y':
//				search = true;
//				break;
			default: break;
			}
		}
		else {
			search = false;
			PrintList(Spisok, head, tail);
			cout << endl;
			system("PAUSE");
		}
	}
	//printf("\nФИО № отдела Должность Дата рождения");
	//_getch();
//	PrintTree(Tree, 0);
	//PrintZapDB((&Tree)->key);
//	PrintZapDB(Tree->key);
	//cout << endl;
	//system("PAUSE");
	return 0;
}




//загрузка БД
node* Load(char *file){
	SetConsoleCP(1251);
 	SetConsoleOutputCP(1251);
	int n=0;	//счетчик записей
	FILE *f;
//	FILE *ff;	//отладка
	node* p=NULL;
	if ((f = fopen(file, "rb"))==NULL){
		printf ("\nОшибка открытия файла %s \n", file);
		return NULL;
	};
//	ff = fopen("BASE2_short.DAT","w");	//отладка
	while (1) {
		ZapDB* zap=new ZapDB;
//		zap->fio[0] = '\0';
		fread(zap, sizeof(ZapDB), 1, f);
//		if (n <= 20)	//отладка
//			fwrite(zap, sizeof(ZapDB), 1, ff);	//отладка
		//PrintZapDB(zap);
		//PrintZapDB(p->key);
		if (feof(f))
			break;
		PrintZapDBInFile(zap, "file_PrintZapDB_by_Load");
		p = Insert(p, zap);
		n++;
		/*if (n == 4000)	//отладка
		{
			_getch();
		}*/
//			break;
	}
	fclose(f);
//	fclose(ff);	//отладка
	printf("\nЗагружено записей: %d\n", n);
	//PrintZapDB(&zap);	//отладка
	//free(zap); освободить память - надо ли?
	return p;
}

//Вывод сруктуры на экран
void PrintZapDB(ZapDB* zap){
	if (zap != NULL) {
		//FILE* ff = fopen("BASE2_short.DAT", "w");
//		fstream ff;
//		ff.open("file.txt", ios::out);
		//cout << endl;
		//printf("\n%.32s %3u %.22s %.8s", zap->fio, zap->otdel, zap->dolzhn, zap->dr+'\0'); отладка
		//printf("%.32s %3u %.22s %.9s\n", zap->fio, zap->otdel, zap->dolzhn, zap->dr);
		//cout << setw(22) << zap->fio << setw(5) << zap->otdel << zap->dolzhn << zap->dr << endl;
		for (int i = 0; i < 32; i++) {
			cout << zap->fio[i];
//			ff << zap->fio[i];	//отладка
		}
		cout << setw(5) << zap->otdel;
//		ff << setw(5) << zap->otdel;	//отладка
		cout << " ";
//		ff << " ";
		//отладка
		for (int i = 0; i < 22; i++) {
			cout << zap->dolzhn[i];
//			ff << zap->dolzhn[i];	//отладка
		}
		cout << " ";
//		ff << " ";	//отладка
		for (int i = 0; i < 8; i++) {
			cout << zap->dr[i];
//			ff << zap->dr[i];	//отладка
		}
		cout << endl;
//		ff << endl;	//отладка
//		ff.close();
	}
}

//Вывод узлов дерева
//TODO Сделать вывод постранично (по 20? записей) с возможностью листать
void PrintTree(node* p) {	//, int level)
	if (!p) {
		return;
	}
	else {
		PrintTree(p->right);	//PrintTree(p->left, level + 1);
		//for (int i = 0;i< level;i++) cout << "   ";	//отладка
		PrintZapDB(p->key);
		PrintTree(p->left);	//PrintTree(p->right, level + 1);
	}
}

//Возвращает высоту дерева
char Height(node* p){
	return p ? p->height:0;
}

//Вычисляет баланс вершины
int BFactor(node* p){
	return Height(p->right) - Height(p->left);
}

//Корректирует высоту узла после корректировки
void HeightReload(node* p){
	char hl = Height(p->left);
	char hr = Height(p->right);
	p->height = (hl>hr ? hl:hr) + 1;
}

//Левый поворот
node* Rotateleft(node* q){
	node* p = q->right;
	q->right = p->left;
	p->left = q;
	HeightReload(q);
	HeightReload(p);
	return p;
}
//Правый поворот
node* Rotateright(node* p){
	node* q = p->left;
	p->left = q->right;
	q->right = p;
	HeightReload(p);
	HeightReload(q);
	return q;
}
//Балансировка (разница высот поддеревьев =2)
node* Balance(node* p){
	HeightReload(p);
	if (BFactor(p)==2){
		if (BFactor(p->right) < 0){
			p->right = Rotateright(p->right);
		}
		return Rotateleft(p);
	}
	if (BFactor(p)==-2){
		if (BFactor(p->left)>0){
			p->left = Rotateleft(p->left);
		}
		return Rotateright(p);
	}
	return p; //выход без балансировки
}

//Вставка ключа k в дерево с корнем p
node* Insert(node* p, ZapDB* z){
	if (z==NULL)
		return NULL;
	if (!p) return new node(z); //добавление узла в пустое дерево
	if (IsGreater(z, p->key))//(z<p->key)
		p->right = Insert(p->right, z);
	else
		p->left = Insert(p->left, z);
	return Balance(p);
}

//Сравнение записей
bool IsGreater(ZapDB* a, ZapDB* b) {
//	SetConsoleCP(1251);
//	SetConsoleOutputCP(1251);
	if (a->otdel > b->otdel)
		return 1;
	else if (a->otdel < b->otdel)
		return 0;
	else{
		if(IsEarlier(a->dr, b->dr)<0)	//if (strcmp(a->dr, b->dr) > 0)
			return 1;
		else if (IsEarlier(a->dr, b->dr)>0)
			return 0;
		else{
			if (strcmp(a->fio, b->fio) > 0)
				return 1;
			else if (strcmp(a->fio, b->fio) < 0)
				return 0;
			else {
				printf("Одинаковые записи в БД!");
				//_getch();
				return -1;
			}
		}
	}
}

//Формирование списка найденных людей:
//input: указатель на дерево в кот-м ищем, искомая фамилия, номер отдела
//output: добавляет в список указатели на сотрудников того же отдела, моложе искомого
//При совпадении фамилии искомым выбираем с более ранней др
void Search(node* Tree, char str[], int otd, list** Spisok, list** head, list** tail) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	if (!Tree)
		return;
	char dr[] = "00-00-00";
	FindDR(Tree, str, otd, dr);	//TODO char dr[8] ???
	if (str == NULL || !(otd >= 0)) {
		cout << "Неверно введены данные для поиска" << endl;
		return;
	}
	SearchRecursion(Tree, otd, dr, Spisok, head, tail);
}

void SearchRecursion(node* Tree, int otd, char* dr, list** Spisok, list** head, list** tail) {
	if (Tree!=NULL) {
		if (Tree->key->otdel > otd) {
			SearchRecursion(Tree->left, otd, dr, Spisok, head, tail);
		}
		else if (Tree->key->otdel < otd) {
			SearchRecursion(Tree->right, otd, dr, Spisok, head, tail);
		}
		if (Tree->key->otdel == otd) {
			if (IsEarlier(Tree->key->dr, dr)<0) {
				InsertToList(Tree->key, Spisok, head, tail);
			}
			SearchRecursion(Tree->left, otd, dr, Spisok, head, tail);
			SearchRecursion(Tree->right, otd, dr, Spisok, head, tail);
		}
	}
}

//По ФИО и № отдела находим самую раннюю дату рождения сотрудника
//    с данной фамилией из этого отдела
void FindDR(node* Tree, char str[], int otd, char dr[]) {
	//char res[8];
	//char res[] = {'0','0','-','0','0','-','0','0'};
	//char res[] = "00-00-00";
	bool found = 0;
	//char *family;О
	//family = GetFamily(str);
	FindDRRecursion(Tree, otd, str, dr, &found);
	//return res;
}

void FindDRRecursion(node* Tree, int otd, char * str, char* res, bool* found){
	if ((Tree != NULL) & (*found==false)) {
		if (Tree->key->otdel == otd) {
			if (strcmp(GetFamily(Tree->key->fio), str) == 0) {
				//strcpy(res, Tree->key->dr);
				for (int i = 0; i < 8; i++) {
					res[i] = Tree->key->dr[i];
				}
				*found = true;
				return;
			}
		}
		FindDRRecursion(Tree->left, otd, str, res, found);
		FindDRRecursion(Tree->right, otd, str, res, found);
	}
}

char* GetFamily(char str[]) {
	char family[32];
	for (int i = 0; i < 32; i++) {
		if (str[i] == ' ') {
			family[i] = '\0';
			break;
		}
		else
			family[i] = str[i];
	}
	return family;
}

//Возвращает true если дата рождения tree_dr меньше dr
//формат записи dr дд-мм-гг
//				   01 34 67
int IsEarlier(char* tree_dr, char* dr) {
	int tree_dr_int=0, dr_int=0;
	tree_dr_int = (tree_dr[6]-'0') * 100000 + \
		(tree_dr[7] - '0') * 10000 + \
		(tree_dr[3] - '0') * 1000 + \
		(tree_dr[4] - '0') * 100 + \
		(tree_dr[0] - '0') * 10 + \
		(tree_dr[1] - '0');
	dr_int = (dr[6] - '0') * 100000 + \
		(dr[7] - '0') * 10000 + \
		(dr[3] - '0') * 1000 + \
		(dr[4] - '0') * 100 + \
		(dr[0] - '0') * 10 + \
		(dr[1] - '0');
	if (tree_dr_int < dr_int)
		return 1;
	else if (tree_dr_int > dr_int)
		return -1;
	else
		return 0;
}

//
void InsertToList(ZapDB* key, list** Spsk, list** head, list** tail) {
	list* lst = new list;
	lst->key = key;
	lst->prev = NULL;
	lst->next = NULL;
	if (*Spsk == nullptr) {
		*Spsk = lst;
		*head = *tail = lst;
	}
	else {
		(*tail)->next = lst;
		lst->prev = *tail;
		*tail = lst;
	}
}

//
void PrintList(list* Spisok, list* head, list* tail) {
	if (Spisok==nullptr) {
		cout << "Записей не найдено!" << endl;
		return;
	}
	list* current=Spisok;
	cout << "\n    Результаты поиска:" << endl;
	cout << " ФИО" <<setw(33) << "№ отд" << setw(11) << "Должность" \
		<< setw(18) << "Д.р." << endl;	//ФИО № отдела Должность Дата рождения
	while (current!=nullptr) {
		PrintZapDB(current->key);
		current = current->next;
	}
}

//----------------------------------------------отладка
void PrintTreeInFile(node* p, string f) {
	if (!p) {
		return;
	}
	else {
		PrintTreeInFile(p->right, f);
		//if (p->key->otdel)
			PrintZapDBInFile(p->key, f);
		PrintTreeInFile(p->left, f);
	}
}
//----------------------------------------------отладка
void PrintZapDBInFile(ZapDB* zap, string f) {
	if (zap != NULL) {
		fstream ff;
		ff.open(f, ios::app);
		for (int i = 0; i < 32; i++) {
			ff << zap->fio[i];
		}
		ff << setw(5) << zap->otdel;
		ff << " ";
		for (int i = 0; i < 22; i++) {
			if (zap->dolzhn[i] == '\0') {
				ff << ' ';
				continue;
			}
			ff << zap->dolzhn[i];
		}
		ff << " ";
		for (int i = 0; i < 8; i++) {
			ff << zap->dr[i];
		}
		ff << endl;
		ff.close();
	}
}