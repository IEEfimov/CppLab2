// CppLab2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include"stdafx.h";
#include <conio.h>
#include<windows.h>
#include<clocale>
#include <omp.h>
#include "Ex1.h"
#include "Ex2.h"
#include "Ex3.h"
#include "Ex4.h"
#include "Ex5.h"
#include "Ex6.h"
#include "Ex7.h"
#include <string>
#include <iostream>

using namespace std;


int main()
{
	Ex1 ex1;
	Ex2 ex2;
	Ex4 ex3;
	Ex4 ex4;
	Ex5 ex5;
	Ex6 ex6;
	Ex7 ex7;



	cout << "Choose task:";
	int choise;
	scanf_s("%d", &choise);

	switch (choise)
	{
	case 1:
		ex1.doAnalise();
		break;
	case 2:
		ex2.doAnalise();
		break;
	case 3:
		ex3.doAnalise();
		break;
	case 4:
		ex4.doAnalise();
		break;
	case 5:
		ex5.doAnalise();
		break;
	case 6:
		ex6.doAnalise();
		break;
	case 7:
		ex7.doAnalise();
		break;
	default:
		break;
	}

	_getch();
	exit(0);
}

