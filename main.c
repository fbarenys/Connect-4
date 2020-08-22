//
//  main.c
//  GAME1
//
//  Created by Felix Barenys Marimon on 26/06/2020.
//  Copyright © 2020 Felix Barenys Marimon. All rights reserved.
//

#include <stdio.h>
#include<stdlib.h>
#include<string.h>


 //COMPUTER TURNS: parells fins 21

#define N 8
typedef struct node {
    int board[N][N];   //-128,127
    struct node *children[N];
    int n_children;
    double value;   //heuristic value
} Node;


//Check winner (HEURISTIC FUNCTION)

int checkDiagonal1(int board[N][N]){ 
    int i;
        for (i = 0; i < 5; i++){
                if(board[i][i]==1 && board[i][i]==board[i+1][i+1] && board[i+2][i+2]==board[i+3][i+3] && board[i][i]==board[i+3][i+3]){
                    return 1;
                }
                else if (board[i][i]==2 && board[i][i]==board[i+1][i+1] && board[i+2][i+2]==board[i+3][i+3] && board[i][i]==board[i+3][i+3]){
                        return -1;
                }
            
            }
        
    return 0;

}


int checkDiagonal2(int board[N][N]){
    int i;
    for (i = 0; i < 5; i++){

            if(board[i][7-i]==1 && board[i][7-i]==board[i+1][7-(i+1)] && board[i+2][7-(i+2)]==board[i+3][7-(i+3)] && board[i][7-i]==board[i+3][7-(i+3)]){
        return 1;
            }
       
           else if(board[i][7-i]==2 && board[i][7-i]==board[i+1][7-(i+1)] && board[i+2][7-(i+2)]==board[i+3][7-(i+3)] && board[i][7-i]==board[i+3][7-(i+3)]){
       return -1;
           }
    
    }
         return 0;
    }
    
int checkHorizontal(int board[N][N]){
    int i,j;
    for (i = 0; i < 8; i++){
        for (j=0;j<5; j++){
            
                if(board[i][j]==1 && board[i][j]==board[i][(j+1)] && board[i][j+2]==board[i][j+3] && board[i][j]==board[i][j+3]){
                    return 1;
                
                }
              
              else if(board[i][j]==2 && board[i][j]==board[i][(j+1)] && board[i][j+2]==board[i][j+3] && board[i][j]==board[i][j+3]){
                  return -1;
              }
            
            }
    }
     return 0;
}
        

int checkVertical(int board[N][N]){
    int i,j;
    for (j = 0; j < 8; j++){
        for (i=0;i<5; i++){
                if(board[i][j]==1&&board[i][j]==board[i+1][j] && board[i+2][j]==board[i+3][j] && board[i][j]==board[i+3][j]){
                return 1;
                }
                else if(board[i][j]==2 &&board[i][j]==board[i+1][j] && board[i+2][j]==board[i+3][j] && board[i][j]==board[i+3][j]){
                return -1;

    
                }}
    
        }
        return 0;
    
}


int checkWin(int board[N][N]){
    if(checkDiagonal1(board)+checkDiagonal2(board)+checkVertical(board)+checkHorizontal(board)>0){
        return -1;
    }
    if(checkDiagonal1(board)+checkDiagonal2(board)+checkVertical(board)+checkHorizontal(board)<0){
        return 1;
        
    }
        
    return 0;
}



int applyThrow(int child[N][N], int numChild){
    int i;
   for (i = 7; i >= 0; i--){
       if(child[i][numChild]==0){
           break;
       }
   }
   return i;
}

void copyBoard(int child[N][N],int parent[N][N],int column, int level){
    int f,j,row;
    
    for (f=0;f<N;f++){
        for (j=0;j<N;j++){
            child[f][j]=parent[f][j];
        }
    }
    if (level>0){
    row=applyThrow(parent, column );
        if(level%2!=0){
             child[row][column]=1;
        }else{
            child[row][column]=2;
        }
   
        
    }
    
   
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//num Child == column because you have as many children as columns are left (and therefore places where to throw your object)


int numOfChildren(int child[N][N]){
    int c,empty_column= 0;
    for (c=0;c<N;c++){
        if(child[0][c]==0){
            empty_column++;
        }
    }
    return empty_column;
}



//NODES CREATION

void setValuesLeaf(Node *n){
    int i,j;
    //set value of leaf nodes
        for(i=0;i<n->n_children;i++) {
            for(j=0;j<n->children[i]->n_children;j++){
                 n->children[i]->children[j]->value= checkWin( n->children[i]->children[j]->board);
            }
        }}

void setValuesnodesInPar(Node* n){
    //MAXIMIZER
    int max=-10,i;
    for (i=0;i<n->n_children;i++){
        if(n->children[i]->value>max){
                max=n->children[i]->value;
            }
        }
        n->value=max;
    }
        

int setValuesnodesPar(Node *f){
    //MINIMIZER
    int min=10,i,col=0;
    for (i=0;i<f->n_children;i++){
        if(f->children[i]->value<min){
            min=f->children[i]->value;
            col=i;
            
        }
    }
    f->value=min;
    return col;
}
        
    
    

int valuesTree(Node *root) {
    int i,col;
    setValuesLeaf(root);
    for(i=0;i<root->n_children;i++){
        setValuesnodesInPar(root->children[i]);
    };
    col=setValuesnodesPar(root);
    return col;//top node
}



Node *createRoot(int array[8][8]){
    int x,j;
    Node *p= malloc(sizeof(Node));
    for (x=0;x<8;x++){
        for(j=0;j<8;j++){
            p->board[x][j]=array[x][j];
        }
    }
    p->n_children=numOfChildren(p->board);
    return p;
}

Node *createNode(Node *parent,int numChild, int level) {
    Node *p= malloc(sizeof(Node));
    copyBoard(p->board,parent->board,numChild,level);
    if (level<2) {
        p->n_children=numOfChildren(p->board);  //returns the quantity of free columns on the board
    }
    else {
        p->n_children=0;
    }
    return p;
}

void createChildren(Node *parent,int level) {
    int chi;
    for (chi=0;chi<parent->n_children;chi++) {
        parent->children[chi]=createNode(parent,chi,level);
    }
}

//We cosider root node alreade created and n_children already set.
void createTree(Node *root) {
    int i;
    createChildren(root,1);   //1st generation
    for(i=0;i<root->n_children;i++) {       //creaes the 2nd generation
        createChildren(root->children[i],2);
    }
}




//DISPLAY OF NODES



void displayNode(Node *n){
    int i,j;
    printf("%f\n",n->value);
    for(i=0;i<8;i++){
        printf("   %i| ",i);
        printf("%f\n",n->children[i]->value);
        for(j=0;j<8;j++){
            printf("       %i| ",j);
            printf("%f\n",n->children[i]->children[j]->value);
        }
    }
    
}




//TAULA

int TAULER[8][8], turn=1;


void initTauler(){//you put the board blank
    int i, j;
      for(i=0; i<8; i++) {
         for(j=0;j<8;j++) {
             TAULER[i][j]= 0 ;
         }
      }
    
}

void displayTauler(int board[8][8]){
    int i,j=0,x;
    printf("\n");
    for (i = 0; i < 8; i++){
        printf("\t%d ",i+1);
    }
    printf("\n");
    for (x = 0; x < 8; x++){
        
        for (j=0; j< 8; j++){
            
            printf("\t%d|", board[x][j]);
        }
        
    printf("\n");
    }
    printf("\n");
}

int columnFull(int column){
    
    if (TAULER[0][column]!= 0){
        return 1;
    }
    
    return 0;
}

int filaGravity (int column){
    int i;
  
    
    for (i = 7; i >= 0; i--){
        if(TAULER[i][column]==0){
            break;
        }
    }
    return i;
}

void freeMemory(Node *n){
    int i,j;
    //set value of leaf nodes
        for(i=0;i<n->n_children;i++) {
            for(j=0;j<n->children[i]->n_children;j++){
                free(n->children[i]->children[j]);
            }
            free(n->children[i]);
                             
        }
    free(n);
}
    



void computerGame(int array [N][N]){
    Node *root;
    int column_wheretothrow;
    root=createRoot(array);
    
    createTree(root);
    column_wheretothrow=valuesTree(root);
    freeMemory(root);//fer funcio perquè alliberi tot l'arbre
    if (columnFull(column_wheretothrow)==1){
         TAULER[filaGravity(column_wheretothrow+1)][column_wheretothrow+1]=2;
    }else{
        //printf("fila= %i",filaGravity(column_wheretothrow));
        TAULER[filaGravity(column_wheretothrow)][column_wheretothrow]=2;}
    
    turn++;
}
    
void playerGame(){
    int joc;
    printf("Seleccioneu la casella on voleu tirar la fitxa (1-8): ");
    scanf("%d",&joc);
    if (joc >8|| joc<1) {
        printf("Seleccioni una altre columna\n");
        playerGame();
        }
    else if (columnFull(joc-1)==1){
        printf("Select another column, this one is FULL!\n");
        playerGame();
    }
    else{
        TAULER[filaGravity(joc-1)][joc-1]=1;
        turn++;
    }

}




int main(){
    int res;
    initTauler();
    displayTauler(TAULER);
    while (turn<21){
        playerGame();
        computerGame(TAULER);
        res=checkWin(TAULER);
        displayTauler(TAULER);
        
        
           if (res==-1){
               printf("\nGame finished, you are the winner\n");
                break;
            }else if (res==1){
                printf("\nGame finished, the computer is the winner\n");
                break;
            }
        
    }
    if (turn==21){
        printf("\nGAME FINISHED, No turns left\n\n");
    }
    
}
     





