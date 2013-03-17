#include "chess.h"
#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8
#define WHITE 69
#define BLACK 669

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

typedef struct Node_t {
        Movement movement;
        int alpha;
        int beta;
        int ev_sign;
        Node_t* children;
        Node_t* next;
}Node;

Point2D findNextPiece(int **board, Point2D start, int color) {
        for (; start.y < BOARD_HEIGHT && board[start.y][start.x] == BLANK; ++start.y)
                for (; start.x < BOARD_WIDTH && board[start.y][start.x]; ++start.x)

        return start;
}

Node* createChildren(int **board) { // TODO CHECK NEEDED ARGUMENTS
}

Node* possiblePieceMove(int **board, Point2D referencePiece) {
        referencePiece = shit;
} 

int scoreOfBoard(int** board) {
	int retval = 0;
	int i,j;
	for(i=0; i<=7; i++) {
		for(j=0; j<=7; j++) {
			retval = retval + weightOf(board[i][j]);	
		}
	}

	return retval;
}







////////////////////// MoveListGenerator ///////////////////////
Node* createNode(Movement movement, int alpha, int beta, int ev_sign, Node* children, Node* next) {
        Node* temp = (Node*)malloc(sizeof(Node));
        temp->movement = ;
        temp->alpha;
        temp->beta;
        temp->ev_sign;
        temp->children;
        temp->next;
        return temp;
}

Node* push(Node* container, Node* pushee) {     // lol. pushee. hahaha
        pushee->next = container;
}

int areEnemies(int piece1, int piece2) {        // assumes that the constant for BLANK is in the middle of constants for WHITE and BLACK
        if (!(piece1 < BLANK) != !(piece2 < BLANK))
                return 1;
        return 0;
}       // TODO DELETE

Node* queenMoves(int **board, Point2D location, Node parent) {
        // TODO fix the beta, alpha, ev_sign in each node
        Point2D traverser;

        // determine if current piece is white or black
        int turn = (board[location.y][location.x] > BLANK)? WHITEKING : BLACKKING;

        // move up
        for (traverser = location, --traverser.y; valid_move(board, location.y, location.x, traverser.y, traverser.x, turn); --traverser.y) {
        }
        // move down
        for (traverser = location, ++traverser.y; valid_move(board, location.y, location.x, traverser.y, traverser.x, turn); ++traverser.y) {
                Node* temp = (Node*)malloc(sizeof(Node));
        }
        // move right
        for (traverser = location, ++traverser.x; valid_move(board, location.y, location.x, traverser.y, traverser.x, turn); ++traverser.x) {
        }
        // move move left
        for (traverser = location, --traverser.x; valid_move(board, location.y, location.x, traverser.y, traverser.x, turn); --traverser.x) {
        }
}

Node* pawnMoves() {
}
