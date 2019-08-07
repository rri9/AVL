//+Fix: ����� �������� � ������ ������� ���������� ������ (error)
//+Fix: � Spisok �������� ��� ������ ������ (IsEarlier func logic error)
//+Fix: ��������� ������ ������ zap (Load func logic error)
//+Fix: IsGreater: �������� ��������� dr �� �������� IsEarlier()
//Change: IsEarlier func to int (-1 0 1)

//TODO ��������� �� ������������ � �������, � �.�. ����� � ����
//TODO ��������� ����-����� �� �++ cin/cout ������ printf


// AVL.cpp: ���������� ����� ����� ��� ����������� ����������.
#include "stdafx.h"
#include <windows.h>
#include <conio.h>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <iostream>	//�������
#include <fstream>	//�������
using namespace std;
//-----
const int n = 4000; 
//ZapDB *base[n];

struct ZapDB{	//��������� ������ ��
	char fio[32];	//���������� ������ ���
	unsigned short otdel;	//16-p��p����� ������������� ����� - � ������
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
void PrintTreeInFile(node* p, string f);	//�������
void PrintZapDBInFile(ZapDB* zap, string f);	//�������

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
		cout << "\n������� ��� ���������� ��� ������: " << endl;
		cin >> search_str;
		cout << "� ����� ������: " << endl;
		cin >> search_otd;
		cout << "\n���� ����������� �� ������ � " << search_otd << " ������ ���������� " << search_str << endl;
		Search(Tree, search_str, search_otd, &Spisok, &head, &tail);
		if (Spisok == nullptr) {
			cout << "\n������� �� �������! \n\n��� ������ ������� ESC, \n��� ������ ������ - ����� �������..." << endl;
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
	//printf("\n��� � ������ ��������� ���� ��������");
	//_getch();
//	PrintTree(Tree, 0);
	//PrintZapDB((&Tree)->key);
//	PrintZapDB(Tree->key);
	//cout << endl;
	//system("PAUSE");
	return 0;
}




//�������� ��
node* Load(char *file){
	SetConsoleCP(1251);
 	SetConsoleOutputCP(1251);
	int n=0;	//������� �������
	FILE *f;
//	FILE *ff;	//�������
	node* p=NULL;
	if ((f = fopen(file, "rb"))==NULL){
		printf ("\n������ �������� ����� %s \n", file);
		return NULL;
	};
//	ff = fopen("BASE2_short.DAT","w");	//�������
	while (1) {
		ZapDB* zap=new ZapDB;
//		zap->fio[0] = '\0';
		fread(zap, sizeof(ZapDB), 1, f);
//		if (n <= 20)	//�������
//			fwrite(zap, sizeof(ZapDB), 1, ff);	//�������
		//PrintZapDB(zap);
		//PrintZapDB(p->key);
		if (feof(f))
			break;
		PrintZapDBInFile(zap, "file_PrintZapDB_by_Load");
		p = Insert(p, zap);
		n++;
		/*if (n == 4000)	//�������
		{
			_getch();
		}*/
//			break;
	}
	fclose(f);
//	fclose(ff);	//�������
	printf("\n��������� �������: %d\n", n);
	//PrintZapDB(&zap);	//�������
	//free(zap); ���������� ������ - ���� ��?
	return p;
}

//����� �������� �� �����
void PrintZapDB(ZapDB* zap){
	if (zap != NULL) {
		//FILE* ff = fopen("BASE2_short.DAT", "w");
//		fstream ff;
//		ff.open("file.txt", ios::out);
		//cout << endl;
		//printf("\n%.32s %3u %.22s %.8s", zap->fio, zap->otdel, zap->dolzhn, zap->dr+'\0'); �������
		//printf("%.32s %3u %.22s %.9s\n", zap->fio, zap->otdel, zap->dolzhn, zap->dr);
		//cout << setw(22) << zap->fio << setw(5) << zap->otdel << zap->dolzhn << zap->dr << endl;
		for (int i = 0; i < 32; i++) {
			cout << zap->fio[i];
//			ff << zap->fio[i];	//�������
		}
		cout << setw(5) << zap->otdel;
//		ff << setw(5) << zap->otdel;	//�������
		cout << " ";
//		ff << " ";
		//�������
		for (int i = 0; i < 22; i++) {
			cout << zap->dolzhn[i];
//			ff << zap->dolzhn[i];	//�������
		}
		cout << " ";
//		ff << " ";	//�������
		for (int i = 0; i < 8; i++) {
			cout << zap->dr[i];
//			ff << zap->dr[i];	//�������
		}
		cout << endl;
//		ff << endl;	//�������
//		ff.close();
	}
}

//����� ����� ������
//TODO ������� ����� ����������� (�� 20? �������) � ������������ �������
void PrintTree(node* p) {	//, int level)
	if (!p) {
		return;
	}
	else {
		PrintTree(p->right);	//PrintTree(p->left, level + 1);
		//for (int i = 0;i< level;i++) cout << "   ";	//�������
		PrintZapDB(p->key);
		PrintTree(p->left);	//PrintTree(p->right, level + 1);
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
void Search(node* Tree, char str[], int otd, list** Spisok, list** head, list** tail) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	if (!Tree)
		return;
	char dr[] = "00-00-00";
	FindDR(Tree, str, otd, dr);	//TODO char dr[8] ???
	if (str == NULL || !(otd >= 0)) {
		cout << "������� ������� ������ ��� ������" << endl;
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

//�� ��� � � ������ ������� ����� ������ ���� �������� ����������
//    � ������ �������� �� ����� ������
void FindDR(node* Tree, char str[], int otd, char dr[]) {
	//char res[8];
	//char res[] = {'0','0','-','0','0','-','0','0'};
	//char res[] = "00-00-00";
	bool found = 0;
	//char *family;�
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

//���������� true ���� ���� �������� tree_dr ������ dr
//������ ������ dr ��-��-��
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
		cout << "������� �� �������!" << endl;
		return;
	}
	list* current=Spisok;
	cout << "\n    ���������� ������:" << endl;
	cout << " ���" <<setw(33) << "� ���" << setw(11) << "���������" \
		<< setw(18) << "�.�." << endl;	//��� � ������ ��������� ���� ��������
	while (current!=nullptr) {
		PrintZapDB(current->key);
		current = current->next;
	}
}

//----------------------------------------------�������
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
//----------------------------------------------�������
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