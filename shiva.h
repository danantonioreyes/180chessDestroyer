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

// IDEA:
/*      isave ko kaya yung game tree tapos yun nalang ituloy pag nag move yung kalaban
 *      ^ instead of that, sisirain ko yung tree habang traversal to save space at baka pwede ko pa pahabain yung max_depth kung sakali
 */

Point2D findNextPiece(int **board, Point2D start, int color) {
        for (; start.y < BOARD_HEIGHT && (board[start.y][start.x] == BLANK || color != getColor(board[start.y][start.x])); ++start.y)
                for (; start.x < BOARD_WIDTH && (board[start.y][start.x] == BLANK || color != getColor(board[start.y][start.x])); ++start.x);

        return start;
}

// TODO ETO NA YUNG SUSUNOD
Node* generateChildren(int **board, int color, int depth) { // TODO CHECK NEEDED ARGUMENTS

        if (depth == MAX_DEPTH-1) {
                // eto rin ata yung normal case. haha
                // traverse all the siblings for the best move
                // return the best move
        }

        if (depth == 0) {
                // return the max/min
        }

        Point2D searchStart = (0,0);
        while (searchStart.x < BOARD_WIDTH && searchStart.y < BOARD_HEIGHT) {
                Point2D location = findNextPiece(board, searchStart);
        }
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

#endif
