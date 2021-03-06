#ifndef SHIVA_H
#define SHIVA_H
// ^ to avoid multiple inclusion of the file

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "movegen.h"
#include "listmanipulator.h"
#include "moveevaluator.h"
#define MAX_DEPTH 5
#define INFTY 1241894

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
        
        Point2D searchStart = {0,0};
        --depth;
        if (depth == 0) {
                int generatedMoves = 0;
                searchStart = findNextPiece(board, searchStart, color);
                while (searchStart.y < BOARD_HEIGHT && searchStart.x < BOARD_WIDTH) {
                        parent->children = generateMoves(board, searchStart, parent);

                        Node* traverser = parent->children->next;
                        
                        if (traverser != NULL) {
                                ++generatedMoves;
                        }
                        while (traverser != NULL && parent->alpha < parent->beta) {
                                ++generatedMoves;
                                movePieceForTraversal(board, traverser);
                                int temp = traverser->ev_sign * scoreOfBoard(board, color*parent->ev_sign) * -1;   // ev_sign*score -> invert sign of leaf
                                if (temp > parent->alpha) {
                                        parent->alpha = temp;
                                        if (depth == MAX_DEPTH - 1) {
                                                parent->movement = traverser->movement;
                                        }
                                }
                                revertPieceMove(board, traverser);
                                traverser = traverser->next;
                        }
                        parent->children = freeList(parent->children);

                        if (searchStart.x+1 >= BOARD_WIDTH) {
                                ++searchStart.y;
                                searchStart.x = 0;
                        } else{
                                ++searchStart.x;
                        }
                        searchStart = findNextPiece(board, searchStart, color);
                }
                if (generatedMoves == 0) {
                        // if no moves are generated, then it must be a checkmate
                        int temp = INFTY*depth*parent->ev_sign;
                        if (!checked(board, (color == WHITEKING)? BLACKKING:WHITEKING))        // if the enemy king is not checked but cannot move anything, then it's a stalemate and both sides try to avoid it
                                temp *= -1;
                        if (temp > parent->alpha) {
                                parent->alpha = temp;
                        }
                }
                parent->alpha *= -1;            // invert sign when it pops
        }
        else {
                // NOTE: 1ST PARENT IS ROOT NODE
                int generatedMoves = 0;
                searchStart = findNextPiece(board, searchStart, color);
                while (searchStart.y < BOARD_HEIGHT && searchStart.x < BOARD_WIDTH) {
                        parent->children = generateMoves(board, searchStart, parent);

                        Node* traverser = parent->children->next;
                        if (traverser != NULL) {
                                ++generatedMoves;
                        }
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
                        parent->children = freeList(parent->children);

                        searchStart.y += (searchStart.x+1)/BOARD_WIDTH;
                        searchStart.x = (searchStart.x+1)%BOARD_WIDTH;
                        searchStart = findNextPiece(board, searchStart, color);
                }
                if (generatedMoves == 0) {
                        // if no moves are generated, then it must be a checkmate or a stalemate
                        int temp = INFTY*depth*parent->ev_sign;
                        if (!checked(board, (color == WHITEKING)? BLACKKING:WHITEKING))         // if the enemy king is not checked but cannot move anything, then it's a stalemate and both sides try to avoid it
                                temp *= -1;
                        if (temp > parent->alpha) {
                                parent->alpha = temp;
                        }
                }
                parent->alpha *= -1;            // invert sign when it pops
        }
}

void shiva_main(int** board,int *i1,int *j1,int *i2,int *j2 ){
        Node* root = createNode((Movement){0}, -INFTY*MAX_DEPTH, INFTY*MAX_DEPTH, 1, NULL, NULL);
        minimax(board, BLACK, MAX_DEPTH, root);
        (*i1) = root->movement.source.y;
        (*j1) = root->movement.source.x;
        (*i2) = root->movement.dest.y;
        (*j2) = root->movement.dest.x;
        printf("Black move: %c%i %c%i", (*i1)+'A', (*j1)+1, (*i2)+'A', (*j2)+1);
        free(root);
}
#endif
