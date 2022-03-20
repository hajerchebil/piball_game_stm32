/*
 * graph_tables.c
 *
 *  Created on: Jan 30, 2017
 *      Author: lab_espl_stud06
 */


#include "includes.h"
#include "graph_tables.h"
void graph_oneplayer2table(int x1_graph1,int y1_graph1,int x2_graph1,int y2_graph1,int x3_graph1,int y3_graph1,
		int x4_graph1,int y4_graph1,int x5_graph1,int y5_graph1,int x6_graph1,int y6_graph1,int x7_graph1
		,int y7_graph1,int x8_graph1,int y8_graph1,int x9_graph1,int y9_graph1){
    // 1 background zeichnen
	struct point pnts_graph1[] = {{x1_graph1,y1_graph1},{x2_graph1,y2_graph1},{x3_graph1,y3_graph1},{0,y3_graph1}};
	gdispDrawPoly(0, 0, pnts_graph1,4, Orange);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph1, 4, Orange);

	struct point pnts_graph2[] = {{0,y3_graph1},{x3_graph1,y3_graph1},{x4_graph1,y4_graph1},{0,y4_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2,4, Orange);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph2, 4, Orange);
	struct point pnts_graph3[] = {{0,y4_graph1},{x4_graph1,y4_graph1},{x5_graph1,y5_graph1},{0,y5_graph1}};
	gdispDrawPoly(0, 0, pnts_graph3,4, Orange);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph3, 4, Orange);
	struct point pnts_graph4[] = {{0,y5_graph1},{x5_graph1,y5_graph1},{x6_graph1,y6_graph1},{0,y6_graph1}};
	gdispDrawPoly(0, 0, pnts_graph4,4, Orange);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph4, 4, Orange);
	struct point pnts_graph5[] = {{0,y6_graph1},{x6_graph1,y6_graph1},{x7_graph1,y7_graph1},{0,y7_graph1}};
	gdispDrawPoly(0, 0, pnts_graph5,4, Orange);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph5, 4, Orange);
	struct point pnts_graph6[] = {{0,y7_graph1},{x7_graph1,y7_graph1},{x8_graph1,y8_graph1},{0,y8_graph1}};
	gdispDrawPoly(0, 0, pnts_graph6,4, Orange);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph6, 4, Orange);
	struct point pnts_graph7[] = {{0,y8_graph1},{x8_graph1,y8_graph1},{x9_graph1,y9_graph1},{0,y9_graph1}};
	gdispDrawPoly(0, 0, pnts_graph7,4, Orange);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph7, 4, Orange);

	// 2 Background
	struct point pnts_graph2_1[] = {{320-x1_graph1,y1_graph1},{320-x2_graph1,y2_graph1},{320-x3_graph1,y3_graph1},{320,y3_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_1,4, Orange);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph2_1, 4, Orange);
	struct point pnts_graph2_2[] = {{320,y3_graph1},{320-x3_graph1,y3_graph1},{320-x4_graph1,y4_graph1},{320,y4_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_2,4, Orange);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph2_2, 4, Orange);
	struct point pnts_graph2_3[] = {{320,y4_graph1},{320-x4_graph1,y4_graph1},{320-x5_graph1,y5_graph1},{320,y5_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_3,4, Orange);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph2_3, 4, Orange);
	struct point pnts_graph2_4[] = {{320,y5_graph1},{320-x5_graph1,y5_graph1},{320-x6_graph1,y6_graph1},{320,y6_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_4,4, Orange);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph2_4, 4, Orange);
	struct point pnts_graph2_5[] = {{320,y6_graph1},{320-x6_graph1,y6_graph1},{320-x7_graph1,y7_graph1},{320,y7_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_5,4, Orange);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph2_5, 4, Orange);
	struct point pnts_graph2_6[] = {{320,y7_graph1},{320-x7_graph1,y7_graph1},{320-x8_graph1,y8_graph1},{320,y8_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_6,4, Orange);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph2_6, 4, Orange);
	struct point pnts_graph2_7[] = {{320,y8_graph1},{320-x8_graph1,y8_graph1},{320-x9_graph1,y9_graph1},{320,y9_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_7,4, Orange);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph2_7, 4, Orange);
	// 2 Background
}
void graph_oneplayer3table(int x1_graph1,int y1_graph1,int x2_graph1,int y2_graph1,int x3_graph1,int y3_graph1,
		int x4_graph1,int y4_graph1,int x5_graph1,int y5_graph1,int x6_graph1,int y6_graph1,int x7_graph1
		,int y7_graph1,int x8_graph1,int y8_graph1,int x9_graph1,int y9_graph1){
    // 1 background zeichnen
	struct point pnts_graph1[] = {{x1_graph1,y1_graph1},{x2_graph1,y2_graph1},{x3_graph1,y3_graph1},{0,y3_graph1}};
	gdispDrawPoly(0, 0, pnts_graph1,4, Olive);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph1, 4, Olive);
	struct point pnts_graph2[] = {{0,y3_graph1},{x3_graph1,y3_graph1},{x4_graph1,y4_graph1},{0,y4_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2,4, Olive);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph2, 4, Olive);
	struct point pnts_graph3[] = {{0,y4_graph1},{x4_graph1,y4_graph1},{x5_graph1,y5_graph1},{0,y5_graph1}};
	gdispDrawPoly(0, 0, pnts_graph3,4, Olive);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph3, 4, Olive);
	struct point pnts_graph4[] = {{0,y5_graph1},{x5_graph1,y5_graph1},{x6_graph1,y6_graph1},{0,y6_graph1}};
	gdispDrawPoly(0, 0, pnts_graph4,4, Olive);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph4, 4, Olive);
	struct point pnts_graph5[] = {{0,y6_graph1},{x6_graph1,y6_graph1},{x7_graph1,y7_graph1},{0,y7_graph1}};
	gdispDrawPoly(0, 0, pnts_graph5,4, Olive);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph5, 4, Olive);
	struct point pnts_graph6[] = {{0,y7_graph1},{x7_graph1,y7_graph1},{x8_graph1,y8_graph1},{0,y8_graph1}};
	gdispDrawPoly(0, 0, pnts_graph6,4, Olive);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph6, 4, Olive);
	struct point pnts_graph7[] = {{0,y8_graph1},{x8_graph1,y8_graph1},{x9_graph1,y9_graph1},{0,y9_graph1}};
	gdispDrawPoly(0, 0, pnts_graph7,4, Olive);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph7, 4, Olive);

	// 2 Background
	struct point pnts_graph2_1[] = {{320-x1_graph1,y1_graph1},{320-x2_graph1,y2_graph1},{320-x3_graph1,y3_graph1},{320,y3_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_1,4, Olive);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph2_1, 4, Olive);
	struct point pnts_graph2_2[] = {{320,y3_graph1},{320-x3_graph1,y3_graph1},{320-x4_graph1,y4_graph1},{320,y4_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_2,4, Olive);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph2_2, 4, Olive);
	struct point pnts_graph2_3[] = {{320,y4_graph1},{320-x4_graph1,y4_graph1},{320-x5_graph1,y5_graph1},{320,y5_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_3,4, Olive);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph2_3, 4, Olive);
	struct point pnts_graph2_4[] = {{320,y5_graph1},{320-x5_graph1,y5_graph1},{320-x6_graph1,y6_graph1},{320,y6_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_4,4, Olive);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph2_4, 4, Olive);
	struct point pnts_graph2_5[] = {{320,y6_graph1},{320-x6_graph1,y6_graph1},{320-x7_graph1,y7_graph1},{320,y7_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_5,4, Olive);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph2_5, 4, Olive);
	struct point pnts_graph2_6[] = {{320,y7_graph1},{320-x7_graph1,y7_graph1},{320-x8_graph1,y8_graph1},{320,y8_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_6,4, Olive);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph2_6, 4, Olive);
	struct point pnts_graph2_7[] = {{320,y8_graph1},{320-x8_graph1,y8_graph1},{320-x9_graph1,y9_graph1},{320,y9_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_7,4, Olive);  //(4 vertices)
	gdispFillConvexPoly(0, 0, pnts_graph2_7, 4, Olive);
	// 2 Background
}
void graph_oneplayer1table(int x1_graph1,int y1_graph1,int x2_graph1,int y2_graph1,int x3_graph1,int y3_graph1,
		int x4_graph1,int y4_graph1,int x5_graph1,int y5_graph1,int x6_graph1,int y6_graph1,int x7_graph1
		,int y7_graph1,int x8_graph1,int y8_graph1,int x9_graph1,int y9_graph1){
    // 1 background zeichnen
	struct point pnts_graph1[] = {{x1_graph1,y1_graph1},{x2_graph1,y2_graph1},{x3_graph1,y3_graph1},{0,y3_graph1}};
	gdispDrawPoly(0, 0, pnts_graph1,4, Blue);
	gdispFillConvexPoly(0, 0, pnts_graph1, 4, Blue);
	struct point pnts_graph2[] = {{0,y3_graph1},{x3_graph1,y3_graph1},{x4_graph1,y4_graph1},{0,y4_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2,4, Blue);
	gdispFillConvexPoly(0, 0, pnts_graph2, 4, Blue);
	struct point pnts_graph3[] = {{0,y4_graph1},{x4_graph1,y4_graph1},{x5_graph1,y5_graph1},{0,y5_graph1}};
	gdispDrawPoly(0, 0, pnts_graph3,4, Blue);
	gdispFillConvexPoly(0, 0, pnts_graph3, 4, Blue);
	struct point pnts_graph4[] = {{0,y5_graph1},{x5_graph1,y5_graph1},{x6_graph1,y6_graph1},{0,y6_graph1}};
	gdispDrawPoly(0, 0, pnts_graph4,4, Blue);
	gdispFillConvexPoly(0, 0, pnts_graph4, 4, Blue);
	struct point pnts_graph5[] = {{0,y6_graph1},{x6_graph1,y6_graph1},{x7_graph1,y7_graph1},{0,y7_graph1}};
	gdispDrawPoly(0, 0, pnts_graph5,4, Blue);
	gdispFillConvexPoly(0, 0, pnts_graph5, 4, Blue);
	struct point pnts_graph6[] = {{0,y7_graph1},{x7_graph1,y7_graph1},{x8_graph1,y8_graph1},{0,y8_graph1}};
	gdispDrawPoly(0, 0, pnts_graph6,4, Blue);
	gdispFillConvexPoly(0, 0, pnts_graph6, 4, Blue);
	struct point pnts_graph7[] = {{0,y8_graph1},{x8_graph1,y8_graph1},{x9_graph1,y9_graph1},{0,y9_graph1}};
	gdispDrawPoly(0, 0, pnts_graph7,4, Blue);
	gdispFillConvexPoly(0, 0, pnts_graph7, 4, Blue);
	// 2 Background
	struct point pnts_graph2_1[] = {{320-x1_graph1,y1_graph1},{320-x2_graph1,y2_graph1},{320-x3_graph1,y3_graph1},{320,y3_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_1,4, Blue);
	gdispFillConvexPoly(0, 0, pnts_graph2_1, 4, Blue);
	struct point pnts_graph2_2[] = {{320,y3_graph1},{320-x3_graph1,y3_graph1},{320-x4_graph1,y4_graph1},{320,y4_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_2,4, Blue);
	gdispFillConvexPoly(0, 0, pnts_graph2_2, 4, Blue);
	struct point pnts_graph2_3[] = {{320,y4_graph1},{320-x4_graph1,y4_graph1},{320-x5_graph1,y5_graph1},{320,y5_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_3,4, Blue);
	gdispFillConvexPoly(0, 0, pnts_graph2_3, 4, Blue);
	struct point pnts_graph2_4[] = {{320,y5_graph1},{320-x5_graph1,y5_graph1},{320-x6_graph1,y6_graph1},{320,y6_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_4,4, Blue);
	gdispFillConvexPoly(0, 0, pnts_graph2_4, 4, Blue);
	struct point pnts_graph2_5[] = {{320,y6_graph1},{320-x6_graph1,y6_graph1},{320-x7_graph1,y7_graph1},{320,y7_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_5,4, Blue);
	gdispFillConvexPoly(0, 0, pnts_graph2_5, 4, Blue);
	struct point pnts_graph2_6[] = {{320,y7_graph1},{320-x7_graph1,y7_graph1},{320-x8_graph1,y8_graph1},{320,y8_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_6,4, Blue);
	gdispFillConvexPoly(0, 0, pnts_graph2_6, 4, Blue);
	struct point pnts_graph2_7[] = {{320,y8_graph1},{320-x8_graph1,y8_graph1},{320-x9_graph1,y9_graph1},{320,y9_graph1}};
	gdispDrawPoly(0, 0, pnts_graph2_7,4, Blue);
	gdispFillConvexPoly(0, 0, pnts_graph2_7, 4, Blue);
	// 2 Background

}
