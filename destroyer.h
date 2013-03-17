/*
	additional functions and constants
	John Smith Paraggua
	Dan Reyes
*/

int alternating = 0;

int weightOf(int arg) {
	if (arg == BLACKKING) return 25;
	if (arg == BLACKQUEEN) return 15;
	if (arg == BLACKBISHOP) return 10;
	if (arg == BLACKKNIGHT) return 10;
	if (arg == BLACKROOK) return 10;
	if (arg == BLACKPAWN) return 5;
	if (arg == WHITEKING) return -25;
	if (arg == WHITEQUEEN) return -15;
	if (arg == WHITEBISHOP) return -10;
	if (arg == WHITEKNIGHT) return -10;
	if (arg == WHITEROOK) return -10;
	if (arg == WHITEPAWN) return -5;
	if (arg == BLANK) return 0;
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