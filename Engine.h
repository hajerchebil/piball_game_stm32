#include "Constants.h"
#include "includes.h"
#ifndef ENGINE_TYPES
#define ENGINE_TYPES

	typedef enum _bool {true, false} bool;
	typedef enum _shape {circle, triangle, quadrangle} shape;
	typedef enum _id	{flipper, normal, kicker, aabb, env} id;
	typedef enum _pos  {right, left} pos;
	typedef enum _direction { up, down} direction;

	typedef struct __circle
	{
		float fPos[iNUM_DIMENSIONS];
		float fVel[iNUM_DIMENSIONS];
		float fSpecVel[iNUM_DIMENSIONS];
		float fMass;
		float fRadius;
		float fResCo;
		bool bCollided;
		id 	ID;
		shape Shape;
	} _circle;

	typedef struct __line
	{
		float fEndpoint[2][iNUM_DIMENSIONS];
	} _line;

	typedef struct _polygon
	{
		_line Line[4];
		shape Shape;
		id	ID;
		bool bCollided;
		float fResCo;
		float fVel[iNUM_DIMENSIONS];
		float fSpecVel[iNUM_DIMENSIONS];
		float fMass;
		float *pfRotPoint;
		float *pfFarthestPoint;
		float fLastPos;
		pos Pos;
		direction Direction;
	} polygon;

	typedef struct _objectList
	{
		void				*pObject;
		polygon				*pAABB;
		shape				Shape;
		id					ID;
		struct _objectList	*pNext;
		struct _objectList 	*pPrev;
	} objectList;


#endif

void v_updateLoop();
float f_sgn( float f);
void v_AddVector( float *s, float *b, float *a);
void v_SubtractVector( float *s, float *b, float *a);
void v_CopyVector( float *dest, float *source);
void v_printVector( float *p);
void v_Support( float *pfPointToReturn, void *pShape1, shape Shape1, void *pShape2, shape Shape2, float *fDirection);
void v_getFarthestPointInDirection(float* fPos, void *pShape, shape Shape, float *fDirection);
float f_Magnitude( float *Vector);
void v_getClosestPointToOrigin( float *pfPointToReturn, float* a, float* b);
void v_getClosestPoints(float *A, float *B, float *fDistance, void *pShape1, shape Shape1, void *pShape2, shape Shape2);
void v_negateDirection( float* fDirection);
float f_getDot( float *a, float*b);
polygon* createQuadrangle( float p1X, float p1Y, float p2X, float p2Y, float p3X, float p3Y, float p4X, float p4Y, id ID);
polygon* createTriangle( float p1X, float p1Y, float p2X, float p2Y, float p3X, float p3Y, id ID);
_circle* createCircle( float pX, float pY, float radius);
float f_Distance( float *a, float *b);
void v_getNormal( float *N, float *p1, float *p2, void *pShape, shape Shape);
void v_getAABB( polygon **r, void *pShape, shape Shape);
void v_createRectangle( polygon **r, float pX, float pY, float Length, float Height, id ID);
void v_appendToList( void *pShape, shape Shape, id ID);
void v_updateAABBs();
objectList* getListItem(int Number);
void v_swap ( objectList* a, objectList* b);
void v_partition( int *sol, int left, int right);
void v_quickSort(int left, int right);
void v_checkForCollision( void *pShape1, shape Shape1, void*pShape2, shape Shape2);
void v_speculativeContact( void *pShape1, shape Shape1, void*pShape2, shape Shape2);
void v_getVelocity (float* fVel, float *p, void *pShape, shape Shape);
float f_getMass( void *pShape, shape Shape);
float f_getResCo( void *pShape, shape Shape);
polygon* createFlipper( float p1X, float p1Y, float p2X, float p2Y, float p3X, float p3Y, pos Pos);
void v_updatePositions( bool ApplyGravity);
void v_getCollisions( bool bSpeculative);
float f_MagnitudeSqrd( float *Vector);
void v_updateCollided( void *pShape, shape Shape);
void v_getAngularVel( float *fVel, float *p, polygon *pShape);
void v_updateFlipperPos ( polygon* pShape);
void v_updateCirclePos( _circle *pCircle, bool ApplyGravity);

