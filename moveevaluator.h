#ifndef MOVE_EVALUATOR_H
#define MOVE_EVALUATOR_H
#include <math.h>

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8
#define KING_VALUE    20000
#define QUEEN_VALUE   900
#define BISHOP_VALUE  330
#define KNIGHT_VALUE  320
#define ROOK_VALUE    500
#define PAWN_VALUE    100
#define BLANK_VALUE   0
#define MAX_OFFICIAL 16
#define END_GAME_OFFICIAL 8


// source: http://chessprogramming.wikispaces.com/Simplified+evaluation+function
//         for the tables only
// since the algorithm is too memory efficient, it will not hurt to keep this board in the memory until the game finishes
static int whitePawnPosition[BOARD_HEIGHT][BOARD_WIDTH] = {
        {0,  0,  0,  0,  0,  0,  0,  0},
        {50, 50, 50, 50, 50, 50, 50, 50},
        {10, 10, 20, 30, 30, 20, 10, 10},
        {5,  5, 10, 25, 25, 10,  5,  5},
        {0,  0,  0, 20, 20,  0,  0,  0},
        {5, -5,-10,  0,  0,-10, -5,  5},
        {5, 10, 10,-20,-20, 10, 10,  5},
        {0,  0,  0,  0,  0,  0,  0,  0}
};

static int blackPawnPosition[BOARD_HEIGHT][BOARD_WIDTH] = {
        {0,  0,  0,  0,  0,  0,  0,  0},
        {5, 10, 10,-20,-20, 10, 10,  5},
        {5, -5,-10,  0,  0,-10, -5,  5},
        {0,  0,  0, 20, 20,  0,  0,  0},
        {5,  5, 10, 25, 25, 10,  5,  5},
        {10, 10, 20, 30, 30, 20, 10, 10},
        {50, 50, 50, 50, 50, 50, 50, 50},
        {0,  0,  0,  0,  0,  0,  0,  0}
};

static int whiteKnightPosition[BOARD_HEIGHT][BOARD_WIDTH] = {
        {-50,-40,-30,-30,-30,-30,-40,-50},
        {-40,-20,  0,  0,  0,  0,-20,-40},
        {-30,  0, 10, 15, 15, 10,  0,-30},
        {-30,  5, 15, 20, 20, 15,  5,-30},
        {-30,  0, 15, 20, 20, 15,  0,-30},
        {-30,  5, 10, 15, 15, 10,  5,-30},
        {-40,-20,  0,  5,  5,  0,-20,-40},
        {-50,-40,-30,-30,-30,-30,-40,-50}
};

static int blackKnightPosition[BOARD_HEIGHT][BOARD_WIDTH] = {
        {-50,-40,-30,-30,-30,-30,-40,-50},
        {-40,-20,  0,  5,  5,  0,-20,-40},
        {-30,  5, 10, 15, 15, 10,  5,-30},
        {-30,  0, 15, 20, 20, 15,  0,-30},
        {-30,  5, 15, 20, 20, 15,  5,-30},
        {-30,  0, 10, 15, 15, 10,  0,-30},
        {-40,-20,  0,  0,  0,  0,-20,-40},
        {-50,-40,-30,-30,-30,-30,-40,-50}
};

static int whiteBishopPosition[BOARD_HEIGHT][BOARD_WIDTH] = {
        {-20,-10,-10,-10,-10,-10,-10,-20},
        {-10,  0,  0,  0,  0,  0,  0,-10},
        {-10,  0,  5, 10, 10,  5,  0,-10},
        {-10,  5,  5, 10, 10,  5,  5,-10},
        {-10,  0, 10, 10, 10, 10,  0,-10},
        {-10, 10, 10, 10, 10, 10, 10,-10},
        {-10,  5,  0,  0,  0,  0,  5,-10},
        {-20,-10,-10,-10,-10,-10,-10,-20}
};

static int blackBishopPosition[BOARD_HEIGHT][BOARD_WIDTH] = {
        {-20,-10,-10,-10,-10,-10,-10,-20},
        {-10,  5,  0,  0,  0,  0,  5,-10},
        {-10, 10, 10, 10, 10, 10, 10,-10},
        {-10,  0, 10, 10, 10, 10,  0,-10},
        {-10,  5,  5, 10, 10,  5,  5,-10},
        {-10,  0,  5, 10, 10,  5,  0,-10},
        {-10,  0,  0,  0,  0,  0,  0,-10},
        {-20,-10,-10,-10,-10,-10,-10,-20}
};

static int whiteRookPosition[BOARD_HEIGHT][BOARD_WIDTH] = {
        { 0,  0,  0,  0,  0,  0,  0,  0},
        { 5, 10, 10, 10, 10, 10, 10,  5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        { 0,  0,  0,  5,  5,  0,  0,  0}
};

static int blackRookPosition[BOARD_HEIGHT][BOARD_WIDTH] = {
        { 0,  0,  0,  5,  5,  0,  0,  0},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        {-5,  0,  0,  0,  0,  0,  0, -5},
        { 5, 10, 10, 10, 10, 10, 10,  5},
        { 0,  0,  0,  0,  0,  0,  0,  0}
};

static int whiteQueenPosition[BOARD_HEIGHT][BOARD_WIDTH] = {
        {-20,-10,-10, -5, -5,-10,-10,-20},
        {-10,  0,  0,  0,  0,  0,  0,-10},
        {-10,  0,  5,  5,  5,  5,  0,-10},
        { -5,  0,  5,  5,  5,  5,  0, -5},
        {  0,  0,  5,  5,  5,  5,  0, -5},
        {-10,  5,  5,  5,  5,  5,  0,-10},
        {-10,  0,  5,  0,  0,  0,  0,-10},
        {-20,-10,-10, -5, -5,-10,-10,-20}
};

static int blackQueenPosition[BOARD_HEIGHT][BOARD_WIDTH] = {
        {-20,-10,-10, -5, -5,-10,-10,-20},
        {-10,  0,  5,  0,  0,  0,  0,-10},
        {-10,  5,  5,  5,  5,  5,  0,-10},
        {  0,  0,  5,  5,  5,  5,  0, -5},
        { -5,  0,  5,  5,  5,  5,  0, -5},
        {-10,  0,  5,  5,  5,  5,  0,-10},
        {-10,  0,  0,  0,  0,  0,  0,-10},
        {-20,-10,-10, -5, -5,-10,-10,-20}
};

static int whiteKingPosition[BOARD_HEIGHT][BOARD_WIDTH] = {
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-20,-30,-30,-40,-40,-30,-30,-20},
        {-10,-20,-20,-20,-20,-20,-20,-10},
        { 20, 20,  0,  0,  0,  0, 20, 20},
        { 20, 30, 10,  0,  0, 10, 30, 20}
};

static int blackKingPosition[BOARD_HEIGHT][BOARD_WIDTH] = {
        { 20, 30, 10,  0,  0, 10, 30, 20},
        { 20, 20,  0,  0,  0,  0, 20, 20},
        {-10,-20,-20,-20,-20,-20,-20,-10},
        {-20,-30,-30,-40,-40,-30,-30,-20},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30},
        {-30,-40,-40,-50,-50,-40,-40,-30}
};

static int whiteKingLatePosition[BOARD_HEIGHT][BOARD_WIDTH] = {
        {-50,-40,-30,-20,-20,-30,-40,-50},
        {-30,-20,-10,  0,  0,-10,-20,-30},
        {-30,-10, 20, 30, 30, 20,-10,-30},
        {-30,-10, 30, 40, 40, 30,-10,-30},
        {-30,-10, 30, 40, 40, 30,-10,-30},
        {-30,-10, 20, 30, 30, 20,-10,-30},
        {-30,-30,  0,  0,  0,  0,-30,-30},
        {-50,-30,-30,-30,-30,-30,-30,-50}
};

static int blackKingLatePosition[BOARD_HEIGHT][BOARD_WIDTH] = {
        {-50,-30,-30,-30,-30,-30,-30,-50},
        {-30,-30,  0,  0,  0,  0,-30,-30},
        {-30,-10, 20, 30, 30, 20,-10,-30},
        {-30,-10, 30, 40, 40, 30,-10,-30},
        {-30,-10, 30, 40, 40, 30,-10,-30},
        {-30,-10, 20, 30, 30, 20,-10,-30},
        {-30,-20,-10,  0,  0,-10,-20,-30},
        {-50,-40,-30,-20,-20,-30,-40,-50}
};

int scoreOfBoard(int** board, int color) {
        int multiplier = (color == BLACK)? 1:-1;
	int retval = 0;
	int i,j;
        int remainingOfficial = MAX_OFFICIAL;
        Point2D blackKingLocation, whiteKingLocation;

	for(i=0; i<=7; i++) {
		for(j=0; j<=7; j++) {
                        switch(board[i][j]) {
                                case BLACKKING:
                                        retval += multiplier*KING_VALUE;
                                        blackKingLocation.x = j;
                                        blackKingLocation.y = i;
                                        break;
                                case BLACKQUEEN:
                                        retval += QUEEN_VALUE;
                                        retval += blackQueenPosition[i][j];
                                        break;
                                case BLACKBISHOP:
                                        retval += BISHOP_VALUE;
                                        retval += blackBishopPosition[i][j];
                                        break;
                                case BLACKKNIGHT:
                                        retval += KNIGHT_VALUE;
                                        retval += blackKnightPosition[i][j];
                                        break;
                                case BLACKROOK:
                                        retval += ROOK_VALUE;
                                        retval += blackRookPosition[i][j];
                                        break;
                                case BLACKPAWN:
                                        --remainingOfficial;
                                        retval += PAWN_VALUE;
                                        retval += blackPawnPosition[i][j];
                                        break;
                                case WHITEKING:
                                        retval -= KING_VALUE;
                                        whiteKingLocation.x = j;
                                        whiteKingLocation.y = i;
                                        break;
                                case WHITEQUEEN:
                                        retval -= QUEEN_VALUE;
                                        retval -= whiteQueenPosition[i][j];
                                        break;
                                case WHITEBISHOP:
                                        retval -= BISHOP_VALUE;
                                        retval -= whiteBishopPosition[i][j];
                                        break;
                                case WHITEKNIGHT:
                                        retval -= KNIGHT_VALUE;
                                        retval -= whiteKnightPosition[i][j];
                                        break;
                                case WHITEROOK:
                                        retval -= ROOK_VALUE;
                                        retval -= whiteRookPosition[i][j];
                                        break;
                                case WHITEPAWN:
                                        --remainingOfficial;
                                        retval -= PAWN_VALUE;
                                        retval -= whitePawnPosition[i][j];
                        }

		}
	}

        if (remainingOfficial > END_GAME_OFFICIAL) {
                retval += blackKingPosition[blackKingLocation.y][blackKingLocation.x];
                retval -= whiteKingPosition[whiteKingLocation.y][whiteKingLocation.x];
        } else {
                retval += blackKingLatePosition[blackKingLocation.y][blackKingLocation.x];
                retval -= whiteKingLatePosition[whiteKingLocation.y][whiteKingLocation.x];
        }

	return multiplier*retval;
}

#endif
