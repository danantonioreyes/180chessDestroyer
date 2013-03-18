#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "chess.h"
#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8
#define WHITE 69
#define BLACK 669
#define GET_TURN(y,x) board[y][x] > BLANK ? WHITEKING : BLACKKING
#define GET_NODE_ATTRIB(parent) \
        int alpha   = -parent->beta;\
        int beta    = -parent->alpha;\
        int ev_sign = -parent->ev_sign;\
        int turn = GET_TURN(location.y, location.x);\
        Movement movement = {.source = location}

// IDEA:
/*      isave ko kaya yung game tree tapos yun nalang ituloy pag nag move yung kalaban
 */

typedef struct {
        int x;
        int y;
}Point2D;

typedef struct{
        Point2D source;
        Point2D dest;
}Movement;

// Note: head nodes point the next to the 1st element and children to the end of the list
typedef struct Node_t {
        Movement movement;
        int eaten;      // for keeping track of eaten pieces when the move was done
        int alpha;
        int beta;
        int ev_sign;
        struct Node_t* children;
        struct Node_t* next;
}Node;

Point2D findNextPiece(int **board, Point2D start, int color) {
        for (; start.y < BOARD_HEIGHT && (board[start.y][start.x] == BLANK || color != getColor(board[start.y][start.x])); ++start.y)
                for (; start.x < BOARD_WIDTH && (board[start.y][start.x] == BLANK || color != getColor(board[start.y][start.x])); ++start.x);

        return start;
}

Node* createChildren(int **board) { // TODO CHECK NEEDED ARGUMENTS
}

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







////////////////////// MoveListGenerator ///////////////////////
Node* createNode(Movement movement, int alpha, int beta, int ev_sign, Node* children, Node* next) {
        Node* temp = (Node*)malloc(sizeof(Node));
        temp->movement = movement;
        temp->alpha    = alpha;
        temp->beta     = beta;
        temp->ev_sign  = ev_sign;
        temp->children = NULL;
        temp->next     = NULL;
        return temp;
}

Node* createHeadNode() {
        Node* temp = (Node*)malloc(sizeof(Node));
        temp->next = NULL;
        temp->children = temp;
        return temp;
}

Node* createNodeWithHead(Movement movement, int alpha, int beta, int ev_sign, Node* children, Node* next) {
        Node* temp     = createHeadNode();
        temp->next     = createNode(movement, alpha, beta, ev_sign, children, next);
        temp->children = temp->next;
        return temp;
}

Node* push(Node* container, Node* pushee) {     // lol. pushee. hahaha
        // NOTE: container is freed
        Node* temp   = pushee;
        pushee       = pushee->children;        // go to the last element
        pushee->next = container->next;
        temp->children  = container->children;
        
        free(container);
        return temp;
}

int areEnemies(int piece1, int piece2) {        // assumes that the constant for BLANK is in the middle of constants for WHITE and BLACK
        if (!(piece1 < BLANK) != !(piece2 < BLANK))
                return 1;
        return 0;
}       // TODO DELETE


Node* horseMoves(int **board, Point2D location, Node* parent) {
        return NULL;
        // TODO FINISH
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
