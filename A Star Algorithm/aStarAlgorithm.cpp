#include "aStar.h"
#include <stdio.h>
#include <stdlib.h>
#include <queue>
using namespace std;
typedef struct Point { int x, y, h, m; } point;
bool operator<(point desc, point src) {
	return desc.h == src.h ? desc.x == src.x ? desc.y < src.y : desc.x < src.y : desc.h < src.h;
}
bool operator>(point desc, point src) {
	return desc.h == src.h ? desc.x == src.x ? desc.y > src.y : desc.x > src.y : desc.h > src.h;
}
int getH(point e, point now) {
	return (abs(e.y - now.y) + abs(e.x - now.x)) * 10;
}
void aStar(int** map, const int size, int**& origin, point e, point s) {
	const int	dx[] = { -1,-1,-1, 0,0, 1,1,1,0 },
				dy[] = { -1, 0, 1,-1,1,-1,0,1,0 },
		v[] = { 14,10,14,10,10,14,10,14 };
	priority_queue<point,vector<point>,greater<point>> que;
	int** vMap = new int*[size];
	int** dirMap = new int*[size];
	for (int i = 0; i < size; i++) {
		vMap[i] = new int[size];
		dirMap[i] = new int[size];
		for (int j = 0; j < size; j++) {
			vMap[i][j] = 2e9;//INFINITY
			dirMap[i][j] = 8;	//방향 없음
		}
	}
	s.h = getH(e, s);
	s.m = 0;
	vMap[s.x][s.y] = s.h;
	que.push(s);
	while (que.size()) {
		point p = que.top(); que.pop();
		if (getH(e,p)==0) {
			point b = p;
			while (true) {
				b.x -= dx[dirMap[p.x][p.y]];
				b.y -= dy[dirMap[p.x][p.y]];
				if (b.x == s.x&&b.y == s.y)return;
				origin[b.x][b.y] = 10;
				p = b;
			}
			return;
		}
		if(getH(s,p)&&origin[p.x][p.y]!=10)
			origin[p.x][p.y] = 11;
		for (int i = 0; i < 8; i++) {
			int tx = p.x + dx[i],
				ty = p.y + dy[i];
			if (map[tx][p.y] && map[p.x][ty] && map[tx][ty] &&
				vMap[tx][ty] > getH(e, { tx,ty }) + p.m + v[i]) {
				vMap[tx][ty] = getH(e, { tx,ty }) + p.m + v[i];
				que.push({ tx,ty,getH(e,{ tx,ty }) + p.m + v[i] ,p.m + v[i] });
				dirMap[tx][ty] = i;
			}
		}
	}
	for (int i = 0; i < size; i++) {
		delete[] vMap[i];
		delete[] dirMap[i];
	}
	delete[] vMap;
	delete[] dirMap;
}

void multi_search(int**& map, const int size) {
	int** simpleMap = new int*[size];
	point p;
	vector<point> m;
	int cnt = 0;
	for (int i = 0; i < size; i++) {
		simpleMap[i] = new int[size];
		for (int j = 0; j < size; j++) {
			switch (map[i][j]) {
			case 0:case 2:case 6:simpleMap[i][j] = 0; break;
			case 1:case 5:case 3:case 4:simpleMap[i][j] = 1; break;
			case 7:simpleMap[i][j] = 1; p = { i,j }; break;
			case 8:simpleMap[i][j] = 1; m.push_back({ i,j }); break;
			}
		}
	}
	for (auto i : m) {
		aStar(simpleMap, size, map, p, i);
	}
	for (int i = 0; i < size; i++)
		delete[] simpleMap[i];
	delete[] simpleMap;
}