#ifndef MOVEGEN_H
#define MOVEGEN_H
#include <math.h>
#include "listmanipulator.h"
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
        // move up
        if (valid_move(board, location.y, location.x, location.y-1, location.x, turn)) {
                movement.dest.y = location.y-1;
                movement.dest.x = location.x;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        // move down
        if (valid_move(board, location.y, location.x, location.y+1, location.x, turn)) {
                movement.dest.y = location.y+1;
                movement.dest.x = location.x;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        // move right
        if (valid_move(board, location.y, location.x, location.y, location.x+1, turn)) {
                movement.dest.y = location.y;
                movement.dest.x = location.x+1;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        // move left
        if (valid_move(board, location.y, location.x, location.y, location.x-1, turn)) {
                movement.dest.y = location.y;
                movement.dest.x = location.x-1;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }

        return head;
}


Node* bishopMoves(int **board, Point2D location, Node* parent) {
        Point2D traverser;
        Node* head  = createHeadNode();
        GET_NODE_ATTRIB(parent);                // NOTE: this initializes some variables

        // move up left
        for (traverser = location, --traverser.y, --traverser.x; valid_move(board, location.y, location.x, traverser.y, traverser.x, turn); --traverser.y, --traverser.x) {
                movement.dest.y = traverser.y;
                movement.dest.x = traverser.x;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        // move down right
        for (traverser = location, ++traverser.y, ++traverser.x; valid_move(board, location.y, location.x, traverser.y, traverser.x, turn); ++traverser.y, ++traverser.x) {
                movement.dest.y = traverser.y;
                movement.dest.x = traverser.x;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        //
        // move up right
        for (traverser = location, ++traverser.x, --traverser.y; valid_move(board, location.y, location.x, traverser.y, traverser.x, turn); ++traverser.x, --traverser.y) {
                movement.dest.y = traverser.y;
                movement.dest.x = traverser.x;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        // move down left
        for (traverser = location, --traverser.x, ++traverser.y; valid_move(board, location.y, location.x, traverser.y, traverser.x, turn); --traverser.x, ++traverser.y) {
                movement.dest.y = traverser.y;
                movement.dest.x = traverser.x;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }

        return head;
}

Node* rookMoves(int **board, Point2D location, Node* parent) {
        Point2D traverser;
        Node* head  = createHeadNode();
        GET_NODE_ATTRIB(parent);                // NOTE: this initializes some variables

        // move up
        for (traverser = location, --traverser.y; valid_move(board, location.y, location.x, traverser.y, traverser.x, turn); --traverser.y) {
                movement.dest.y = traverser.y;
                movement.dest.x = traverser.x;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        // move down
        for (traverser = location, ++traverser.y; valid_move(board, location.y, location.x, traverser.y, traverser.x, turn); ++traverser.y) {
                movement.dest.y = traverser.y;
                movement.dest.x = traverser.x;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        // move right
        for (traverser = location, ++traverser.x; valid_move(board, location.y, location.x, traverser.y, traverser.x, turn); ++traverser.x) {
                movement.dest.y = traverser.y;
                movement.dest.x = traverser.x;
                head = push(head, createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL));
        }
        // move move left
        for (traverser = location, --traverser.x; valid_move(board, location.y, location.x, traverser.y, traverser.x, turn); --traverser.x) {
                movement.dest.y = traverser.y;
                movement.dest.x = traverser.x;
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
        if (valid_move(board, location.y, location.x+advance, location.y+advance, location.x, turn)) {
                movement.dest.y = location.y+advance;
                movement.dest.x = location.x+advance;
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
#endif
