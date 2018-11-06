#include "aStar.h"
#include <stdio.h>
#include <malloc.h>
#include <Windows.h>
#include <string.h>
#include <conio.h>
#include <io.h>
#define blank "　"
#define block "■"
#define star "★"


void gotoxy(short x, short y) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 2 * x,y });
}

void color(short fc, short bgc) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fc + bgc * 16);
}

void resizeWindow(int size) {
	char str[100];
	sprintf(str, "mode con: lines=%d cols=%d", 5000, size * 2 + 3);
	system(str);
}

void drawMap(int** map, const int size) {
	for (int j = 0; j < size; j++) {
		for (int i = 0; i < size; i++) {
			gotoxy(i, j);
			switch (map[i][j]) {
			case 0:	//noGround
				color(0, 4);
				printf(blank);
				break;
			case 1:	//floor
			case 5:	//road
				color(0, 8);
				printf(blank);
				break;
			case 2:	//wall
				color(0, 15);
				printf(blank);
				break;
			//case 3:	//door
			//case 4:
			//	color(6, 8);
			//	printf(block);
			//	break;
			case 6:	//road-wall
				color(0, 7);
				printf(blank);
				break;
			case 7:	//Player
				color(9, 8);
				printf(star);
				break;
			case 8:	//Monster?
				color(12, 8);
				printf(star);
				break;
			case 9:
				color(0, 10);
				printf(blank);
				break;
			case 10:
				color(0, 13);
				printf(blank);
			case 11:
				color(0, 14);
				printf(blank);
				break;
			default:
				color(0, 8);
				printf(blank);
			}
		}
	}
	color(0, 0);
	for (int i = 0; i < size; i++) {
		gotoxy(size, i);
		printf(blank);
	}
}

int** load(int& size,unsigned int& seed) {
	char* location = "Map.mpb";
	if (_access(location, 0) != 0) {
		location = "C:\\Users\\양정모\\Documents\\Visual Studio 2017\\Projects\\MapBuilder\\MapBuilder\\Map.mpb";
		if (_access(location, 0) != 0) {
			printf("파일 경로 입력 : ");
			scanf("%s", location);
		}
	}
	FILE *fp = fopen(location,"rt");
	if (fp == NULL) {
		printf(location);
		printf(" 파일을 불러오는데 실패했습니다.\n");
		return NULL;
	}
	fscanf(fp, "%d %u", &size, &seed);
	int **arr = (int**)malloc(sizeof(int*)*size);
	for (int i = 0; i < size; i++) {
		arr[i] = (int*)malloc(sizeof(int)*size);
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			fscanf(fp, "%d", &arr[j][i]);
		}
	}
	fclose(fp);
	return arr;
}
typedef struct point { int x, y; } pt;
int move(int **map, int n,pt p) {
	switch (_getch()) {
	case -32:
		switch (_getch()) {
		case 72:
			break;
		case 75:
			break;
		case 77:
			break;
		case 80:
			break;
		}
		break;
	}
}

int main() {
	int  **map, n;
	unsigned int seed;
	map = load(n, seed);
	resizeWindow(n);
	//drawMap(map, n);
	//_getch();
	//search(map, n);
	pt p;
	multi_search(map, n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (map[i][j] == 7) { p = { i,j }; break; }
		}
	}
	drawMap(map, n);
	color(15, 0);
	_getch();
}