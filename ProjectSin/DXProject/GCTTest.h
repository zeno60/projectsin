#ifndef GCTTEST_HEADER
#define GCTTEST_HEADER

#include "GCTManager.h"
#include "GCTemplate.h"
#include "GOCTest.h"

class GCTTest : public GCTemplate
{
public:
	virtual const GOComponent::goc_id_type& componentId() const
	{
		return mComponentId;
	}

	virtual const GOComponent::goc_id_type& familyId() const
	{
		return mFamilyId;
	}

	virtual GOComponent* makeComponent()
	{
		GOCTest* goc = new GOCTest(this);
		return goc;
	}

	static void RegisterTemplate();
	static GCTemplate* CreateMe();

private:
	static GOComponent::goc_id_type mComponentId;
	static GOComponent::goc_id_type mFamilyId;
};

#endif