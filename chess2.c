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
#include "chess2.h"
#include "shiva.h"

void display_board(int *board[],char *msg) {
    int i,j;
    int width=8,rows=8;
    
    printf("\n    1    2    3    4    5    6    7    8\n");    
    printf("  -----------------------------------------\n");    
    for (i=0; i<width; i++) {
        printf("%c |",i+65);
        for (j=0; j<rows; j++)
            switch (board[i][j]) {
                case BLACKKING:   printf(" BK |"); break;
                case BLACKQUEEN:  printf(" BQ |"); break;
                case BLACKBISHOP: printf(" BB |"); break;
                case BLACKKNIGHT: printf(" BH |"); break;
                case BLACKROOK:   printf(" BR |"); break;
                case BLACKPAWN:   printf(" BP |"); break;
                case WHITEKING:   printf(" WK |"); break;
                case WHITEQUEEN:  printf(" WQ |"); break;
                case WHITEBISHOP: printf(" WB |"); break;
                case WHITEKNIGHT: printf(" WH |"); break;
                case WHITEROOK:   printf(" WR |"); break;
                case WHITEPAWN:   printf(" WP |"); break;                    
                default:          printf("    |");
            }
        
        
        printf("\n  -----------------------------------------\n");
    }
    printf("    1    2    3    4    5    6    7    8\n");    

    printf("\n");
}


void white_move(int *i1,int *j1,int *i2,int *j2) {
    char from[3],to[3];
    
    printf("\nWhite move: ");
    scanf("%s %s",from,to);
    
    *i1=from[0]-97;
    *i2=to[0]-97;
    *j1=from[1]-49;
    *j2=to[1]-49;

}

void black_move(int** board,int *i1,int *j1,int *i2,int *j2) {
    //char from[3],to[3];

        Node* root = createNode((Movement){0}, -INFTY, INFTY, 1, NULL, NULL);
        minimax(board, BLACK, MAX_DEPTH, root);
        (*i1) = root->movement.source.y;
        (*j1) = root->movement.source.x;
        (*i2) = root->movement.dest.y;
        (*j2) = root->movement.dest.x;
        printf("Black move: %c%i %c%i", (*i1)+'A', (*j1)+1, (*i2)+'A', (*j2)+1);
        free(root);
    
    /*
        Your code goes here.
        *i1, *j1 should contain the coordinates of the piece to be moved (both 0 to 7)
        *i2, *j2 should containt the coordinates of the target move (both 0 to 7)
     
     */
    
    //printf("\nBlack move: ");    
    //scanf("%s %s",from,to);
    //
    //*i1=from[0]-97;
    //*i2=to[0]-97;
    //*j1=from[1]-49;
    //*j2=to[1]-49;
    
}



int main(int argc,char *argv[]) {
    int i,i1,j1,i2,j2,checkdir,winner,valid;
    int **board;
    char msg[100];
    
    board=(int**)malloc(sizeof(int*)*8);
    for (i=0; i<8; i++) 
        board[i]=(int*)malloc(sizeof(int)*8);
    
    winner=0;
    init_board(board);
    do {
        valid=0;
        while (!winner && !valid) {
            display_board(board,msg);
            checkdir=checked(board,WHITEKING);
            if (checkdir) {
                if (checkmate(board,WHITEKING,checkdir)) {
                    printf("\nCHECKMATE: ");
                    winner=BLACKKING;
                    break;
                }
                else
                    printf("Check");
            }
            else 
                if (stalemate(board,WHITEKING)) {
                    winner=7;
                    break;
                }
            
            white_move(&i1,&j1,&i2,&j2);
            valid=valid_move(board,i1,j1,i2,j2,WHITEKING);
            if (valid) 
                movepiece(board,i1,j1,i2,j2);
            else
                printf("\nInvalid Move\n");            
        }
        valid=0;
        while (!winner && !valid) {
            display_board(board,msg);
            checkdir=checked(board,BLACKKING);
            if (checkdir) {
                if (checkmate(board,BLACKKING,checkdir)) {
                    printf("\nCHECKMATE: ");
                    winner=WHITEKING;
                    break;
                }
                else
                    printf("Check");
            }
            else
                if (stalemate(board,BLACKKING)) {
                    winner=7;
                    break;
                }
            
            black_move(board,&i1,&j1,&i2,&j2);
            valid=valid_move(board,i1,j1,i2,j2,BLACKKING);
            if (valid) 
                movepiece(board,i1,j1,i2,j2);
            else
                printf("\nInvalid Move\n");
        }
    } while (!winner);
        
    if (winner==7)
        printf("DRAW!\n");
    else
        printf("Winner is %s!\n\n",winner>BLANK?"WHITE":"BLACK");
    
    for (i=0; i<8; i++)
        free(board[i]);
    free(board);
    
	return 0;
}
