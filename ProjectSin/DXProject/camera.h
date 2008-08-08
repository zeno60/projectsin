#ifndef camera_h
#define camera_h

#include "Application.h"
#include "IEventHandler.h"
#include "vector3d.h"
#include "vector2d.h"
#include "quaternion.h"

class Camera : public IEventHandler
{
public:
	enum Type
	{
		LANDOBJECT,
		AIRCRAFT,
		TRACKBALL
	};

	Camera();
	Camera(Type t);
	~Camera();

	void EventHandler(const Event &e);
	void update(float timeDelta);

	void strafe(float units);
	void fly(float units);
	void walk(float units);

	void pitch(float angle);
	void yaw(float angle);
	void roll(float angle);
	void rotate(float speed);
	
	bool isDragging;
	void beginDrag(int x, int y);
	void drag(int x, int y);

	vector2df ScreenToNDC(int x, int y);
	vector3df ProjectToSphere(vector2df ndc);

	void getViewMatrix(matrixf* m);
	matrixf getMatrix();

private:
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
};

#endif