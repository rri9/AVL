//TODO ��ࠢ��� ����-�뢮� �� �++ cin/cout ����� printf

// AVL.cpp: ��।���� ��� �室� ��� ���᮫쭮�� �ਫ������.
#include "stdafx.h"
#include <windows.h>
#include <conio.h>
#include <cstring>
#include <iostream>
using namespace std;
//-----
const int n = 4000; 
//ZapDB *base[n];

struct ZapDB{	//������� ����� ��
	char fio[32];	//ᨬ����� ���ᨢ 䨮
	unsigned short int otdel;	//16-p��p來�� ������⥫쭮� �᫮ - � �⤥��
	char dolzhn[22];	//ᨬ����� ���ᨢ ���������
	char dr[8];	//ᨬ����� ���ᨢ �� ��-��-��
};
struct node {	//������� 㧫� ��ॢ�
	ZapDB* key;
	char height;
	node* left;
	node* right;
	node(ZapDB* k) { key = k, height = 1; left = right = 0; }
	//} *Tree = NULL;	//�������� 㪠��⥫� � main, ����� ��� ��� ��ࠬ��� �� �맮�� �㭪権
};
struct list {
	ZapDB* key;
	list* prev;
	list* next;
};
//node Tree=NULL;		//�����⥫� �� ���設� ��ॢ�

//������� �㭪権
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
	Tree = Load("BASE2.DAT");
	cout << "\n������ ��� ���㤭��� ��� ���᪠: " << endl;
	cin >> search_str;
	cout << "\n� ����� �⤥��: " << endl;
	cin >> search_otd;
	cout << "�饬 ��� " << search_str << " �� �⤥�� � " << search_otd << endl;
	Search(Tree, search_str, search_otd, Spisok, head, tail);
	//TODO �᫨ ��୥� ���⮩ ᯨ᮪: ����� �� ���� ����
	//printf("\n��� � �⤥�� ��������� ��� ஦�����");
	_getch();
	//PrintTree(Tree);
	//PrintZapDB((&Tree)->key);
//	PrintZapDB(Tree->key);

	system("PAUSE");
	return 0;
}




//����㧪� ��
node* Load(char *file){
	int n=0;	//���稪 ����ᥩ
	FILE *f;
	node* p=NULL;
	if ((f = fopen(file, "rb"))==NULL){
		printf ("\n�訡�� ������ 䠩�� %s \n", file);
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
		/*if (n==5)	//�⫠���
			break;*/ 
	}
	fclose(f);
	printf("\n����㦥�� ����ᥩ: %d", n);
	//PrintZapDB(&zap);	//�⫠���
	//free(zap); �᢮������ ������ - ���� ��?
	return p;
}

//�뢮� ������� �� �࠭
void PrintZapDB(ZapDB* zap){
	if (zap != NULL) {
		//printf("\n%.32s %3u %.22s %.8s", zap->fio, zap->otdel, zap->dolzhn, zap->dr+'\0'); �⫠���
		printf("\n%.32s %3u %.22s %.8s", zap->fio, zap->otdel, zap->dolzhn, zap->dr);
	}
}

//�뢮� 㧫�� ��ॢ�
//TODO ������� �뢮� ����࠭�筮 (�� 20? ����ᥩ) � ������������ ������
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

//�����頥� ����� ��ॢ�
char Height(node* p){
	return p ? p->height:0;
}

//������ ������ ���設�
int BFactor(node* p){
	return Height(p->right) - Height(p->left);
}

//���४���� ����� 㧫� ��᫥ ���४�஢��
void HeightReload(node* p){
	char hl = Height(p->left);
	char hr = Height(p->right);
	p->height = (hl>hr ? hl:hr) + 1;
}

//���� ������
node* Rotateleft(node* q){
	node* p = q->right;
	q->right = p->left;
	p->left = q;
	HeightReload(q);
	HeightReload(p);
	return p;
}
//�ࠢ� ������
node* Rotateright(node* p){
	node* q = p->left;
	p->left = q->right;
	q->right = p;
	HeightReload(p);
	HeightReload(q);
	return q;
}
//������஢�� (ࠧ��� ���� �����ॢ쥢 =2)
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
	return p; //��室 ��� ������஢��
}

//��⠢�� ���� k � ��ॢ� � ��୥� p
node* Insert(node* p, ZapDB* z){
	if (z==NULL)
		return NULL;
	if (!p) return new node(z); //���������� 㧫� � ���⮥ ��ॢ�
	if (IsGreater(z, p->key))//(z<p->key)
		p->right = Insert(p->right, z);
	else
		p->left = Insert(p->left, z);
	return Balance(p);
}

//�ࠢ����� ����ᥩ
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
				printf("��������� ����� � ��!");
				//_getch();
				return -1;
			}
		}
	}
}

//��ନ஢���� ᯨ᪠ ��������� ��:
//input: 㪠��⥫� �� ��ॢ� � ���-� �饬, �᪮��� 䠬����, ����� �⤥��
//output: �������� � ᯨ᮪ 㪠��⥫� �� ���㤭���� ⮣� �� �⤥��, ������ �᪮����
//�� ᮢ������� 䠬���� �᪮�� �롨ࠥ� � ����� ࠭��� ��
void Search(node* Tree, char str[], int otd, list* Spisok, list* head, list* tail) {
	if (!Tree)
		return;
	char* dr = FindDR(Tree, str, otd);
	if (str == NULL || otd == NULL) {
		cout << "����୮ ������� ����� ��� ���᪠" << endl;
		return;
	}
	while (!Tree) {
		if (Tree->key->otdel > otd) {
			break;
		}
		else if (Tree->key->otdel < otd) {
			Tree = Tree->left;
		}
		if (Tree->key->otdel == otd) {
			if (IsEarlier(Tree->key->dr, dr)){
				InsertToList(Tree->key, Spisok, head, tail);
			}
			Tree = Tree->left;
		}
	}
}

//�� ��� � � �⤥�� ��室�� ᠬ�� ࠭��� ���� ஦����� ���㤭���
//    � ������ 䠬����� �� �⮣� �⤥��
//� ��⮬ ���஢�� ��ॢ� �� ������ �⤥��/�.�./䨮
//    �᫨ �⤥� � 㧫� ��ॢ� ����� �᪮���� �⤥��, � ���� �⤥� ��ᬮ�७
char* FindDR(node* Tree, char str[], int otd) {
	char res[8], *family;
	family = GetFamily(str);
	while (!Tree) {	//!Tree==NULL
		if (Tree->key->otdel > otd) {
			break;
		}
		else if (Tree->key->otdel < otd) {
			Tree = Tree->left;
		}
		else if (Tree->key->otdel == otd) {
			if (strcmp(GetFamily(Tree->key->fio), family)==0) {
				strcpy(res, Tree->key->dr);
				break;
			}
			Tree->left;
		}
	}

	return res;
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

//�����頥� true �᫨ ��� ஦����� tree_dr ����� dr
//�ଠ� ����� dr ��-��-��
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