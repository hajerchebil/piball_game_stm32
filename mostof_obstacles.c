/*
 * mostof_obstacles.c
 *
 *  Created on: Jan 30, 2017
 *      Author: lab_espl_stud06
 */

#include "includes.h"
#include "mostof_obstacles.h"

void obstacles_function(int x_obst_marr, int y_obst_marr){
	// for the obstacle1
	int x1_obst=75;
	int y1_obst=100;
	int x2_obst=75;
	int y2_obst=135;
	int x3_obst=93;
	int y3_obst=145;
	// for the obstacle2
    /// 1 Obstacle

	struct point pnts3[] = {{x1_obst,y1_obst},{x2_obst,y2_obst},{x3_obst,y3_obst}};
	gdispDrawPoly(0, 0, pnts3, 3, Magenta);  //(9 vertices
	gdispFillConvexPoly(0, 0, pnts3, 3, Magenta);
	/// 2 obstacle: the as the first
	struct point pnts4[] = {{320-x1_obst,y1_obst},{320-x2_obst,y2_obst},{320-x3_obst,y3_obst}};
	gdispDrawPoly(0, 0, pnts4, 3, Magenta);  //(9 vertices
	gdispFillConvexPoly(0, 0, pnts4, 3, Magenta);
	//// 3 Obstacle1
	// rectangle
	gdispDrawBox(x_obst_marr, y_obst_marr, 10, 20, Maroon);
	gdispFillArea(x_obst_marr, y_obst_marr, 10, 20, Maroon);
	// cercle for the rectangle

	//// 3 Obstacle2
	// rectangle
	gdispDrawBox(x_obst_marr-40, y_obst_marr+10, 10, 20, Maroon);
	gdispFillArea(x_obst_marr-40, y_obst_marr+10, 10, 20, Maroon);
	// cercle for the rectangle
	//// 3 Obstacle3
	// rectangle
	gdispDrawBox(x_obst_marr+40, y_obst_marr+10, 10, 20, Maroon);
	gdispFillArea(x_obst_marr+40, y_obst_marr+10, 10, 20, Maroon);
}

void path_fortheball(){
	struct point pntspath[] = {{290,240},{303,240},{260,190},{260,205}};
	gdispDrawPoly(0, 0, pntspath, 4, Cyan);
	gdispFillConvexPoly(0, 0, pntspath, 4, Cyan);
}
void circle_obstacle_tab1(){

	gdispFillCircle(160, 15, 10, Orange);
	gdispFillCircle(160, 15, 5, White);
	gdispFillCircle(160, 15, 2, Red);

	gdispFillCircle(120, 15, 10, Blue);
	gdispFillCircle(120, 15, 5, White);
	gdispFillCircle(120, 15, 2, SkyBlue);

	gdispFillCircle(200, 15, 10, Olive);
	gdispFillCircle(200, 15, 5, White);
	gdispFillCircle(200, 15, 3, Green);
}
void triangle_greenobstacle(){
	struct point pnts3[] = {{77,50},{90,60},{75,65}};
	gdispDrawPoly(0, 0, pnts3, 3, Green);  // green triangle als obstacle links
	gdispFillConvexPoly(0, 0, pnts3, 3, Green);

	struct point pnts4[] = {{77+10,50+20},{90+10,60+20},{75+10,65+20}};
	gdispDrawPoly(0, 0, pnts4, 3, Green);  // green triangle als obstacle links
	gdispFillConvexPoly(0, 0, pnts4, 3, Green);


	struct point pnts5[] = {{320-77,50},{320-90,60},{320-75,65}};
	gdispDrawPoly(0, 0, pnts5, 3, Green);  // green triangle als obstacle rechts
	gdispFillConvexPoly(0, 0, pnts5, 3, Green);

	struct point pnts6[] = {{320-(77+10),50+20},{320-(90+10),60+20},{320-(75+10),65+20}};
	gdispDrawPoly(0, 0, pnts6, 3, Green);  // green triangle als obstacle rechts
	gdispFillConvexPoly(0, 0, pnts6, 3, Green);
}
void obstacle_middle(){
	// orange obstacles in the middle
	struct point pnts_obstaclemitte[] = {{140,140},{155,148},{155,176},{145, 168}};
	gdispDrawPoly(0, 0, pnts_obstaclemitte,4, Orange);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_obstaclemitte, 4, Orange);
	gdispDrawThickLine(145, 148, 153, 152, Red,  4, TRUE);
	gdispDrawThickLine(145, 156, 153, 160, Red,  4, TRUE);
	gdispDrawThickLine(145, 164, 153, 168, Red,  4, TRUE);
	struct point pnts_obstaclemitte2[] = {{155,148},{170,140},{165,168},{155,176}};
	gdispDrawPoly(0, 0, pnts_obstaclemitte2,4, Orange);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_obstaclemitte2, 4, Orange);	// orange obstacles in the middle
	// lines in this Obstable
	gdispDrawThickLine(155, 152,163 ,148, Red,  4, TRUE);
	gdispDrawThickLine(155, 160, 163, 156, Red,  4, TRUE);
	gdispDrawThickLine(155, 168, 163, 164, Red,  4, TRUE);
}






