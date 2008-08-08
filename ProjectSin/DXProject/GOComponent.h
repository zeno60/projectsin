#ifndef GOCOMPONENT_H
#define GOCOMPONENT_H

#include <string>

class GameObject;

class GOComponent
{
public:
	typedef std::string goc_id_type;

	GOComponent():mParentGO(NULL){}
	virtual ~GOComponent() = 0 {}

	virtual const goc_id_type& componentId() const = 0;
	virtual const goc_id_type& familyId() const = 0;

	virtual void update(float timeDelta){}

	void setParentGO(GameObject* go){ mParentGO = go; }
	GameObject* getParentGO() const { return mParentGO; }

private:
	GameObject* mParentGO;
};

#endif