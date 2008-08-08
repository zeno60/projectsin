#include "gocCamera.h"
#include "Application.h"

GOComponent::goc_id_type gocCamera::mComponentId = "gocCamera";
GOComponent::goc_id_type gocCamera::mFamilyId = "gocCamera";

gocCamera::gocCamera()
{
	type = Type::LANDOBJECT;

	position = vector3df(0.0f, 0.0f, -10.0f);
	right = vector3df(1.0f, 0.0f, 0.0f);
	up = vector3df(0.0f, 1.0f, 0.0f);
	look = vector3df(0.0f, 0.0f, 1.0f);

	isDragging = false;
}

void gocCamera::update(float timeDelta)
{
	walk(-1.0 * timeDelta);
}

void gocCamera::beginDrag(int x, int y)
{
	startVector = ProjectToSphere(ScreenToNDC(x, y));
	startQuat = currentQuat;
	isDragging = true;
}

void gocCamera::drag(int x, int y)
{
	if(!isDragging) return;

	currentVector = ProjectToSphere(ScreenToNDC(x, y));

	vector3df axis = startVector.crossProduct(currentVector);
	double theta = acos(startVector.dotProduct(currentVector));

    quaternion delta(axis.x, axis.y, axis.z, -theta);

	currentQuat = delta * startQuat;
}

vector2df gocCamera::ScreenToNDC(int mx, int my)
{
	float x = 0.0f;
	float y = 0.0f;
	
	int width = Instance()->mWidth;
	int height = Instance()->mHeight;

	x = (2.0f * mx - width) / (float)width;
	y = (2.0f * my - height) / (float)height;

	x = (x + 1.0f)/2.0f;
	y = (y + 1.0f)/-2.0f;

	vector2df vector(x, y);
	position2d = vector;

	return vector;
}

vector3df gocCamera::ProjectToSphere(vector2df ndc)
{
	vector3df theVector(ndc.x, ndc.y, 0.0f);
    float theLength = ndc.length();

    // If the mouse coordinate lies outside the sphere
    // choose the closest point on the sphere by 
    // setting z to zero and renomalizing
    if (theLength >= 1.0f)
    {
        theVector.x /= (float)sqrt(theLength);
        theVector.y /= (float)sqrt(theLength);
    }
    else
    {
        theVector.z = 1.0f - theLength;
    }
	
	//D3DXVec3Normalize(&theVector, &theVector);
    return theVector;
}

void gocCamera::strafe(float units)
{
	if(type == LANDOBJECT) position += vector3df(right.x, 0.0f, right.z) * units;
    if(type == AIRCRAFT) position += right * units;
}

void gocCamera::fly(float units)
{
	if(type == AIRCRAFT) position += up * units;
}

void gocCamera::walk(float units)
{
     if(type == LANDOBJECT) position += vector3df(look.x, 0.0f, look.z) * units;
     if(type == AIRCRAFT) position += look * units;
}

void gocCamera::roll(float angle)
{
     if(type == AIRCRAFT)
     {
          matrixf t;
		  t.makeIdentity();
		  t.setRotationAxis(look,angle);
		  
		  t.transform(right);
		  t.transform(up);

          //D3DXVec3TransformCoord(&_right, &_right, &T);
          //D3DXVec3TransformCoord(&_up, &_up, &T);
     }
}

void gocCamera::yaw(float angle)
{
     matrixf t;
	 t.makeIdentity();
     
	 if(type == LANDOBJECT) t.setRotationY(angle);
     if(type == AIRCRAFT) t.setRotationAxis(up,angle);
     
	 t.transform(right);
	 t.transform(look);
}

void gocCamera::pitch(float angle)
{
     matrixf t;
	 t.makeIdentity();

	 t.setRotationAxis(right,angle);

	 t.transform(up);
	 t.transform(look);
}

void gocCamera::rotate(float speed)
{
	vector3df view = position - look;

	position.z = (float)(look.z + sin(speed)*view.x + cos(speed)*view.z);
	position.x = (float)(look.x + cos(speed)*view.x - sin(speed)*view.z);
}

void gocCamera::getViewMatrix(matrixf* m)
{
	//D3DXMatrixLookAtLH(V, &_pos, &_look, &_up);
	
	look.normalize();

	up = look.crossProduct(right);
	up.normalize();

	right = up.crossProduct(look);
	right.normalize();

	// Build the view matrix

	float x = -right.dotProduct(position);
	float y = -up.dotProduct(position);
	float z = -look.dotProduct(position);
	
	(*m)[ 0] = right.x;
	(*m)[ 1] = up.x;
	(*m)[ 2] = look.x;
	(*m)[ 3] = 0.0f;
	(*m)[ 4] = right.y;
	(*m)[ 5] = up.y;
	(*m)[ 6] = look.y;
	(*m)[ 7] = 0.0f;
	(*m)[ 8] = right.z;
	(*m)[ 9] = up.z;
	(*m)[10] = look.z;
	(*m)[11] = 0.0f;
	(*m)[12] = x;
	(*m)[13] = y;
	(*m)[14] = z;
	(*m)[15] = 1.0f;
}

matrixf gocCamera::getMatrix()
{
	quaternion theRotation(currentQuat.x, currentQuat.y, currentQuat.z, currentQuat.w);
	theRotation.normalize();
    return theRotation.getMatrix();
}