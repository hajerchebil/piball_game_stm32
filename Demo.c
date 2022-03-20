#include "includes.h"
#include<time.h>
#include<Engine.h>
#include<stdlib.h>
#include<math.h>

// start and stop bytes for the UART protocol
#define ESPL_StartByte 0xAA
#define ESPL_StopByte 0x55
volatile int __errno = 0;

QueueHandle_t ESPL_RxQueue; // Already defined in ESPL_Functions.h
SemaphoreHandle_t ESPL_DisplayReady;
SemaphoreHandle_t ButtonASemaphore = NULL;
SemaphoreHandle_t ButtonBSemaphore;
SemaphoreHandle_t ButtonCSemaphore;
SemaphoreHandle_t ButtonDSemaphore;
SemaphoreHandle_t ButtonESemaphore;
SemaphoreHandle_t ButtonKSemaphore;
SemaphoreHandle_t axiySemaphore;

QueueHandle_t DrawQueue;
// new project
int menu_multiplayer;
int counter_A=0;		// line bytes

int counter_C=0;
int counter_D=0;
int counter_E=0;
int counter_K=0;
int off_x=0;
int off_y=0;
int counterB_new=0;  // button B : Task notification
int offsetx_ball=0;
int offsety_ball=0;
int var_joystick_ball=0; // to remove later
// for task notification notwendig
int read_A;
int read_B;
int read_C;
int read_D;
int read_E;
int init_f1_x1=170; // abstand zwischen den beiden Flipper: 15
int init_f1_y1=233;
int init_f1_x2=225;//190
int init_f1_y2=210;
int init_f1_x3=223;//190
int init_f1_y3=220;
// flipper 2
int init_f2_x1=93;//100 : abstand dazwischenpnts5
int init_f2_y1=220;

int init_f2_x2=95;//140
int init_f2_y2=210;

int init_f2_x3=150;//    157;  20 dazwischen
int init_f2_y3=233;
// background initialisierungen
int x1_graph1=0;
int y1_graph1=0;
int x2_graph1=40;
int y2_graph1=0;
int x3_graph1=60;
int y3_graph1=60;

int x4_graph1=50;
int y4_graph1=80;

int x5_graph1=60;
int y5_graph1=100;

int x6_graph1=50;
int y6_graph1=120;

int x7_graph1=50;
int y7_graph1=180;
int x8_graph1=80;
int y8_graph1=240;
int x9_graph1=0;
int y9_graph1=240;
int x_obst_marr=160;
int y_obst_marr=60;
// for the obstacle1
int x1_obst=75;
int y1_obst=100;
int x2_obst=75;
int y2_obst=135;
int x3_obst=93;
int y3_obst=145;
int new_play_resume=0;
int global_resume_over=0;
int table_resume_running=0;
BaseType_t xHigherPriorityTaskWoken;
// task handles
TaskHandle_t xHandlingTask;
TaskHandle_t xHandle1;
TaskHandle_t xHandle2;  // one player first table
TaskHandle_t xHandle3;
TaskHandle_t xHandle5;
TaskHandle_t xHandle;
TaskHandle_t xHandle_joystick;
TaskHandle_t xHandle_multiplayer;
TaskHandle_t xHandle_resume;
TaskHandle_t xHandle_oneplayer2table;
TaskHandle_t xHandle_oneplayer3table;
TaskHandle_t xHandle_gameover;
struct coord joystick_now = { 0, 0 }, joystick_last = { 0, 0 };
extern int iLastObject ;
extern _circle *Ball;
extern polygon *pFlipper;
extern polygon *pFlipperR;
extern polygon *Test1;
extern polygon *Test2;
int level = 1;

void my_ball(int x_my_ball,int y_my_ball) {
gdispFillCircle(x_my_ball,y_my_ball,6, Black);
}

int var_test=1;
int joystick_state(){
  int var_joystick;
  if(joystick_now.y==255){  // nach unten gehen
	  var_joystick=1;
 }
  else if(joystick_now.y==0){  // nach oben gehen
   var_joystick=-1;
  }
  else{
	   var_joystick=0;
  }
return(var_joystick);
}
static resume(){
TickType_t xLastWakeTime;
const TickType_t xFrequency = 20;
font_t font1 =  gdispOpenFont("DejaVuSans24*");; // Load font for ugfx		// rectangle
struct line line; // Init buffer for line
char str[100]; // Init buffer for message
int choice;
int read_K;
int y_cercle_init=90;
vTaskSuspend( NULL );
while(TRUE){
  vTaskDelayUntil( &xLastWakeTime, xFrequency );
  while (xQueueReceive(DrawQueue, &line, 0) == pdTRUE) {
	 }
  gdispClear(Green);
  if(global_resume_over==0){

	  read_K=GPIO_ReadInputDataBit(ESPL_Register_Button_K, ESPL_Pin_Button_K);
	  if((read_K==0)&& (y_cercle_init==90)){
		  if (table_resume_running==1){ // 1 table
		    vTaskResume(xHandle2);
		  }
		  else if(table_resume_running==2) {// 2 table
			  vTaskResume(xHandle_oneplayer2table);
		  }
		  else if(table_resume_running==3){  // 3 table
			  vTaskResume(xHandle_oneplayer3table);
		  }
		  vTaskSuspend(NULL);
	  }
	  else if((read_K==0)&& (y_cercle_init==130)){
		  new_play_resume=1;
		  vTaskResume(xHandle2);
		  vTaskSuspend(NULL);
	  }
	  choice=(choice+joystick_state())%3;
	  if(((joystick_now.y==255))&&(y_cercle_init==90)){
		y_cercle_init=130;
	  }
	  else if(((joystick_now.y==0))&&(y_cercle_init==130)){
		y_cercle_init=90;
	  }
	  gdispFillCircle(90,y_cercle_init,8, Black);
	  sprintf(str, "Resume");
	  gdispDrawString(100, 80, str, font1, Red);
	  sprintf(str, "New play");
	  gdispDrawString(100, 120, str, font1, Red);
  }
  else{


	  sprintf(str, "Game over");
	  gdispDrawString(100, 80, str, font1, Red);
  }
  xSemaphoreTake(ESPL_DisplayReady, portMAX_DELAY);
  ESPL_DrawLayer();
  vTaskDelay(50);
}
}

void create_function(){
			iLastObject = 0;

			createQuadrangle( 0, 60, 50, 59, 80, 0, 0, 0, env);

			createQuadrangle( 5, 240, 40, 240, 50, 200, 5, 200, env);

			createQuadrangle( 2, 200, 50, 200, 50, 60, 2, 60, env);
			createTriangle( 50, 200, 60, 180, 50, 160, env);
			createTriangle( 50, 160, 60, 140, 50, 120, env);

			createQuadrangle( 320 - 50, 60, 320, 60, 320, 0, 320-80, 0, env);

			createQuadrangle( 320 - 40, 240, 320, 240, 320, 200, 320-50, 2000, env);

			createQuadrangle( 320 - 50, 200, 320, 200, 320, 60, 320 - 50, 60, env);
			createTriangle( 320 - 60, 180, 320 - 50, 200, 320-50, 160, env);
			createTriangle( 320 - 60, 140, 320 - 50, 160, 320-50, 120, env);
}

static draw_oneplayer_2table(){
TickType_t xLastWakeTime;
const TickType_t xFrequency = 20;
struct line line; // Init buffer for line
int j=0;
char str[100]; // Init buffer for message


// for the obstacle2

int f=0;
int var_physics=0;
int counting_score=0;
int lives=3;
int var_nureinmal=0;
font_t font1; // Load font for ugfx		// rectangle
font1 = gdispOpenFont("DejaVuSans10*");
vTaskSuspend( NULL );
//xTaskCreate(resume, "resume", 2000, NULL, 4,&xHandle_resume);
if (menu_multiplayer==0){  // single player
	while (TRUE) {
	if (var_physics==0){
		iLastObject = 0;
		create_function();
		// after creating the background=> physics function
		int offset = 85, offsety = 0, os = 110;
		Ball = createCircle( 180, 100, 1);
		Ball->fVel[0] = 0;
		//Ball->fVel[1] = (-10) / dTIME_STEP;
		Ball->fVel[1] = 0;
		Ball->fMass = 1;
		pFlipper = createFlipper( 20 + offset, 40 , 60 +offset, 15 -offsety, 20+offset, 30 - offsety, left);
		pFlipperR = createFlipper( 60 +offset + os, 15 -offsety, 20 + offset + os, 40, 20+offset +os, 30 - offsety, right);
	}
	v_updateLoop();
	var_physics = 1;
	var_joystick_ball=1;
	vTaskDelayUntil( &xLastWakeTime, xFrequency );
	// wait for buffer swap
	while (xQueueReceive(DrawQueue, &line, 0) == pdTRUE) {
	 }
	// draw to display
	gdispClear(Yellow);
	read_A=GPIO_ReadInputDataBit(ESPL_Register_Button_A, ESPL_Pin_Button_A);
	read_B=GPIO_ReadInputDataBit(ESPL_Register_Button_B, ESPL_Pin_Button_B);
	read_D=GPIO_ReadInputDataBit(ESPL_Register_Button_D, ESPL_Pin_Button_D);
	read_C=GPIO_ReadInputDataBit(ESPL_Register_Button_C, ESPL_Pin_Button_C);
	read_E=GPIO_ReadInputDataBit(ESPL_Register_Button_E, ESPL_Pin_Button_E);
	if(read_E==0){
		global_resume_over=0; // resume and not game over
		table_resume_running=2;  // resume second table
		vTaskResume(xHandle_resume);
		vTaskSuspend(NULL);

	}

	// beide flipper funktionen: Initialisierung
	// beide flipper funktionen: Initialisierung

	if ((read_B==1)&&(read_D==1)){

		pFlipper->Direction = down;
		pFlipperR->Direction = down;

	   // ball_moving();
	//beide flipper funktionen:Initialisierung
	}
	else if ((read_B==0)&&(read_D==1)){
		pFlipperR->Direction = up;
		pFlipper->Direction = down;
		//ball_moving();
	}
	else if ((read_B==1)&&(read_D==0)){ // Button D for flipper 2: links

		pFlipper->Direction = up;
		pFlipperR->Direction = down;

	}
	else if ((read_B==0)&&(read_D==0)){

		pFlipper->Direction = up;
		pFlipperR->Direction = up;
	}
	gdispFillCircle( Ball->fPos[0], 240 - Ball->fPos[1], Ball->fRadius + fCOLLISION_DIST, Green);
	gdispFillConvexPoly( 0, 0, getPoints(pFlipper), 3, Red);
	gdispFillConvexPoly( 0, 0, getPoints(pFlipperR), 3, Red);
	gdispFillCircle( pFlipper->pfRotPoint[0], 239- pFlipper->pfRotPoint[1] + 5, 6, Black);
	gdispFillCircle( pFlipperR->pfRotPoint[0], 239- pFlipperR->pfRotPoint[1] + 5, 6, Black);
	if ((Ball->fPos[0]<160) &&((239-Ball->fPos[1])<10)) {
		vTaskResume(xHandle2);
		vTaskSuspend( NULL );
	}
	else if((Ball->fPos[0]>160) &&(239-(Ball->fPos[1])<10)){
		vTaskResume(xHandle_oneplayer3table);
		vTaskSuspend( NULL );
	}
	if (((160<=Ball->fPos[0] && Ball->fPos<=170)&&(60<=239-(Ball->fPos[1]) &&239-(Ball->fPos[1])<=80))
			||((160-40<=Ball->fPos[0] && Ball->fPos[0]<=170-40)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))
			||((160+40<=Ball->fPos[0] && Ball->fPos[0]<=170+40)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))||((75<=Ball->fPos[0]&&239-(Ball->fPos[0])<=100)&&(50<=239-(Ball->fPos[1])&&239-(Ball->fPos[1])<=85))
			||((320-75<=Ball->fPos[0] && Ball->fPos[0]<=320-100)&&(50<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=85))||((80<=Ball->fPos[0]&& Ball->fPos[0]<=90)&&(180<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=190))
			||((320-80<=Ball->fPos[0] && Ball->fPos[0]<=320-90)&&(180<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=190))||((75<=Ball->fPos[0]&& 239-(Ball->fPos[0])<=93)&&(100<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=145))
			||((320-75<=Ball->fPos[0] && Ball->fPos[0]<=320-93)&&(100<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=145))||((80<=Ball->fPos[0] && Ball->fPos[0]<=90)&&(180<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=190)) ){
		counting_score++;
	}
	if (((160<=Ball->fPos[0] && Ball->fPos<=170)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))
			||((120<=Ball->fPos[0] && Ball->fPos[0]<=170)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80)) // war 130
			||((200<=Ball->fPos[0] && Ball->fPos[0]<=210)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))){
		for(f=0;f<40;f++){
		int ball_posx=Ball->fPos[0];
		int ball_posy=239-Ball->fPos[1];
		animation(f);
		}
	}
	if(239-Ball->fPos[1]>=230)
	{
		lives--;
		var_nureinmal = 0;
	}

    if (lives==0){
    	global_resume_over=1; // game over
    	vTaskResume(xHandle_resume);
    	vTaskSuspend( NULL );
    }
	else if (((0<lives)&&(lives<=2))&&(var_nureinmal==0)){
		//iLastObject = 0;

		Ball->fPos[0] = 170;
		Ball->fPos[1] = 100;
		Ball->fVel[0] = 0;
		Ball->fVel[1] = 0;
		var_nureinmal =1;
	}

	// for the background
	graph_oneplayer2table(x1_graph1,y1_graph1,x2_graph1, y2_graph1,x3_graph1,y3_graph1,
			x4_graph1,y4_graph1,x5_graph1,y5_graph1,x6_graph1,y6_graph1, x7_graph1
			, y7_graph1, x8_graph1, y8_graph1,x9_graph1,y9_graph1);

	// for the background
	//Obstacles zeichnen
	obstacles_function(x_obst_marr,y_obst_marr);
	// no path in table 2 or 3
	triangle_greenobstacle();
	//obstacle_middle();
	gdispFillCircle(85, 185, 10, Blue);
	gdispFillCircle(85, 185, 5, White);
	gdispFillCircle(85, 185, 2, SkyBlue);
	gdispFillCircle(320-85, 185, 10, Olive);
	gdispFillCircle(320-85, 185, 5, White);
	gdispFillCircle(320-85, 185, 2, Green);
	/*for(j=0;j<7;j++){
		gdispFillCircle(240+8*cos(j),190+8*sin(j),3, Teal);
	}
	for(j=0;j<7;j++){
		gdispFillCircle(320-240+8*cos(j),190+8*sin(j),3, Teal);
	}
	*/

	// door for table 3
	gdispDrawThickLine(77, 0, 50, 10, Red,  3, TRUE);
	// door for table 2
	gdispDrawThickLine(320-77, 0, 320-50, 10, Red,  3, TRUE);

    if ((counting_score>1000)&&(counting_score<2000)){
    	level=2;
    }
    else if(counting_score>2000){
    	level=3;
    }
    else{
    	level=1;
    }
	sprintf(str, "Table 1");
	gdispDrawString(7, 20, str, font1, Black);
	sprintf(str, "Level:");
	gdispDrawString(7, 80, str, font1, Black);
	sprintf(str, "%d", level);
	gdispDrawString(7, 90, str, font1, Black);
	sprintf(str, "Score:");
	gdispDrawString(7, 120, str, font1, Black);
	sprintf(str, "%d", counting_score);
	gdispDrawString(7, 130, str, font1, Black);
	sprintf(str, "Lives:");
	gdispDrawString(7, 160, str, font1, White);
	sprintf(str, "%d", lives);
	gdispDrawString(7, 170, str, font1, White);
	sprintf(str, "Table 3");
	gdispDrawString(280, 20, str, font1, Black);
	sprintf(str, "Press A to go back to menu");  // go back to menu
	gdispDrawString(90, 220, str, font1, Black); // go back to menu
	if (read_C==0){
		animation(f);
	}
	if (read_A==0){
		vTaskResume(xHandle1); // we can go to menu_view task from first_view task when pressing A
		vTaskSuspend( NULL );
	}
	obstacle_middle();
	xSemaphoreTake(ESPL_DisplayReady, portMAX_DELAY);
	ESPL_DrawLayer();
	vTaskDelay(10);
	}
  }
else if(menu_multiplayer==1){
	int variable_uart=1;  ///==0 dann senden else receiving
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 20;
	struct line line; // Init buffer for line
	//struct line line2; // Init buffer for line

	char str[100]; // Init buffer for message
	int read_B;
	int read_D;
	/// intialization for the ball
	int x_ball=255;
	int y_ball= 196;
	/// intialization for the ball

    int x_ball_received;
	// for the obstacle1

	// for ball shooting: engine: initialization of the red engine
	int engine_x3=263;
	int engine_y3=194;
	int engine_x4=258;
	int engine_y4=205;

	font_t font1; // Load font for ugfx		// rectangle
	font1 = gdispOpenFont("DejaVuSans10*");
	int var=0;
	int choice=0;

	int first_shoot=1;
	// for the obstacle1
	int x1_obst=75;
	int y1_obst=100;
	int x2_obst=75;
	int y2_obst=135;
	int x3_obst=93;
	int y3_obst=145;
	int f=0;
	int var_physics=0;
	int counting_score=0;
	int lives=3;
	int slave_gototable=0;
	level=1;
	int var_nureinmal=0;
	struct coord c1={ 0, 0 },c2={ 0, 0 },c3={ 0, 0 },c4={ 0, 0 },c5={ 0, 0 },c6={ 0, 0 };  // c1,c2,c3 are to send via uart, coord flipper1, flipper 2 and coord ball
	vTaskSuspend( NULL );
	while (TRUE) {
		if (var_physics==0){
			create_function();
			int offset = 85, offsety = 0, os = 110;
			if (variable_uart==0){
			  //Ball = createCircle( 40 +offset, 100, 1);
			  Ball = createCircle( 180, 100, 1);
			  Ball->fVel[0] = 0;
			//Ball->fVel[1] = (-10) / dTIME_STEP;
			  Ball->fVel[1] = 0;
			  Ball->fMass = 1;
			}
			pFlipper = createFlipper( 20 + offset, 40 , 60 +offset, 15 -offsety, 20+offset, 30 - offsety, left);
			pFlipperR = createFlipper( 60 +offset + os, 15 -offsety, 20 + offset + os, 40, 20+offset +os, 30 - offsety, right);
		}
		//####
		v_updateLoop();
		var_physics=1;
		var_joystick_ball=1;
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
		// wait for buffer swap
		while (xQueueReceive(DrawQueue, &line, 0) == pdTRUE) {
		 }

		// draw to display
		gdispClear(Cyan);
		//xTaskCreate(draw_oneplayer, "draw_oneplayer", 1000, NULL, 4,&xHandle2);
	    // B and D buttons are used for playing
		read_B=GPIO_ReadInputDataBit(ESPL_Register_Button_B, ESPL_Pin_Button_B);
		read_D=GPIO_ReadInputDataBit(ESPL_Register_Button_D, ESPL_Pin_Button_D);
		read_E=GPIO_ReadInputDataBit(ESPL_Register_Button_E, ESPL_Pin_Button_E);
		read_A=GPIO_ReadInputDataBit(ESPL_Register_Button_A, ESPL_Pin_Button_A);
	    if(read_E==0){  // when pressing button E, stop playing and go to resume Task
	    	global_resume_over=0; // resume
	    	table_resume_running=1;  // resume first table
	    	vTaskResume(xHandle_resume);
	    	vTaskSuspend(NULL);

	    }
  // drawing
		// for the background
		graph_oneplayer2table(x1_graph1,y1_graph1,x2_graph1, y2_graph1,x3_graph1,y3_graph1,
				x4_graph1,y4_graph1,x5_graph1,y5_graph1,x6_graph1,y6_graph1, x7_graph1
				, y7_graph1, x8_graph1, y8_graph1,x9_graph1,y9_graph1);

		// for the background
		//Obstacles zeichnen
		obstacles_function(x_obst_marr,y_obst_marr);
		// no path in table 2 or 3
		triangle_greenobstacle();
		//obstacle_middle();
		gdispFillCircle(85, 185, 10, Blue);
		gdispFillCircle(85, 185, 5, White);
		gdispFillCircle(85, 185, 2, SkyBlue);
		gdispFillCircle(320-85, 185, 10, Olive);
		gdispFillCircle(320-85, 185, 5, White);
		gdispFillCircle(320-85, 185, 2, Green);
		/*for(j=0;j<7;j++){
			gdispFillCircle(240+8*cos(j),190+8*sin(j),3, Teal);
		}
		for(j=0;j<7;j++){
			gdispFillCircle(320-240+8*cos(j),190+8*sin(j),3, Teal);
		}
		*/
	    if ((counting_score>1000)&&(counting_score<2000)){
	    	level=2;
	    }
	    else if(counting_score>2000){
	    	level=3;
	    }
	    else{
	    	level=1;
	    }
		sprintf(str, "Table 1");
		gdispDrawString(7, 20, str, font1, Black);
		sprintf(str, "Level:");
		gdispDrawString(7, 80, str, font1, Black);
		sprintf(str, "%d", level);
		gdispDrawString(7, 90, str, font1, Black);
		sprintf(str, "Score:");
		gdispDrawString(7, 120, str, font1, Black);
		sprintf(str, "%d", counting_score);
		gdispDrawString(7, 130, str, font1, Black);
		sprintf(str, "Lives:");
		gdispDrawString(7, 160, str, font1, White);
		sprintf(str, "%d", lives);
		gdispDrawString(7, 170, str, font1, White);
		sprintf(str, "Table 3");
		gdispDrawString(280, 20, str, font1, Black);

		// door for table 3
		gdispDrawThickLine(77, 0, 50, 10, Red,  3, TRUE);
		// door for table 2
		gdispDrawThickLine(320-77, 0, 320-50, 10, Red,  3, TRUE);


	    // drawing
	    //new for multiplayer uart
		while (xQueueReceive(DrawQueue, &line, 0) == pdTRUE) {
			 }

	    if (variable_uart==0){   // senden
		    //for the master only the right flipper: with button B: the master can t change the state of the flipper 2
	    	  // go to other tables+score..
			if ((Ball->fPos[0]<160) &&(239-(Ball->fPos[1])<15)) {
				    slave_gototable=3;
					vTaskResume(xHandle_oneplayer3table);
					vTaskSuspend( NULL );
				}
			else if((Ball->fPos[0]>=160) &&(239-(Ball->fPos[1])<15)){
				    slave_gototable=2;
					vTaskResume(xHandle_oneplayer2table);
					vTaskSuspend( NULL );
				}
			if (((160<=Ball->fPos[0] && Ball->fPos<=170)&&(60<=239-(Ball->fPos[1]) &&239-(Ball->fPos[1])<=80))
						||((160-40<=Ball->fPos[0] && Ball->fPos[0]<=170-40)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))
						||((160+40<=Ball->fPos[0] && Ball->fPos[0]<=170+40)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))||((75<=Ball->fPos[0]&&239-(Ball->fPos[0])<=100)&&(50<=239-(Ball->fPos[1])&&239-(Ball->fPos[1])<=85))
						||((320-75<=Ball->fPos[0] && Ball->fPos[0]<=320-100)&&(50<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=85))||((75<=Ball->fPos[0]&& Ball->fPos[0]<=95)&&(175<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=195))
						||((320-75<=Ball->fPos[0] && Ball->fPos[0]<=320-85)&&(175<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=185))||((75<=Ball->fPos[0]&& 239-(Ball->fPos[0])<=93)&&(100<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=145))
						||((320-75<=Ball->fPos[0] && Ball->fPos[0]<=320-93)&&(100<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=145))||((80<=Ball->fPos[0] && Ball->fPos[0]<=90)&&(180<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=190)) ){
					counting_score++;
				}
			if (((160<=Ball->fPos[0] && Ball->fPos<=170)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))
						||((120<=Ball->fPos[0] && Ball->fPos[0]<=170)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80)) // war 130
						||((200<=Ball->fPos[0] && Ball->fPos[0]<=210)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))){
					for(f=0;f<40;f++){
					int ball_posx=Ball->fPos[0];
					int ball_posy=239-Ball->fPos[1];
					animation(f);   //,ball_posx,ball_posy);
					}
				}

		    if(239-Ball->fPos[1]>=230)
				{
					lives--;
					var_nureinmal = 0;
				}
			if (lives==0){
					global_resume_over=1; // game over
					vTaskResume(xHandle_resume);
					vTaskSuspend( NULL );
				}
			else if (((0<lives)&&(lives<=2))&&(var_nureinmal==0)){
				//iLastObject = 0;

				Ball->fPos[0] = 170;
				Ball->fPos[1] = 100;
				Ball->fVel[0] = 0;
				Ball->fVel[1] = 0;
				var_nureinmal++;
			}

				// go to other tables+score..
			if ((read_B==1)){
					//init_f1_y1=233;
				pFlipperR->Direction = down;
			  }
			  if (read_B==0){
				//init_f1_y1=233-30;
			    pFlipperR->Direction = up;
			   }

			  sprintf(str, "%d", counting_score);
			  gdispDrawString(7, 130, str, font1, White);
		      sprintf(str, "Lives:");
			  gdispDrawString(7, 160, str, font1, White);
			  sprintf(str, "%d", lives);
			  gdispDrawString(7, 170, str, font1, White);
			  sprintf(str, "Table 2");
			  gdispDrawString(7, 130, str, font1, White);
			  sprintf(str, "%d", level);  //level value
			  gdispDrawString(280, 20, str, font1, Yellow);
			  sprintf(str, "Press A to go back to menu");  // go back to menu
			  c1.x = read_B;  // save the x and y of the first point of the flipper 1 in c1
	          c1.y=0;
	          c3.x=(Ball->fPos[0])/100;
	          c3.y=240 - Ball->fPos[1];
	          int x_convert=(Ball->fPos[0]);
	          c4.x=x_convert % 100;
			  c4.y=0;
			  c5.x=slave_gototable;
			  c5.y=counting_score;
			  c6.x=lives;
			  c6.y=level;
	         // c3.x=Ball->fPos[0];
	         // c3.y=240 - Ball->fPos[1];

	    }
	    else{
	    	//for the slave only the link flipper: with button D

			if ((read_D==1)){
			  //init_f2_y3=233;
			  pFlipper->Direction = down;
			}
		    if (read_D==0){  // when pressing the button D, the second flipper will be oben:the slave can t change the state of the flipper 1
		      //init_f2_y3=233-30;
		     pFlipper->Direction = up; //links
             }
			c2.x= read_D; // save the x and y of the third point of the flipper 2 in c2
			c2.y=0;
	    }




	    sendLine(c1, c2,c3,c4,c5,c6);  // c3 for the x and y of the ball
	    if(variable_uart==0){ // variable==0=> master, will receive the  x and y of the other flipper, flipper 2 and just draw this flipper in the right position
	    	  gdispFillCircle( Ball->fPos[0], 240 - Ball->fPos[1], Ball->fRadius + fCOLLISION_DIST, Green);
	    	  gdispFillConvexPoly( 0, 0, getPoints(pFlipperR), 3, Red);
			  if(line.x_2==0){  // button D is pressed(slave)
				pFlipper->Direction = up; //links
				gdispFillConvexPoly( 0, 0, getPoints(pFlipper), 3, Red);
				gdispFillCircle( pFlipper->pfRotPoint[0], 239- pFlipper->pfRotPoint[1] + 5, 6, Black);
			  }
			  else if(line.x_2==1){
				pFlipper->Direction = down; //links
				gdispFillConvexPoly( 0, 0, getPoints(pFlipper), 3, Red);
				gdispFillCircle( pFlipper->pfRotPoint[0], 239- pFlipper->pfRotPoint[1] + 5, 6, Black);
			  }
	    }
	    else{// variable!=0=> slave, will receive the  x and y of the other flipper, flipper 1 and just draw this flipper in the right position
			/*  my_ball(line.x_3,line.y_3);
		      flipper1(line.x_1,line.y_1,init_f1_x2,init_f1_y2,init_f1_x3,init_f1_y3);
		      flipper2(init_f2_x1,init_f2_y1,init_f2_x2,init_f2_y2,init_f2_x3,init_f2_y3);
	   */
	    	  x_ball_received=line.x_3*100+line.x_4;
	    	  gdispFillCircle(x_ball_received,line.y_3 , Ball->fRadius + fCOLLISION_DIST, Green);
	    	  gdispFillConvexPoly( 0, 0, getPoints(pFlipper), 3, Red);
			  gdispFillCircle( pFlipper->pfRotPoint[0], 239- pFlipper->pfRotPoint[1] + 5, 6, Black);
	    	  if(line.x_1==0){  // button D is pressed(slave)
				pFlipperR->Direction = up; //links
				gdispFillConvexPoly( 0, 0, getPoints(pFlipperR), 3, Red);
				gdispFillCircle( pFlipperR->pfRotPoint[0], 239- pFlipperR->pfRotPoint[1] + 5, 6, Black);
			  }
			  else if(line.x_1==1){
				pFlipperR->Direction = down; //links
				gdispFillConvexPoly( 0, 0, getPoints(pFlipperR), 3, Red);
				gdispFillCircle( pFlipperR->pfRotPoint[0], 239- pFlipperR->pfRotPoint[1] + 5, 6, Black);
			  }
			  sprintf(str, "%d", line.y_5);
			  gdispDrawString(7, 130, str, font1, White);
			  sprintf(str, "%d", line.y_6);  //level value
			  gdispDrawString(7, 90, str, font1, Yellow);

		      sprintf(str, "Lives:");
			  gdispDrawString(7, 160, str, font1, White); //lives
			  sprintf(str, "%d", line.x_6);
			  gdispDrawString(7, 170, str, font1, White);
			  sprintf(str, "Table 2");
			  gdispDrawString(280, 20, str, font1, Yellow);
			  sprintf(str, "Press A to go back to menu");  // go back to menu
	    }

	    obstacle_middle();
		xSemaphoreTake(ESPL_DisplayReady, portMAX_DELAY);
		ESPL_DrawLayer();
		vTaskDelay(10);
		}
}

}
// 3 table
static draw_oneplayer_3table(){
TickType_t xLastWakeTime;
const TickType_t xFrequency = 20;
struct line line; // Init buffer for line
int j=0;
char str[100]; // Init buffer for message
int counting_score=0;
// for the obstacle2
font_t font1; // Load font for ugfx		// rectangle
font1 = gdispOpenFont("DejaVuSans10*");
int f=0;
int var_physics=0;
int lives=3;
level=1;
int var_nureinmal=0;
vTaskSuspend( NULL );
//xTaskCreate(resume, "resume", 2000, NULL, 4,&xHandle_resume);
//xTaskCreate(game_over, "game_over", 2000, NULL, 4,&xHandle_gameover);
if(menu_multiplayer==0){
while (TRUE) {
	if (var_physics==0){
		iLastObject = 0;
		create_function();
		int offset = 85, offsety = 0, os = 110;
		Ball = createCircle( 180, 100, 1);
		Ball->fVel[0] = 0;
		//Ball->fVel[1] = (-10) / dTIME_STEP;
		Ball->fVel[1] = 0;
		Ball->fMass = 1;
		pFlipper = createFlipper( 20 + offset, 40 , 60 +offset, 15 -offsety, 20+offset, 30 - offsety, left);
		pFlipperR = createFlipper( 60 +offset + os, 15 -offsety, 20 + offset + os, 40, 20+offset +os, 30 - offsety, right);

	    // after creating the background=> physics function
	}
	v_updateLoop();
	var_physics=1;
	var_joystick_ball=1;
	vTaskDelayUntil( &xLastWakeTime, xFrequency );
	// wait for buffer swap
	while (xQueueReceive(DrawQueue, &line, 0) == pdTRUE) {
	 }
	// draw to display
	gdispClear(Lime);
	read_A=GPIO_ReadInputDataBit(ESPL_Register_Button_A, ESPL_Pin_Button_A);
	read_B=GPIO_ReadInputDataBit(ESPL_Register_Button_B, ESPL_Pin_Button_B);
	read_D=GPIO_ReadInputDataBit(ESPL_Register_Button_D, ESPL_Pin_Button_D);
	read_C=GPIO_ReadInputDataBit(ESPL_Register_Button_C, ESPL_Pin_Button_C);
	read_E=GPIO_ReadInputDataBit(ESPL_Register_Button_E, ESPL_Pin_Button_E);
    if(read_E==0){
    	global_resume_over=0; // resume
    	table_resume_running=3;  // resume third table
    	vTaskResume(xHandle_resume);
    	vTaskSuspend(NULL);

    }

    // beide flipper funktionen: Initialisierung
    if ((read_B==1)&&(read_D==1)){

		pFlipper->Direction = down;
		pFlipperR->Direction = down;

	   // ball_moving();
	//beide flipper funktionen:Initialisierung
	}
	else if ((read_B==0)&&(read_D==1)){
		pFlipperR->Direction = up;
		pFlipper->Direction = down;
		//ball_moving();
	}
	else if ((read_B==1)&&(read_D==0)){ // Button D for flipper 2: links

		pFlipper->Direction = up;
		pFlipperR->Direction = down;

	}
	else if ((read_B==0)&&(read_D==0)){

		pFlipper->Direction = up;
		pFlipperR->Direction = up;
	}
	gdispFillCircle( Ball->fPos[0], 240 - Ball->fPos[1], Ball->fRadius + fCOLLISION_DIST, Green);
	gdispFillConvexPoly( 0, 0, getPoints(pFlipper), 3, Red);
	gdispFillConvexPoly( 0, 0, getPoints(pFlipperR), 3, Red);
	gdispFillCircle( pFlipperR->pfRotPoint[0], 239- pFlipperR->pfRotPoint[1] + 5, 6, Black);
	gdispFillCircle( pFlipper->pfRotPoint[0], 239- pFlipper->pfRotPoint[1] + 5, 6, Black);
    if ((Ball->fPos[0]<160) &&(239-(Ball->fPos[1])<10)) {
    	vTaskResume(xHandle_oneplayer2table);
    	vTaskSuspend( NULL );
    }
    else if((Ball->fPos[0]>160) &&(239-(Ball->fPos[1])<10)){

        vTaskResume(xHandle2);
    	vTaskSuspend( NULL );
    }
    // obstacles=> counter++
    if (((160<=Ball->fPos[0] && Ball->fPos<=170)&&(60<=239-(Ball->fPos[1]) &&239-(Ball->fPos[1])<=80))
    		||((160-40<=Ball->fPos[0] && Ball->fPos[0]<=170-40)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))
    		||((160+40<=Ball->fPos[0] && Ball->fPos[0]<=170+40)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))||((75<=Ball->fPos[0]&&239-(Ball->fPos[0])<=100)&&(50<=239-(Ball->fPos[1])&&239-(Ball->fPos[1])<=85))
			||((320-75<=Ball->fPos[0] && Ball->fPos[0]<=320-100)&&(50<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=85))||((75<=Ball->fPos[0]&& Ball->fPos[0]<=95)&&(175<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=195))
			||((320-75<=Ball->fPos[0] && Ball->fPos[0]<=320-85)&&(175<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=185))||((75<=Ball->fPos[0]&& 239-(Ball->fPos[0])<=93)&&(100<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=145))
			||((320-75<=Ball->fPos[0] && Ball->fPos[0]<=320-93)&&(100<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=145))||((80<=Ball->fPos[0] && Ball->fPos[0]<=90)&&(180<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=190)) ){
    	counting_score++;
    }
    // with some obstacles => animation
    if (((160<=Ball->fPos[0] && Ball->fPos<=170)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))
       		||((120<=Ball->fPos[0] && Ball->fPos[0]<=170)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80)) // war 130
       		||((200<=Ball->fPos[0] && Ball->fPos[0]<=210)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))){
		for(f=0;f<40;f++){
        int ball_posx=Ball->fPos[0];
        int ball_posy=239-Ball->fPos[1];   //239- => wegen den physics

		animation(f);   // animation function
		}
    }

    if(239-Ball->fPos[1]>=230)
    {
    	lives--;
		var_nureinmal = 0;
    }
    if (lives==0){
    	global_resume_over=1;  // game over
    	vTaskResume(xHandle_resume);
    	vTaskSuspend( NULL );
    }
	else if (((0<lives)&&(lives<=2))&&(var_nureinmal==0)){
		//iLastObject = 0;

		Ball->fPos[0] = 170;
		Ball->fPos[1] = 100;
		Ball->fVel[0] = 0;
		Ball->fVel[1] = 0;
		var_nureinmal++;
	}
    // for the background
    graph_oneplayer3table(x1_graph1,y1_graph1,x2_graph1, y2_graph1,x3_graph1,y3_graph1,
    		x4_graph1,y4_graph1,x5_graph1,y5_graph1,x6_graph1,y6_graph1, x7_graph1
    		, y7_graph1, x8_graph1, y8_graph1,x9_graph1,y9_graph1);

	//Obstacles zeichnen
	obstacles_function(x_obst_marr,y_obst_marr);
	// no path in table 2 or 3
	triangle_greenobstacle();
	//obstacle_middle();
	// circle beide Seiten
	gdispFillCircle(85, 185, 10, Blue);
	gdispFillCircle(85, 185, 5, White);
	gdispFillCircle(85, 185, 2, SkyBlue);
	gdispFillCircle(320-85, 185, 10, Olive);
	gdispFillCircle(320-85, 185, 5, White);
	gdispFillCircle(320-85, 185, 2, Green);
	// door for table 3
	gdispDrawThickLine(77, 0, 50, 10, Red,  3, TRUE);
    // door for table 2
	gdispDrawThickLine(320-77, 0, 320-50, 10, Red,  3, TRUE);
    if ((counting_score>1000)&&(counting_score<2000)){
    	level=2;
    }
    else if(counting_score>2000){
    	level=3;
    }
    else{
    	level=1;
    }
	sprintf(str, "Table 2");
	gdispDrawString(7, 20, str, font1, White);
	sprintf(str, "Level:");
	gdispDrawString(7, 80, str, font1, White);
	sprintf(str, "%d", level);
	gdispDrawString(7, 90, str, font1, White);
	sprintf(str, "Score:");
	gdispDrawString(7, 120, str, font1, White);
	sprintf(str, "%d", counting_score);
	gdispDrawString(7, 130, str, font1, White);
	sprintf(str, "Lives:");
	gdispDrawString(7, 160, str, font1, White);
	sprintf(str, "%d", lives);
	gdispDrawString(7, 170, str, font1, White);
	sprintf(str, "Table 1");
	gdispDrawString(280, 20, str, font1, White);

    sprintf(str, "Press A to go back to menu");  // go back to menu
    gdispDrawString(90, 220, str, font1, Red); // go back to menu

	if (read_C==0){
		animation(f);
	}
    f++;
    if(f>40){f=0;}
    if (read_A==0){
    	vTaskResume(xHandle1); // we can go to menu_view task from first_view task when pressing A
    	vTaskSuspend( NULL );
    }
    obstacle_middle();
	xSemaphoreTake(ESPL_DisplayReady, portMAX_DELAY);
	ESPL_DrawLayer();
	vTaskDelay(10);
	}
}
else if(menu_multiplayer==1){   //
	int variable_uart=1;  ///==0 => master else slave
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 20;
	struct line line; // Init buffer for line
	//struct line line2; // Init buffer for line

	char str[100]; // Init buffer for message
	int read_B;
	int read_D;

    int x_ball_received;
	// for the obstacle1
	font_t font1; // Load font for ugfx		// rectangle
	font1 = gdispOpenFont("DejaVuSans10*");
	int var=0;
	int choice=0;

	int first_shoot=1;
	// for the obstacle1

	int f=0;
	int var_physics=0;
	int counting_score=0;
	int lives=3;
	int slave_gototable=0;

	int var_nureinmal=0;
	struct coord c1={ 0, 0 },c2={ 0, 0 },c3={ 0, 0 },c4={ 0, 0 },c5={ 0, 0 },c6={ 0, 0 };  // c1,c2,c3,c4,c5.. are to send via uart, coord flipper1, flipper 2 and coord ball, score, level..
	vTaskSuspend( NULL );
	while (TRUE) {
		if (var_physics==0){
			create_function();
			int offset = 85, offsety = 0, os = 110;
			if (variable_uart==0){
			  Ball = createCircle( 40 +offset, 100, 1);
			  Ball->fVel[0] = 0;
			//Ball->fVel[1] = (-10) / dTIME_STEP;
			  Ball->fVel[1] = 0;
			  Ball->fMass = 1;
			}
			pFlipper = createFlipper( 20 + offset, 40 , 60 +offset, 15 -offsety, 20+offset, 30 - offsety, left);
			pFlipperR = createFlipper( 60 +offset + os, 15 -offsety, 20 + offset + os, 40, 20+offset +os, 30 - offsety, right);
		}
		//####
		v_updateLoop();
		var_physics=1;
		var_joystick_ball=1;
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
		// wait for buffer swap
		while (xQueueReceive(DrawQueue, &line, 0) == pdTRUE) {
		 }

		// draw to display
		gdispClear(Cyan);
		//xTaskCreate(draw_oneplayer, "draw_oneplayer", 1000, NULL, 4,&xHandle2);
	    // B and D buttons are used for playing
		read_B=GPIO_ReadInputDataBit(ESPL_Register_Button_B, ESPL_Pin_Button_B);
		read_D=GPIO_ReadInputDataBit(ESPL_Register_Button_D, ESPL_Pin_Button_D);
		read_E=GPIO_ReadInputDataBit(ESPL_Register_Button_E, ESPL_Pin_Button_E);
		read_A=GPIO_ReadInputDataBit(ESPL_Register_Button_A, ESPL_Pin_Button_A);
	    if(read_E==0){  // when pressing button E, stop playing and go to resume Task
	    	global_resume_over=0; // resume
	    	table_resume_running=1;  // resume first table
	    	vTaskResume(xHandle_resume);
	    	vTaskSuspend(NULL);

	    }
  // drawing
		// for the background
		graph_oneplayer2table(x1_graph1,y1_graph1,x2_graph1, y2_graph1,x3_graph1,y3_graph1,
				x4_graph1,y4_graph1,x5_graph1,y5_graph1,x6_graph1,y6_graph1, x7_graph1
				, y7_graph1, x8_graph1, y8_graph1,x9_graph1,y9_graph1);

	    // for the background
	    graph_oneplayer3table(x1_graph1,y1_graph1,x2_graph1, y2_graph1,x3_graph1,y3_graph1,
	    		x4_graph1,y4_graph1,x5_graph1,y5_graph1,x6_graph1,y6_graph1, x7_graph1
	    		, y7_graph1, x8_graph1, y8_graph1,x9_graph1,y9_graph1);

		//Obstacles zeichnen
		obstacles_function(x_obst_marr,y_obst_marr);
		// no path in table 2 or 3
		triangle_greenobstacle();
		obstacle_middle();
		// circle beide Seiten
		gdispFillCircle(85, 185, 10, Blue);
		gdispFillCircle(85, 185, 5, White);
		gdispFillCircle(85, 185, 2, SkyBlue);
		gdispFillCircle(320-85, 185, 10, Olive);
		gdispFillCircle(320-85, 185, 5, White);
		gdispFillCircle(320-85, 185, 2, Green);
		// door for table 3
		gdispDrawThickLine(77, 0, 50, 10, Red,  3, TRUE);
	    // door for table 2
		gdispDrawThickLine(320-77, 0, 320-50, 10, Red,  3, TRUE);

	    if ((counting_score>1000)&&(counting_score<2000)){
	    	level=2;
	    }
	    else if(counting_score>2000){
	    	level=3;
	    }
	    else{
	    	level=1;
	    }
		sprintf(str, "Table 3");
		gdispDrawString(7, 20, str, font1, Yellow);
		sprintf(str, "Level:");
		gdispDrawString(7, 80, str, font1, Yellow);
		sprintf(str, "%d", level);
		gdispDrawString(7, 90, str, font1, Yellow);
		sprintf(str, "Score:");
		gdispDrawString(7, 120, str, font1, Yellow);
		sprintf(str, "%d", counting_score);
		gdispDrawString(7, 130, str, font1, Yellow);
		sprintf(str, "Lives:");
		gdispDrawString(7, 160, str, font1, Yellow);
		sprintf(str, "%d", lives);
		gdispDrawString(7, 170, str, font1, Yellow);
		sprintf(str, "Table 1");
		gdispDrawString(280, 20, str, font1, Yellow);





	    // drawing
	    //new for multiplayer uart
		while (xQueueReceive(DrawQueue, &line, 0) == pdTRUE) {
			 }

	    if (variable_uart==0){   // for master => 0
		    //for the master only the right flipper: with button B: the master can t change the state of the flipper 2
	    	  // go to other tables+score..
			if ((Ball->fPos[0]<160) &&(239-(Ball->fPos[1])<15)) {
				    slave_gototable=3;
					vTaskResume(xHandle_oneplayer3table);
					vTaskSuspend( NULL );
				}
			else if((Ball->fPos[0]>=160) &&(239-(Ball->fPos[1])<15)){
				    slave_gototable=2;
					vTaskResume(xHandle_oneplayer2table);
					vTaskSuspend( NULL );
				}
			if (((160<=Ball->fPos[0] && Ball->fPos<=170)&&(60<=239-(Ball->fPos[1]) &&239-(Ball->fPos[1])<=80))
						||((160-40<=Ball->fPos[0] && Ball->fPos[0]<=170-40)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))
						||((160+40<=Ball->fPos[0] && Ball->fPos[0]<=170+40)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))||((75<=Ball->fPos[0]&&239-(Ball->fPos[0])<=100)&&(50<=239-(Ball->fPos[1])&&239-(Ball->fPos[1])<=85))
						||((320-75<=Ball->fPos[0] && Ball->fPos[0]<=320-100)&&(50<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=85))||((75<=Ball->fPos[0]&& Ball->fPos[0]<=95)&&(175<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=195))
						||((320-75<=Ball->fPos[0] && Ball->fPos[0]<=320-85)&&(175<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=185))||((75<=Ball->fPos[0]&& 239-(Ball->fPos[0])<=93)&&(100<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=145))
						||((320-75<=Ball->fPos[0] && Ball->fPos[0]<=320-93)&&(100<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=145))||((80<=Ball->fPos[0] && Ball->fPos[0]<=90)&&(180<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=190)) ){
					counting_score++;
				}
			if (((160<=Ball->fPos[0] && Ball->fPos<=170)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))
						||((120<=Ball->fPos[0] && Ball->fPos[0]<=170)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80)) // war 130
						||((200<=Ball->fPos[0] && Ball->fPos[0]<=210)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))){
					for(f=0;f<40;f++){
					int ball_posx=Ball->fPos[0];
					int ball_posy=239-Ball->fPos[1];
					animation(f);   //,ball_posx,ball_posy);
					}
				}

		    if(239-Ball->fPos[1]>=230)
				{
					lives--;
					var_nureinmal = 0;
				}
			if (lives==0){
					global_resume_over=1; // game over
					vTaskResume(xHandle_resume);
					vTaskSuspend( NULL );
				}
			else if (((0<lives)&&(lives<=2))&&(var_nureinmal==0)){
				//iLastObject = 0;

				Ball->fPos[0] = 170;
				Ball->fPos[1] = 100;
				Ball->fVel[0] = 0;
				Ball->fVel[1] = 0;
				var_nureinmal++;
			}

				// go to other tables+score..
			if ((read_B==1)){
					//init_f1_y1=233;
				pFlipperR->Direction = down;
			  }
			  if (read_B==0){
				//init_f1_y1=233-30;
			    pFlipperR->Direction = up;
			   }


		      sprintf(str, "Lives:");
			  gdispDrawString(7, 170, str, font1, White);
			  sprintf(str, "Table 2");
			  gdispDrawString(7, 130, str, font1, White);

			  sprintf(str, "Press A to go back to menu");  // go back to menu
			  c1.x = read_B;  // save the x and y of the first point of the flipper 1 in c1
	          c1.y=0;
	          c3.x=(Ball->fPos[0])/100;
	          c3.y=240 - Ball->fPos[1];
	          int x_convert=(Ball->fPos[0]);
	          c4.x=x_convert % 100;
			  c4.y=0;
			  c5.x=slave_gototable;
			  c5.y=counting_score;
			  c6.x=lives;
			  c6.y=level;
	         // c3.x=Ball->fPos[0];
	         // c3.y=240 - Ball->fPos[1];

	    }
	    else{  // for slave
	    	//for the slave only the link flipper: with button D

			if ((read_D==1)){
			  //init_f2_y3=233;
			  pFlipper->Direction = down;
			}
		    if (read_D==0){  // when pressing the button D, the second flipper will be oben:the slave can t change the state of the flipper 1
		      //init_f2_y3=233-30;
		     pFlipper->Direction = up; //links
             }
			c2.x= read_D; // save the x and y of the third point of the flipper 2 in c2
			c2.y=0;
	    }
	    sendLine(c1, c2,c3,c4,c5,c6);  // c3 for the x and y of the ball
	    if(variable_uart==0){ // variable==0=> master, will receive the  x and y of the other flipper, flipper 2 and just draw this flipper in the right position
	    	  gdispFillCircle( Ball->fPos[0], 240 - Ball->fPos[1], Ball->fRadius + fCOLLISION_DIST, Green);
	    	  gdispFillConvexPoly( 0, 0, getPoints(pFlipperR), 3, Red);
			  if(line.x_2==0){  // button D is pressed(slave)
				pFlipper->Direction = up; //links
				gdispFillConvexPoly( 0, 0, getPoints(pFlipper), 3, Red);
				gdispFillCircle( pFlipper->pfRotPoint[0], 239- pFlipper->pfRotPoint[1] + 5, 6, Black);
			  }
			  else if(line.x_2==1){
				pFlipper->Direction = down; //links
				gdispFillConvexPoly( 0, 0, getPoints(pFlipper), 3, Red);
				gdispFillCircle( pFlipper->pfRotPoint[0], 239- pFlipper->pfRotPoint[1] + 5, 6, Black);
			  }
	    }
	    else{// variable!=0=> slave, will receive the  x and y of the other flipper, flipper 1 and just draw this flipper in the right position
			/*  my_ball(line.x_3,line.y_3);
		      flipper1(line.x_1,line.y_1,init_f1_x2,init_f1_y2,init_f1_x3,init_f1_y3);
		      flipper2(init_f2_x1,init_f2_y1,init_f2_x2,init_f2_y2,init_f2_x3,init_f2_y3);
	   */
	    	  x_ball_received=line.x_3*100+line.x_4;
	    	  gdispFillCircle(x_ball_received,line.y_3 , Ball->fRadius + fCOLLISION_DIST, Green);
	    	  gdispFillConvexPoly( 0, 0, getPoints(pFlipper), 3, Red);
			  gdispFillCircle( pFlipper->pfRotPoint[0], 239- pFlipper->pfRotPoint[1] + 5, 6, Black);
	    	  if(line.x_1==0){  // button D is pressed(slave)
				pFlipperR->Direction = up; //links
				gdispFillConvexPoly( 0, 0, getPoints(pFlipperR), 3, Red);
				gdispFillCircle( pFlipperR->pfRotPoint[0], 239- pFlipperR->pfRotPoint[1] + 5, 6, Black);
			  }
			  else if(line.x_1==1){
				pFlipperR->Direction = down; //links
				gdispFillConvexPoly( 0, 0, getPoints(pFlipperR), 3, Red);
				gdispFillCircle( pFlipperR->pfRotPoint[0], 239- pFlipperR->pfRotPoint[1] + 5, 6, Black);
			  }
			  sprintf(str, "%d", line.y_5);
			  gdispDrawString(7, 130, str, font1, White);
			  sprintf(str, "%d", line.y_6);  //level value
			  gdispDrawString(7, 90, str, font1, Yellow);

		      sprintf(str, "Lives:");
			  gdispDrawString(7, 160, str, font1, White); //lives
			  sprintf(str, "%d", line.x_6);
			  gdispDrawString(7, 170, str, font1, White);
			  sprintf(str, "Table 2");
			  gdispDrawString(280, 20, str, font1, Yellow);
			  sprintf(str, "Press A to go back to menu");  // go back to menu
	    }

		xSemaphoreTake(ESPL_DisplayReady, portMAX_DELAY);
		ESPL_DrawLayer();
		vTaskDelay(10);
		}
}
}
static draw_oneplayer(){
TickType_t xLastWakeTime;
const TickType_t xFrequency = 20;
struct line line; // Init buffer for line

char str[100]; // Init buffer for message
int read_A=0;
int read_B;
int read_D;
int read_E;
/// intialisierungen for the ball
int x_ball=273;  //295;
int y_ball=215;   //190;
/// intialisierungen for the ball

// engine
int engine_x3=280;
int engine_y3=215;
int engine_x4=275;
int engine_y4=226;
font_t font1; // Load font for ugfx

font1 = gdispOpenFont("DejaVuSans10*");
int var=0;
int choice=0;
int first_shoot=1;
int f=0;
// for the obstacle1
int x1_obst=75;
int y1_obst=100;
int x2_obst=75;
int y2_obst=135;
int x3_obst=93;
int y3_obst=145;
int i;
int counting_score=0;
int lives=3;
int var_nureinmal=0;
// for the obstacle2
int var_physics=0;
vTaskSuspend( NULL );
//xTaskCreate(resume, "resume", 1000, NULL, 4,&xHandle_resume);
//xTaskCreate(game_over, "game_over", 2000, NULL, 5,&xHandle_gameover);
while (TRUE) {
	if (var_physics==0){
		iLastObject = 0;
		create_function();
		// after creating the background=> physics function
		int offset = 85, offsety = 0, os = 110;
		Ball = createCircle( 180, 100, 1);
		Ball->fVel[0] = 0;
		//Ball->fVel[1] = (-10) / dTIME_STEP;
		Ball->fVel[1] = 0;
		Ball->fMass = 1;
		pFlipper = createFlipper( 20 + offset, 40 , 60 +offset, 15 -offsety, 20+offset, 30 - offsety, left);
		pFlipperR = createFlipper( 60 +offset + os, 15 -offsety, 20 + offset + os, 40, 20+offset +os, 30 - offsety, right);
	}
	v_updateLoop();
	var_physics=1;
	var_joystick_ball=1;
	vTaskDelayUntil( &xLastWakeTime, xFrequency );
	// wait for buffer swap
	while (xQueueReceive(DrawQueue, &line, 0) == pdTRUE) {
	 }
	// draw to display
	gdispClear(Cyan);
	read_A=GPIO_ReadInputDataBit(ESPL_Register_Button_A, ESPL_Pin_Button_A);
	read_B=GPIO_ReadInputDataBit(ESPL_Register_Button_B, ESPL_Pin_Button_B);
	read_D=GPIO_ReadInputDataBit(ESPL_Register_Button_D, ESPL_Pin_Button_D);
	read_C=GPIO_ReadInputDataBit(ESPL_Register_Button_C, ESPL_Pin_Button_C);
	read_E=GPIO_ReadInputDataBit(ESPL_Register_Button_E, ESPL_Pin_Button_E);
	if(read_E==0){
		global_resume_over=0; // resume and not game over
		table_resume_running=2;  // resume second table
		vTaskResume(xHandle_resume);
		vTaskSuspend(NULL);

	}

	// beide flipper funktionen: Initialisierung
	// beide flipper funktionen: Initialisierung

	if ((read_B==1)&&(read_D==1)){

		pFlipper->Direction = down;
		pFlipperR->Direction = down;

	   // ball_moving();
	//beide flipper funktionen:Initialisierung
	}
	else if ((read_B==0)&&(read_D==1)){
		pFlipperR->Direction = up;
		pFlipper->Direction = down;
		//ball_moving();
	}
	else if ((read_B==1)&&(read_D==0)){ // Button D for flipper 2: links

		pFlipper->Direction = up;
		pFlipperR->Direction = down;

	}
	else if ((read_B==0)&&(read_D==0)){

		pFlipper->Direction = up;
		pFlipperR->Direction = up;
	}
	gdispFillCircle( Ball->fPos[0], 240 - Ball->fPos[1], Ball->fRadius + fCOLLISION_DIST, Green);
	gdispFillConvexPoly( 0, 0, getPoints(pFlipper), 3, Red);
	gdispFillConvexPoly( 0, 0, getPoints(pFlipperR), 3, Red);
	gdispFillCircle( pFlipper->pfRotPoint[0], 239- pFlipper->pfRotPoint[1] + 5, 6, Black);
	gdispFillCircle( pFlipperR->pfRotPoint[0], 239- pFlipperR->pfRotPoint[1] + 5, 6, Black);

    if ((Ball->fPos[0]<160) &&(239-(Ball->fPos[1])<15)) {
        vTaskResume(xHandle_oneplayer3table);
    	vTaskSuspend( NULL );
    }
    else if((Ball->fPos[0]>=160) &&(239-(Ball->fPos[1])<15)){
        vTaskResume(xHandle_oneplayer2table);
    	vTaskSuspend( NULL );
    }
    if (((160<=Ball->fPos[0] && Ball->fPos<=170)&&(60<=239-(Ball->fPos[1]) &&239-(Ball->fPos[1])<=80))
    		||((160-40<=Ball->fPos[0] && Ball->fPos[0]<=170-40)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))
    		||((160+40<=Ball->fPos[0] && Ball->fPos[0]<=170+40)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))||((75<=Ball->fPos[0]&&239-(Ball->fPos[0])<=100)&&(50<=239-(Ball->fPos[1])&&239-(Ball->fPos[1])<=85))
			||((320-75<=Ball->fPos[0] && Ball->fPos[0]<=320-100)&&(50<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=85))||((75<=Ball->fPos[0]&& Ball->fPos[0]<=95)&&(175<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=195))
			||((320-75<=Ball->fPos[0] && Ball->fPos[0]<=320-85)&&(175<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=185))||((75<=Ball->fPos[0]&& 239-(Ball->fPos[0])<=93)&&(100<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=145))
			||((320-75<=Ball->fPos[0] && Ball->fPos[0]<=320-93)&&(100<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=145))||((80<=Ball->fPos[0] && Ball->fPos[0]<=90)&&(180<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=190)) ){
    	counting_score++;
    }
    if (((160<=Ball->fPos[0] && Ball->fPos<=170)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))
       		||((120<=Ball->fPos[0] && Ball->fPos[0]<=170)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80)) // war 130
       		||((200<=Ball->fPos[0] && Ball->fPos[0]<=210)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))){
		for(f=0;f<40;f++){
        int ball_posx=Ball->fPos[0];
        int ball_posy=239-Ball->fPos[1];
		animation(f);   //,ball_posx,ball_posy);
		}
    }

    if(239-Ball->fPos[1]>=230)
    {
    	lives--;
		var_nureinmal = 0;
    }
    if (lives==0){
    	global_resume_over=1; // game over
    	vTaskResume(xHandle_resume);
    	vTaskSuspend( NULL );
    }
	else if (((0<lives)&&(lives<=2))&&(var_nureinmal==0)){
		//iLastObject = 0;

		Ball->fPos[0] = 170;
		Ball->fPos[1] = 100;
		Ball->fVel[0] = 0;
		Ball->fVel[1] = 0;
		var_nureinmal++;
	}
    //  background zeichnen : divided into many blocks in order to make it easier when doing physics
	graph_oneplayer1table(x1_graph1,y1_graph1,x2_graph1,y2_graph1,x3_graph1,y3_graph1,
			 x4_graph1, y4_graph1, x5_graph1, y5_graph1, x6_graph1,y6_graph1,x7_graph1
			,y7_graph1, x8_graph1, y8_graph1,x9_graph1,y9_graph1);

	obstacles_function(x_obst_marr,y_obst_marr);
	//path_fortheball();
	triangle_greenobstacle();

	circle_obstacle_tab1();


    // door for table 3
	gdispDrawThickLine(77, 0, 50, 10, Red,  3, TRUE);
    // door for table 2
	gdispDrawThickLine(320-77, 0, 320-50, 10, Red,  3, TRUE);
//##
    if ((counting_score>1000)&&(counting_score<2000)){
    	level=2;
    }
    else if(counting_score>2000){
    	level=3;
    }
    else{
    	level=1;
    }
	sprintf(str, "Table 3");
	gdispDrawString(7, 20, str, font1, Yellow);
	sprintf(str, "Level:");
	gdispDrawString(7, 80, str, font1, Yellow);
	sprintf(str, "%d", level);
	gdispDrawString(7, 90, str, font1, Yellow);
	sprintf(str, "Score:");
	gdispDrawString(7, 120, str, font1, Yellow);
	sprintf(str, "%d", counting_score);
	gdispDrawString(7, 130, str, font1, Yellow);
	sprintf(str, "Lives:");
	gdispDrawString(7, 160, str, font1, Yellow);
	sprintf(str, "%d", lives);
	gdispDrawString(7, 170, str, font1, Yellow);
	sprintf(str, "Table 2");
	gdispDrawString(280, 20, str, font1, Yellow);
	//###
    sprintf(str, "Press A to go back to menu");  // go back to menu
    gdispDrawString(90, 220, str, font1, Black); // go back to menu
	my_ball(x_ball,y_ball);
    // engine
	struct point pntsengine[] = {{290,240},{303,240},{engine_x3,engine_y3},{engine_x4,engine_y4}};
	gdispDrawPoly(0, 0, pntsengine, 4, Red);
	gdispFillConvexPoly(0, 0, pntsengine, 4, Red);
	// engine

    choice=(choice+joystick_state())%3;
    if ((choice==1)&&(var<10)&&(first_shoot==1)){
    	var++;
        engine_x3+=2;
        engine_y3+=2;
        engine_x4+=2;
        engine_y4+=2;

      }
    if ((var==10)){
    	y_ball-=2;
    	x_ball-=2;

    }
    if ((x_ball==150)){
    	for(f=0;f<40;f++){
    	animation(f);
    	//for(i=0;i<50;i++){}
    	}
     }
    if (read_A==0){
    	vTaskResume(xHandle1); // we can go to menu_view task from first_view task when pressing A
    	vTaskSuspend( NULL );
    }
    obstacle_middle();

	xSemaphoreTake(ESPL_DisplayReady, portMAX_DELAY);
	ESPL_DrawLayer();
	vTaskDelay(10);
	}
}

// triangle menu function is used to draw the triangle in the menu View
int triangle_menux1;
int triangle_menuy1;
int triangle_menux2;
int triangle_menuy2;
int triangle_menux3;
int triangle_menuy3;
int rectangle_menux;
int rectangle_menuy;
// triangle menu function is used to draw the triangle in the menu View
void triangle_menu(){
struct point pnts[] = {{triangle_menux1,triangle_menuy1},{triangle_menux2, triangle_menuy2}, {triangle_menux3, triangle_menuy3}};
gdispDrawPoly(1, 1, pnts, 3, Red);
gdispFillConvexPoly(1, 1, pnts, 3, Red);
}

static high_scores(){
TickType_t xLastWakeTime;
const TickType_t xFrequency = 20;
// i used hier 2 different fonts because the Sans 24* is too big
font_t font1 =  gdispOpenFont("DejaVuSans10*");; // Load font
font_t font2 =  gdispOpenFont("DejaVuSans24*");; // Load font
struct line line; // Init buffer for line
char str[100]; // Init buffer for message
int read_E;
vTaskSuspend( NULL );
while(TRUE){
  vTaskDelayUntil( &xLastWakeTime, xFrequency );
  while (xQueueReceive(DrawQueue, &line, 0) == pdTRUE) {
	 }
  gdispClear(Green);
  sprintf(str, "Best Scores Pinball");  // Title
  gdispDrawString(60, 10, str, font2, Red);
  sprintf(str, "One player:");
  gdispDrawString(30, 50, str, font2, Blue);
  sprintf(str, "first best score:");
  gdispDrawString(50, 80, str, font1, Black);
  sprintf(str, "second best score:");
  gdispDrawString(50, 100, str, font1, Black);
  sprintf(str, "Multi-players:");
  gdispDrawString(30, 140, str, font2, Blue);
  sprintf(str, "first best score:");
  gdispDrawString(50, 170, str, font1, Black);
  sprintf(str, "second best score:");

  gdispDrawString(50, 190, str, font1, Black);
  read_E=GPIO_ReadInputDataBit(ESPL_Register_Button_E, ESPL_Pin_Button_E);
  if (read_E==0){
	   vTaskResume( xHandle1 );
	   vTaskSuspend(NULL);
  }
  xSemaphoreTake(ESPL_DisplayReady, portMAX_DELAY);
  ESPL_DrawLayer();
  vTaskDelay(50);
}
}
/**
 * Example function to send data over UART
 */
void sendLine(struct coord coord_1, struct coord coord_2,struct coord coord_3,struct coord coord_4,struct coord coord_5,struct coord coord_6) { // not to forget to make an extension in the header file too
	char checksum;
	// Generate simple error detection checksum
	checksum = coord_1.x ^ coord_1.y ^ coord_2.x ^ coord_2.y
		^ coord_3.x ^ coord_3.y^ coord_4.x ^ coord_4.y^ coord_5.x ^ coord_5.y^ coord_6.x ^ coord_6.y; // extension
	UART_SendData((uint8_t) ESPL_StartByte);
	UART_SendData((uint8_t) coord_1.x); // for flipper 1: x of the first point
	UART_SendData((uint8_t) coord_1.y);// for flipper 1: y of the first point
	UART_SendData((uint8_t) coord_2.x);// for flipper 2: x of the third point
	UART_SendData((uint8_t) coord_2.y);// for flipper 2: y of the third point
	UART_SendData((uint8_t) coord_3.x); // extension: for ball , x of the ball
	UART_SendData((uint8_t) coord_3.y);// extension: for ball , y of the ball
	UART_SendData((uint8_t) coord_4.x); // extension: for ball , x of the ball
	UART_SendData((uint8_t) coord_4.y);// extension: for ball , y of the ball
	UART_SendData((uint8_t) coord_5.x); // extension: for ball , x of the ball
	UART_SendData((uint8_t) coord_5.y);// extension: for ball , y of the ball
	UART_SendData((uint8_t) coord_6.x); // extension: for ball , x of the ball
	UART_SendData((uint8_t) coord_6.y);// extension: for ball , y of the ball
	UART_SendData((uint8_t) checksum);
	UART_SendData((uint8_t) ESPL_StopByte);
}

/**
 * Task which receives data via UART and decodes it.
 */
static void uartReceive() {
	char input;
	uint8_t pos = 0;
	char checksum;
	char buffer[15]; //   Start byte,6* line byte, checksum (all xor), End byte
	struct line line; // ich habe die send line um 2 erweitert

	while (TRUE) {
		// wasendLineit for data in queue
		xQueueReceive(ESPL_RxQueue, &input, portMAX_DELAY);
		// decode package by buffer position
		switch (pos) {
			// start byte
			case 0:
				if (input == ESPL_StartByte) {
					buffer[0] = input;
					pos = 1;
				}
				break;
			// line bytes
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			// Check sum
			case 13:
				buffer[pos] = input;
				pos++;
				break;
			// Last byte should be stop byte
			case 14:
				if (input == ESPL_StopByte) {
					// Check if checksum is accurate
					checksum = buffer[1] ^ buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5] ^ buffer[6]
								^ buffer[7]^ buffer[8]^ buffer[9]^ buffer[10]^ buffer[11]^ buffer[12];
					// Decode packet to line struct
					if (buffer[13] == checksum) {// extension
						line.x_1 = buffer[1];  // extension of the buffer for one more struct coord x_3 and y_3 :
						line.y_1 = buffer[2];
						line.x_2 = buffer[3];
						line.y_2 = buffer[4];
						line.x_3 = buffer[5];
						line.y_3 = buffer[6];
						line.x_4 = buffer[7];
						line.y_4 = buffer[8];

						line.x_5 = buffer[9];
						line.y_5 = buffer[10];
						line.x_6 = buffer[11];
						line.y_6 = buffer[12];
						xQueueSend(DrawQueue, &line, 100);
						pos = 0;
					} else {
						// Reset buffer
						pos = 0;
					}
				} else {
					pos = 0;
				}
		}
	}
}
static multi_player(){
	int variable_uart=0;  ///==0 dann senden else receiving
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 20;
	struct line line; // Init buffer for line
	//struct line line2; // Init buffer for line

	char str[100]; // Init buffer for message
	int read_B;
	int read_D;
	/// intialization for the ball
	int x_ball=255;
	int y_ball= 196;
	/// intialization for the ball

    int x_ball_received;
	// for the obstacle1

	// for ball shooting: engine: initialization of the red engine
	int engine_x3=263;
	int engine_y3=194;
	int engine_x4=258;
	int engine_y4=205;

	font_t font1; // Load font for ugfx		// rectangle
	font1 = gdispOpenFont("DejaVuSans10*");
	int var=0;
	int choice=0;

	int first_shoot=1;
	// for the obstacle1
	int x1_obst=75;
	int y1_obst=100;
	int x2_obst=75;
	int y2_obst=135;
	int x3_obst=93;
	int y3_obst=145;
	int f=0;
	int var_physics=0;
	int counting_score=0;
	int lives=3;
	int slave_gototable;
	level=1;
	int var_nureinmal=0;
	struct coord c1={ 0, 0 },c2={ 0, 0 },c3={ 0, 0 },c4={ 0, 0 },c5={ 0, 0 },c6={ 0, 0 };  // c1,c2,c3 are to send via uart, coord flipper1, flipper 2 and coord ball
	vTaskSuspend( NULL );
	while (TRUE) {
		if (var_physics==0){
			iLastObject = 0;
			create_function();
			// after creating the background=> physics function
			int offset = 85, offsety = 0, os = 110;
			Ball = createCircle( 180, 100, 1);
			Ball->fVel[0] = 0;
			//Ball->fVel[1] = (-10) / dTIME_STEP;
			Ball->fVel[1] = 0;
			Ball->fMass = 1;
			pFlipper = createFlipper( 20 + offset, 40 , 60 +offset, 15 -offsety, 20+offset, 30 - offsety, left);
			pFlipperR = createFlipper( 60 +offset + os, 15 -offsety, 20 + offset + os, 40, 20+offset +os, 30 - offsety, right);
		}
		v_updateLoop();
		var_physics=1;
		var_joystick_ball=1;
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
		// wait for buffer swap

		while (xQueueReceive(DrawQueue, &line, 0) == pdTRUE) {
		 }

		// draw to display
		gdispClear(Cyan);
		//xTaskCreate(draw_oneplayer, "draw_oneplayer", 1000, NULL, 4,&xHandle2);
	    // B and D buttons are used for playing
		read_B=GPIO_ReadInputDataBit(ESPL_Register_Button_B, ESPL_Pin_Button_B);
		read_D=GPIO_ReadInputDataBit(ESPL_Register_Button_D, ESPL_Pin_Button_D);
		read_E=GPIO_ReadInputDataBit(ESPL_Register_Button_E, ESPL_Pin_Button_E);
		read_A=GPIO_ReadInputDataBit(ESPL_Register_Button_A, ESPL_Pin_Button_A);
	    if(read_E==0){  // when pressing button E, stop playing and go to resume Task
	    	global_resume_over=0; // resume
	    	table_resume_running=1;  // resume first table
	    	vTaskResume(xHandle_resume);
	    	vTaskSuspend(NULL);

	    }
  // drawing
		graph_oneplayer1table(x1_graph1,y1_graph1,x2_graph1,y2_graph1,x3_graph1,y3_graph1,
				 x4_graph1, y4_graph1, x5_graph1, y5_graph1, x6_graph1,y6_graph1,x7_graph1
				,y7_graph1, x8_graph1, y8_graph1,x9_graph1,y9_graph1);

		obstacles_function(x_obst_marr,y_obst_marr);
		//path_fortheball();
		triangle_greenobstacle();
		//obstacle_middle();
		circle_obstacle_tab1();
	    sprintf(str, "Table 3");
	    gdispDrawString(7, 20, str, font1, Yellow);
	    sprintf(str, "Level:");
	    gdispDrawString(7, 80, str, font1, Yellow);
	    sprintf(str, "Score:");
	    gdispDrawString(7, 120, str, font1, Yellow);



	    // drawing
	    //new for multiplayer uart
		//while (xQueueReceive(DrawQueue, &line, 0) == pdTRUE) {
			// }

	    if (variable_uart==0){   // master
		    //for the master only the right flipper: with button B: the master can t change the state of the flipper 2
		    if ((Ball->fPos[0]<160) &&(239-(Ball->fPos[1])<10)) {
		    	vTaskResume(xHandle_oneplayer2table);
		    	vTaskSuspend( NULL );
		    }
		    else if((Ball->fPos[0]>160) &&(239-(Ball->fPos[1])<10)){

		        vTaskResume(xHandle_oneplayer3table);
		    	vTaskSuspend( NULL );
		    }
		    // go to other tables
			if (((160<=Ball->fPos[0] && Ball->fPos[0]<=170)&&(60<=239-(Ball->fPos[1]) &&239-(Ball->fPos[1])<=80))
						||((160-40<=Ball->fPos[0] && Ball->fPos[0]<=170-40)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))
						||((160+40<=Ball->fPos[0] && Ball->fPos[0]<=170+40)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))||((75<=Ball->fPos[0]&&239-(Ball->fPos[0])<=100)&&(50<=239-(Ball->fPos[1])&&239-(Ball->fPos[1])<=85))
						||((320-75<=Ball->fPos[0] && Ball->fPos[0]<=320-100)&&(50<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=85))||((75<=Ball->fPos[0]&& Ball->fPos[0]<=95)&&(175<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=195))
						||((320-75<=Ball->fPos[0] && Ball->fPos[0]<=320-85)&&(175<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=185))||((75<=Ball->fPos[0]&& 239-(Ball->fPos[0])<=93)&&(100<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=145))
						||((320-75<=Ball->fPos[0] && Ball->fPos[0]<=320-93)&&(100<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=145))||((80<=Ball->fPos[0] && Ball->fPos[0]<=90)&&(180<=239-(Ball->fPos[1])&& 239-(Ball->fPos[1])<=190)) ){
					counting_score++;
				}
			if (((160<=Ball->fPos[0] && Ball->fPos<=170)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))
						||((120<=Ball->fPos[0] && Ball->fPos[0]<=170)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80)) // war 130
						||((200<=Ball->fPos[0] && Ball->fPos[0]<=210)&&(60<=239-(Ball->fPos[1]) && 239-(Ball->fPos[1])<=80))){
					for(f=0;f<40;f++){
					int ball_posx=Ball->fPos[0];
					int ball_posy=239-Ball->fPos[1];
					animation(f);   //,ball_posx,ball_posy);
					}
				}

		    if(239-Ball->fPos[1]>=230)
				{
					lives--;
					var_nureinmal = 0;
				}
			if (lives==0){
					global_resume_over=1; // game over
					vTaskResume(xHandle_resume);
					vTaskSuspend( NULL );
				}
			else if (((0<lives)&&(lives<=2))&&(var_nureinmal==0)){
				//iLastObject = 0;

				Ball->fPos[0] = 170;
				Ball->fPos[1] = 100;
				Ball->fVel[0] = 0;
				Ball->fVel[1] = 0;
				var_nureinmal++;
			}

				// go to other tables+score..
			if ((read_B==1)){
					//init_f1_y1=233;
				pFlipperR->Direction = down;
			  }
			  if (read_B==0){
				//init_f1_y1=233-30;
			    pFlipperR->Direction = up;
			   }

			  sprintf(str, "%d", counting_score);
			  gdispDrawString(7, 130, str, font1, White);
		      sprintf(str, "Lives:");
			  gdispDrawString(7, 160, str, font1, White);
			  sprintf(str, "%d", lives);
			  gdispDrawString(7, 170, str, font1, White);
			  sprintf(str, "Table 2");
			  gdispDrawString(7, 130, str, font1, White);
			  sprintf(str, "%d", level);  //level value
			  gdispDrawString(280, 20, str, font1, Yellow);
			  sprintf(str, "Press A to go back to menu");  // go back to menu
			  c1.x = read_B;  // save the x and y of the first point of the flipper 1 in c1
	          c1.y=0;
	          c3.x=(Ball->fPos[0])/100;
	          c3.y=240 - Ball->fPos[1];
	          int x_convert=(Ball->fPos[0]);
	          c4.x=x_convert % 100;
			  c4.y=0;
			  c5.x=slave_gototable;
			  c5.y=counting_score;
			  c6.x=lives;
			  c6.y=level;

	    }
	    else{  //slave
	    	//for the slave only the link flipper: with button D

			if ((read_D==1)){
			  //init_f2_y3=233;
			  pFlipper->Direction = down;
			}
		    if (read_D==0){  // when pressing the button D, the second flipper will be oben:the slave can t change the state of the flipper 1
		      //init_f2_y3=233-30;
		     pFlipper->Direction = up; //links
             }
			c2.x= read_D; // save the x and y of the third point of the flipper 2 in c2
			c2.y=0;
	    }
	    sendLine(c1, c2,c3,c4,c5,c6);  // c3 for the x and y of the ball
		while (xQueueReceive(DrawQueue, &line, 0) == pdTRUE) {
			 }
	    if(variable_uart==0){ // variable==0=> master, will receive the  x and y of the other flipper, flipper 2 and just draw this flipper in the right position
	    	  gdispFillCircle( Ball->fPos[0], 240 - Ball->fPos[1], Ball->fRadius + fCOLLISION_DIST, Green);
	    	  gdispFillConvexPoly( 0, 0, getPoints(pFlipperR), 3, Red);
	    	  gdispFillCircle( pFlipperR->pfRotPoint[0], 239- pFlipperR->pfRotPoint[1] + 5, 6, Black);
			  if(line.x_2==0){  // button D is pressed(slave)
				pFlipper->Direction = up; //links
				gdispFillConvexPoly( 0, 0, getPoints(pFlipper), 3, Red);
				gdispFillCircle( pFlipper->pfRotPoint[0], 239- pFlipper->pfRotPoint[1] + 5, 6, Black);
			  }
			  else if(line.x_2==1){
				pFlipper->Direction = down; //links
				gdispFillConvexPoly( 0, 0, getPoints(pFlipper), 3, Red);
				gdispFillCircle( pFlipper->pfRotPoint[0], 239- pFlipper->pfRotPoint[1] + 5, 6, Black);
			  }
	    }
	    else{// variable!=0=> slave, will receive the  x and y of the other flipper, flipper 1 and just draw this flipper in the right position
	    	  x_ball_received=line.x_3*100+line.x_4;
	    	  gdispFillCircle(x_ball_received,line.y_3 , Ball->fRadius + fCOLLISION_DIST, Green);
	    	  gdispFillConvexPoly( 0, 0, getPoints(pFlipper), 3, Red);
			  gdispFillCircle( pFlipper->pfRotPoint[0], 239- pFlipper->pfRotPoint[1] + 5, 6, Black);
			  if(line.x_1==0){  // button D is pressed(slave)
				pFlipperR->Direction = up; //links
				gdispFillConvexPoly( 0, 0, getPoints(pFlipperR), 3, Red);
				gdispFillCircle( pFlipperR->pfRotPoint[0], 239- pFlipperR->pfRotPoint[1] + 5, 6, Black);
			  }
			  else if(line.x_1==1){
				pFlipperR->Direction = down; //links
				gdispFillConvexPoly( 0, 0, getPoints(pFlipperR), 3, Red);
				gdispFillCircle( pFlipperR->pfRotPoint[0], 239- pFlipperR->pfRotPoint[1] + 5, 6, Black);
			  }
			    if ((x_ball_received<160) &&(line.y_3<10)) {
			    	vTaskResume(xHandle_oneplayer2table);
			    	vTaskSuspend( NULL );
			    }
			    else if((x_ball_received>160) &&(line.y_3<10)){

			        vTaskResume(xHandle_oneplayer3table);
			    	vTaskSuspend( NULL );
			    }
				if (((160<=x_ball_received && x_ball_received<=170)&&(60<=line.y_3&& line.y_3<=80))
							||((120<=x_ball_received && x_ball_received<=170)&&(60<=line.y_3 && line.y_3<=80)) // war 130
							||((200<=x_ball_received && x_ball_received<=210)&&(60<=line.y_3 && line.y_3<=80))){
						for(f=0;f<40;f++){
						int ball_posx=Ball->fPos[0];
						int ball_posy=239-Ball->fPos[1];
						animation(f);   //,ball_posx,ball_posy);
						}
					}
			  sprintf(str, "%d", line.y_5);
			  gdispDrawString(7, 130, str, font1, White);
			  sprintf(str, "%d", line.y_6);  //level value
			  gdispDrawString(7, 90, str, font1, Yellow);

		      sprintf(str, "Lives:");
			  gdispDrawString(7, 160, str, font1, White); //lives
			  sprintf(str, "%d", line.x_6);
			  gdispDrawString(7, 170, str, font1, White);
			  sprintf(str, "Table 2");
			  gdispDrawString(280, 20, str, font1, Yellow);
			  sprintf(str, "Press A to go back to menu");  // go back to menu
	    }
	    obstacle_middle();

		xSemaphoreTake(ESPL_DisplayReady, portMAX_DELAY);
		ESPL_DrawLayer();
		vTaskDelay(10);
		}
}
void menu_view(){
TickType_t xLastWakeTime;
const TickType_t xFrequency = 20;
int read_K,Read_D,read_C;
int choice=0;
struct line line; // Init buffer for line
int int_balloon=-50;
char str[100];
font_t font1 =  gdispOpenFont("DejaVuSans24*");;
vTaskSuspend( NULL );
xTaskCreate(high_scores, "high_scores", 2000, NULL, 4,&xHandle5);
xTaskCreate(draw_oneplayer, "draw_oneplayer", 2000, NULL, 4,&xHandle2);
xTaskCreate(multi_player, "multi_player", 1000, NULL, 4,&xHandle_multiplayer);
xTaskCreate(draw_oneplayer_2table, "draw_oneplayer_2table", 1000, NULL, 4,&xHandle_oneplayer2table);
xTaskCreate(draw_oneplayer_3table, "draw_oneplayer_3table", 1000, NULL, 4,&xHandle_oneplayer3table);


// triangle for choosing the
triangle_menux1=50;
triangle_menuy1=90;
triangle_menux2=50;
triangle_menuy2=70;
triangle_menux3=70;
triangle_menuy3=80;
rectangle_menux=87;
rectangle_menuy=75;

while (TRUE) {
	vTaskDelayUntil( &xLastWakeTime, xFrequency );
	// wait for buffer swap
	while (xQueueReceive(DrawQueue, &line, 0) == pdTRUE) {
	 }
	gdispClear(Fuchsia);
    balloon(int_balloon);
	sprintf(str, "Single player");  // Menu table
	gdispDrawString(90, 80, str, font1, Black);//gdispDrawString(110, 80, str, font1, Black);
	sprintf(str, "Multiple players");
	gdispDrawString(90, 120, str, font1, Black);
	sprintf(str, "High Scores");
	gdispDrawString(90, 160, str, font1, Black);

	gdispDrawBox(rectangle_menux, rectangle_menuy, 190,30, Red);
	triangle_menu();
	read_K=GPIO_ReadInputDataBit(ESPL_Register_Button_K, ESPL_Pin_Button_K);
	Read_D=GPIO_ReadInputDataBit(ESPL_Register_Button_D, ESPL_Pin_Button_D);
	read_C=GPIO_ReadInputDataBit(ESPL_Register_Button_C, ESPL_Pin_Button_C);
	if ((read_K==0)&&(rectangle_menuy==75)){
       menu_multiplayer=0;
	   vTaskResume( xHandle2 );
	   vTaskSuspend(NULL);
	}
    if ((read_K==0)&&(rectangle_menuy==155)){
      menu_multiplayer=0;
	  vTaskResume( xHandle5 );  //5
	  vTaskSuspend(NULL);
	}
    if((read_K==0)&&(rectangle_menuy==75+40)){
      menu_multiplayer=1;
      vTaskResume(xHandle_multiplayer);
	  vTaskSuspend(NULL);
    }
    if(Read_D==0){
      vTaskResume(xHandle_oneplayer2table);
      vTaskSuspend(NULL);
    }
    if(read_C==0){
    	vTaskResume(xHandle_oneplayer3table);
    	vTaskSuspend(NULL);
    }
    choice=(choice+joystick_state())%3;
    if (choice==0){
	    rectangle_menuy=75;
	    triangle_menuy1=90;
	    triangle_menuy2=70;
	    triangle_menuy3=80;
    }
    else if(choice==1){
	    rectangle_menuy=75+40;
	    triangle_menuy1=90+40;
	    triangle_menuy2=70+40;
	    triangle_menuy3=80+40;
    }
    else{
	    rectangle_menuy=75+80;
	    triangle_menuy1=90+80;
	    triangle_menuy2=70+80;
	    triangle_menuy3=80+80;
    }
    int_balloon+=10;
    if(int_balloon>=220){int_balloon=-50;}
	xSemaphoreTake(ESPL_DisplayReady, portMAX_DELAY);
	ESPL_DrawLayer();
	vTaskDelay(200);
	}
}
static void first_view(){
int read_A;
int beauty=0;
TickType_t xLastWakeTime;
const TickType_t xFrequency = 20;
struct line line; // Init buffer for line
int j=0;
double k=0;
char str[100]; // Init buffer for message
font_t font1; // Load font for ugfx
font_t font2; // Load font for ugfx
font1 = gdispOpenFont("DejaVuSans24*");
font2 = gdispOpenFont("DejaVuSans10*");
xTaskCreate(menu_view, "menu_view", 1000, NULL, 2,&xHandle1); // we can go to menu_view task from first_view task when pressing A
while (TRUE) {
	vTaskDelayUntil( &xLastWakeTime, xFrequency );
	while (xQueueReceive(DrawQueue, &line, 0) == pdTRUE) {
	 }
	// draw to display
	gdispClear(Cyan);
	// contour : red and maroon: make the view more beautiful
	for(j=0;j<310;j++){ // for the x axis
	if ((j % 2==0)&&(beauty%3!=0)){  // in order to change the colors and make the rectangles blink
	  gdispDrawBox(0+10*j,0, 10, 10, Maroon);
	  gdispFillArea(0+10*j, 0, 10, 10, Maroon);
	}
	else{
	  gdispDrawBox(0+10*j,0, 10, 10,Red);
	  gdispFillArea(0+10*j, 0, 10, 10, Red);
	}
	}

	for(j=0;j<310;j++){   // for the x axis
	if ((j % 2==0)&&(beauty%3!=0)){// in order to change the colors and make the rectangles blink
	  gdispDrawBox(0+10*j,230, 10, 10, Maroon);
	  gdispFillArea(0+10*j, 230, 10, 10, Maroon);
	}
	else{
	  gdispDrawBox(0+10*j,230, 10, 10,Red);
	  gdispFillArea(0+10*j, 230, 10, 10, Red);
	}
	}

	for(j=0;j<230;j++){ // for the y axis
	if ((j % 2==0)&&(beauty%3==0)){
	  gdispDrawBox(0,10*j, 10, 10, Red);
	  gdispFillArea(0, 10*j, 10, 10, Red);
	}
	else{
	  gdispDrawBox(0,10*j, 10, 10,Maroon);
	  gdispFillArea(0, 10*j, 10, 10, Maroon);
	}
	}
	for(j=0;j<230;j++){  // for the y axis,
	if ((j % 2==0)&&(beauty%3==0)){
	  gdispDrawBox(310,10*j, 10, 10, Red);
	  gdispFillArea(310, 10*j, 10, 10, Red);
	}
	else{
	  gdispDrawBox(310,10*j, 10, 10,Maroon);
	  gdispFillArea(310, 10 * j, 10, 10, Maroon);
	}
	}

	sprintf(str, "PINBALL GAME");
	gdispDrawString(50, 80, str, font1, Red);
	gdispFillCircle(130+100*cos(k),100+100*sin(k),20, Silver); //    circle moving
	read_A=GPIO_ReadInputDataBit(ESPL_Register_Button_A, ESPL_Pin_Button_A);
     // text at the bottom
	sprintf(str, "realised by: Philipp");
	gdispDrawString(10, 180, str, font1, Black);
	sprintf(str, "Nassua, Hajer Chebil ");
	gdispDrawString(10, 200, str, font1, Black);
    sprintf(str, "Press A to go to menu");  // go back to menu
    gdispDrawString(150, 160, str, font2, Black); // go back to menu
    k+=0.1;
    beauty++;
     // in order to go to menu_view task, press button A
	if (read_A==0){
	   vTaskResume( xHandle1 );
	   vTaskSuspend(NULL);
	}
	xSemaphoreTake(ESPL_DisplayReady, portMAX_DELAY);
	ESPL_DrawLayer();
	vTaskDelay(50);
	}
}

int main() {
	// Initialize Board functions
	ESPL_SystemInit(); // system_init
	ButtonASemaphore = xSemaphoreCreateBinary();
	ButtonCSemaphore = xSemaphoreCreateBinary();
	ButtonDSemaphore = xSemaphoreCreateBinary();
	ButtonESemaphore = xSemaphoreCreateBinary();
	ButtonKSemaphore = xSemaphoreCreateBinary();
	DrawQueue = xQueueCreate(100, 12 * sizeof(char));  // extension of the buffer: 6
	xTaskCreate(uartReceive, "queueReceive", 1000, NULL, 2, NULL);
	xTaskCreate(first_view, "first_view", 1000, NULL, 1,NULL);
	xTaskCreate(resume, "resume", 2000, NULL, 4,&xHandle_resume);
	xTaskCreate(menu_view, "menu_view", 1000, NULL, 4,&xHandle1); // we can go to menu_view task from first_view task when pressing A
	//xTaskCreate(resume, "resume", 2000, NULL, 4,&xHandle_resume);
	//xTaskCreate(game_over, "game_over", 2000, NULL, 4,&xHandle_gameover);
	vTaskStartScheduler();

}

/**
 * Example task which draws to the display.
 */
/**
 * This task polls the joystick value every 20 ticks
 */

static void checkJoystick() {
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	while (TRUE) {
		// Remember last joystick valuesESPL_DisplayReady
		joystick_last = joystick_now;
		joystick_now.x = (uint8_t) (ADC_GetConversionValue(ESPL_ADC_Joystick_2) >> 4);
		joystick_now.y = (uint8_t) 255 - (ADC_GetConversionValue(ESPL_ADC_Joystick_1) >> 4);
		// Send over UART
		//sendLine(joystick_last, joystick_now);
		vTaskDelayUntil(&xLastWakeTime, 20);
	}
}


void VApplicationIdleHook() {
	while (TRUE) {
	};
}
