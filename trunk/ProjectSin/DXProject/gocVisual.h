#ifndef GOCVISUAL_HEADER
#define GOCVISUAL_HEADER

#include "D3DDevice.h"
#include "GOComponent.h"

class gocVisual : public GOComponent
{
public:
	virtual const GOComponent::goc_id_type& familyId() const { return mFamilyId; }
	virtual void render() const = 0;

private:
	static GOComponent::goc_id_type mFamilyId;
};

#endif