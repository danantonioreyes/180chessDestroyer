/*
 *  chess.h
 *  Created by Tristan Basa on 3/6/13. Copyright 2013. All rights reserved.
 *
 */

#define AND &&
#define OR  ||

#define BLACKKING   1
#define BLACKQUEEN  2
#define BLACKBISHOP 3
#define BLACKKNIGHT 4
#define BLACKROOK   5
#define BLACKPAWN   6

#define BLANK       7

#define WHITEKING   8
#define WHITEQUEEN  9
#define WHITEBISHOP 10
#define WHITEKNIGHT 11
#define WHITEROOK   12
#define WHITEPAWN   13

int bkmoved,wkmoved; //flags if black/white king has been moved (for castling purposes)

int **init_board(int *board[]) {
    int i,j;
    
    for (i=0; i<8; i++) 
        for (j=0; j<8; j++) 
            board[i][j]=BLANK;
   
    board[0][0]=BLACKROOK;
    board[0][7]=BLACKROOK;
    board[7][0]=WHITEROOK;
    board[7][7]=WHITEROOK;
   
    board[0][1]=BLACKKNIGHT;
    board[0][6]=BLACKKNIGHT;
    board[7][1]=WHITEKNIGHT;
    board[7][6]=WHITEKNIGHT;
    
    board[0][2]=BLACKBISHOP;
    board[0][5]=BLACKBISHOP;
    board[7][2]=WHITEBISHOP;
    board[7][5]=WHITEBISHOP;
    
    board[0][3]=BLACKQUEEN;
    board[0][4]=BLACKKING;
    board[7][3]=WHITEQUEEN;
    board[7][4]=WHITEKING;
    
    for (i=0; i<8; i++) {
        board[1][i]=BLACKPAWN;
        board[6][i]=WHITEPAWN;
    }
    
    bkmoved=wkmoved=0; //reset flags for castling
    
    return board;
}

int findpiece(int **board,int piece,int *x,int *y) {
    int i,j,found;
    
    found=i=0;
    while (!found AND i<8) {
        j=0;
        while (!found AND j<8) {
            if (board[i][j]==piece)
                found=1;
            j++;
        }
        i++;
    }
    
    //adjust indices to get the right coordinate of king
    *x=i-1;
    *y=j-1;
    
    return found; 
}

int invalid_move(int **board,int i1,int j1,int i2,int j2) {
    
    //check border limits
    if (i1<0 OR j1<0 OR i2<0 OR j2<0 OR i1>7 OR j1>7 OR i2>7 OR j2>7)
        return 1;
    
    //check if from and to are the same square
    if (i1==i2 AND j1==j2)
        return 1;
    
    //check if attacking the same color
    if ((board[i1][j1]<BLANK AND board[i2][j2]<BLANK) OR
        (board[i1][j1]>BLANK AND board[i2][j2]>BLANK)
        ) return 1;    
    
    return 0;
}

int safeboard(int **board,int i,int j) {//checks whether the indices are valid, if yes, return the piece at i,j
    
    if (i<0 OR i>7 OR j<0 OR j>7)
        return 0;
    else 
        return board[i][j];
}

int attacked(int **board,int king,int i1,int j1,int blocking) { //check if a position is attacked,returns from what direction
    int i,upleft,up,upright,right,downright,down,downleft,left;
    
    //king here determines the attacker side
    
    if (blocking==0)
        if (safeboard(board,i1,j1)!=BLANK) { //check if king can attack the attacker
            if (safeboard(board,i1-1,j1-1)==BLACKKING+king-1) return 16;
            if (safeboard(board,i1-1,j1)  ==BLACKKING+king-1) return 17;
            if (safeboard(board,i1-1,j1+1)==BLACKKING+king-1) return 18;
            if (safeboard(board,i1,j1-1)  ==BLACKKING+king-1) return 19;
            if (safeboard(board,i1,j1+1)  ==BLACKKING+king-1) return 20;
            if (safeboard(board,i1+1,j1-1)==BLACKKING+king-1) return 21;
            if (safeboard(board,i1+1,j1)  ==BLACKKING+king-1) return 22;
            if (safeboard(board,i1+1,j1+1)==BLACKKING+king-1) return 23; 
        }
    
    //attacked by pawn?
    if (safeboard(board,i1+ceil((double)(king-7)/6),j1-1)==BLACKPAWN+king-1) 
        return king<7?16:21;    
    if (safeboard(board,i1+ceil((double)(king-7)/6),j1+1)==BLACKPAWN+king-1) 
        return king<7?18:23;    
    
    //attacked by knight?
    if (safeboard(board,i1-2,j1-1)==BLACKKNIGHT+king-1  OR
        safeboard(board,i1-2,j1+1)==BLACKKNIGHT+king-1  OR
        safeboard(board,i1-1,j1-2)==BLACKKNIGHT+king-1  OR
        safeboard(board,i1-1,j1+2)==BLACKKNIGHT+king-1  OR
        safeboard(board,i1+1,j1-2)==BLACKKNIGHT+king-1  OR
        safeboard(board,i1+1,j1+2)==BLACKKNIGHT+king-1  OR
        safeboard(board,i1+2,j1-1)==BLACKKNIGHT+king-1  OR
        safeboard(board,i1+2,j1+1)==BLACKKNIGHT+king-1 
        ) return 15;
    
    //attacked by bishop?
    upleft=up=upright=right=downright=down=downleft=left=1;
    for (i=1; i<8; i++) {            
        if (upleft    AND safeboard(board,i1-i,j1-i)==BLACKBISHOP+king-1) return 16;
        if (upright   AND safeboard(board,i1-i,j1+i)==BLACKBISHOP+king-1) return 18;           
        if (downleft  AND safeboard(board,i1+i,j1-i)==BLACKBISHOP+king-1) return 21;            
        if (downright AND safeboard(board,i1+i,j1+i)==BLACKBISHOP+king-1) return 23;           
        
        if (upleft    AND safeboard(board,i1-i,j1-i)!=BLANK) upleft=0;
        if (upright   AND safeboard(board,i1-i,j1+i)!=BLANK) upright=0; 
        if (downleft  AND safeboard(board,i1+i,j1-i)!=BLANK) downleft=0; 
        if (downright AND safeboard(board,i1+i,j1+i)!=BLANK) downright=0;
    }
    
    //attacked by rook?
    upleft=up=upright=right=downright=down=downleft=left=1;    
    for (i=1; i<8; i++) {            
        if (up    AND safeboard(board,i1-i,j1)==BLACKROOK+king-1) return 17;          
        if (left  AND safeboard(board,i1,j1-i)==BLACKROOK+king-1) return 19;   
        if (right AND safeboard(board,i1,j1+i)==BLACKROOK+king-1) return 20;    
        if (down  AND safeboard(board,i1+i,j1)==BLACKROOK+king-1) return 22;          
        
        if (up    AND safeboard(board,i1-i,j1)!=BLANK) up=0;
        if (left  AND safeboard(board,i1,j1-i)!=BLANK) left=0; 
        if (right AND safeboard(board,i1,j1+i)!=BLANK) right=0;
        if (down  AND safeboard(board,i1+i,j1)!=BLANK) down=0; 
    }
    
    //attacked by queen?
    upleft=up=upright=right=downright=down=downleft=left=1;
    for (i=1; i<8; i++) {            
        if (upleft    AND safeboard(board,i1-i,j1-i)==BLACKQUEEN+king-1) return 16;
        if (up        AND safeboard(board,i1-i,j1)  ==BLACKQUEEN+king-1) return 17;  
        if (upright   AND safeboard(board,i1-i,j1+i)==BLACKQUEEN+king-1) return 18;   
        if (left      AND safeboard(board,i1,j1-i)  ==BLACKQUEEN+king-1) return 19;      
        if (right     AND safeboard(board,i1,j1+i)  ==BLACKQUEEN+king-1) return 20;          
        if (downleft  AND safeboard(board,i1+i,j1-i)==BLACKQUEEN+king-1) return 21;  
        if (down      AND safeboard(board,i1+i,j1)  ==BLACKQUEEN+king-1) return 22;   
        if (downright AND safeboard(board,i1+i,j1+i)==BLACKQUEEN+king-1) return 23;    
        
        if (upleft    AND safeboard(board,i1-i,j1-i)!=BLANK) upleft=0;
        if (up        AND safeboard(board,i1-i,j1)  !=BLANK) up=0;
        if (upright   AND safeboard(board,i1-i,j1+i)!=BLANK) upright=0; 
        if (left      AND safeboard(board,i1,j1-i)  !=BLANK) left=0; 
        if (right     AND safeboard(board,i1,j1+i)  !=BLANK) right=0;
        if (downleft  AND safeboard(board,i1+i,j1-i)!=BLANK) downleft=0; 
        if (down      AND safeboard(board,i1+i,j1)  !=BLANK) down=0; 
        if (downright AND safeboard(board,i1+i,j1+i)!=BLANK) downright=0;
    }
    
    return 0;
}

int checked(int **board,int king) { //king is checked?
    int i1,j1;
    
    if (!findpiece(board,king,&i1,&j1)) //King was not found (this should never happen!)
        return -1;
    
    return attacked(board,(king+7)%14,i1,j1,0);
    
}

int moveok(int **board,int i1,int j1,int i2,int j2) { //if move configuration is ok
    int i;
    
    if (invalid_move(board,i1,j1,i2,j2))
        return 0;
    
    switch (board[i1][j1]) { //check specific moves
        case BLACKKING:
        case WHITEKING:
            if ((i2==i1-1 AND j2==j1-1) OR 
                (i2==i1-1 AND j2==j1)   OR
                (i2==i1-1 AND j2==j1+1) OR
                (i2==i1   AND j2==j1-1) OR
                (i2==i1   AND j2==j1+1) OR
                (i2==i1+1 AND j2==j1-1) OR
                (i2==i1+1 AND j2==j1)   OR
                (i2==i1+1 AND j2==j1+1)                     
                ) return 1; 
            
            //check castling move for BLACK
            if (i1==0 AND j1==4   AND board[i1][j1]==BLACKKING AND !bkmoved            AND
                ((i2==0 AND j2==6 AND board[0][5]==BLANK AND board[0][7]==BLACKROOK) OR
                 (i2==0 AND j2==1 AND board[0][2]==BLANK AND board[0][3]==BLANK AND board[0][0]==BLACKROOK)
                 )) return 1; 

            //check castling move for WHITE
            if (i1==7 AND j1==4   AND board[i1][j1]==WHITEKING AND !wkmoved            AND
                ((i2==7 AND j2==6 AND board[7][5]==BLANK AND board[7][7]==WHITEROOK)   OR
                 (i2==7 AND j2==1 AND board[7][2]==BLANK AND board[7][3]==BLANK AND board[7][0]==WHITEROOK)
                 )) return 1;        
                
            break;
        case BLACKQUEEN:
        case WHITEQUEEN:
            for (i=1; i<8; i++) {
                if ((i2==i1-i AND j2==j1-i) OR
                    (i2==i1-i AND j2==j1)   OR
                    (i2==i1-i AND j2==j1+i) OR
                    (i2==i1   AND j2==j1-i) OR
                    (i2==i1   AND j2==j1+i) OR
                    (i2==i1+i AND j2==j1-i) OR
                    (i2==i1+i AND j2==j1)   OR
                    (i2==i1+i AND j2==j1+i)
                    ) return 1;  
                
                if (i2<i1-i AND j2<j1-i)
                    if (board[i1-i][j1-i]!=BLANK) 
                        return 0;
                if (i2<i1-i AND j2>j1+i)
                    if (board[i1-i][j1+i]!=BLANK) 
                        return 0;
                if (i2>i1+i AND j2<j1-i)
                    if (board[i1+i][j1-i]!=BLANK) 
                        return 0;
                if (i2>i1+i AND j2>j1+i)
                    if (board[i1+i][j1+i]!=BLANK) 
                        return 0;
                if (i2>i1+i AND j1==j2)
                    if (board[i1+i][j1]!=BLANK) 
                        return 0;
                if (i2<i1-i AND j1==j2)
                    if (board[i1-i][j1]!=BLANK) 
                        return 0;
                if (j2>j1+i AND i1==i2)
                    if (board[i1][j1+i]!=BLANK) 
                        return 0;
                if (j2<j1-i AND i1==i2)
                    if (board[i1][j1-i]!=BLANK) 
                        return 0;
            }
            break;
        case BLACKBISHOP:
        case WHITEBISHOP:
            for (i=1; i<8; i++) {
                if ((i2==i1-i AND j2==j1-i) OR
                    (i2==i1-i AND j2==j1+i) OR
                    (i2==i1+i AND j2==j1-i) OR
                    (i2==i1+i AND j2==j1+i)
                    ) return 1;  
                
                if (i2<i1-i AND j2<j1-i)
                    if (board[i1-i][j1-i]!=BLANK) 
                        return 0;
                if (i2<i1-i AND j2>j1+i)
                    if (board[i1-i][j1+i]!=BLANK) 
                        return 0;
                if (i2>i1+i AND j2<j1-i)
                    if (board[i1+i][j1-i]!=BLANK) 
                        return 0;
                if (i2>i1+i AND j2>j1+i)
                    if (board[i1+i][j1+i]!=BLANK) 
                        return 0;
            }
            break;
        case BLACKKNIGHT:
        case WHITEKNIGHT:
            if ((i2==i1-2 AND j2==j1-1) OR
                (i2==i1-2 AND j2==j1+1) OR
                (i2==i1-1 AND j2==j1-2) OR
                (i2==i1-1 AND j2==j1+2) OR
                (i2==i1+1 AND j2==j1-2) OR
                (i2==i1+1 AND j2==j1+2) OR
                (i2==i1+2 AND j2==j1-1) OR
                (i2==i1+2 AND j2==j1+1)  
                ) return 1;           
            break;            
        case BLACKROOK:
        case WHITEROOK:
            for (i=1; i<8; i++) {
                if ((i2==i1+i AND j2==j1)   OR
                    (i2==i1-i AND j2==j1)   OR
                    (i2==i1   AND j2==j1+i) OR
                    (i2==i1   AND j2==j1-i)
                    ) return 1;
                
                if (i2>i1+i AND j1==j2)
                    if (board[i1+i][j1]!=BLANK) 
                        return 0;
                if (i2<i1-i AND j1==j2)
                    if (board[i1-i][j1]!=BLANK) 
                        return 0;
                if (j2>j1+i AND i1==i2)
                    if (board[i1][j1+i]!=BLANK) 
                        return 0;
                if (j2<j1-i AND i1==i2)
                    if (board[i1][j1-i]!=BLANK) 
                        return 0;
            }
            break;
        case BLACKPAWN:
            if ((i2==i1+1 AND j2==j1 AND board[i2][j2]==BLANK)             OR
                (i1==1    AND i2==i1+2 AND j2==j1 AND board[3][j1]==BLANK) OR
                (i2==i1+1 AND j2==j1-1 AND board[i2][j2]>BLANK)            OR
                (i2==i1+1 AND j2==j1+1 AND board[i2][j2]>BLANK)                 
                ) return 1;
            break;
        case WHITEPAWN:
            if ((i2==i1-1 AND j2==j1 AND board[i2][j2]==BLANK)             OR
                (i1==6    AND i2==i1-2 AND j2==j1 AND board[4][j1]==BLANK) OR
                (i2==i1-1 AND j2==j1-1 AND board[i2][j2]<BLANK)            OR
                (i2==i1-1 AND j2==j1+1 AND board[i2][j2]<BLANK)                 
                ) return 1;
        default:
            break;
    }
    
    return 0;
}

int valid_move(int **board,int i1,int j1,int i2,int j2,int turn) {
    int validity,temp;
    
    validity=0;
    if ((turn-7)*(safeboard(board,i1,j1)-7)>0) {//check if correct turn
        if (moveok(board,i1,j1,i2,j2)) {
            temp=board[i2][j2];
            board[i2][j2]=board[i1][j1];
            board[i1][j1]=BLANK;
            if (!checked(board,board[i2][j2]<7?BLACKKING:WHITEKING))
                validity=1;
            board[i1][j1]=board[i2][j2];
            board[i2][j2]=temp;
        }
    }
    else 
        printf("\nWrong turn: ");
    
    return validity;
}

int blocked(int **board,int king,int checkdir) { //check if any piece can block check
    int i,j,i1,j1,i2,j2,isign,jsign;
    
    if (!findpiece(board,king,&i1,&j1)) //King was not found (this should never happen!)
        return -1;
    
    //get direction of check
    switch (checkdir) {
        case 16: isign=-1; jsign=-1; break; //upleft
        case 17: isign=-1; jsign=0;  break; //up
        case 18: isign=-1; jsign=1;  break; //upright
        case 19: isign=0;  jsign=-1; break; //left
        case 20: isign=0;  jsign=1;  break; //right
        case 21: isign=1;  jsign=-1; break; //downleft
        case 22: isign=1;  jsign=0;  break; //down
        case 23: isign=1;  jsign=1;  break; //downright
        default:
            return 0; //checked by knight (cannot be blocked)
    }
        
    for (i=1; i<8; i++) { //see if any piece can block check
        
        i2=i1+(isign*i);
        j2=j1+(jsign*i);
        
        if (safeboard(board,i2,j1)!=BLANK) //if past the checking piece, stop checking for blockers
            i=8;
        if (attacked(board,king,i2,j2,1)) 
            return 1;
    }
    
    return 0;
}

int checkmate(int **board,int king,int checkdir) {    
    int i,j;
    
    if (!findpiece(board,king,&i,&j)) //King was not found (this should never happen!)
        return -1;
    
    if (!valid_move(board,i,j,i-1,j-1,king) AND
        !valid_move(board,i,j,i-1,j,king)   AND
        !valid_move(board,i,j,i-1,j+1,king) AND
        !valid_move(board,i,j,i,j-1,king)   AND
        !valid_move(board,i,j,i,j+1,king)   AND
        !valid_move(board,i,j,i+1,j-1,king) AND
        !valid_move(board,i,j,i+1,j,king)   AND
        !valid_move(board,i,j,i+1,j+1,king) 
        AND !blocked(board,king,checkdir)
        ) return 1;
    
    return 0;
}

int stalemate(int **board,int king) {
    int i,i1,j1;
    
    for (i1=0; i1<8; i1++)
        for (j1=0; j1<8; j1++)
            if ((king-7)*(board[i1][j1]-7)>0) //if piece is same side with the king
                switch (board[i1][j1]) {
                    case BLACKPAWN:
                        if (valid_move(board,i1,j1,i1+1,j1,king)   OR
                            valid_move(board,i1,j1,i1+2,j1,king)   OR                            
                            valid_move(board,i1,j1,i1+1,j1-1,king) OR                            
                            valid_move(board,i1,j1,i1+1,j1+1,king)                            
                            ) return 0;
                        break;
                    case WHITEPAWN:
                        if (valid_move(board,i1,j1,i1-1,j1,king)   OR
                            valid_move(board,i1,j1,i1-2,j1,king)   OR                            
                            valid_move(board,i1,j1,i1-1,j1-1,king) OR
                            valid_move(board,i1,j1,i1-1,j1+1,king)
                            ) return 0;
                        break;
                    case BLACKQUEEN:
                    case WHITEQUEEN:
                        for (i=1; i<8; i++) {
                            if (valid_move(board,i1,j1,i1-i,j1-i,king) OR
                                valid_move(board,i1,j1,i1-i,j1,king)   OR                               
                                valid_move(board,i1,j1,i1-i,j1+i,king) OR                               
                                valid_move(board,i1,j1,i1,j1-i,king)   OR                               
                                valid_move(board,i1,j1,i1,j1+i,king)   OR                               
                                valid_move(board,i1,j1,i1+i,j1-i,king) OR                               
                                valid_move(board,i1,j1,i1+i,j1,king)   OR                               
                                valid_move(board,i1,j1,i1+i,j1+i,king)                                 
                                ) return 0;  
                        }
                        break;
                    case BLACKBISHOP:
                    case WHITEBISHOP:
                        for (i=1; i<8; i++) {
                            if (valid_move(board,i1,j1,i1-i,j1-i,king) OR
                                valid_move(board,i1,j1,i1-i,j1+i,king) OR                               
                                valid_move(board,i1,j1,i1+i,j1-i,king) OR                               
                                valid_move(board,i1,j1,i1+i,j1+i,king)                                 
                                ) return 0;  
                        }
                        break;
                    case BLACKKNIGHT:
                    case WHITEKNIGHT:
                        if (valid_move(board,i1,j1,i1-2,j1-1,king) OR
                            valid_move(board,i1,j1,i1-2,j1+1,king) OR                               
                            valid_move(board,i1,j1,i1-1,j1-2,king) OR                               
                            valid_move(board,i1,j1,i1-1,j1+2,king) OR                               
                            valid_move(board,i1,j1,i1+1,j1-2,king) OR                               
                            valid_move(board,i1,j1,i1+1,j1+2,king) OR                               
                            valid_move(board,i1,j1,i1+2,j1-1,king) OR                               
                            valid_move(board,i1,j1,i1+2,j1+1,king)                                 
                            ) return 0;  
                        break;            
                    case BLACKROOK:
                    case WHITEROOK:
                        for (i=1; i<8; i++) {
                            if (valid_move(board,i1,j1,i1-i,j1,king)   OR                               
                                valid_move(board,i1,j1,i1,j1-i,king)   OR                               
                                valid_move(board,i1,j1,i1,j1+i,king)   OR                               
                                valid_move(board,i1,j1,i1+i,j1,king)                           
                                ) return 0;  
                        }
                        break;
                    case WHITEKING:
                    case BLACKKING:
                        if (valid_move(board,i1,j1,i1-1,j1-1,king) OR
                            valid_move(board,i1,j1,i1-1,j1,king)   OR
                            valid_move(board,i1,j1,i1-1,j1+1,king) OR
                            valid_move(board,i1,j1,i1,j1-1,king)   OR
                            valid_move(board,i1,j1,i1,j1+1,king)   OR
                            valid_move(board,i1,j1,i1+1,j1-1,king) OR
                            valid_move(board,i1,j1,i1+1,j1,king)   OR
                            valid_move(board,i1,j1,i1+1,j1+1,king) 
                            ) return 0;
                        break;
                    default:
                        break;
                }
    return 1;
}

void checkqueening(int **board) {
    int i;
    
    for (i=0; i<8; i++) {
        if (board[0][i]==WHITEPAWN)
            board[0][i]=WHITEQUEEN;
        if (board[7][i]==BLACKPAWN) 
            board[7][i]=BLACKQUEEN;
    }
}
