// AVL.cpp: ��।���� ��� �室� ��� ���᮫쭮�� �ਫ������.
#include "stdafx.h"
#include <windows.h>
#include <conio.h>
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
struct node{	//������� 㧫� ��ॢ�
	ZapDB* key;
	char height;
	node* left;
	node* right;
	node(ZapDB* k) {key=k, height=1; left=right=0;}
} Tree = NULL;

//node Tree=NULL;		//�����⥫� �� ���設� ��ॢ�

//������� �㭪権
void PrintZapDB(ZapDB* zap);
void Load(char* file);
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
	Load("BASE2.DAT");
	printf("\n��� � �⤥�� ��������� ��� ஦�����");
	PrintZapDB(Tree.key);

	_getch();
	return 0;
}




//����㧪� ��
void Load(char *file){
	int n=0;	//���稪 ����ᥩ
	FILE *f;
	ZapDB zap;
	if ((f = fopen(file, "rb"))==NULL){
		printf ("\n�訡�� ������ 䠩�� %s \n", file);
		return;
	};
	fread(&zap, sizeof(ZapDB),1,f);
	while(!feof(f)){
		//���ᨬ ������ � �������� ��ॢ�
		Insert(&Tree, &zap);
		fread(&zap, sizeof(ZapDB),1,f);
		n++;
	}
	fclose(f);
	printf("\n����㦥�� ����ᥩ n= %d", n);
	PrintZapDB(&zap);	//�⫠���
}

//�뢮� ������� �� �࠭
void PrintZapDB(ZapDB* zap){
	if (zap != NULL) {
		printf("\n%s %d %s %s\n", zap->fio, zap->otdel, zap->dolzhn, zap->dr);
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
	if (!p) return new node(z); //���������� 㧫� � ���⮥ ��ॢ�
	if (z<p->key)
		p->left = Insert(p->left, z);
	else
		p->right = Insert(p->right, z);
	return Balance(p);
}


