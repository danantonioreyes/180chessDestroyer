/*
	additional functions, constants, and structures
	John Smith Paraggua
	Dan Reyes
*/

typedef struct vertex VERTEX;
struct vertex
{
	int** boardstate;
	int boardscore;
	int fromx;
	int fromy;
	int tox;
	int toy;
	int depth;
	VERTEX* children[9999];
};

typedef struct node NODE;
struct node
{
	VERTEX* v; 	
	NODE* next;
};

void boardcopy(int** src, int** dest) {
	int i,j;
	for(i=0; i<=7; i++)
		for(j=0; j<=7; j++) dest[i][j] = src[i][j];
}

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

void expansion(VERTEX* arg, int** boardstate, int i, int j, int x, int y, int depth) {
	arg->boardstate = (int**)malloc(sizeof(int*)*8);
	for (i=0; i<8; i++) 
		arg->boardstate[i]=(int*)malloc(sizeof(int)*8);
	boardcopy(boardstate, arg->boardstate); //assign to the boardstate of the vertex
	
	arg->fromx = i;
	arg->fromy = j;
	arg->tox = x;
	arg->toy = y;
	arg->depth = depth;
	
	arg->boardscore = scoreOfBoard(arg->boardstate);
	
}

void insert(NODE* list, VERTEX* arg) {
	NODE* temp = (NODE*)malloc(sizeof(NODE));
	temp->v = arg;
	temp->next = NULL;
	
	NODE* alpha = list;
	while(alpha->next != NULL) alpha = alpha->next;
	alpha->next = temp;
}