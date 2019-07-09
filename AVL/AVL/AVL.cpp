// AVL.cpp: ��।���� ��� �室� ��� ���᮫쭮�� �ਫ������.
//

#include "stdafx.h"
#include <windows.h>
#include <conio.h>
//#include <stdio.h>


void Load(char *file);

//������� ��� ������ ��
struct ZapDB{
	char fio[32];	//ᨬ����� ���ᨢ 䨮
	unsigned short int otdel;	//16-p��p來�� ������⥫쭮� �᫮ - � �⤥��
	char dolzhn[22];	//ᨬ����� ���ᨢ ���������
	char dr[8];	//ᨬ����� ���ᨢ �� ��-��-��
};
void PrintZapDB(ZapDB zap);

int _tmain(int argc, _TCHAR* argv[])
{
	system("CLS");
	printf("   AVL-tree");
	Load("BASE2.DAT");
	//	PrintZapDB(zap);

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
	while(1){
		fread(&zap, sizeof(ZapDB),1,f);
		if (feof(f))
			break;
		n++;
	}
	fclose(f);
	printf("\n����㦥�� ����ᥩ n= %d", n);
	PrintZapDB(zap);	//�⫠���
}

//�뢮� ������� �� �࠭
void PrintZapDB(ZapDB zap){

}