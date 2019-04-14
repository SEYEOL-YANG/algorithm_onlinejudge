#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <queue>
using namespace std;

const int MAX = 26;
const int PIPE_TYPE = 7;
int R, C;
char map[MAX][MAX];
/*
Z.1----4..
|.|....|..
|..14..M..
2-+++4....
..2323....
..........

BFS 탐색 25*25
한 빈칸을 수정하는데 25*25*7
총 2백만
*/

char pipe[PIPE_TYPE] = {'|', '-', '+', '1','2' ,'3' ,'4' };
int totalPipeCount = 0;

struct Pos {
	int mX;
	int mY;
	int mPrevX;
	int mPrevY;
	int mPipeCount;
	Pos(int x, int y, int prevX, int prevY, int pipeCount):
		mX(x), mY(y), mPrevX(prevX), mPrevY(prevY), mPipeCount(pipeCount)
	{}
};
//           오,왼,아,위
int dirX[] = {0,0,1,-1};
int dirY[] = {1,-1,0,0};
bool CheckConnected(int newX, int newY, int x, int y, int prevX, int prevY) 
{
	if (map[newX][newY] == '.')
		return false;
	// 아래쪽으로
	else if (newX - x == 1) {
		if (prevX == newX)
			return false;
		if (map[x][y] == '+')
		{
			return (map[newX][newY] == '2' || map[newX][newY] == '3' || map[newX][newY] == '|' || 
				    map[newX][newY] == '+' || map[newX][newY] == 'Z') &&
					prevY == newY;
		}
		else if (map[newX][newY] == '|' || map[newX][newY] == '2' ||
		    	map[newX][newY] == '3' || map[newX][newY] == '+' || map[newX][newY] == 'Z')
		{
			return map[x][y] == '|' || map[x][y] == '1' || 
				   map[x][y] == '4' || map[x][y] == 'M';
		}
	}
	// 위쪽으로
	else if (x - newX == 1) 
	{
		if (prevX == newX)
			return false;
		if (map[x][y] == '+') 
		{
			return (map[newX][newY] == '1' || map[newX][newY] == '4' || map[newX][newY] == '|' ||
				    map[newX][newY] == '+' || map[newX][newY] == 'Z') &&
					prevY == newY;
		}
		else if (map[newX][newY] == '|' || map[newX][newY] == '1' ||
		      	map[newX][newY] == '4' || map[newX][newY] == '+' || map[newX][newY] == 'Z')
		{
			return (map[x][y] == '|' || map[x][y] == '2' || 
				   map[x][y] == '3' || map[x][y] == 'M') && prevX != newX;
		}
	}
	// 왼쪽으로
	else if (y - newY == 1) {
		if (prevY == newY) 
			return false;
		else if (map[x][y] == '+') {
			return (map[newX][newY] == '1' || map[newX][newY] == '2' || map[newX][newY] == '-' || 
				    map[newX][newY] == '+' || map[newX][newY] == 'Z') &&
				    prevX == newX ;
		}
		else if (map[newX][newY] == '-' || map[newX][newY] == '1' ||
			     map[newX][newY] == '2' || map[newX][newY] == '+' || map[newX][newY] == 'Z')
		{
			return (map[x][y] == '-' || map[x][y] == '3' || 
				   map[x][y] == '4' || map[x][y] == 'M') && prevY != newY;
		}
	}
	// 오른쪽으로
	else if (newY - y == 1) {
		if (prevY == newY) 
			return false;
		if (map[x][y] == '+') {
			return (map[newX][newY] == '3' || map[newX][newY] == '4' || map[newX][newY] == '-' || 
			    	map[newX][newY] == '+' || map[newX][newY] == 'Z') &&
				    prevX == newX;
		}
		else if (map[newX][newY] == '-' || map[newX][newY] == '3' || 
			     map[newX][newY] == '4' || map[newX][newY] == '+' || map[newX][newY] == 'Z')
		{
			return (map[x][y] == '-' || map[x][y] == '1' ||
				   map[x][y] == '2' || map[x][y] == 'M');
		}
	}

	return false;
}

bool BFS(int startX,int startY,int endX, int endY)
{
	queue<Pos> Queue;
	int visited[MAX][MAX] = { 0, };
	visited[startX][startY] = 1;
	Queue.push(Pos(startX, startY, -1, -1, 1));
	while (!Queue.empty()) {
		Pos pos = Queue.front();
		Queue.pop();
		if (pos.mX == endX && pos.mY == endY) {
			return pos.mPipeCount >= totalPipeCount +1;			
		}

		// 4방향 체크 -> 길있는지 확인하기
		for (int i = 0; i < 4; i++) {
			int newX = pos.mX + dirX[i];
			int newY = pos.mY + dirY[i];
			if (newX < 0 || newX >= R || newY < 0 || newY >= C) 
				continue;
			if ((visited[newX][newY] == 0 || visited[newX][newY] == 1 && map[newX][newY] == '+') &&
				CheckConnected(newX, newY, pos.mX, pos.mY, pos.mPrevX, pos.mPrevY)) 
			{			
				int pipeCount = pos.mPipeCount;
				if (visited[newX][newY] == 0) pipeCount++;
				Queue.push(Pos(newX, newY, pos.mX, pos.mY, pipeCount));
				visited[newX][newY]++;
				break;
			}
		}
	}
	return false;
}


int main() {
	int startX, startY;
	int endX, endY;
	scanf("%d%d\n", &R, &C);
	for (int i = 0; i < R; i++) 
	{
		for (int j = 0; j < C; j++)
		{
			scanf("%c", &map[i][j]);
			if (map[i][j] == 'M')
			{
				startX = i;
				startY = j;
			}
			else if (map[i][j] == 'Z') 
			{
				endX = i;
				endY = j;
			}

			if (map[i][j] != '.') {
				totalPipeCount++;
			}
		}
		getchar();
	}

	for (int i = 0;i < R; i++) 
	{
		for (int j = 0; j < C; j++) 
		{
			if (map[i][j] == '.') {
				for (int k = 0; k < PIPE_TYPE; k++) {
					map[i][j] = pipe[k];
					if (BFS(startX, startY, endX, endY)) {
						printf("%d %d %c", i+1, j+1, pipe[k]);
						return 0;
					}
				}
				map[i][j] = '.';
			}
		}
	}
	

	return 0;
}