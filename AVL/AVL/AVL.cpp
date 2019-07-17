// AVL.cpp: определяет точку входа для консольного приложения.
#include "stdafx.h"
#include <windows.h>
#include <conio.h>
//#include <stdio.h>

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

//-----MAIN---------------
int _tmain(int argc, _TCHAR* argv[]){
	system("CLS");
	printf("   AVL-tree");
	node* Tree = NULL;
	Tree = Load("BASE2.DAT");
	printf("\nФИО № отдела Должность Дата рождения");
	//PrintTree(Tree);
	//PrintZapDB((&Tree)->key);
	PrintZapDB(Tree->key);
	_getch();
	return 0;
}




//загрузка БД
node* Load(char *file){
	int n=0;	//счетчик записей
	FILE *f;
	ZapDB zap;
	node* p=NULL;
	if ((f = fopen(file, "rb"))==NULL){
		printf ("\nОшибка открытия файла %s \n", file);
		return NULL;
	};
	while (1) {
		fread(&zap, sizeof(ZapDB), 1, f);
		p = Insert(p, &zap);
		PrintZapDB(&zap);
		if (feof(f))
			break;
		n++;
		break; //отладка
	}
	fclose(f);
	printf("\nЗагружено записей n= %d", n);
	//PrintZapDB(&zap);	//отладка
	//free(zap); освободить память - надо ли?
	return p;
}

//Вывод сруктуры на экран
void PrintZapDB(ZapDB* zap){
	if (zap != NULL) {
		//printf("\n%.32s %3u %.22s %.8s", zap->fio, zap->otdel, zap->dolzhn, zap->dr+'\0'); отладка - это верная строка
		printf("\n%.32s %3u %.22s %.8s", zap->fio, zap->otdel, zap->dolzhn, zap->dr);
	}
}

//Вывод узлов дерева
void PrintTree(node* p) {
	if (!p)
		return;
	PrintZapDB(p->key);
	PrintTree(p->left);
	PrintTree(p->right);
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
	if (z<p->key)
		p->left = Insert(p->left, z);
	else
		p->right = Insert(p->right, z);
	return Balance(p);
}


