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
#include "chess.h"
#include "destroyer.h"

 

void display_board(int *board[]) {
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

void movepiece(int **board,int i1,int j1,int i2,int j2) {
    
    board[i2][j2]=board[i1][j1];
    board[i1][j1]=BLANK;
    
    //check if castling
    if (board[i2][j2]==BLACKKING OR board[i2][j2]==WHITEKING)
    {
        if (j2>j1+1) {
            board[board[i2][j2]-1][5]=board[i2][j2]+4;
            board[board[i2][j2]-1][7]=BLANK;
        }
        if (j2<j1-2) {
            board[board[i2][j2]-1][2]=board[i2][j2]+4;
            board[board[i2][j2]-1][0]=BLANK;            
        }
    }
    
    checkqueening(board);
    
    //if kings moved, no more castling
    if (board[i2][j2]==BLACKKING)
        bkmoved=1;
    if (board[i2][j2]==WHITEKING)
        wkmoved=1;
}

void black_move(int** board, int *i1,int *j1,int *i2,int *j2) {
    
    /*
        Your code goes here.
        *i1, *j1 should contain the coordinates of the piece to be moved (both 0 to 7)
        *i2, *j2 should containt the coordinates of the target move (both 0 to 7)
     
     */
	 
	
	/*
		score of a boardstate = summation of pieces
		
		BLACK pawn = 1
		rook = 5
		knight = 5
		bishop = 5
		queen = 10
		king = 15
		
		WHITE weights = negative of black weights
		e.g. boardstate with all my pieces, 1 pawn lost for white. score = 1
		
		For each boardstate, generate all possible moves from that state and recompute score
		Keep in each node the first move from the root boardstate (para alam kung ano igagalaw pag nag cut-off na yung search)
	*/
	int i;
	score = -9999; //reset para may moves pa rin kahit bano-bano na
	
	NODE* expansionQ = (NODE*)malloc(sizeof(NODE));
	expansionQ->v = NULL;
	expansionQ->next = NULL; 
	
	//start generating the game tree up to depth 3
	VERTEX* root = (VERTEX*)malloc(sizeof(VERTEX));
	root->fromx = -1;
	root->fromy = -1;
	root->tox = -1;
	root->toy = -1;
	root->depth = 0;
	root->parent = NULL;
	root->boardstate = (int**)malloc(sizeof(int*)*8);
	for (i=0; i<8; i++) 
        root->boardstate[i]=(int*)malloc(sizeof(int)*8);
	boardcopy(board, root->boardstate);
	
	NODE* temp = (NODE*)malloc(sizeof(NODE));
	temp->v = root;
	temp->next = NULL;
	expansionQ->next = temp;

	//expand the root
	expansion(root, BLACKKING);
	//insert root children to expansion Q
	for(i=0; root->children[i] != NULL; i++) insert(expansionQ, root->children[i]);
	
	
	VERTEX* alpha;
	NODE* beta;
	int turnCounter = 0;
	
	int dummy = 0;
	//this block expands until depth 3, need to fix
	// while(expansionQ->next->v->depth <= 3)
	// {
		// printf("Generating node %d\n", ++dummy); 	
		// alpha = expansionQ->next->v;
		
		// //beta = expansionQ->next;
		// if(turnCounter % 2 == 0) expansion(alpha, WHITEKING);
		// else expansion(alpha, BLACKKING);
		// for(i=0; alpha->children[i] != NULL; i++) {
			// printf("Insertion %d\n", i);
			// insert(expansionQ, alpha->children[i]);
		// }
		// turnCounter++;
		// expansionQ->next = expansionQ->next->next;
		// //free(beta);
	// }
	
	//end game tree generation
	
	*i1 = x_1;
	*j1 = y_1;
	*i2 = x_2;
	*j2 = y_2;
	
	//need to free up game tree here
	
	
	
	//gg exploit
	/*int i,j,x,y;
	for(i=7; i>=0; i--) 
		for(j=7; j>=0; j--) {
			for(x=7; x>=0; x--)
				for(y=7; y>=0; y--) {
					if(valid_move(board,i,j,x,y,BLACKKING)) {
					movepiece(board,i,j,x,y);
					}
				}
		}*/
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