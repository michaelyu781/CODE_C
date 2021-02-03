/**************************************
*******tic-toc-toe-game
**************************************/
#include <stdio.h>
#include<malloc.h>
#include<stdlib.h>

char *create_board()                                    //create a blank board
{
    char (*b)[3]=(char**)malloc(sizeof(char**));
    int i,j;
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            b[i][j]=' ';
    return b;
}

//another way to judge who win the game:by comparing whether the characters are the same.
int winner_judge(int (*board)[3])
{
    for(int i=0;i<3;i++)
    {
        if (board[i][0] == board[i][1] && board[i][0] == board[i][2])
            return 1;
        else if(board[0][i] == board[1][i] && board[0][i] == board[2][i])
            return 1;
        else if(board[0][0]==board[1][1]&&board[0][0]==board[2][2])
            return 1;
        else if(board[2][0]==board[1][1]&&board[2][0]==board[0][2])
            return 1;
    }
    return 0;
}

void print_board(char(*b)[3])                           //print board
{
    for(int i=0;i<4;i++)
        printf("%d|",i);
    printf("\n");
    for(int i=0;i<3;i++)
    {
        printf("%c|", i + 'A');
        for(int j=0;j<3;j++)
            printf("%c|",b[i][j]);
        printf("\n");
    }
}

void chess_place_X(char letter,int number,char (*b)[3])
{
    b[letter-'A'][number-1]='X';
}

void chess_place_O(char letter,int number,char(*b)[3])
{
    b[letter-'A'][number-1]='O';
}

int is_player1_win(char (*b)[3])
{
    if(b[1][1]=='X')
    {
        if(b[0][1]=='X'&&b[2][1]=='X') return 1;
        if(b[0][1]=='X'&&b[1][2]=='X') return 1;
        if(b[0][0]=='X'&&b[2][2]=='X') return 1;
        if(b[2][0]=='X'&&b[0][2]=='X') return 1;
    }
   if(b[0][0]=='X')
   {
       if(b[0][1]=='X'&&b[0][2]=='X') return 1;
       if(b[1][0]=='X'&&b[2][0]=='X') return 1;
   }
   if(b[2][2]=='X')
   {
       if(b[2][0]=='X'&&b[2][1]=='X') return 1;
       if(b[0][2]=='X'&&b[1][2]=='X') return 1;
   }
   return 0;
}

int is_player2_win(char (*b)[3])
{
    if(b[1][1]=='O')
    {
        if(b[0][1]=='O'&&b[2][1]=='O') return 1;
        if(b[0][1]=='O'&&b[1][2]=='O') return 1;
        if(b[0][0]=='O'&&b[2][2]=='O') return 1;
        if(b[2][1]=='O'&&b[0][2]=='O') return 1;
    }
    if(b[0][0]=='O')
    {
        if(b[0][1]=='O'&&b[0][2]=='O') return 1;
        if(b[1][0]=='O'&&b[2][0]=='O') return 1;
    }
    if(b[2][2]=='O')
    {
        if(b[2][0]=='O'&&b[2][1]=='O') return 1;
        if(b[0][2]=='O'&&b[1][2]=='O') return 1;
    }
    return 0;
}

int is_draw(char(*board)[3])                                  //this is used to judge whether the game is over
{
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            if(board[i][j]==' ') return 0;
    return 1;
}

int main()
{
    int i=1;                                                  //i is used to judge who's turn
    void (*p)(char,int,char(*)[3]);                           //p is used to judge palce which chess
    char (*board)[3]=create_board();
    print_board(board);
    while(is_draw(board))
    {
        char letter;int number;
        if(i%2)
        {
            p=chess_place_X;
            printf("player1's turn:");
        }
        if((i+1)%2)
        {
            p=chess_place_O;
            printf("player2's turn:");
        }
        start:scanf("%c%d",&letter,&number);
        fflush(stdin);
        if(letter<'A'||letter>'C'||number<1||number>3) {printf("wrong!input again:");goto start;}
        if(board[letter-'A'][number-1]!=' ') {printf("space occupied!input again:");goto start;}
        p(letter,number,board);
        print_board(board);
        i++;
        if(is_player1_win(board)) {printf("player1 win!\n");break;}
        if(is_player2_win(board)) {printf("player2 win!\n");break;}
    }
    system("pause");
    return 0;
}