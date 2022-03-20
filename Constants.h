#ifndef CONSTANTS_H
#define CONSTANTS_H
#define iREFRESH_RATE_HZ 		(int) 		50
#define dTIME_STEP		 		(float) 	1 / iREFRESH_RATE_HZ
#define dGRAVITY				(float)		(-1) * 9.8 / dTIME_STEP
#define dSLOPE					(float)		0.3
#define iNUM_TABLES				(int)		3
#define iTABLESIZE_PX			(int)		320
#define iMAP_SIZE_X				(int)		iNUM_TABLES * iTABLESIZE_PX
#define iMAP_SIZE_Y				(int)		240
#define iBALL_RADIUS			(int)		5
#define iNUM_DIMENSIONS			(int)		2
#define fTOLERANCE				(float)		0.0001
#define fCOLLISION_DIST			(float)		6
#define iMAX_NUM_OBJECTS		(int)		60
#define fFRICTION				(float)		0.1
#define dPI						(double)	3.14
#define fFLIPPER_MAX_DEG		(float)		0.05
#define fFLIPPER_LOW_DEG		(float)		0.95
#define fOMEGA					(float)		0.007
#define iMAX_IT					(int)		30
#endif
