/**
 * Function definitions for the main project file.
 *
 * @author: Jonathan Müller-Boruttau, Nadja Peters nadja.peters@tum.de (RCS, TUM)
 */

#ifndef Demo_INCLUDED
#define Demo_INCLUDED

struct line
	{
		char x_1;
		char y_1;
		char x_2;
		char y_2;
		char x_3;
		char y_3;
		char x_4;
		char y_4;
		char x_5;
		char y_5;
		char x_6;
		char y_6;
	};
	struct coord
	{
		char x;
		char y;
	};

#endif

static void uartReceive();
void sendLine(struct coord coord_1, struct coord coord_2, struct coord coord_3, struct coord coord_4, struct coord coord_5, struct coord coord_6);
static void checkJoystick();
//static void drawTask();

void VApplicationIdleHook();
