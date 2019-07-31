//TODO ��������� ����-����� �� �++ cin/cout ������ printf

// AVL.cpp: ���������� ����� ����� ��� ����������� ����������.
#include "stdafx.h"
#include <windows.h>
#include <conio.h>
#include <cstring>
#include <iostream>
using namespace std;
//-----
const int n = 4000; 
//ZapDB *base[n];

struct ZapDB{	//��������� ������ ��
	char fio[32];	//���������� ������ ���
	unsigned short int otdel;	//16-p��p����� ������������� ����� - � ������
	char dolzhn[22];	//���������� ������ ���������
	char dr[8];	//���������� ������ �� ��-��-��
};
struct node {	//��������� ���� ������
	ZapDB* key;
	char height;
	node* left;
	node* right;
	node(ZapDB* k) { key = k, height = 1; left = right = 0; }
	//} *Tree = NULL;	//�������� ��������� � main, ������ ��� ��� �������� ��� ������ �������
};
struct list {
	ZapDB* key;
	list* prev;
	list* next;
};
//node Tree=NULL;		//��������� �� ������� ������

//���������� �������
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
void Search(node* Tree, char str[], int otd, list* Spisok, list* head, list* tail);
char* FindDR(node* Tree, char str[], int otd);
char* GetFamily(char str[]);
bool IsEarlier(char* tree_dr, char* dr);
void InsertToList(ZapDB* key, list* Spisok, list* head, list* tail);
void PrintList(list* Spisok, list* head, list* tail);
void SearchRecursion(node* Tree, int otd, char* dr, list* Spisok, list* head, list* tail);
void FindDRRecursion(node * Tree, int otd, char * str, char* res);

//-----MAIN---------------
int _tmain(int argc, _TCHAR* argv[]){
	system("CLS");
	printf("   AVL-tree");
	node* Tree = NULL;
	list* Spisok = NULL;
	list* head = NULL;
	list* tail = NULL;
	char search_str[32];
	int search_otd;
	Tree = Load("BASE2_lat.DAT");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	cout << "\n������� ��� ���������� ��� ������: " << endl;
	cin >> search_str;
	cout << "� ����� ������: " << endl;
	cin >> search_otd;
	cout << "\n���� ��� " << search_str << " �� ������ � " << search_otd << endl;
	Search(Tree, search_str, search_otd, Spisok, head, tail);
	//TODO ���� ������ ������ ������: �������� ��� ����� �����
	//printf("\n��� � ������ ��������� ���� ��������");
	_getch();
	//PrintTree(Tree);
	//PrintZapDB((&Tree)->key);
//	PrintZapDB(Tree->key);

	system("PAUSE");
	return 0;
}




//�������� ��
node* Load(char *file){
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int n=0;	//������� �������
	FILE *f;
	node* p=NULL;
	if ((f = fopen(file, "rb"))==NULL){
		printf ("\n������ �������� ����� %s \n", file);
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
		/*if (n==5)	//�������
			break;*/ 
	}
	fclose(f);
	printf("\n��������� �������: %d\n", n);
	//PrintZapDB(&zap);	//�������
	//free(zap); ���������� ������ - ���� ��?
	return p;
}

//����� �������� �� �����
void PrintZapDB(ZapDB* zap){
	if (zap != NULL) {
		//printf("\n%.32s %3u %.22s %.8s", zap->fio, zap->otdel, zap->dolzhn, zap->dr+'\0'); �������
		printf("\n%.32s %3u %.22s %.8s", zap->fio, zap->otdel, zap->dolzhn, zap->dr);
	}
}

//����� ����� ������
//TODO ������� ����� ����������� (�� 20? �������) � ������������ �������
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

//���������� ������ ������
char Height(node* p){
	return p ? p->height:0;
}

//��������� ������ �������
int BFactor(node* p){
	return Height(p->right) - Height(p->left);
}

//������������ ������ ���� ����� �������������
void HeightReload(node* p){
	char hl = Height(p->left);
	char hr = Height(p->right);
	p->height = (hl>hr ? hl:hr) + 1;
}

//����� �������
node* Rotateleft(node* q){
	node* p = q->right;
	q->right = p->left;
	p->left = q;
	HeightReload(q);
	HeightReload(p);
	return p;
}
//������ �������
node* Rotateright(node* p){
	node* q = p->left;
	p->left = q->right;
	q->right = p;
	HeightReload(p);
	HeightReload(q);
	return q;
}
//������������ (������� ����� ����������� =2)
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
	return p; //����� ��� ������������
}

//������� ����� k � ������ � ������ p
node* Insert(node* p, ZapDB* z){
	if (z==NULL)
		return NULL;
	if (!p) return new node(z); //���������� ���� � ������ ������
	if (IsGreater(z, p->key))//(z<p->key)
		p->right = Insert(p->right, z);
	else
		p->left = Insert(p->left, z);
	return Balance(p);
}

//��������� �������
//TODO �������� ��������� dr �� �������� IsEarlier()
bool IsGreater(ZapDB* a, ZapDB* b) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
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
				printf("���������� ������ � ��!");
				//_getch();
				return -1;
			}
		}
	}
}

//������������ ������ ��������� �����:
//input: ��������� �� ������ � ���-� ����, ������� �������, ����� ������
//output: ��������� � ������ ��������� �� ����������� ���� �� ������, ������ ��������
//��� ���������� ������� ������� �������� � ����� ������ ��
void Search(node* Tree, char str[], int otd, list* Spisok, list* head, list* tail) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	if (!Tree)
		return;
	char* dr = FindDR(Tree, str, otd);
	if (str == NULL || otd == NULL) {
		cout << "������� ������� ������ ��� ������" << endl;
		return;
	}
	SearchRecursion(Tree, otd, dr, Spisok, head, tail);
}

void SearchRecursion(node* Tree, int otd, char* dr, list* Spisok, list* head, list* tail) {
	while (Tree!=NULL) {
		if (Tree->key->otdel > otd) {
			SearchRecursion(Tree->left, otd, dr, Spisok, head, tail);
		}
		else if (Tree->key->otdel < otd) {
			SearchRecursion(Tree->right, otd, dr, Spisok, head, tail);
		}
		if (Tree->key->otdel == otd) {
			if (IsEarlier(Tree->key->dr, dr)) {
				InsertToList(Tree->key, Spisok, head, tail);
			}
			SearchRecursion(Tree->left, otd, dr, Spisok, head, tail);
			SearchRecursion(Tree->right, otd, dr, Spisok, head, tail);
		}
	}
}

//�� ��� � � ������ ������� ����� ������ ���� �������� ����������
//    � ������ �������� �� ����� ������
//� ������ ���������� ������ �� ������ ������/�.�./���
//    ���� ����� � ���� ������ ������ �������� ������, �� ���� ����� ����������
//TODO �������� �������� ������: ������ ������������� �� ��� ������, �.�.
//		
char* FindDR(node* Tree, char str[], int otd) {
	char res[8];
	//char *family;
	//family = GetFamily(str);
	FindDRRecursion(Tree, otd, str, res);
	return res;
}

void FindDRRecursion(node* Tree, int otd, char * str, char* res){
	while (Tree != NULL) {
		if (Tree->key->otdel < otd) {
			FindDRRecursion(Tree->right, otd, str, res);
		}
		else if (Tree->key->otdel > otd) {
			FindDRRecursion(Tree->left, otd, str, res);
		}
		else if (Tree->key->otdel == otd) {
			if (strcmp(GetFamily(Tree->key->fio), str) == 0) {
				strcpy(res, Tree->key->dr);
				break;
			}
			FindDRRecursion(Tree->left, otd, str, res);
			FindDRRecursion(Tree->right, otd, str, res);
		}
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

//���������� true ���� ���� �������� tree_dr ������ dr
//������ ������ dr ��-��-��
//				   01 34 67
bool IsEarlier(char* tree_dr, char* dr) {
	if (tree_dr[6] > dr[6])
		return 1;
	else if (tree_dr[7] > dr[7])
		return 1;
	else if (tree_dr[3] > dr[3])
		return 1;
	else if (tree_dr[4] > dr[4])
		return 1;
	else if (tree_dr[0] > dr[0])
		return 1;
	else
		return 0;
}

//
void InsertToList(ZapDB* key, list* Spisok, list* head, list* tail) {
	list* lst = new list;
	lst->key = key;
	lst->prev = NULL;
	lst->next = NULL;
	if (Spisok == NULL) {
		Spisok = head = tail = lst;
	}
	else {
		tail->next = lst;
		lst->prev = tail;
		tail = lst;
	}
}

//
void PrintList(list* Spisok, list* head, list* tail) {
	list* current=Spisok;
	while (!current->next) {
		PrintZapDB(current->key);
		current = current->next;
	}
}