// AVL.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <windows.h>
#include <conio.h>
//#include <stdio.h>


void Load(char *file);

//Структура для записис БД
struct ZapDB{
	char fio[32];	//символьный массив фио
	unsigned short int otdel;	//16-pазpядное положительное число - № отдела
	char dolzhn[22];	//символьный массив должность
	char dr[8];	//символьный массив ДР дд-мм-гг
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




//загрузка БД
void Load(char *file){
	int n=0;	//счетчик записей
	FILE *f;
	ZapDB zap;
	if ((f = fopen(file, "rb"))==NULL){
		printf ("\nОшибка открытия файла %s \n", file);
		return;
	};
	while(1){
		fread(&zap, sizeof(ZapDB),1,f);
		if (feof(f))
			break;
		n++;
	}
	fclose(f);
	printf("\nЗагружено записей n= %d", n);
	PrintZapDB(zap);	//отладка
}

//Вывод сруктуры на экран
void PrintZapDB(ZapDB zap){

}