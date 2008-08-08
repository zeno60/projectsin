#ifndef GCTVISUALSPHERE_HEADER
#define GCTVISUALSPHERE_HEADER

#include "GCTManager.h"
#include "GCTemplate.h"
#include "gocVisualSphere.h"

class gctVisualSphere : public GCTemplate
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
		gocVisualSphere* goc = new gocVisualSphere(8);
		return goc;
	}

	static void RegisterTemplate();
	static GCTemplate* CreateMe();

public:
	static GOComponent::goc_id_type mComponentId;
	static GOComponent::goc_id_type mFamilyId;
};

#endif