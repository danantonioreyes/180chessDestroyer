/*
 *  Chess Template
 *  Copyright 2013. Tristan M. Basa. All rights reserved.
 *
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include "shiva.h"


void white_move(int *i1,int *j1,int *i2,int *j2) {
        char from[3],to[3];

        printf("\nWhite move: ");
        scanf("%s %s",from,to);

        *i1=from[0]-97;
        *i2=to[0]-97;
        *j1=from[1]-49;
        *j2=to[1]-49;

}

//void movepiece(int **board,int i1,int j1,int i2,int j2) {
//
//        board[i2][j2]=board[i1][j1];
//        board[i1][j1]=BLANK;
//
//        //check if castling
//        if (board[i2][j2]==BLACKKING OR board[i2][j2]==WHITEKING)
//        {
//                if (j2>j1+1) {
//                        board[board[i2][j2]-1][5]=board[i2][j2]+4;
//                        board[board[i2][j2]-1][7]=BLANK;
//                }
//                if (j2<j1-2) {
//                        board[board[i2][j2]-1][2]=board[i2][j2]+4;
//                        board[board[i2][j2]-1][0]=BLANK;            
//                }
//        }
//
//        checkqueening(board);
//
//        //if kings moved, no more castling
//        if (board[i2][j2]==BLACKKING)
//                bkmoved=1;
//        if (board[i2][j2]==WHITEKING)
//                wkmoved=1;
//}

void black_move(int** board, int *i1,int *j1,int *i2,int *j2) {
        Node* root = createNode((Movement){0}, INFTY, -INFTY, 1, NULL, NULL);
        minimax(board, BLACK, MAX_DEPTH, root);
        *i1 = root->movement.source.x;
        *j1 = root->movement.source.y;
        *i2 = root->movement.dest.x;
        *j2 = root->movement.dest.y;
}



int main(int argc,char *argv[]) {
        srand(time(NULL));

        int i,i1,j1,i2,j2,checkdir,winner;
        int **board;

        board=(int**)malloc(sizeof(int*)*8);
        for (i=0; i<8; i++) 
                board[i]=(int*)malloc(sizeof(int)*8);

        winner=0;
        init_board(board);
        do {
                display_board(board);
                //printf("Board score: %d", scoreOfBoard(board));
                white_move(&i1,&j1,&i2,&j2);
                if (valid_move(board,i1,j1,i2,j2,WHITEKING)) {
                        movepiece(board,i1,j1,i2,j2);
                        checkdir=checked(board,BLACKKING);
                        if (checkdir)
                                if (checkmate(board,BLACKKING,checkdir)) {
                                        printf("\nCheckmate!\n");
                                        winner=WHITEKING;
                                        break;
                                }
                                else
                                        printf("\nCheck\n");
                        display_board(board);
                        black_move(board, &i1,&j1,&i2,&j2);
                        if (valid_move(board,i1,j1,i2,j2,BLACKKING)) {
                                movepiece(board,i1,j1,i2,j2);
                                checkdir=checked(board,WHITEKING);
                                if (checkdir)
                                        if (checkmate(board,WHITEKING,checkdir)) {
                                                printf("\nCheckmate!\n");
                                                winner=BLACKKING;
                                                break;
                                        }
                                        else
                                                printf("\nCheck\n");
                        }
                        else
                                printf("\nInvalid Move\n");
                }
                else 
                        printf("\nInvalid Move\n");
        } while (!winner);

        for (i=0; i<8; i++)
                free(board[i]);
        free(board);

        return 0;
}
