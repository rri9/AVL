//TODO исправить ввод-вывод на с++ cin/cout вместо printf

// AVL.cpp: определяет точку входа для консольного приложения.
#include "stdafx.h"
#include <windows.h>
#include <conio.h>
#include <cstring>
#include <iostream>
using namespace std;
//-----
const int n = 4000; 
//ZapDB *base[n];

struct ZapDB{	//Структура записи БД
	char fio[32];	//символьный массив фио
	unsigned short int otdel;	//16-pазpядное положительное число - № отдела
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
void PrintTree(node* p);
node* Load(char* file);
char Height(node* p);
int BFactor(node* p);
void HeightReload(node* p);
node* Rotateleft(node* q);
node* Rotateright(node* p);
node* Balance(node* p);
node* Insert(node* p, ZapDB* k);
bool IsGreater(ZapDB* a, ZapDB* b);
void Search(node* Tree, char str[32], int otd, list* Spisok);

//-----MAIN---------------
int _tmain(int argc, _TCHAR* argv[]){
	system("CLS");
	printf("   AVL-tree");
	node* Tree = NULL;
	list* Spisok = NULL;
	char search_str[32];
	int search_otd;
	Tree = Load("BASE2.DAT");
	cout << "\nВведите ФИО сотрудника для поиска: " << endl;
	cin >> search_str;
	cout << "\nи номер отдела: " << endl;
	cin >> search_otd;
	cout << "Ищем ФИО " << search_str << " из отдела № " << search_otd << endl;
	Search(Tree, search_str, search_otd, Spisok);
	//TODO если вернет пустой список: спросить про новый поиск
	//printf("\nФИО № отдела Должность Дата рождения");
	_getch();
	PrintTree(Tree);
	//PrintZapDB((&Tree)->key);
//	PrintZapDB(Tree->key);
	system("PAUSE");
	return 0;
}




//загрузка БД
node* Load(char *file){
	int n=0;	//счетчик записей
	FILE *f;
	node* p=NULL;
	if ((f = fopen(file, "rb"))==NULL){
		printf ("\nОшибка открытия файла %s \n", file);
		return NULL;
	};
	while (1) {
		ZapDB* zap=new ZapDB;
		fread(zap, sizeof(ZapDB), 1, f);
		p = Insert(p, zap);
//		PrintZapDB(zap);
//		PrintZapDB(p->key);
		if (feof(f))
			break;
		n++;
		/*if (n==5)	//отладка
			break;*/ 
	}
	fclose(f);
	printf("\nЗагружено записей: %d", n);
	//PrintZapDB(&zap);	//отладка
	//free(zap); освободить память - надо ли?
	return p;
}

//Вывод сруктуры на экран
void PrintZapDB(ZapDB* zap){
	if (zap != NULL) {
		//printf("\n%.32s %3u %.22s %.8s", zap->fio, zap->otdel, zap->dolzhn, zap->dr+'\0'); отладка
		printf("\n%.32s %3u %.22s %.8s", zap->fio, zap->otdel, zap->dolzhn, zap->dr);
	}
}

//Вывод узлов дерева
//TODO Сделать вывод постранично (по 20? записей) с возможностью листать
void PrintTree(node* p) {
	if (!p) {
		return;
	}
	else {
		PrintTree(p->left);
		PrintZapDB(p->key);
		PrintTree(p->right);
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
	if (a->otdel > b->otdel)
		return 1;
	else if (a->otdel < b->otdel)
		return 0;
	else{
		if (strcmp(a->dr, b->dr) > 0)
			return 1;
		else if (strcmp(a->dr, b->dr) < 0)
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
//output: добавляет в список сотрудников того же отдела, моложе искомого
//при совпадении фамилии искомым выбираем с более ранней др
void Search(node* Tree, char str[32], int otd, list* Spisok) {
	if (!Tree)
		return;
	char dr[8] = FindDR(str, otd);
	string dr_str = dr;
	if (str == NULL || otd == NULL) {
		cout << "Неверно введены данные для поиска" << endl;
		return;
	}
	while (1) {
		if (Tree->key->otdel == otd) {
			if (сравнить др) ){

				InsertToList(Tree->key, Spisok);
			}
		}
		if (Tree->key->otdel > otd) {

		}
	}
}

