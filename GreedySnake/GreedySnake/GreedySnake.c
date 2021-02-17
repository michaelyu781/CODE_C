#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include <conio.h>
#include<time.h>
#include<malloc.h>
#define triangle "▲"
#define square "■" 
const char* block_pattern[2] = { square,triangle };
char* map[20][20] = { {0} };
const int dir[4][2] = { {0,-1},{0,1},{-1,0},{1,0} };//dir:WEST=0,EAST=1,NORTH=2,SOUTH=3
int score = 0;

struct snake_struct {
	int x;
	int y;
	int dir;
	struct snake_struct* next;
};
struct snake_struct* head,*tail;


/***************************************************************************
  函数名称：gotoxy
  功    能：将光标移动到指定位置
  输入参数：HANDLE hout：输出设备句柄
			int X      ：指定位置的x坐标
			int Y      ：指定位置的y坐标
  返 回 值：无
***************************************************************************/
void gotoxy(const HANDLE hout, const int X, const int Y)
{
	COORD coord;
	coord.X = X;
	coord.Y = Y;
	SetConsoleCursorPosition(hout, coord);
}

/***************************************************************************
  函数名称：HideCursor
  功    能：隐藏光标
  输入参数：无
  返 回 值：无
***************************************************************************/
void HideCursor() {
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hout, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hout, &cci);
}

struct snake_struct* create_snake() {
	struct snake_struct* snake = (struct snake_struct*)malloc(sizeof(struct snake_struct));
	return snake;
}

void init_head() {
	srand((unsigned int)time(NULL));
	int x, y;
start:
	x = rand() % 18 + 1;
	y = rand() % 18 + 1;
	if (x > 16) goto start;
	head = create_snake();
	tail = head;
	head->x = x;
	head->y = y;
	head->dir = 0;
	for (int i = 0; i < 2; i++)
	{
		struct snake_struct* snake = create_snake();
		tail=tail->next = snake;
		snake->x = x;
		snake->y = y + i + 1;
		snake->dir = 0;
	}
	tail->next = NULL;
}

void draw_snake() {
	struct snake_struct* p=head;
	while (p != NULL) {
		map[p->x][p->y] = block_pattern[0];
		p = p->next;
	}
}


void draw_food() {
	//srand(time(NULL));
	int x = rand() % 18 + 1;
	int y = rand() % 18 + 1;
	map[x][y] = block_pattern[1];
}

//function init()is used to intialize map includeing(block,snake and the first place of food)
void init_map() {
	for (int i = 0; i < 20; i++) map[19][i] = map[0][i] = block_pattern[0];
	for (int i = 1; i < 20; i++) map[i][0] = map[i][19] = block_pattern[0];
	for (int i = 1; i < 19; i++)
		for (int j = 1; j < 19; j++)
			map[i][j] = "  ";
	init_head();
	draw_snake();
	draw_food();
}

void snake_move() { 
	struct snake_struct* p = create_snake();
	p->x = head->x + dir[head->dir][0];
	p->y = head->y + dir[head->dir][1];
	p->dir = head->dir;
	p->next = head;
	head = p;//move the head
	while (p->next != tail) p = p->next;
	map[p->next->x][p->next->y] = "  ";
	free(p->next);
	p->next = NULL;
	tail = p;
	draw_snake();//move other spaces of the snake
}

/***************************************************************************
  函数名称：GetAsyncKeyState()
  功    能：确定用户当前是否按下了键盘上某个键.若按，返回非0，否则返回0
			特别的：持续按着高位非0，否则低位非0
			使用中将参数&1，用于区分
			（异步检测）
  输入参数：希望被检测的字母
  返 回 值：short型
***************************************************************************/
//dir:WEST=0,EAST=1,NORTH=2,SOUTH=3
void snake_dir() {
	if (GetAsyncKeyState('A'))
		head->dir = 0;
	if (GetAsyncKeyState('D'))
		head->dir = 1;
	if (GetAsyncKeyState('W'))
		head->dir = 2;
	if (GetAsyncKeyState('S'))
		head->dir = 3;
}

void print_map() {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++)
			printf("%s", map[i][j]);
		printf("\n");
	}		
}

char menu() {
	printf("Welcome to the GreedySnake\n");
	printf("\tAuthor:Michael\n");
	printf("using WASD to control the moving directon of the snake\n\n");
	printf("Enter 'E' or 'e' for easy\n");
	printf("Enter 'M' or 'm' for medium\n");
	printf("Enter 'H' or 'h' for hard\n");
	printf("your choice:");
	char ch = getchar();
	return ch;
}

//1 refer to snake die,0 refer to not
int is_snake_die() {
	if (head->x == 0 || head->x == 19) return 1;   //knock the wall
	if (head->y == 0 || head->y == 19) return 1;   //knock the wall
	if (map[head->x+dir[head->dir][0]][head->y+dir[head->dir][1]] == block_pattern[0]) {
		if (head->dir == 0 && head->next->dir == 1) return 0;
		if (head->dir == 1 && head->next->dir == 0) return 0;
		if (head->dir == 2 && head->next->dir == 3) return 0;
		if (head->dir == 3 && head->next->dir == 2) return 0;
		return 1;								   //eat itself
	}
	return 0;
}

int is_clean() {
	for (int i = 1; i < 19; i++)
		for (int j = 1; j < 19; j++)
			if (map[i][j] == block_pattern[1]) return 0;
	return 1;
}

void is_food_eaten() {
	if (map[head->x+dir[head->dir][0]][head->y+dir[head->dir][1]] == block_pattern[1]) {
		score += 10;
		struct snake_struct* p;
		p = (struct snake_struct*)malloc(sizeof(struct snake_struct));
		p->next = NULL;
		p->x = tail->x - dir[tail->dir][0];
		p->y = tail->y - dir[tail->dir][1];
		tail = tail->next = p;
		draw_snake();
		draw_food();
	}
	if (is_clean()) draw_food();
}

int main() {
	const HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE); //取标准输出设备对应的句柄
	HideCursor();// 使标准输出（控制台）影藏光标。
	char level = menu();
	system("cls");
	fflush(stdin);
	init_map();
	print_map();
	while (1) {
		gotoxy(hout, 0, 0);
		switch (level)
		{
		case 'e':case 'E':for (long i = 0; i < 100000000; i++); break;//this is used to slow the moving speed
		case 'm':case 'M':for (long i = 0; i < 100000; i++); break;
		case 'h':case 'H':break;
		default:printf("Please enter again:");
		}
		snake_dir();
		is_food_eaten();
		if (is_snake_die()) break;
		snake_move();
		print_map();
		gotoxy(hout, 0, 20);
		printf("SCORE:%d", score);
	}
	gotoxy(hout, 0, 21);
	printf("GAME OVER\n");
	system("pause");
	return 0;
}