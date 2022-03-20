/*
 * balloon_menu_view.c
 *
 *  Created on: Jan 29, 2017
 *      Author: lab_espl_stud06
 */




#include "includes.h"
#include "balloon_menu_view.h"
void balloon(int move){
int i;
for(i=0;i<20;i++){
    if (i % 2==0){
	gdispFillCircle(10+40*i, 120+70-move, 7, Blue);
	gdispFillCircle(10+40*i, 112+70-move, 5, Blue);
	gdispDrawThickLine(10+40*i,117+70-move ,15+40*i,137+70-move , Blue,2,TRUE);
	gdispDrawThickLine(15+40*i,137+70-move,10+40*i,157+70-move , Blue,2,TRUE);
    }
    else{
	gdispFillCircle(10+40*i, 120+70-move, 7, Green);
	gdispFillCircle(10+40*i, 112+70-move, 5, Green);
	gdispDrawThickLine(10+40*i,117+70-move ,15+40*i,137+70-move , Green,2,TRUE);
	gdispDrawThickLine(15+40*i,137+70-move,10+40*i,157+70-move , Green,2,TRUE);
    }
}
}
