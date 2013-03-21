#ifndef MOVEGEN_H
#define MOVEGEN_H
#include <math.h>
#include "listmanipulator.h"
#define WHITE 69
#define BLACK -69
#define GET_TURN(y,x) board[y][x] > BLANK ? WHITEKING : BLACKKING
#define GET_NODE_ATTRIB(parent) \
        int alpha   = -parent->beta;\
        int beta    = -parent->alpha;\
        int ev_sign = -parent->ev_sign;\
        int turn = GET_TURN(location.y, location.x);\
        Movement movement = {.source = location}

Node* horseMoves(int **board, Point2D location, Node* parent) {
        Node* head = createHeadNode();
        GET_NODE_ATTRIB(parent);
        int x = location.x, y = location.y;
        int i,j;
        for (i = -2; i <= 2 ; ++i) {
                for (j = -2; j <= 2; ++j) {
                        if ( (abs(i) + abs(j) == 3) && valid_move(board, y, x, y+i, x+j, turn)) {
                                movement.dest.y = y+i;
                                movement.dest.x = x+j;
                                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
                        }
                }
        }
        return head;
}


// TODO fix redundant codes
// PLAN straight movement given parameters with step size and diag/straight
Node* kingMoves(int **board, Point2D location, Node* parent) {
        Node* head  = createHeadNode();
        GET_NODE_ATTRIB(parent);                // NOTE: this initializes some variables

        movement.dest.x = location.x;
        // move up
        if (valid_move(board, location.y, location.x, location.y-1, location.x, turn)) {
                movement.dest.y = location.y-1;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        // move down
        if (valid_move(board, location.y, location.x, location.y+1, location.x, turn)) {
                movement.dest.y = location.y+1;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        movement.dest.y = location.y;
        // move right
        if (valid_move(board, location.y, location.x, location.y, location.x+1, turn)) {
                movement.dest.x = location.x+1;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        // move left
        if (valid_move(board, location.y, location.x, location.y, location.x-1, turn)) {
                movement.dest.x = location.x-1;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        
        // move upper right
        movement.dest.y = location.y-1;
        movement.dest.x = location.x+1;
        if (valid_move(board, location.y, location.x, movement.dest.y, movement.dest.x, turn)) 
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        
        // move upper left
        movement.dest.x = location.x-1;
        if (valid_move(board, location.y, location.x, movement.dest.y, movement.dest.x, turn)) 
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
                
        // move lower right
        movement.dest.y = location.y+1;
        movement.dest.x = location.x+1;
        if (valid_move(board, location.y, location.x, movement.dest.y, movement.dest.x, turn)) 
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        
        // move lower left
        movement.dest.x = location.x-1;
        if (valid_move(board, location.y, location.x, movement.dest.y, movement.dest.x, turn)) 
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
                
        // TODO check if we can castle
        if (location.x == 4) {
                if (location.y == 7 && board[location.y][location.x] == WHITEKING) {
                        movement.dest.y = 7;
                        if(valid_move(board, 7, 4, 7, 1, WHITE)) {
                                movement.dest.x = 1;
                                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
                        }
                        if(valid_move(board, 7, 4, 7, 6, WHITE)) {
                                movement.dest.x = 6;
                                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
                        }
                } else if (location.y == 0 && board[location.y][location.x] == BLACKKING) {
                        movement.dest.y = 0;
                        if(valid_move(board, 0, 4, 0, 1, BLACKKING)) {
                                movement.dest.x = 1;
                                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
                        }
                        if(valid_move(board, 0, 4, 0, 6, BLACKKING)) {
                                movement.dest.x = 6;
                                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
                        }
                }
        }

        return head;
}


Node* bishopMoves(int **board, Point2D location, Node* parent) {
        Point2D traverser;
        Node* head  = createHeadNode();
        GET_NODE_ATTRIB(parent);                // NOTE: this initializes some variables

        // move up left
        for (traverser = location, --traverser.y, --traverser.x; traverser.x >=0 && traverser.y >=0; --traverser.y, --traverser.x) {
                movement.dest = traverser;
                if (valid_move(board, location.y, location.x, traverser.y, traverser.x, turn))
                        head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        // move down right
        for (traverser = location, ++traverser.y, ++traverser.x; traverser.x < BOARD_WIDTH && traverser.y < BOARD_HEIGHT; ++traverser.y, ++traverser.x) {
                movement.dest = traverser;
                if (valid_move(board, location.y, location.x, traverser.y, traverser.x, turn))
                        head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        //
        // move up right
        for (traverser = location, ++traverser.x, --traverser.y; traverser.x < BOARD_WIDTH && traverser.y >= 0; ++traverser.x, --traverser.y) {
                movement.dest = traverser;
                if (valid_move(board, location.y, location.x, traverser.y, traverser.x, turn))
                        head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        // move down left
        for (traverser = location, --traverser.x, ++traverser.y; traverser.y < BOARD_HEIGHT && traverser.x >= 0; --traverser.x, ++traverser.y) {
                movement.dest = traverser;
                if (valid_move(board, location.y, location.x, traverser.y, traverser.x, turn))
                        head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }

        return head;
}

Node* rookMoves(int **board, Point2D location, Node* parent) {
        Point2D traverser;
        Node* head  = createHeadNode();
        GET_NODE_ATTRIB(parent);                // NOTE: this initializes some variables

        // move up
        for (traverser.x = location.x, traverser.y = location.y-1; traverser.y >= 0; --traverser.y) {
                movement.dest = traverser;
                if (valid_move(board, location.y, location.x, traverser.y, traverser.x, turn))
                        head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        // move down
        for (traverser = location, ++traverser.y; traverser.y < BOARD_HEIGHT; ++traverser.y) {
                movement.dest = traverser;
                if (valid_move(board, location.y, location.x, traverser.y, traverser.x, turn))
                        head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        // move right
        for (traverser = location, ++traverser.x; traverser.x < BOARD_WIDTH; ++traverser.x) {
                movement.dest = traverser;
                if (valid_move(board, location.y, location.x, traverser.y, traverser.x, turn))
                        head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        // move move left
        for (traverser = location, --traverser.x; traverser.x >= 0; --traverser.x) {
                movement.dest = traverser;
                if (valid_move(board, location.y, location.x, traverser.y, traverser.x, turn))
                        head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }

        return head;
}

Node* pawnMoves(int **board, Point2D location, Node* parent) {
        Node* head  = createHeadNode();
        GET_NODE_ATTRIB(parent);                // NOTE: this initializes some variables
        int advance = (turn == WHITEKING)? -1: 1;

        // advance once
        if (valid_move(board, location.y, location.x, location.y+advance, location.x, turn)) {
                movement.dest.y = location.y+advance;
                movement.dest.x = location.x;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        // advance twice
        if (valid_move(board, location.y, location.x, location.y+advance*2, location.x, turn)) {
                movement.dest.y = location.y+advance*2;
                movement.dest.x = location.x;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        // eat
        if (valid_move(board, location.y, location.x, location.y+advance, location.x+advance, turn)) {
                movement.dest.y = location.y+advance;
                movement.dest.x = location.x+advance;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        if (valid_move(board, location.y, location.x, location.y+advance, location.x-advance, turn)) {
                movement.dest.y = location.y+advance;
                movement.dest.x = location.x-advance;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }

        return head;
}

Node* queenMoves(int **board, Point2D location, Node* parent) {
        return push(bishopMoves(board, location, parent), rookMoves(board, location, parent));
}

Node* generateMoves(int** board, Point2D location, Node* parent) {
        switch (board[location.y][location.x]) {
                case WHITEKING:
                case BLACKKING:
                        return kingMoves(board, location, parent);
                case WHITEPAWN:
                case BLACKPAWN:
                        return pawnMoves(board, location, parent);
                case WHITEQUEEN:
                case BLACKQUEEN:
                        return queenMoves(board, location, parent);
                case WHITEKNIGHT:
                case BLACKKNIGHT:
                        return horseMoves(board, location, parent);
                case WHITEROOK:
                case BLACKROOK:
                        return rookMoves(board, location, parent);
                case WHITEBISHOP:
                case BLACKBISHOP:
                        return bishopMoves(board, location, parent);
        }
        printf("ERROR: Unknown piece\n");
        printf("%i %i %i", board[location.y][location.x], location.y, location.x);
        return NULL;
}

int getColor(int pieceNum) {
        return (pieceNum < BLANK)? BLACK:WHITE;
}

#endif
