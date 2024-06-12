#include "pch.h"

#define _CLR_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include <windows.h>
#include <string>
using namespace std;
using namespace System;
using namespace System::IO;
#define ENTER 13
#define ESC 27
#define UP 72
#define DOWN 80
#define HOME 75
#define END 77

char menuList[7][120] = {
"Самая большая порция шашлыка на человека              ",
"Все виды шашлыка по алфавиту                          ",
"Самый долгий в мариновании шашлык с определенным мясом",
"Все виды шашлыка с мясом до N грамм                   ",
"Разные виды шашлыка с одинаковым временем             ",
"Диаграмма среднего времени маринования шашлыка        ",
"Выход                                                 "
};
char BlankLine[] = "                                                        ";
// BlankLine сотоит из 56 пробелов
// или так: char BlankLine[80]; memset (BlankLine,‘ ?,56);
//BlankLine[56] = 0;
int chisloStrok;

struct shashlik {
	char nazvanie[30];
	char myaso[20];
	int vremya;
	int kilo;
	long porcii;
};
struct spisokShashlik {
	char nazvanie[20];
	int vremya;
	struct spisokShashlik* sled;
	struct spisokShashlik* pred;
} *spisok;

int menu(int);
void PorciyaNaCheloveka(struct shashlik*);
void ShashlSOdnimMyasom(struct shashlik*);
void Alfavit(struct shashlik*);
void DolgiyShashlPoMyasu(struct shashlik*, char moloko[30]);
void ShashlDoMassi(struct shashlik*, int cost);
void Diagramma(struct shashlik*);
void SlozhniyVopros(struct shashlik*);

int main()
{
	system("chcp 1251");
	int i, menuLenght;
	FILE* inputFile;
	struct shashlik* mashlik;
	Console::CursorVisible::set(false);
	Console::BufferHeight = Console::WindowHeight;
	Console::BufferWidth = Console::WindowWidth;
	if ((inputFile = fopen("shashlik.txt", "r")) == NULL)
	{
		printf("\nФайл shashlik.txt не открыт !");
		getch(); exit(1);
	}
	fscanf(inputFile, "%d", &chisloStrok);
	mashlik = (struct shashlik*)malloc(chisloStrok * sizeof(struct shashlik));
	for (i = 0; i < chisloStrok; i++)
		fscanf(inputFile, "%s%s%d%d%ld", mashlik[i].nazvanie, mashlik[i].myaso,
			&mashlik[i].vremya, &mashlik[i].kilo, &mashlik[i].porcii);
	for (i = 0; i < chisloStrok; i++)
		printf("\n%-30s %-20s %7d %7d %ld", mashlik[i].nazvanie, mashlik[i].myaso,
			mashlik[i].vremya, mashlik[i].kilo, mashlik[i].porcii);
	getch();

	while (true)
	{
		Console::ForegroundColor = ConsoleColor::White;
		Console::BackgroundColor = ConsoleColor::DarkRed;
		Console::Clear();
		Console::ForegroundColor = ConsoleColor::White;
		Console::BackgroundColor = ConsoleColor::DarkBlue;
		Console::CursorLeft = 10;
		Console::CursorTop = 4;
		printf(BlankLine);
		for (i = 0; i < 7; i++)
		{
			Console::CursorLeft = 10;
			Console::CursorTop = i + 5;
			printf(" %s ", menuList[i]);
		}
		Console::CursorLeft = 10;
		Console::CursorTop = 12;
		printf(BlankLine);

		char tmpCharInput[20];
		menuLenght = menu(7);
		switch (menuLenght) {
		case 1: {
			Console::CursorTop = 14;
			Console::CursorLeft = 11;
			PorciyaNaCheloveka(mashlik); break;
		}
		case 2: Alfavit(mashlik); break;
		case 3: {
			Console::CursorTop = 14;
			Console::CursorLeft = 11;
			char tmp[20];
			printf("Введите вид мяса: ");
			scanf("%s", tmp);
			DolgiyShashlPoMyasu(mashlik, tmp); break;
		}
		case 4: {
			Console::CursorTop = 14;
			Console::CursorLeft = 11;
			int tmpInp;
			printf("Введите массу в граммах: ");
			scanf("%ld", &tmpInp);
			ShashlDoMassi(mashlik, tmpInp); break;
		}
		case 5: SlozhniyVopros(mashlik); break;
		case 6: Diagramma(mashlik); break;
		case 7: exit(0);
		}
	} // конец while(1)...
	return 0;
} // конец main()

int menu(int n)
{
	int y1 = 0, y2 = n - 1;
	char c = 1;
	while (c != ESC)
	{
		switch (c) {
		case DOWN: y2 = y1; y1++; break;
		case UP: y2 = y1; y1--; break;
		case HOME: y2 = y1; y1 = 0; break;
		case END: y2 = y1; y1 = 6; break;
		case ENTER: return y1 + 1;
		}
		if (y1 > n - 1) { y2 = n - 1; y1 = 0; }
		if (y1 < 0) { y2 = 0; y1 = n - 1; }
		Console::ForegroundColor = ConsoleColor::DarkYellow;
		Console::BackgroundColor = ConsoleColor::White;
		Console::CursorLeft = 11;
		Console::CursorTop = y1 + 5;
		printf("%s", menuList[y1]);
		Console::ForegroundColor = ConsoleColor::White;
		Console::BackgroundColor = ConsoleColor::DarkBlue;
		Console::CursorLeft = 11;
		Console::CursorTop = y2 + 5;
		printf("%s", menuList[y2]);
		c = getch();
	} // конец while(c!=ESC)...
	exit(0);
}

void PorciyaNaCheloveka(struct shashlik* mashlik) {
	Console::ForegroundColor = ConsoleColor::White;
	Console::BackgroundColor = ConsoleColor::DarkRed;
	Console::CursorTop = 16;
	Console::CursorLeft = 11;
	char tmp[30], tmp2[30];
	int tmpSum = mashlik[0].kilo / mashlik[0].porcii;
	strcpy(tmp, mashlik[0].nazvanie);
	strcpy(tmp2, mashlik[0].myaso);

	for (int i = 1; i < chisloStrok; i++) {
		if (mashlik[i].kilo / mashlik[i].porcii > tmpSum) {
			tmpSum = mashlik[i].kilo / mashlik[i].porcii;
			strcpy(tmp, mashlik[i].nazvanie);
			strcpy(tmp2, mashlik[i].myaso);
		}
	}
	printf("Порция шашлыка %s c мясом %s весит %d грамм", tmp, tmp2, tmpSum);
	getch();
}

void vstavka(struct shashlik* client, char* name)
{
	int i;
	struct spisokShashlik* nov, * nt, * z = 0;
	for (nt = spisok; nt != 0 && strcmp(nt->nazvanie, name) < 0; z = nt, nt = nt->sled);
	if (nt && strcmp(nt->nazvanie, name) == 0) return;
	nov = (struct spisokShashlik*)malloc(sizeof(struct spisokShashlik));
	strcpy(nov->nazvanie, name);
	nov->sled = nt;
	nov->pred = z;
	nov->vremya = 0;
	int tmp;
	int tmpi = 0;
	for (i = 0; i < chisloStrok; i++) {
		if (strcmp(client[i].nazvanie, name) == 0) {
			nov->vremya += client[i].vremya;
			tmpi++;
		}
	}
	nov->vremya = nov->vremya / tmpi;
	if (!z)
		spisok = nov;
	else
		z->sled = nov;
	if (nt)
		nt->pred = nov;
	return;
}


void Alfavit(struct shashlik* client)
{
	int i;
	struct spisokShashlik* nt;
	struct spisokShashlik* z;
	Console::ForegroundColor = ConsoleColor::White;
	Console::BackgroundColor = ConsoleColor::DarkBlue;
	Console::Clear();
	if (!spisok) {
		for (i = 0; i < chisloStrok; i++) {
			vstavka(client, client[i].nazvanie);
		}
	}
	Console::Clear();
	printf("\n Алфавитный список видов шашлыка                      Обратный алфавитный список видов шашлыка ");
	printf("\n \n");
	printf(" %-30s %s         %-30s %s", "Шашлык", "Среднее время", "Шашлык", "Среднее время");
	printf("\n");
	for (nt = spisok, z = 0; nt != 0; z = nt, nt = nt->sled);
	for (nt = z, i = 0; nt != 0; i++, nt = nt->pred)
	{
		printf("\n %s %-30s %d", "                                                    ", nt->nazvanie, nt->vremya);
	}
	for (nt = spisok, i = 0; nt != 0; i++, nt = nt->sled) {
		Console::CursorTop = 4 + i;
		printf("\n %-30s %d", nt->nazvanie, nt->vremya);
	}
	getch();
}

void DolgiyShashlPoMyasu(struct shashlik* mashlik, char myaso[30])
{
	Console::ForegroundColor = ConsoleColor::White;
	Console::BackgroundColor = ConsoleColor::DarkRed;
	int overall = 0;
	char tmp[30], tmp2[30];
	Console::CursorTop = 16;
	Console::CursorLeft = 11;

	for (int i = 0; i < chisloStrok; i++) {
		if ((mashlik[i].vremya) > overall && strcmp(mashlik[i].myaso, myaso) == 0) {
			strcpy(tmp, mashlik[i].nazvanie);
			strcpy(tmp2, mashlik[i].myaso);
			overall = mashlik[i].vremya;
		}
	}
	printf("Шашлык %s с мясом %s маринуется дольше всего: %d минут", tmp, tmp2, overall);
	getch();
}

void ShashlDoMassi(struct shashlik* clients, int kilo)
{
	Console::ForegroundColor = ConsoleColor::White;
	Console::BackgroundColor = ConsoleColor::DarkRed;
	Console::CursorTop = 16;
	Console::CursorLeft = 11;
	printf("Все виды шашлыка с мясом до %d грамм: ", kilo);
	for (int i = 0; i < chisloStrok; i++) {
		if (clients[i].kilo < kilo) {
			printf("%s, ", clients[i].nazvanie);
		}
	}
	getch();
}

void Diagramma(struct shashlik* clients)
{
	struct spisokShashlik* nt;
	int len, i, NColor;
	long sum = 0;
	char str1[30];
	char str2[20];
	System::ConsoleColor Color;
	Console::ForegroundColor = ConsoleColor::Black;
	Console::BackgroundColor = ConsoleColor::White;
	Console::Clear();
	if (!spisok)
		for (i = 0; i < chisloStrok; i++)
			vstavka(clients, clients[i].nazvanie);
	for (nt = spisok, i = 0; nt != 0; nt = nt->sled, i++)
	{
		sum += nt->vremya;
	}
	if (!spisok)
		for (i = 0; i < chisloStrok; i++)
			vstavka(clients, clients[i].nazvanie);
	Color = ConsoleColor::Black; NColor = 0;
	for (nt = spisok, i = 0; nt != 0; nt = nt->sled, i++)
	{
		sprintf(str1, "%s", nt->nazvanie);
		sprintf(str2, "%5.1f%%", (nt->vremya * 100. / sum));
		Console::ForegroundColor = ConsoleColor::White;
		Console::BackgroundColor = ConsoleColor::DarkBlue;
		Console::CursorLeft = 5; Console::CursorTop = i + 1;
		printf(str1);
		Console::CursorLeft = 30;
		printf(" % s", str2);
		Console::BackgroundColor = ++Color; NColor++;
		Console::CursorLeft = 38;
		for (len = 0; len < nt->vremya * 100 / sum; len++) printf(" ");
		if (NColor == 14)
		{
			Color = ConsoleColor::Black; NColor = 0;
		}
	}
	getch();
	return;
}

void SlozhniyVopros(struct shashlik* mashlik) {
	bool check = false;
	Console::BackgroundColor = ConsoleColor::DarkMagenta;
	Console::CursorTop = 16;
	Console::CursorLeft = 11;
	printf("Одинаковое время мариновки у ");
	for (int i = 0; i < chisloStrok; i++) {
		for (int j = i + 1; j < chisloStrok; j++) {
			if ((mashlik[i].vremya == mashlik[j].vremya) && strcmp(mashlik[i].nazvanie, mashlik[j].nazvanie) != 0) {
				Console::CursorTop = 17;
				Console::CursorLeft = 11;
				printf("%s %d минут, ", mashlik[i].nazvanie, mashlik[i].vremya);
				printf("%s %d минут", mashlik[j].nazvanie, mashlik[j].vremya);
				check = true;
				break;
			}
		}
		if (check) break;
	}
	if (!check) printf("Нет одинакового времени");
	getch();
}