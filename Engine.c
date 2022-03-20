#include "includes.h"
#include "Constants.h"
#include "Engine.h"
#include "math.h"


polygon Polies[iMAX_NUM_OBJECTS];
_circle Circles[iMAX_NUM_OBJECTS];
objectList AxisList[iMAX_NUM_OBJECTS];
objectList ActiveList[iMAX_NUM_OBJECTS];
objectList *ActiveListStart;
objectList *AxisListStart;
objectList *LastObject;
int iLastObject = 0;
int iLastCircle = 0;
int iLastPoly	= 0;
int iLastListItem = 0;
int ObjectCount = 0;
int iLastAABB = 0;
_circle *Ball;
polygon *Rect;
polygon *pFlipper;
polygon *pFlipperR;
polygon *Test1;
polygon *Test2;
point PNTS[4];
extern int level;

void v_test( void *t)
{
	int i = *((int*)t);
	i = 2 *i;
}

point* getPoints( polygon* pPolygon)
{
	int iNumLines;
	switch (pPolygon->Shape)
	{
	case triangle		: iNumLines = 3; break;
	case quadrangle		: iNumLines = 4; break;
	case circle			: iNumLines = 0; break;
	}
	for (int i=0; i<iNumLines; i++)
	{
		PNTS[i].x = pPolygon->Line[i].fEndpoint[0][0];
		PNTS[i].y = 239 - pPolygon->Line[i].fEndpoint[0][1];
	}
	return PNTS;
}

float f_MagnitudeSqrd( float *Vector)
{
	return pow( Vector[0], 2) + pow( Vector[1], 2);
}

void v_updateLoop()
{
	//update Axis Aligned Bounding Boxes
	v_updateAABBs();
	//Sort AABS according to their X Position
	v_quickSort( 0, iLastObject - 1);
	//check for possible Collion on a presorted List
	v_getCollisions( false);
	//in Case there were any Collisions, the AABBs get updated to reflect their new state
	v_updateAABBs();
	v_quickSort( 0, iLastObject -1);
	//check for possible Collisions, but only speculative
	//remove enough Velocity from the Objects to leave them just in touching distance after the next frame
	v_getCollisions( true);
	//finally update the positions of the objects according to their velocity
	v_updatePositions( true);
	return;
}

void v_updateCirclePos( _circle *pCircle, bool ApplyGravity)
{
	if (pCircle->ID == env)
	{
		return;
	}
	pCircle->bCollided = false;
	pCircle->fPos[0] += pCircle->fVel[0];
	pCircle->fPos[1] += pCircle->fVel[1];
	if (ApplyGravity == true && pCircle->bCollided == false)
	{
		pCircle->fVel[1] += dGRAVITY * dSLOPE * (float)level;
	}
}

void v_updatePolyPos( polygon *pPoly, bool ApplyGravity)
{
	int iNumLines;
	if (pPoly->ID == env)
	{
		return;
	}
	switch (pPoly->Shape)
	{
	case quadrangle : iNumLines = 4; break;
	case triangle 	: iNumLines = 3; break;
	}
	if (pPoly->ID == flipper)
	{
		v_updateFlipperPos( pPoly);
		pPoly->Line[0].fEndpoint[1][0] = pPoly->Line[1].fEndpoint[0][0];
		pPoly->Line[0].fEndpoint[1][1] = pPoly->Line[1].fEndpoint[0][1];
		pPoly->Line[1].fEndpoint[1][0] = pPoly->Line[2].fEndpoint[0][0];
		pPoly->Line[1].fEndpoint[1][1] = pPoly->Line[2].fEndpoint[0][1];
		pPoly->Line[2].fEndpoint[1][0] = pPoly->Line[0].fEndpoint[0][0];
		pPoly->Line[2].fEndpoint[1][1] = pPoly->Line[0].fEndpoint[0][1];
		return;
	}
	if ( ApplyGravity == true && pPoly->ID != flipper)
	{
		pPoly->fVel[1] += dGRAVITY * dSLOPE;
	}
	pPoly->bCollided = false;
	for (int i=0; i<iNumLines; i++)
	{
		for (int r=0; r<iNUM_DIMENSIONS; r++)
		{
			pPoly->Line[i].fEndpoint[0][r] += pPoly->fVel[r];
		}
	}
}
void v_updateFlipperPos ( polygon* pShape)
{
	float fVel[iNUM_DIMENSIONS];
	v_getAngularVel( fVel, pShape->pfFarthestPoint, pShape);
	pShape->pfFarthestPoint[0] += fVel[0];
	pShape->pfFarthestPoint[1] += fVel[1];
	return;
}
void v_updatePositions( bool ApplyGravity)
{
	void *pObject;
	for(int i=0; i<iLastObject; i++)
	{
		pObject = AxisList[i].pObject;
		switch (AxisList[i].Shape)
		{
		case circle 		: v_updateCirclePos( (_circle*)pObject, ApplyGravity); 	continue;
		case quadrangle		: v_updatePolyPos( (polygon*)pObject, ApplyGravity); 	continue;
		case triangle		: v_updatePolyPos( (polygon*)pObject, ApplyGravity);	continue;
		}
	}
}

void v_applyGravity( int *piVel )
{
	for( int i=0; i<iNUM_DIMENSIONS; i++)
	{
		piVel[i] -= dGRAVITY * dSLOPE;
	}
}

float f_sgn( float f)
{
	if (f<0)
	{
		f = f * (-1);
	}
	return f;
}

void v_AddVector( float *s, float *b, float *a)
{
	s[0] = a[0] + b[0];
	s[1] = a[1] + b[1];
}

void v_SubtractVector( float *s, float *b, float *a)
{
	s[0] = b[0] - a[0];
	s[1] = b[1] - a[1];
}

void v_CopyVector( float *dest, float *source)
{
	dest[0] = source[0];
	dest[1] = source[1];
}

void v_printVector( float *p)
{
	printf("%lf,%lf", p[0], p[1]);
}

void v_Support( float *pfPointToReturn, void *pShape1, shape Shape1, void *pShape2, shape Shape2, float *fDirection)
{
	float pfPoint[4], iReverseDirection[iNUM_DIMENSIONS];
	v_getFarthestPointInDirection( &pfPoint[0], pShape1, Shape1, &fDirection[0]);
	iReverseDirection[0] = fDirection[0] * (-1);
	iReverseDirection[1] = fDirection[1] * (-1);
	v_getFarthestPointInDirection( &pfPoint[2], pShape2, Shape2, &iReverseDirection[0]);
	pfPointToReturn[0] = pfPoint[0] - pfPoint[2];
	pfPointToReturn[1] = pfPoint[1] - pfPoint[3];
	pfPointToReturn[2] = pfPoint[0];
	pfPointToReturn[3] = pfPoint[1];
	pfPointToReturn[4] = pfPoint[2];
	pfPointToReturn[5] = pfPoint[3];
	return;
}

void v_getFarthestPointInDirection(float* fPos, void *pShape, shape Shape, float *fDirection)
{
	int i, iFarthestPointID = 0, iNumLines;
	float fNormDirection[iNUM_DIMENSIONS], fHighestDot = -1e100, fDot, fRadius, fMagnitude;
	if (Shape == circle)
	{
		fMagnitude = f_Magnitude( fDirection);
		fNormDirection[0] = fDirection[0] / fMagnitude;
		fNormDirection[1] = fDirection[1] / fMagnitude;
		fRadius = ((_circle*) pShape)->fRadius;
		fPos[0] = ((_circle*) pShape)->fPos[0] +  fNormDirection[0] * fRadius;
		fPos[1] = ((_circle*) pShape)->fPos[1] +  fNormDirection[1] * fRadius;
		return;
	}

	switch(Shape)
	{
	case quadrangle : iNumLines = 4; break;
	case triangle 	: iNumLines = 3; break;
	}

	for ( i=0; i<iNumLines; i++)
	{
		fDot = 0;
		fDot = f_getDot(((polygon*)pShape)->Line[i].fEndpoint[0], fDirection);
		if (fDot > fHighestDot)
		{
			fHighestDot = fDot;
			iFarthestPointID = i;
		}
	}
	fPos[0] = ((polygon*)pShape)->Line[iFarthestPointID].fEndpoint[0][0];
	fPos[1] =  ((polygon*)pShape)->Line[iFarthestPointID].fEndpoint[0][1];
	return;

}

float f_Magnitude( float *Vector)
{
	return sqrtf( pow(Vector[0], 2) + pow(Vector[1], 2));
}

float f_DistanceSqrd( float *a, float *b)
{
	float f[iNUM_DIMENSIONS];
	v_SubtractVector( f, b, a);
	return f_MagnitudeSqrd( f);
}

void v_getClosestPointToOrigin( float *pfPointToReturn, float* a, float* b)
{
	float v[iNUM_DIMENSIONS], p[iNUM_DIMENSIONS], nb[iNUM_DIMENSIONS];
	v_SubtractVector( v, b, a);
	nb[0] = b[0] * (-1);
	nb[1] = b[1] * (-1);
	//Das Lot auf die von a und b gespannte Gerade fällen
	p[0] = b[0] + f_getDot(nb, v) / f_getDot(v, v) * v[0];
	p[1] = b[1] + f_getDot(nb, v) / f_getDot(v, v) * v[1];
	//check weather the calculated point lies on the line segment between a & b
	if ( f_DistanceSqrd( a, p)  + f_DistanceSqrd( b, p) - f_DistanceSqrd( a, b) < 0.1)
	{
		v_CopyVector( pfPointToReturn, p);
		return;
	}
	if ( f_MagnitudeSqrd(a) < f_MagnitudeSqrd(b))
	{
		v_CopyVector( pfPointToReturn, a);
		return;
	}
	v_CopyVector( pfPointToReturn, b);
}

void v_copySimplex ( float *a, float *b)
{
	a[0] = b[0];
	a[1] = b[1];
	a[2] = b[2];
	a[3] = b[3];
	a[4] = b[4];
	a[5] = b[5];
}

void v_getClosestPoints(float *A, float *B, float *fDistance, void *pShape1, shape Shape1, void *pShape2, shape Shape2)
{
	int i=0;
	float pfSimplex1[6], pfSimplex2[6], pfSimplex3[6], fDot1, fDot2, p1[2], p2[2], fLength[2], fLambda[2];
	float pfDirection[iNUM_DIMENSIONS];
	static float LastA[iNUM_DIMENSIONS], LastB[iNUM_DIMENSIONS];
	//choose initial search direction
	pfDirection[0] = 0; pfDirection[1] = 1;
	// obtain the first Minkowski Difference point using
	// the direction and the support function
	v_Support( pfSimplex1, pShape1, Shape1, pShape2, Shape2, &pfDirection[0]);
	// like the previous post just in the other
	// direction
	v_negateDirection( pfDirection);
	v_Support( pfSimplex2, pShape1, Shape1, pShape2, Shape2, &pfDirection[0]);
	// obtain the point on the current simplex closest
	// start the loop
	v_getClosestPointToOrigin( pfDirection, pfSimplex1, pfSimplex2);
	while (i<iMAX_IT)
	{
		// the direction we get from the closest point is pointing
		// from the origin to the closest point, we need to reverse
		// it so that it points towards the origin
		v_negateDirection( pfDirection);
		// check if pfDirection is the zero vector
		if ( f_sgn(0 - pfDirection[0]) < 0.1 && f_sgn(0.1 - pfDirection[1]) < 0.01)
		{
			// The origin is on the Minowski difference
			// (Collision)
			v_CopyVector( A, LastA);
			v_CopyVector( B, LastB);
			*fDistance = fCOLLISION_DIST - 0.02;
			return;
		}
		// obtain a new Minkowski Difference point along
		// the new direction
		v_Support( pfSimplex3, pShape1, Shape1, pShape2, Shape2, pfDirection);
		// pfSimlex3 is the point we obtained making progress
		// towards the goal (to get the closest points
		// to the origin)
		fDot1 = f_getDot( pfSimplex3, pfDirection);
		fDot2 = f_getDot( pfSimplex1, pfDirection);

		if ( f_sgn(fDot1 - fDot2) < 0.01)
		{
			// if we haven't made enough progress,
			// given some tolerance, to the origin,
			// then we can assume that we are done
			v_SubtractVector( fLength, pfSimplex2, pfSimplex1);
			// Use the definition of a convex polygon and
			// the latest 2 points, that created the minowski
			// difference to calculate the closest points on
			// the respective shapes
			fLambda[1] = (-1) * f_getDot( fLength, pfSimplex1) / f_getDot( fLength, fLength);
			fLambda[0] = 1 - fLambda[1];
			A[0] = fLambda[0] * pfSimplex1[2] + fLambda[1] * pfSimplex2[2];
			A[1] = fLambda[0] * pfSimplex1[3] + fLambda[1] * pfSimplex2[3];
			B[0] = fLambda[0] * pfSimplex1[4] + fLambda[1] * pfSimplex2[4];
			B[1] = fLambda[0] * pfSimplex1[5] + fLambda[1] * pfSimplex2[5];
			// if the length is zero, the points, used for the
			// minowski difference are the same
			if( f_sgn(fLength[0]) - 0.001 == 0 && f_sgn(fLength[1]) - 0.001 == 0)
			{
				v_CopyVector( A, &pfSimplex1[2]);
				v_CopyVector( LastA, &pfSimplex1[2]);
				v_CopyVector( B, &pfSimplex1[4]);
				v_CopyVector( LastB, &pfSimplex1[2]);
			}
			// if one of the lambdas is negative, we can
			// assume, that the respective other point of
			// the minowski difference is the same
			if (fLambda[0] < 0)
			{
				v_CopyVector( A, &pfSimplex2[2]);
				v_CopyVector( LastA, &pfSimplex2[2]);
				v_CopyVector( B, &pfSimplex2[4]);
				v_CopyVector( LastB, &pfSimplex2[4]);
			}
			else if (fLambda[1] < 0)
			{
				v_CopyVector( A, &pfSimplex1[2]);
				v_CopyVector( LastA, &pfSimplex1[2]);
				v_CopyVector( B, &pfSimplex1[4]);
				v_CopyVector( LastB, &pfSimplex1[4]);
			}
			// The direction vector is the closest point
			// it can be used to calculate the distance
			*fDistance = f_Magnitude( pfDirection);
			return;
		}
		// if we are still getting closer then only keep
		// the points in the simplex that are closest to
		// the origin (we already know that c is closer
		// than both a and b so we only need to choose
		// between these two)
		v_getClosestPointToOrigin( p1, pfSimplex1, pfSimplex3);
		v_getClosestPointToOrigin( p2, pfSimplex3, pfSimplex2);
		// getting the closest point on the edges AC and
		// CB allows us to compare the distance between
		// the origin and edge and choose the closer one
		if (f_MagnitudeSqrd( p1) < f_MagnitudeSqrd( p2))
		{
			v_copySimplex( pfSimplex2, pfSimplex3);
			v_CopyVector( pfDirection, p1);
		}
		else
		{
			v_copySimplex( pfSimplex1, pfSimplex3);
			v_CopyVector( pfDirection, p2);
		}
		i++;
	}
	// if the algorithm doesn't finish, use the last known
	// closest points and report a distance smaller than the
	// predefined collision distance
	v_CopyVector( A, LastA);
	v_CopyVector( B, LastB);
	*fDistance = fCOLLISION_DIST - 0.02;
}

void v_negateDirection( float* fDirection)
{
	for (int i=0; i<iNUM_DIMENSIONS; i++)
	{
		fDirection[i] = fDirection[i] * (-1);
	}
}

float f_getDot( float *a, float*b)
{
	return a[0] * b[0] + a[1] * b[1];
}

polygon* createQuadrangle( float p1X, float p1Y, float p2X, float p2Y, float p3X, float p3Y, float p4X, float p4Y, id ID)
{
	polygon *r;
	if (ID != aabb)
	{
		r = &Polies[iLastPoly];
		iLastPoly++;
	}
	if (ID == aabb)
	{
		r = &Polies[iLastPoly + iLastAABB];
		iLastAABB++;
	}
	r->Line[0].fEndpoint[0][0] = p1X;
	r->Line[0].fEndpoint[0][1] = p1Y;
	r->Line[0].fEndpoint[1][0] = p2X;
	r->Line[0].fEndpoint[1][1] = p2Y;
	r->Line[1].fEndpoint[0][0] = p2X;
	r->Line[1].fEndpoint[0][1] = p2Y;
	r->Line[1].fEndpoint[1][0] = p3X;
	r->Line[1].fEndpoint[1][1] = p3Y;
	r->Line[2].fEndpoint[0][0] = p3X;
	r->Line[2].fEndpoint[0][1] = p3Y;
	r->Line[2].fEndpoint[1][0] = p4X;
	r->Line[2].fEndpoint[1][1] = p4Y;
	r->Line[3].fEndpoint[0][0] = p4X;
	r->Line[3].fEndpoint[0][1] = p4Y;
	r->Line[3].fEndpoint[1][0] = p1X;
	r->Line[3].fEndpoint[1][1] = p1Y;
	r->fResCo = 1.0;
	r->ID = ID;
	r->fMass = 0;
	r->Shape = quadrangle;
	if (ID != aabb)
	{
		v_appendToList( r, quadrangle, ID);
	}
	return r;
}

polygon* createFlipper( float p1X, float p1Y, float p2X, float p2Y, float p3X, float p3Y, pos Pos)
{
	float fBuf[iNUM_DIMENSIONS], fDistance;
	polygon *t = createTriangle( p1X, p1Y, p2X, p2Y, p3X, p3Y, flipper);
	if (Pos == left)
	{
		t->pfRotPoint = t->Line[0].fEndpoint[0];
		t->pfFarthestPoint = t->Line[1].fEndpoint[0];
		t->Direction = down;
		t->Pos = Pos;
		t->fVel[0] = 0;
		t->fVel[1] = 0;
	}
	if (Pos == right)
	{
		t->pfRotPoint = t->Line[1].fEndpoint[0];
		t->pfFarthestPoint = t->Line[0].fEndpoint[0];
		t->Direction = down;
		t->Pos = Pos;
		t->fVel[0] = 0;
		t->fVel[1] = 0;
	}
	return t;
}

polygon* createTriangle( float p1X, float p1Y, float p2X, float p2Y, float p3X, float p3Y, id ID)
{
	polygon *r = &Polies[iLastPoly];
	iLastPoly++;
	r->Line[0].fEndpoint[0][0] = p1X;
	r->Line[0].fEndpoint[0][1] = p1Y;
	r->Line[0].fEndpoint[1][0] = p2X;
	r->Line[0].fEndpoint[1][1] = p2Y;
	r->Line[1].fEndpoint[0][0] = p2X;
	r->Line[1].fEndpoint[0][1] = p2Y;
	r->Line[1].fEndpoint[1][0] = p3X;
	r->Line[1].fEndpoint[1][1] = p3Y;
	r->Line[2].fEndpoint[0][0] = p3X;
	r->Line[2].fEndpoint[0][1] = p3Y;
	r->Line[2].fEndpoint[1][0] = p1X;
	r->Line[2].fEndpoint[1][1] = p1Y;
	r->fResCo = 1.0;
	r->fMass = 0;
	r->fVel[0] = 0;
	r->fVel[1] = 0;
	r->ID = ID;
	r->Shape = triangle;
	v_appendToList( r, triangle, ID);
	if(ID == aabb)
	{
		iLastObject--;
	}
	return r;
}

_circle* createCircle( float pX, float pY, float radius)
{
	_circle *c = &Circles[iLastCircle];
	iLastCircle++;
	c->fPos[0] = pX; c->fPos[1] = pY;
	c->fRadius = radius;
	c->fResCo = 1;
	c->fSpecVel[0] = 0;
	c->fSpecVel[1] = 0;
	v_appendToList( c, circle, normal);
	return c;
}

float f_Distance( float *a, float *b)
{
	float t[2];
	v_SubtractVector( t, b, a);
	return f_Magnitude( t);
}

void v_getNormal( float *N, float *p1, float *p2, void *pShape, shape Shape)
{
	int iNumLines = 0;
	float fDistanceSqrd, V[iNUM_DIMENSIONS], PossibleN[2][iNUM_DIMENSIONS] = {0};
	if (Shape == circle)
	{
		v_SubtractVector( N, p1, ((_circle*)pShape)->fPos);
		return;
	}
	switch (Shape)
	{
		case quadrangle : iNumLines = 4; break;
		case triangle 	: iNumLines = 3; break;
	}
	for (int i=0; i<iNumLines; i++)
	{
		fDistanceSqrd = 0;
		for (int r=0; r<2; r++)
		{
			fDistanceSqrd += f_DistanceSqrd( ((polygon*)pShape)->Line[i].fEndpoint[r], p1);
		}
		if ( fDistanceSqrd - f_DistanceSqrd( ((polygon*)pShape)->Line[i].fEndpoint[0], ((polygon*)pShape)->Line[i].fEndpoint[1]) < 0.01)
		{
			v_SubtractVector( V, ((polygon*)pShape)->Line[i].fEndpoint[1], ((polygon*)pShape)->Line[i].fEndpoint[0]);
			PossibleN[0][0] = V[1] * (-1);
			PossibleN[0][1] = V[0];
			if (PossibleN[1][0] == 0 && PossibleN[1][1] == 0)
			{
				PossibleN[1][0] = PossibleN[0][0];
				PossibleN[1][1] = PossibleN[0][1];
			}
		}
	}
	if ( f_getDot( PossibleN[0], p2 ) > 0)
	{
		N[0] = PossibleN[0][0];
		N[1] = PossibleN[0][1];
		return;
	}
	N[0] = PossibleN[1][0];
	N[1] = PossibleN[1][1];
}

void v_getAABB( polygon **r, void *pShape, shape Shape)
{
	int iNumLines = 0;
	polygon *b;
	float fLength, fHeight, fMin[iNUM_DIMENSIONS], fMax[iNUM_DIMENSIONS];
	float fLeftTop[iNUM_DIMENSIONS], fVel[iNUM_DIMENSIONS], fV;
	if (Shape == circle)
	{
		fV = 0;
		// obtain velocity of the circle (ball)
		fVel[0] = ((_circle*)pShape)->fVel[0];
		fVel[1] = ((_circle*)pShape)->fVel[1];
		// check weather we need to add the velocity
		// to the dimensions of the AABB (Axis Aligned
		// Bounding Box)
		if (fVel[0] < 0)
		{
			fV = ((_circle*)pShape)->fVel[0];
		}
		fLeftTop[0] = ((_circle*)pShape)->fPos[0] + fV - ((_circle*)pShape)->fRadius - fCOLLISION_DIST;
		if (fVel[0] < 0)
		{
			fVel[0] = fVel[0] * (-1);
		}
		fV = 0;
		if (fVel[1] > 0)
		{
			fV = ((_circle*)pShape)->fVel[1];
		}
		fLeftTop[1] = ((_circle*)pShape)->fPos[1] + fV + ((_circle*)pShape)->fRadius + fCOLLISION_DIST;
		if (fVel[1] < 0)
		{
			fVel[1] = fVel[1] * (-1);
		}
		fLength = 2 * ((_circle*)pShape)->fRadius + fCOLLISION_DIST + fVel[0];
		fHeight = 2 * ((_circle*)pShape)->fRadius + fCOLLISION_DIST + fVel[1];
		v_createRectangle( &b, fLeftTop[0], fLeftTop[1], fLength, fHeight, aabb);
		*r = b;
		return;
	}
	switch (Shape)
	{
		case quadrangle : iNumLines = 4; break;
		case triangle 	: iNumLines = 3; break;
	}
	for (int r=0; r<iNUM_DIMENSIONS; r++)
	{
		fMax[r] = ((polygon*)pShape)->Line[0].fEndpoint[0][r];
		fMin[r] = ((polygon*)pShape)->Line[0].fEndpoint[0][r];
	}
	for (int i=0; i<iNumLines; i++)
	{
		fVel[0] = ((polygon*)pShape)->fVel[0];
		fVel[1] = ((polygon*)pShape)->fVel[1];
		// find the minimum and maximum points of the shape
		// to calculate the AABB, take the velocity into account
		// and add it to the AABB in order for speculative contacts
		// to work
		for (int r=0; r<iNUM_DIMENSIONS; r++)
		{
			if (((polygon*)pShape)->Line[i].fEndpoint[0][r] + fVel[r] > fMax[r])
			{
				fMax[r] = ((polygon*)pShape)->Line[i].fEndpoint[0][r] + fVel[r];
			}
			if (((polygon*)pShape)->Line[i].fEndpoint[0][r] + fVel[r] < fMin[r])
			{
				fMin[r] = ((polygon*)pShape)->Line[i].fEndpoint[0][r] + fVel[r];
			}
			if (((polygon*)pShape)->Line[i].fEndpoint[0][r] > fMax[r])
			{
				fMax[r] = ((polygon*)pShape)->Line[i].fEndpoint[0][r];
			}
			if (((polygon*)pShape)->Line[i].fEndpoint[0][r] < fMin[r])
			{
				fMin[r] = ((polygon*)pShape)->Line[i].fEndpoint[0][r];
			}
		}
	}
	*r = createQuadrangle( fMin[0], fMax[1], fMax[0], fMax[1], fMax[0], fMin[1], fMin[0], fMin[1], aabb);
	return;
}

void v_createRectangle( polygon **r, float pX, float pY, float Length, float Height, id ID)
{
	*r = createQuadrangle( pX, pY, pX + Length, pY, pX + Length, pY - Height, pX, pY - Height, aabb);
}

void v_appendToList( void *pShape, shape Shape, id ID)
{
	AxisList[iLastObject].Shape = Shape;
	AxisList[iLastObject].pObject = pShape;
	AxisList[iLastObject].ID = ID;
	AxisList[iLastObject].pNext = NULL;
	if(iLastObject == 0)
	{
		AxisList[iLastObject].pPrev = NULL;
		iLastObject++;
		return;
	}
	AxisList[iLastObject].pPrev = &AxisList[iLastObject-1];
	iLastObject++;
	return;
}

void v_updateAABBs()
{
	polygon *p;
	for (int i=0; i<iLastObject; i++)
	{
		v_getAABB( &p, AxisList[i].pObject, AxisList[i].Shape);
		AxisList[i].pAABB = p;
	}
	iLastAABB = 0;

}


objectList* getListItem(int Number)
{
	objectList *pCurrent = LastObject;
	for (int i=0; i<Number; i++)
	{
		pCurrent = pCurrent->pNext;
	}
	return pCurrent;
}

void v_swap ( objectList* a, objectList* b)
{
	void *vBuffer = a->pObject;
	polygon *polBuffer = a->pAABB;
	shape shBuffer = a->Shape;
	a->pObject = b->pObject;
	a->pAABB = b->pAABB;
	a->Shape = b->Shape;
	b->pObject = vBuffer;
	b->pAABB = polBuffer;
	b->Shape = shBuffer;
	return;
}

void v_partition( int *sol, int left, int right)
{
	void *pObjectSwap;
	polygon *pAABBSwap;
	shape ShapeSwap;
	id IDSwap;
	int	i = left;
	int	j = right - 1;
	float pivot = AxisList[right].pAABB->Line[0].fEndpoint[0][0];
	do
	{
		while (AxisList[i].pAABB->Line[0].fEndpoint[0][0] <= pivot && i < right)
		{
			i++;
		}
		while(AxisList[j].pAABB->Line[0].fEndpoint[0][0] >= pivot && j > left)
		{
			j--;
		}
		if (i < j)
		{
			pObjectSwap = AxisList[i].pObject;
			pAABBSwap = AxisList[i].pAABB;
			ShapeSwap = AxisList[i].Shape;
			IDSwap = AxisList[i].ID;
			AxisList[i].pObject = AxisList[j].pObject;
			AxisList[i].pAABB = AxisList[j].pAABB;
			AxisList[i].Shape = AxisList[j].Shape;
			AxisList[i].ID = AxisList[j].ID;
			AxisList[j].pObject = pObjectSwap;
			AxisList[j].pAABB = pAABBSwap;
			AxisList[j].Shape = ShapeSwap;
			AxisList[j].ID = IDSwap;
		}
	} while (i < j);
	if (AxisList[i].pAABB->Line[0].fEndpoint[0][0] > pivot)
	{
		pObjectSwap = AxisList[i].pObject;
		pAABBSwap = AxisList[i].pAABB;
		ShapeSwap = AxisList[i].Shape;
		IDSwap = AxisList[i].ID;
		AxisList[i].pObject = AxisList[right].pObject;
		AxisList[i].pAABB = AxisList[right].pAABB;
		AxisList[i].Shape = AxisList[right].Shape;
		AxisList[i].ID = AxisList[right].ID;
		AxisList[right].pObject = pObjectSwap;
		AxisList[right].pAABB = pAABBSwap;
		AxisList[right].Shape = ShapeSwap;
		AxisList[right].ID = IDSwap;
	}
	*sol = i;
}


void v_quickSort(int left, int right)
{
	int pivot;
	if (left < right)
	{
		v_partition( &pivot, left, right);
		v_quickSort( left, pivot-1);
		v_quickSort( pivot+1, right);
	}
}


void v_getCollisions( bool bSpeculative)
{
	float X, Y1, Y2;
	objectList *r, *last;
	ActiveListStart = NULL;
	id ID;
	//Cycle through all possible Objects
	for (int i=0; i<iLastObject; i++)
	{
		// X is the most left Value on the x-axis
		// this is the sorting element
		X = AxisList[i].pAABB->Line[0].fEndpoint[0][0];
		// highest Point on the y-axis
		Y1 = AxisList[i].pAABB->Line[0].fEndpoint[0][1];
		// lowest Point on the y-axis
		Y2 = AxisList[i].pAABB->Line[3].fEndpoint[0][1];
		ID = AxisList[i].ID;
		// if the list is empty, use the current object
		// as the start of the list
		if (ActiveListStart == NULL)
		{
			ActiveListStart = &AxisList[i];
			ActiveListStart->pPrev = NULL;
			ActiveListStart->pNext = NULL;
			continue;
		}
		r = ActiveListStart;
		while (r != NULL)
		{
			// if X is bigger than the rightmost point
			// on the AABB, remove the AABB from the list
			if (X > r->pAABB->Line[1].fEndpoint[0][0])
			{
				if (r==ActiveListStart)
				{
					ActiveListStart = &AxisList[i];
					r->pNext = NULL;
					r->pPrev = NULL;
					last = NULL;
					break;
				}
				else
				{
					r->pPrev->pNext = r->pNext;
					last = NULL;
					break;
				}
			}
			// if X is smaller than the rightmost point, and
			// if both objects don't belong to the environment
			// report a collision
			if (X < r->pAABB->Line[1].fEndpoint[0][0] && r->ID != ID)
			{
				if ((Y2 < r->pAABB->Line[0].fEndpoint[0][1] + fCOLLISION_DIST && Y2 > r->pAABB->Line[3].fEndpoint[0][1])
						|| (Y1 < r->pAABB->Line[0].fEndpoint[0][1] && Y1 > r->pAABB->Line[3].fEndpoint[0][1] - fCOLLISION_DIST))
				{
					if (bSpeculative == true)
					{
						v_speculativeContact( AxisList[i].pObject, AxisList[i].Shape, r->pObject, r->Shape);
					}
					else
					{
						v_checkForCollision( AxisList[i].pObject, AxisList[i].Shape, r->pObject, r->Shape);
					}
				}
			}
			last = r;
			r = r->pNext;
		}
		// append the current object to the list
		if (last != NULL)
		{
		last->pNext = &AxisList[i];
		last->pNext->pPrev = last;
		last->pNext->pNext = NULL;
		}
	}
}

void v_updateCollided( void *pShape, shape Shape)
{
	if (Shape == circle)
	{
		((_circle*)pShape)->bCollided = true;
		return;
	}
	((polygon*)pShape)->bCollided = true;
}

void v_updateVelocity ( float* fImpulse, void *pShape, shape Shape, bool bSpeculative)
{
	if (Shape == circle)
	{
		if (bSpeculative == true)
		{
			// save the removed velocity from a speculative contact
			((_circle*)pShape)->fSpecVel[0] = fImpulse[0] * f_getMass( pShape, Shape);
			((_circle*)pShape)->fSpecVel[1] = fImpulse[1] * f_getMass( pShape, Shape);
			((_circle*)pShape)->fVel[0] += fImpulse[0] * f_getMass( pShape, Shape);
			((_circle*)pShape)->fVel[1] += fImpulse[1] * f_getMass( pShape, Shape);
			return;
		}
		((_circle*)pShape)->fVel[0] += fImpulse[0] * f_getMass( pShape, Shape) * (1 - fFRICTION);
		((_circle*)pShape)->fVel[1] += fImpulse[1] * f_getMass( pShape, Shape) * (1 - fFRICTION);
		return;
	}
	if (bSpeculative == true)
	{
		// save the removed velocity from a speculative contact
		((polygon*)pShape)->fVel[0] = fImpulse[0] * f_getMass( pShape, Shape);
		((polygon*)pShape)->fVel[1] = fImpulse[1] * f_getMass( pShape, Shape);
		((polygon*)pShape)->fVel[0] += fImpulse[0] * f_getMass( pShape, Shape);
		((polygon*)pShape)->fVel[1] += fImpulse[1] * f_getMass( pShape, Shape);
		return;
	}
	((polygon*)pShape)->fVel[0] += fImpulse[0] * f_getMass( pShape, Shape) * (1 - fFRICTION);
	((polygon*)pShape)->fVel[1] += fImpulse[1] * f_getMass( pShape, Shape) * (1 - fFRICTION);
}

void v_speculativeContact( void *pShape1, shape Shape1, void*pShape2, shape Shape2)
{
	float A[iNUM_DIMENSIONS], B[iNUM_DIMENSIONS], fNormalB[iNUM_DIMENSIONS], fDistance = 12, fMassRatio;
	float fVel1[iNUM_DIMENSIONS], fVel2[iNUM_DIMENSIONS], fRelVel[iNUM_DIMENSIONS], fImpulse[iNUM_DIMENSIONS];
	float fMagnitude = 0, fNormalRelVel, fRemove;
	fNormalB[0] = 0;
	fNormalB[1] = 1;
	v_getClosestPoints ( A, B, &fDistance, pShape1, Shape1, pShape2, Shape2);
	v_getVelocity( fVel1, A, pShape1, Shape1);
	v_getVelocity( fVel2, B, pShape2, Shape2);
	v_getNormal( fNormalB, B, A, pShape2, Shape2);
	v_SubtractVector( fRelVel, fVel1, fVel2);
	fMagnitude = f_Magnitude(fNormalB);
	fNormalB[0] = fNormalB[0] / fMagnitude;
	fNormalB[1] = fNormalB[1] / fMagnitude;
	fNormalRelVel = f_getDot( fRelVel, fNormalB);
	fRemove = fNormalRelVel + (fDistance - fCOLLISION_DIST + 0.02 );
	// only remove velocity, when the objects will collide during
	// the next frame
	// remove just enough velocity, to put the two objects in
	// collision distance
	if (fRemove < 0) {
		fMassRatio = (f_getMass(pShape1, Shape1) + f_getMass(pShape2, Shape2));
		fImpulse[0] = (fRemove * fNormalB[0]) / fMassRatio;
		fImpulse[1] = (fRemove * fNormalB[1]) / fMassRatio;
		v_negateDirection(fImpulse);
		v_updateVelocity(fImpulse, pShape1, Shape1, true);
		v_negateDirection(fImpulse);
		v_updateVelocity(fImpulse, pShape2, Shape2, true);
	}
}

void v_checkForCollision( void *pShape1, shape Shape1, void*pShape2, shape Shape2)
{
	float A[iNUM_DIMENSIONS], B[iNUM_DIMENSIONS], fNormalB[iNUM_DIMENSIONS], fDistance = 12, fMassRatio;
	float fVel1[iNUM_DIMENSIONS], fVel2[iNUM_DIMENSIONS], fRelVel[iNUM_DIMENSIONS], fImpulse[iNUM_DIMENSIONS];
	float fRelResCo, fMagnitude = 0;
	fNormalB[0] = 0;
	fNormalB[1] = 1;
	v_getClosestPoints ( A, B, &fDistance, pShape1, Shape1, pShape2, Shape2);
	if ( fDistance < fCOLLISION_DIST)
	{
		v_getNormal( fNormalB, B, A, pShape2, Shape2);
		v_getVelocity( fVel1, A, pShape1, Shape1);
		v_getVelocity( fVel2, B, pShape2, Shape2);
		// only update the velocities, if the 2 objects
		// move toward each other
		if ( f_getDot( fVel1, fNormalB ) < 0)
		{
			fRelResCo = (f_getResCo( pShape1, Shape1) + f_getResCo( pShape2, Shape2)) / 2;
			v_SubtractVector( fRelVel, fVel1, fVel2);
			fMagnitude = f_Magnitude(fNormalB);
			fNormalB[0] = fNormalB[0] / fMagnitude;
			fNormalB[1] = fNormalB[1] / fMagnitude;
			fMassRatio = (f_getMass( pShape1, Shape1) + f_getMass( pShape2, Shape2));
			fImpulse[0] = (1 + fRelResCo) * fNormalB[0] * f_getDot( fRelVel, fNormalB) / fMassRatio;
			fImpulse[1] = (1 + fRelResCo) * fNormalB[1] * f_getDot( fRelVel, fNormalB) / fMassRatio;
			v_negateDirection( fImpulse);
			v_updateVelocity( fImpulse, pShape1, Shape1, false);
			v_negateDirection( fImpulse);
			v_updateVelocity( fImpulse, pShape2, Shape2, false);
		}
	}
}

void v_getVelocity (float* fVel, float *p, void *pShape, shape Shape)
{
	if (Shape == circle)
	{
		((_circle*)pShape)->fVel[0] -= ((_circle*)pShape)->fSpecVel[0];
		((_circle*)pShape)->fVel[1] -= ((_circle*)pShape)->fSpecVel[1] + dGRAVITY * dSLOPE;
		fVel[0] = ((_circle*)pShape)->fVel[0];
		fVel[1] = ((_circle*)pShape)->fVel[1];
		((_circle*)pShape)->fSpecVel[0] = 0;
		((_circle*)pShape)->fSpecVel[1] = 0;
		return;
	}
	if (((polygon*)pShape)->ID == flipper)
	{
		v_getAngularVel( fVel, p, pShape);
		return;
	}
	((polygon*)pShape)->fVel[0] -= ((polygon*)pShape)->fSpecVel[0];
	((polygon*)pShape)->fVel[1] -= ((polygon*)pShape)->fSpecVel[1] + dGRAVITY * dSLOPE;
	fVel[0] = ((polygon*)pShape)->fVel[0] - ((polygon*)pShape)->fSpecVel[0];
	fVel[1] = ((polygon*)pShape)->fVel[1] - ((polygon*)pShape)->fSpecVel[1];
	((polygon*)pShape)->fSpecVel[0] = 0;
	((polygon*)pShape)->fSpecVel[1] = 0;
}

void v_getAngularVel( float *fVel, float *p, polygon *pShape)
{
	float fBuf[iNUM_DIMENSIONS], fRadius, fNewPos[iNUM_DIMENSIONS], fOmega;
	if( pShape->Direction == down)
	{
		if (pShape->fLastPos - fOMEGA < 0)
		{
			pShape->fLastPos = 1;
		}
		fOmega = pShape->fLastPos - fOMEGA;
		pShape->fLastPos -= fOMEGA;
		if ( fOmega < fFLIPPER_LOW_DEG && fOmega > fFLIPPER_MAX_DEG)
		{
			fOmega = fFLIPPER_LOW_DEG;
			pShape->fLastPos = fFLIPPER_LOW_DEG;
		}
	}
	if( pShape->Direction == up)
	{
		if (pShape->fLastPos + fOMEGA > 1)
		{
			fOmega = 0;
		}
		fOmega = pShape->fLastPos + fOMEGA;
		pShape->fLastPos += fOMEGA;

		if ( pShape->fLastPos > fFLIPPER_MAX_DEG && pShape->fLastPos < fFLIPPER_LOW_DEG)
		{
			fOmega = fFLIPPER_MAX_DEG;
			pShape->fLastPos = fFLIPPER_MAX_DEG;
		}
	}

	v_SubtractVector( fBuf, p, pShape->pfRotPoint);
	fRadius = f_Magnitude( fBuf);
	// get position in the next frame
	if (pShape->Pos == left)
	{
		fNewPos[0] = pShape->pfRotPoint[0] + fRadius * cos( fOmega * 2*dPI);
		fNewPos[1] = pShape->pfRotPoint[1] + fRadius * sin( fOmega * 2*dPI);
	}
	if (pShape->Pos == right)
	{
		fNewPos[0] = pShape->pfRotPoint[0] + fRadius * cos( (-1) * fOmega * 2*dPI + dPI);
		fNewPos[1] = pShape->pfRotPoint[1] + fRadius * sin( (-1) * fOmega * 2*dPI + dPI);
	}
	// subtract new position from the old position
	// to express movement in terms of velocity
	v_SubtractVector( fBuf, fNewPos, p);
	fVel[0] = fBuf[0];
	fVel[1] = fBuf[1];
}

float f_getMass( void *pShape, shape Shape)
{
	if (Shape == circle)
	{
		return ((_circle*)pShape)->fMass;
	}
	return ((polygon*)pShape)->fMass;
}

float f_getResCo( void *pShape, shape Shape)
{
	if (Shape == circle)
	{
		return ((_circle*)pShape)->fResCo;
	}
	return ((polygon*)pShape)->fResCo;
}
