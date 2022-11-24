// simutalionOfEngine.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "engine.h"

using namespace std;
//Пользователь вводит: Темп. окр.среды (цельсия)
void repaintConsole()
{
	HANDLE hd = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cd;
	cd.X = 0;
	cd.Y = 0;
	SetConsoleCursorPosition(hd, cd);
}

template <typename T>
void checkEnter(istream& ios, T& value) {//проверка ввода
	while (!(ios >> value) || ios.get() != '\n') // cin.fail() возвращает 1 если установлет failbit
	{
		cout << "Нужны только цифры!\n";
		ios.clear(); // сбрасывает все биты потока, тоесть поток становится "хорошим"
		ios.sync();//Удалим данные из буффера
		cin.ignore(999, '\n');
		cout << "Введите число: ";
	}
}

void drawMenu(int& index) {
	string head = "Симуляция ДВС. По умолчанию температура окружающей среды = 20С (Выберите стрелочками одну из доступных команд )\n";
	string arrStr[] = {
		" Старт двигателя (Начало симуляции)[space] \n",
		" Ввести температуру окр. среды \n" ,
		" Результаты  \n",
		" Выход \n"  };

	int size = sizeof(arrStr) / sizeof(string);
	if (index >= size) {
		index = 0;
	}
	else {
		if (index < 0) {
			index = size - 1;
		}
	}
	arrStr[index].insert(arrStr[index].begin(), '>');
	repaintConsole();
	cout << head;
	for (string var : arrStr)
	{
		cout << var;
	}
}

void hideCursor() {
	void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &structCursorInfo);
}

//Вывод время до перегрева в секундах
int main()
{
	system("chcp 1251 > nul");

	hideCursor();
	int index = 0;
	engine eng ;
	float temp;
	while (true) {
		
		drawMenu(index);
		cout << "Текущая температура окружающей среды = " << eng.getEnvTemp() <<"C";
		switch (_getch())//27-esc 13-enter 8 - backspace 32-space
		{
		case 80://up
			index++;
			break;
		case 72://down
			index--;
			break;
		case 32://space
			system("cls");
			eng.start();
			cout << "Симуляция проведена!\n";
			system("pause");
			system("cls");
			break;
		case 8:
		case 27://exit
			return 0;
			break;
		case 13://enter
			switch (index)
			{
			case 0://старт
				system("cls");
				eng.start();
				cout << "Симуляция проведена!\n";
				system("pause");
				system("cls");
				break;
			case 1://Ввести темпер среды
				system("cls");
				cout << "Введите температуру окружающей среды:";
				checkEnter(cin, temp);
				eng = engine(temp);
				system("pause");
				system("cls");
				break;
			case 2://Результаты
				system("cls");
				cout << "Время, которое понадобилось двигателю для перегрева при температуре окр.среды " << eng.getEnvTemp() << "C :";
				eng.getSimulationTime() == 0 ? 	cout << " симуляция не запускалась!": cout << eng.getSimulationTime() << " секунд";
				cout << endl;
				system("pause");
				system("cls");
				break;
			case 3://Выход
				system("cls");
				exit(0);
				system("pause");
				system("cls");
				break;
			}
			break;

		}
	}
}

