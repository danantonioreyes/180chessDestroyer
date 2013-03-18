#ifndef SHIVA_H
#define SHIVA_H
// ^ to avoid multiple inclusion of the file

#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "chess.h"
#include "movegen.h"
#include "listmanipulator.h"
#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8
#define WHITE 69
#define BLACK 669
#define MAX_DEPTH 3
#define INFTY 121487

// IDEA:
/*      isave ko kaya yung game tree tapos yun nalang ituloy pag nag move yung kalaban
 *      ^ instead of that, sisirain ko yung tree habang traversal to save space at baka pwede ko pa pahabain yung max_depth kung sakali
 */

// galing to sa template. binura ko muna yung nandun. 
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

Point2D findNextPiece(int **board, Point2D start, int color) {
        for (; start.y < BOARD_HEIGHT && (board[start.y][start.x] == BLANK || color != getColor(board[start.y][start.x])); ++start.y)
                for (; start.x < BOARD_WIDTH && (board[start.y][start.x] == BLANK || color != getColor(board[start.y][start.x])); ++start.x);

        return start;
}

void movePieceForTraversal(int** board, Node* node) {
        node->eaten = board[node->movement.dest.y][node->movement.dest.x];
        movepiece(board, node->movement.source.y, node->movement.source.x, node->movement.dest.y, node->movement.dest.x);
}

void revertPieceMove(int** board, Node* node) {
        // TODO HANDLE castling here and in kingMoves
        board[node->movement.source.y][node->movement.source.x] = board[node->movement.dest.y][node->movement.dest.x];
        board[node->movement.dest.y][node->movement.dest.x] = node->eaten;
}

Node* freeList(Node* head) {
        Node* traverser1 = head;

        while(traverser1 != NULL) {
                Node* traverser2 = traverser1->next;
                free(traverser1);
                traverser1 = traverser2;
        }
        return NULL;
}

// TODO ETO NA YUNG SUSUNOD
void minimax(int **board, int color, int depth, Node* parent) { // TODO CHECK NEEDED ARGUMENTS
        Point2D searchStart = {0,0};
        --depth;
        if (depth == 0) {
                // return the max/min sa buong list
                while (searchStart.y < BOARD_HEIGHT && searchStart.x < BOARD_WIDTH) {
                        searchStart = findNextPiece(board, searchStart, color);
                        parent->children = generateMoves(board, searchStart, parent);

                        display_board(board);
                        Node* traverser = parent->children->next;
                        while (traverser != NULL && parent->alpha < traverser->beta) {
                                movePieceForTraversal(board, traverser);
                                int temp = traverser->ev_sign * scoreOfBoard(board, color) * -1;   // ev_sign*score -> invert sign of leaf
                                if (temp > parent->alpha) {
                                        parent->alpha = temp;
                                }
                                revertPieceMove(board, traverser);
                                traverser = traverser->next;
                        }
                        parent->children = freeList(parent->children);
                        parent->alpha   *= -1;                          // invert sign after everything has been considered

                        searchStart.y += (searchStart.x+1)/BOARD_WIDTH;
                        searchStart.x = (searchStart.x+1)%BOARD_WIDTH;
                }
        }

        // NOTE: 1ST PARENT IS ROOT NODE
        while (searchStart.y < BOARD_HEIGHT && searchStart.x < BOARD_WIDTH) {
                searchStart = findNextPiece(board, searchStart, color);
                parent->children = generateMoves(board, searchStart, parent);

                Node* traverser = parent->children->next;
                while (traverser != NULL && parent->alpha < traverser->beta) {
                        movePieceForTraversal(board, traverser);
                        minimax(board, color, depth, traverser);
                        if (traverser->alpha > parent->alpha) {
                                parent->alpha = traverser->alpha;
                                if (depth == MAX_DEPTH - 1) {
                                        // then the parent == root
                                        parent->movement = traverser->movement;
                                }
                        }
                        revertPieceMove(board, traverser);
                        traverser = traverser->next;
                }
                parent->alpha *= -1;            // invert sign when it pops
                parent->children = freeList(parent->children);

                searchStart.y += (searchStart.x+1)/BOARD_WIDTH;
                searchStart.x = (searchStart.x+1)%BOARD_WIDTH;
        }
}

//void getMaxOnLeaf(Node* parent) {
//        Node* traverser = head->next;
//        free(head);
//}


Node* possiblePieceMove(int **board, Point2D referencePiece) {
        //referencePiece = shit;
        // TODO FIX THIS
} 

int weightOf(int arg, int color) {
        int multiplier = (color == BLACK)? 1:-1;

        switch (arg) {
                case BLACKKING:         return multiplier*25;
                case BLACKQUEEN:        return multiplier*15;
                case BLACKBISHOP:       return multiplier*10;
                case BLACKKNIGHT:       return multiplier*10;
                case BLACKROOK:         return multiplier*10;
                case BLACKPAWN:         return multiplier*5;
                case WHITEKING:         return multiplier*-25;
                case WHITEQUEEN:        return multiplier*-15;
                case WHITEBISHOP:       return multiplier*-10;
                case WHITEKNIGHT:       return multiplier*-10;
                case WHITEROOK:         return multiplier*-10;
                case WHITEPAWN:         return multiplier*-5;
                case BLANK:             return multiplier*0;
        }
}

int scoreOfBoard(int** board, int color) {
	int retval = 0;
	int i,j;
	for(i=0; i<=7; i++) {
		for(j=0; j<=7; j++) {
			retval = retval + weightOf(board[i][j], color);
		}
	}

	return retval;
}

int getColor(int pieceNum) {
        return (pieceNum < BLANK)? BLACK:WHITE;
}

#endif
