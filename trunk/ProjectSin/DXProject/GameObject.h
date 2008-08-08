#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <map>

#include "GOComponent.h"
#include "D3DDevice.h"
#include "matrix4.h"

typedef std::string go_id_type;

class GameObject
{
public:
	GameObject(const go_id_type& id);

	const go_id_type& getID() const { return mId; }
	void setId(const go_id_type& id) { mId = id; }

	GOComponent* getGOComponent(const GOComponent::goc_id_type& familyId);
	GOComponent* setGOComponent(GOComponent* newGOC);
	void clearGOComponents();
	
	const matrixf& getGOTransformation() const { return transform; }
	void setGOTransformation(const matrixf& mtx) { transform = mtx; }

	void setPosition(vector3df pos) { transform.setTranslation(pos); }
	void setRotation(vector3df rotation) { transform.setRotation(rotation); }

private:
	go_id_type mId;

	matrixf transform;

	typedef std::map<const GOComponent::goc_id_type, GOComponent*> component_table_type;
	component_table_type mComponents;
};

#endif