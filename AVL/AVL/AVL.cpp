// AVL.cpp: ��।���� ��� �室� ��� ���᮫쭮�� �ਫ������.
#include "stdafx.h"
#include <windows.h>
#include <conio.h>
#include <cstring>
//#include <stdio.h>

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

//-----MAIN---------------
int _tmain(int argc, _TCHAR* argv[]){
	system("CLS");
	printf("   AVL-tree");
	node* Tree = NULL;
	Tree = Load("BASE2.DAT");
	printf("\n��� � �⤥�� ��������� ��� ஦�����");
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
	printf("\n����㦥�� ����ᥩ n= %d", n);
	//PrintZapDB(&zap);	//�⫠���
	//free(zap); �᢮������ ������ - ���� ��?
	return p;
}

//�뢮� ������� �� ��࠭
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
//TODO �������� ���冷� �ࠢ����� ��: �⤥�, ��, 䨮
bool IsGreater(ZapDB* a, ZapDB* b) {
	if (strcmp(a->fio, b->fio) > 0)
		return 1;
	else if (strcmp(a->fio, b->fio) < 0)
		return 0;
	else{	//䨮 ࠢ��, �ࠢ������ �� �⤥��
		if (a->otdel > b->otdel)
			return 1;
		else if (a->otdel < b->otdel)
			return 0;
		else{	//䨮, �⤥� ࠢ��, �ࠢ������ �� ��
			if (strcmp(a->dr, b->dr) > 0)
				return 1;
			else if (strcmp(a->dr, b->dr) < 0)
				return 0;
			else {
				printf("��������� ����� � ��!");
				//_getch();
				return -1;
			}
		}
	}
}
