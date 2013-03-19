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
#include "moveevaluator.h"
#define MAX_DEPTH 6
#define INFTY 1241894

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
        for(; start.y < BOARD_HEIGHT; ++start.y) {
                for (; start.x < BOARD_WIDTH; ++start.x) {
                        if (board[start.y][start.x] != BLANK && color == getColor(board[start.y][start.x]))
                                return start;
                }
                start.x = 0;
        }

        return start;
}

void movePieceForTraversal(int** board, Node* node) {
        node->eaten = board[node->movement.dest.y][node->movement.dest.x];
        node->pieceMoved = board[node->movement.source.y][node->movement.source.x];
        movepiece(board, node->movement.source.y, node->movement.source.x, node->movement.dest.y, node->movement.dest.x);
}

void revertPieceMove(int** board, Node* node) {
        // TODO HANDLE castling here and in kingMoves
        if (node->pieceMoved == BLACKKING || node->pieceMoved == WHITEKING) {
                if (abs(node->movement.dest.x - node->movement.source.x) > 1)  {        // castling is the only reason for the king to be able to move to more than one tile
                        // put the rook to the end of the board
                        if (node->movement.dest.x == 1) {
                                board[node->movement.dest.y][2] = BLANK;
                                board[node->movement.dest.y][0] = (node->pieceMoved == BLACKKING)? BLACKROOK:WHITEROOK;
                        } else if (node->movement.dest.x == 6){
                                board[node->movement.dest.y][5] = BLANK;
                                board[node->movement.dest.y][7] = (node->pieceMoved == BLACKKING)? BLACKROOK:WHITEROOK;
                        }
                }
        }
        board[node->movement.source.y][node->movement.source.x] = node->pieceMoved;
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

void displayTree(Node* root, int depth, int color) {
        printf("\nat depth %i; color %i", depth, color);
        while (root != NULL) {
                Node* i = root;
                printf("\n");
                while (i != NULL) {
                        printf("M: %i%i-%i%i A: %i B: %i E: %i ", i->movement.source.y, i->movement.source.x, i->movement.dest.y, i->movement.dest.x, i->alpha, i->beta, i->ev_sign);
                        i = i->next;
                }
                root = root->children;
                if (root != NULL)
                        root = root->next;
        }
}

// TODO ETO NA YUNG SUSUNOD
void minimax(int **board, int color, int depth, Node* parent) { // TODO CHECK NEEDED ARGUMENTS
        //printf("alphabeta %i %i \ndepth %i\n", parent->alpha, parent->beta, depth);
        
        Point2D searchStart = {0,0};
        --depth;
        if (depth == 0) {
                // return the max/min sa buong list
                //searchStart = findNextPiece(board, searchStart, color*parent->ev_sign);
                searchStart = findNextPiece(board, searchStart, color);
                while (searchStart.y < BOARD_HEIGHT && searchStart.x < BOARD_WIDTH) {
                        //printf("Found shit at: %i%i\n ", searchStart.y, searchStart.x);
                        parent->children = generateMoves(board, searchStart, parent);

                        Node* traverser = parent->children->next;
                        //TODO REMOVE
                        //if (traverser == NULL) {
                        //        printf("   NULL eh\n");
                        //}
                        //else if (parent->alpha >= parent->beta) {
                        //        printf("   pruned\n");
                        //}
                        
                        while (traverser != NULL && parent->alpha < parent->beta) {
                                movePieceForTraversal(board, traverser);
                                //TODO REMOVE
                                //if (traverser->movement.dest.x == 2, traverser->movement.dest.y == 2) {
                                        //printf("FUCK THIS SHIT \n");
                                        //display_board(board);
                                //}
                                        ////////////////
                                //display_board(board);
                                int temp = traverser->ev_sign * scoreOfBoard(board, color*parent->ev_sign) * -1;   // ev_sign*score -> invert sign of leaf
                                traverser->alpha = temp; //TODO REMOVE
                                if (temp > parent->alpha) {
                                        parent->alpha = temp;
                                        if (depth == MAX_DEPTH - 1) {
                                                // then the parent == root
                                                parent->movement = traverser->movement;
                                        }
                                }
                                revertPieceMove(board, traverser);
                                traverser = traverser->next;
                        }
                        //displayTree(parent,depth,color);
                        parent->children = freeList(parent->children);

                        if (searchStart.x+1 >= BOARD_WIDTH) {
                                ++searchStart.y;
                                searchStart.x = 0;
                        } else{
                                ++searchStart.x;
                        }
                        searchStart = findNextPiece(board, searchStart, color);
                        //printf("search res at %i%i", searchStart.y, searchStart.x);
                }
                parent->alpha   *= -1;                          // invert sign after everything has been considered
                //printf("alphabeta %i %i \n", parent->alpha, parent->beta);
        }
        else {
                // NOTE: 1ST PARENT IS ROOT NODE
                searchStart = findNextPiece(board, searchStart, color);
                while (searchStart.y < BOARD_HEIGHT && searchStart.x < BOARD_WIDTH) {
                        parent->children = generateMoves(board, searchStart, parent);

                        Node* traverser = parent->children->next;
                        while (traverser != NULL && parent->alpha < parent->beta) {
                                movePieceForTraversal(board, traverser);
                                minimax(board, color*-1, depth, traverser);
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
                        //displayTree(parent,depth, color);
                        parent->children = freeList(parent->children);

                        searchStart.y += (searchStart.x+1)/BOARD_WIDTH;
                        searchStart.x = (searchStart.x+1)%BOARD_WIDTH;
                        searchStart = findNextPiece(board, searchStart, color);
                }
                parent->alpha *= -1;            // invert sign when it pops
        }
}

#endif
