#include "shiva.h"


// NOTE: THIS METHOD IS ALREADY IN SHITTY TEMPLATE
int bkmoved,wkmoved; //flags if black/white king has been moved (for castling purposes)
void movepiece(int **board,int i1,int j1,int i2,int j2) {

        board[i2][j2]=board[i1][j1];
        board[i1][j1]=BLANK;

        //check if castling
        if (board[i2][j2]==BLACKKING OR board[i2][j2]==WHITEKING)
        {
                if (j2>j1+1) {
                        board[board[i2][j2]-1][5]=board[i2][j2]+4;
                        board[board[i2][j2]-1][7]=BLANK;
                }
                if (j2<j1-2) {
                        board[board[i2][j2]-1][2]=board[i2][j2]+4;
                        board[board[i2][j2]-1][0]=BLANK;            
                }
        }

        checkqueening(board);

        //if kings moved, no more castling
        if (board[i2][j2]==BLACKKING)
                bkmoved=1;
        if (board[i2][j2]==WHITEKING)
                wkmoved=1;
}

// NOTE: THIS METHOD IS ALREADY IN SHITTY TEMPLATE
void display_board(int *board[]) {
        int i,j;
        int width=8,rows=8;

        printf("\n    1    2    3    4    5    6    7    8\n");    
        printf("  -----------------------------------------\n");    
        for (i=0; i<width; i++) {
                printf("%c |",i+65);
                for (j=0; j<rows; j++)
                        switch (board[i][j]) {
                                case BLACKKING:   printf(" BK |"); break;
                                case BLACKQUEEN:  printf(" BQ |"); break;
                                case BLACKBISHOP: printf(" BB |"); break;
                                case BLACKKNIGHT: printf(" BH |"); break;
                                case BLACKROOK:   printf(" BR |"); break;
                                case BLACKPAWN:   printf(" BP |"); break;
                                case WHITEKING:   printf(" WK |"); break;
                                case WHITEQUEEN:  printf(" WQ |"); break;
                                case WHITEBISHOP: printf(" WB |"); break;
                                case WHITEKNIGHT: printf(" WH |"); break;
                                case WHITEROOK:   printf(" WR |"); break;
                                case WHITEPAWN:   printf(" WP |"); break;                    
                                default:          printf("    |");
                        }


                printf("\n  -----------------------------------------\n");
        }
        printf("    1    2    3    4    5    6    7    8\n");    

        printf("\n");
}

void displayMoves(int** board, Node* list) {
        Node* traverser = list;

        while (traverser->next != NULL) {
                traverser = traverser->next;
                traverser->eaten = board[traverser->movement.dest.y][traverser->movement.dest.x];
                movepiece(board, traverser->movement.source.y, traverser->movement.source.x, traverser->movement.dest.y, traverser->movement.dest.x);
                display_board(board);
                revertMove(board, traverser);
        }
}

int main() {
        int **board;
        int i;
        Point2D location = {3,3};
        Movement movement = {location, location};
        Node* parent = createNode(movement, 1,1,1,NULL,NULL);
        //createNodeWithHead(movement, alpha, beta, ev_sign, NULL, NULL)

        board=(int**)malloc(sizeof(int*)*8);
        for (i=0; i<8; i++) 
                board[i]=(int*)malloc(sizeof(int)*8);

        init_board(board);
        board[3][3] = WHITEKNIGHT;
        display_board(board);
        Node* head = horseMoves(board, location, parent);
        displayMoves(board, head);
        printf("%i %i %i %i\n", head->children->movement.dest.x, head->children->movement.dest.y, head->children->movement.source.x, head->children->movement.source.y);
        if (head->children == NULL) {
                printf("haha mali\n");
        }
        //Node* pawnMoves(int **board, Point2D location, Node* parent)

        return 0;
}
