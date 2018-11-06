#include "aStar.h"
#include <stdio.h>
#include <malloc.h>
typedef struct Point {
	int x, y;
}pt;
typedef struct Data { pt n, p; int f, g, h; struct Data* d; } dt;

#define swap(x,y){dt z=x;x=y;y=z;}

int getHuristic(pt p, pt n) {
	int x = p.x - n.x;
	int y = p.y - n.y;
	x = x < 0 ? -x : x;
	y = y < 0 ? -y : y;
	return (x + y) * 10;
}

int select(dt* openList,const int oLSize) {
	int min = 1000000000, hNum = 1000000000 , pos = -1;
	for (int i = 0; i < oLSize; i++) {
		if (openList[i].f == min) {
			if (openList[i].h > hNum) {
				pos = i;
				hNum = openList[i].h;
			}
		}
		if (openList[i].f < min) {
			min = openList[i].f;
			hNum = openList[i].h;
			pos = i;
		}
	}
	return pos;
}

void open(dt* openList, int& oLSize,dt data,dt* closedList,const int closedSize) {
	for (int i = 0; i < oLSize; i++) {
		if (openList[i].n.x == data.n.x&&openList[i].n.y == data.n.y) {
			for (int j = 0; j < closedSize; j++) {
				if (closedList[j].p.x == data.n.x&&closedList[j].p.y == data.n.y)return;
			}
			if (openList[i].f > data.f) {
				openList[i] = data;
			}
			return;
		}
	}
	openList[oLSize++] = data;
}

void aStar(int **map,const int size,int** originMap,pt p,pt m) {
	dt *openList = (dt*)malloc(sizeof(dt) * 100000), *closedList = (dt*)malloc(sizeof(dt) * 100000);
	int oLSize = 0;
	int cLSize = 0;
	//openList[oLSize++] = { m ,{-1,-1},getHuristic(p,m)+0, 0,getHuristic(p,m)};
	open(openList, oLSize, { m,{-1,-1},getHuristic(p,m) + 0,0,getHuristic(p,m),NULL },closedList,cLSize);
	int testFlag = 0;
	while (oLSize > 0) {
		int sel = select(openList, oLSize);//openList에서 sel을 제거 후 closedList에 추가
		closedList[cLSize++] = openList[sel];
		swap(openList[sel], openList[oLSize - 1]);
		oLSize--;
		dt tmp=closedList[cLSize - 1];
		if (tmp.n.x == p.x&&tmp.n.y == p.y) {
			tmp = *tmp.d;
			while (tmp.d!=NULL) {
				originMap[tmp.n.x][tmp.n.y] = 10;
				tmp = *tmp.d;
			}
			break; 
		}
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (i == 0 && j == 0)continue;
				if ((tmp.n.x+i>=0&&tmp.n.x+i<size&&
					tmp.n.y+j>=0&&tmp.n.y+j<size)&&
					(map[tmp.n.x+i][tmp.n.y+j]&&map[tmp.n.x+i][tmp.n.y]&&map[tmp.n.x][tmp.n.y+j])) {
					dt data = {
						{ tmp.n.x + i,tmp.n.y + j },
						tmp.n,
						tmp.g + 10 + (i*j ? 4 : 0) + getHuristic(p,{ tmp.n.x + i,tmp.n.y + j }),
						tmp.g + 10 + (i*j ? 4 : 0),
						getHuristic(p,{ tmp.n.x + i,tmp.n.y + j }),
						&closedList[cLSize - 1]
					};
					open(openList, oLSize, data,closedList,cLSize);
					//printf("(%2d,%2d)\tf : %d\tg : %d\th : %d\n", data.n.x, data.n.y, data.f, data.g, data.h);
				}
			}
		}
		//printf("%d\n", oLSize);
		//if (testFlag++ > 50)return;
	}
}

void search(int** map, const int size) {
	int **simpleMap = (int**)malloc(sizeof(int*)*size);
	pt p, m;
	for (int i = 0; i < size; i++) {
		simpleMap[i] = (int*)malloc(sizeof(int)*size);
		for (int j = 0; j < size; j++) {
			switch (map[i][j]) {
			case 0:case 2:case 6:simpleMap[i][j] = 0;break;
			case 1:case 5:case 3:case 4:simpleMap[i][j] = 1;break;
			case 7:simpleMap[i][j] = 1;p = { i,j };break;
			case 8:simpleMap[i][j] = 1;m = { i,j };break;}
			//map[i][j] = simpleMap[i][j];
		}
	}
	aStar(simpleMap, size, map, p, m);
}