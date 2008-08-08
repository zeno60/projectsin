#include "GameObject.h"

GameObject::GameObject(const go_id_type &id)
{
	mId = id;
	//D3DXMatrixIdentity(&transform);
	transform.makeIdentity();
}

GOComponent* GameObject::getGOComponent(const GOComponent::goc_id_type &familyId)
{
	return mComponents[familyId];
}

GOComponent* GameObject::setGOComponent(GOComponent *newGOC)
{
	GOComponent::goc_id_type famId = newGOC->familyId();
	GOComponent* oldGOComponent = mComponents[famId];
	newGOC->setParentGO(this);
	mComponents[famId] = newGOC;
	return oldGOComponent;
}

void GameObject::clearGOComponents()
{
	component_table_type::iterator it;
	for(it = mComponents.begin(); it != mComponents.end(); it++)
	{
		delete it->second;
	}
	mComponents.clear();
}