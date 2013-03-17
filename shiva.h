#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8

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
}Node;

Point2D findNextPiece(int board[BOARD_HEIGHT][BOARD_WIDTH], Point2D start) {
        for (; start.y < BOARD_HEIGHT && board[start.y][start.x] == BLANK; ++start.y)
                for (; start.x < BOARD_WIDTH && board[start.y][start.x]; ++start.x)

        return start;
}

Node* createChildren(int board[BOARD_HEIGHT][BOARD_WIDTH]) { // TODO CHECK NEEDED ARGUMENTS
}

Node* possiblePieceMove(int board[BOARD_HEIGHT][BOARD_WIDTH], Point2D referencePiece) {
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
