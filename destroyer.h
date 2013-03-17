/*
	additional functions, constants, and structures
	John Smith Paraggua
	Dan Reyes
*/

void movepiece(int **board,int i1,int j1,int i2,int j2);


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
	VERTEX* parent;
	VERTEX* children[9999];
};

typedef struct node NODE;
struct node
{
	VERTEX* v; 	
	NODE* next;
};

int x_1;
int y_1;
int x_2;
int y_2;
int score = -9999;
int iterator = 0;

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

void expansion(VERTEX* arg, int turn) {
	int i,j,x,y;
	int counter;
	int c = 0;
	int** tempboard;
	tempboard = (int**)malloc(sizeof(int*)*8);
	for (i=0; i<8; i++) 
        tempboard[i]=(int*)malloc(sizeof(int)*8);//lalagyanan ng board na ilalagay sa vertices
		
		
	//fill the children with nulls
	for(i=0; i<9999; i++) arg->children[i] = NULL;

	for(i=0; i<=7; i++) 
	for(j=0; j<=7; j++) {
		for(x=0; x<=7; x++)
		for(y=0; y<=7; y++) {
			if (valid_move(arg->boardstate,i,j,x,y,turn)) {
				
				boardcopy(arg->boardstate, tempboard); //get the original boardstate
				movepiece(tempboard,i,j,x,y); //move the piece
				
				arg->children[c] = (VERTEX*)malloc(sizeof(VERTEX));
				arg->children[c]->boardstate = (int**)malloc(sizeof(int*)*8);
				for (counter=0; counter<8; counter++) 
					arg->children[c]->boardstate[counter]=(int*)malloc(sizeof(int)*8);
				
		
				boardcopy(tempboard, arg->children[c]->boardstate); //assign to the boardstate of the vertex

				arg->children[c]->fromx = i;
				arg->children[c]->fromy = j;
				arg->children[c]->tox = x;
				arg->children[c]->toy = y;

				arg->children[c]->depth = arg->depth + 1;

				arg->children[c]->parent = arg;

				arg->children[c]->boardscore = scoreOfBoard(arg->children[c]->boardstate);

				if(arg->children[c]->boardscore >= score) {
					score = arg->children[c]->boardscore;
					x_1 = i;
					y_1 = j;
					x_2 = x;
					y_2 = y;					
				}
				
				c++;
			}
		}
	}
	
}

void insert(NODE* list, VERTEX* arg) {
	NODE* temp = (NODE*)malloc(sizeof(NODE));
	temp->v = arg;
	temp->next = NULL;
	
	NODE* alpha = list;
	while(alpha->next != NULL) alpha = alpha->next;
	alpha->next = temp;
}