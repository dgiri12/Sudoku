//
//  main.c
//  sudoku
//
//  Created by DJMac on 9/21/21.
//

#include <stdio.h>
#include<pthread.h>
#include<stdlib.h>

typedef int bool;

#define true 1
#define false 0;

int sudokuPuzzle[9][9]; //stores the sudoku numbers
bool columns[9]; //check if column 1 to 9 is valid
bool rows[9]; //check if rows 1 to 9 is valid
bool subgrids[9]; //check if 9 subgrids are valid
int dummy;

void *columnRunner (void *param);



typedef struct{
    int index;
    int topRow;
    int bottomRow;
    int leftColumn;
    int rightColumn;
} puzzleRange;

puzzleRange subgridFormat[9];

void createSubgridFormat();

int main(int argc, const char * argv[]) {
    
    
    FILE *fp;
    fp = fopen ("SudokuPuzzle.txt","r");
    int r=0;
    int c=0;
    while(r<9){
        c=0;
        while(c<9){
            fscanf(fp,"%d",&sudokuPuzzle[r][c]);
            if(c!=8)fscanf(fp,"\t");
            c++;
        }
        r++;
    }
    r=0;
    c=0;
    while(r<9){
        c=0;
        while(c<9){
            printf("%d\t",sudokuPuzzle[r][c]);
            c++;
        }
        printf("\n");
        r++;
    }

    
    //initialize structures
    //column parsing
    puzzleRange columnFormat[9], rowFormat[9];
    int i =0;
    for (i=0;i<9;i++){//goes through 0-8, has been tested
        columnFormat[i].topRow = 0;
        columnFormat[i].bottomRow = 8;
        columnFormat[i].leftColumn = i;
        columnFormat[i].rightColumn = i;
        columnFormat[i].index=i;
        
//        printf("topRow: %d\n", columnFormat[i].topRow);
//        printf("bottomRow: %d\n",columnFormat[i].bottomRow);
//        printf("leftColumn: %d\n",columnFormat[i].leftColumn);
//        printf("rightColumn: %d\n",columnFormat[i].rightColumn);
//        printf("index: %d\n",columnFormat[i].index);
//        printf("\n");
        
        rowFormat[i].topRow = i;
        rowFormat[i].bottomRow = i;
        rowFormat[i].leftColumn = 0;
        rowFormat[i].rightColumn = 8;
        rowFormat[i].index=i;
        
//        printf("Rowformats: topRow: %d\n", rowFormat[i].topRow);
//        printf("bottomRow: %d\n",rowFormat[i].bottomRow);
//        printf("leftColumn: %d\n",rowFormat[i].leftColumn);
//        printf("rightColumn: %d\n",rowFormat[i].rightColumn);
//        printf("index: %d\n",rowFormat[i].index);
//        printf("\n");
        
    }
    
    createSubgridFormat(); //create subgrids
    
    //COLUMNS
    pthread_t columnsTid[9];
    pthread_attr_t columnsAttr[9];
    
    //get default attributes.
    i=0;
    for (i=0;i<9;i++){
        pthread_attr_init(&(columnsAttr[i]));
    }
    
    //create the threads.
    i=0;
    for (i=0;i<9;i++){
        pthread_create(&(columnsTid[i]),&(columnsAttr[i]),columnRunner,&subgridFormat[i]);
        printf("Thread Created: %d\n",i+1);
    }
    printf("\n");
    
        i=0;
    for(i=0;i<9;i++){
        pthread_join(columnsTid[i],NULL);
    }
    
    return 0;
}

void *runner (void *param){
    puzzleRange *pzR;
    int topRow,bottomRow,leftColumn,rightColumn,index;
    
    pthread_t self;
    
    pzR = (puzzleRange *)param;
    
    topRow = pzR->topRow;
    printf("CT: topRow=%d\n",topRow);
    
    bottomRow = pzR->bottomRow;
    printf("CT: bottomRow=%d\n",bottomRow);
    
    leftColumn = pzR->leftColumn;
    printf("CT: leftColumn=%d\n",leftColumn);
    
    rightColumn = pzR->rightColumn;
    printf("CT: rightColumn=%d\n",rightColumn);
    
    index = pzR->index;
    printf("CT: index=%d\n",index);
            
    self = pthread_self();

    int r=topRow;
    int c=leftColumn;
    int i=1;
    int counter=0;
    
    printf("Loop start...\n");
    for(i=1;i<=9;i++){
        r=topRow;
        printf("Checked times: i=%d",i);
    while(r <=bottomRow){
        printf("r=%d\n",r);
        c=leftColumn;
        while(c <=rightColumn){
            printf("c=%d\n",c);
            printf("Value in puzzle [%d][%d] is %d\n",r,c,sudokuPuzzle[r][c]);
            if(sudokuPuzzle[r][c]==i){
                printf("Equals i!\n");
                counter++;
            }
            c++;
        }
        printf("\n");
        r++;
    }
        if (counter>=2){
            columns[index]=false;
            printf("Thread: %lX TRow: %d, BRow: %d, LCol: %d, RCol: %d invalid!\n",(unsigned long)self,topRow,bottomRow,leftColumn,rightColumn);
            pthread_exit(0);
        }
        counter=0;
    }
    //if this line is reached then its a match.
    columns[index]=true;
    printf("Thread: %lX TRow: %d, BRow: %d, LCol: %d, RCol: %d valid!\n",(unsigned long)self,topRow,bottomRow,leftColumn,rightColumn);
    pthread_exit(0);
}

void createSubgridFormat(){
    subgridFormat[0].topRow=0;
    subgridFormat[0].bottomRow=2;
    subgridFormat[0].rightColumn=2;
    subgridFormat[0].leftColumn=0;
    subgridFormat[0].index=0;
    
    subgridFormat[1].topRow=0;
    subgridFormat[1].bottomRow=2;
    subgridFormat[1].rightColumn=5;
    subgridFormat[1].leftColumn=3;
    subgridFormat[0].index=1;
    
    subgridFormat[2].topRow=0;
    subgridFormat[2].bottomRow=2;
    subgridFormat[2].rightColumn=8;
    subgridFormat[2].leftColumn=6;
    subgridFormat[0].index=2;
    
    subgridFormat[3].topRow=3;
    subgridFormat[3].bottomRow=5;
    subgridFormat[3].rightColumn=2;
    subgridFormat[3].leftColumn=0;
    subgridFormat[0].index=3;
    
    subgridFormat[4].topRow=3;
    subgridFormat[4].bottomRow=5;
    subgridFormat[4].rightColumn=5;
    subgridFormat[4].leftColumn=3;
    subgridFormat[0].index=4;
    
    subgridFormat[5].topRow=3;
    subgridFormat[5].bottomRow=4;
    subgridFormat[5].rightColumn=8;
    subgridFormat[5].leftColumn=6;
    subgridFormat[0].index=5;
    
    subgridFormat[6].topRow=6;
    subgridFormat[6].bottomRow=8;
    subgridFormat[6].rightColumn=2;
    subgridFormat[6].leftColumn=0;
    subgridFormat[0].index=6;
    
    subgridFormat[7].topRow=6;
    subgridFormat[7].bottomRow=8;
    subgridFormat[7].rightColumn=5;
    subgridFormat[7].leftColumn=3;
    subgridFormat[0].index=7;
    
    subgridFormat[8].topRow=6;
    subgridFormat[8].bottomRow=8;
    subgridFormat[8].rightColumn=8;
    subgridFormat[8].leftColumn=6;
    subgridFormat[0].index=8;
}
