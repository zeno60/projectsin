#ifndef GOCCAMERA_HEADER
#define GOCCAMERA_HEADER

#include "GOComponent.h"
#include "vector3d.h"
#include "vector2d.h"
#include "quaternion.h"
#include "matrix4.h"

class gocCamera : public GOComponent
{
public:
	//Component Interface
	virtual const goc_id_type& componentId() const { return mComponentId; }
	virtual const goc_id_type& familyId() const { return mFamilyId; }

	//Camera Interface
	gocCamera();
	void update(float timeDelta);

	enum Type
	{
		LANDOBJECT,
		AIRCRAFT,
		TRACKBALL
	};

	void strafe(float units);
	void fly(float units);
	void walk(float units);

	void pitch(float angle);
	void yaw(float angle);
	void roll(float angle);
	void rotate(float speed);

	void beginDrag(int x, int y);
	void drag(int x, int y);

	vector2df ScreenToNDC(int x, int y);
	vector3df ProjectToSphere(vector2df ndc);

	void getViewMatrix(matrixf* m);
	matrixf getMatrix();

private:
	static goc_id_type mComponentId;
	static goc_id_type mFamilyId;

	Type type;
	vector3df right;
	vector3df up;
	vector3df look;
	vector3df position;
	
	vector3df startVector;
	vector3df currentVector;
	quaternion startQuat;
	quaternion currentQuat;
	vector2df position2d;

	bool isDragging;
};

#endif