#ifndef GOCTest_H
#define GOCTest_H

#include "GOComponent.h"
#include "IEventHandler.h"

class GCTTest;

class GOCTest : public GOComponent
{
public:
	//Component Interface
	virtual const goc_id_type& componentId() const { return mComponentId; }
	virtual const goc_id_type& familyId() const { return mFamilyId; }

	//Test Interface
	GOCTest(){};
	GOCTest(GCTTest* gct);

	void update(float timeDelta);

private:
	static goc_id_type mComponentId;
	static goc_id_type mFamilyId;
};

#endif