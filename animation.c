/*
 * animation.c
 *
 *  Created on: Jan 30, 2017
 *      Author: lab_espl_stud06
 */


#include "includes.h"
#include "animation.h"

void animation(int j){  //   , int ball_posx,int ball_posy){
    if (j % 5==0){
    gdispDrawThickLine(140+j,150, 150+j, 150,Red, 2, TRUE);
    gdispDrawThickLine(125-j,150, 135-j, 150,Red, 2, TRUE);
    gdispDrawThickLine(137,137-j, 137, 147-j,Green, 2, TRUE);
    gdispDrawThickLine(137,152+j, 137, 162+j,Green, 2, TRUE);
    // dazwischen
    gdispFillCircle(145+j,140-j,3, Red);
    gdispFillCircle(128-j,140-j,3, Green);
    gdispFillCircle(128-j,157+j,3, Red);
    gdispFillCircle(145+j,157+j,3, Green);
     //gdispDrawThickLine(140,148, 160, 138,Red, 2, TRUE);
    }
    else{
        gdispDrawThickLine(140+j,150, 150+j, 150,Green, 2, TRUE);
        gdispDrawThickLine(125-j,150, 135-j, 150,Green, 2, TRUE);
        gdispDrawThickLine(137,137-j, 137, 147-j,Red, 2, TRUE);
        gdispDrawThickLine(137,152+j, 137, 162+j,Red, 2, TRUE);
        // dazwischen
        gdispFillCircle(145+j,140-j,3, Green);
        gdispFillCircle(128-j,140-j,3, Red);
        gdispFillCircle(128-j,157+j,3, Green);
        gdispFillCircle(145+j,157+j,3, Red);
         //gdispDrawThickLine(140,148, 160, 138,Red, 2, TRUE);

    }

   /* if (j % 5==0){
    gdispDrawThickLine(140+j,150, 150+j, 150,Red, 2, TRUE);
    gdispDrawThickLine(125-j,150, 135-j, 150,Red, 2, TRUE);
    gdispDrawThickLine(137,137-j, 137, 147-j,Green, 2, TRUE);
    gdispDrawThickLine(137,152+j, 137, 162+j,Green, 2, TRUE);
    // dazwischen
    gdispFillCircle(145+j,140-j,3, Red);
    gdispFillCircle(128-j,140-j,3, Green);
    gdispFillCircle(128-j,157+j,3, Red);
    gdispFillCircle(145+j,157+j,3, Green);
     //gdispDrawThickLine(140,148, 160, 138,Red, 2, TRUE);
    }
    else{
        gdispDrawThickLine(140+j,150, 150+j, 150,Green, 2, TRUE);
        gdispDrawThickLine(125-j,150, 135-j, 150,Green, 2, TRUE);
        gdispDrawThickLine(137,137-j, 137, 147-j,Red, 2, TRUE);
        gdispDrawThickLine(137,152+j, 137, 162+j,Red, 2, TRUE);
        // dazwischen
        gdispFillCircle(145+j,140-j,3, Green);
        gdispFillCircle(128-j,140-j,3, Red);
        gdispFillCircle(128-j,157+j,3, Green);
        gdispFillCircle(145+j,157+j,3, Red);
         //gdispDrawThickLine(140,148, 160, 138,Red, 2, TRUE);

    }
    */
}
