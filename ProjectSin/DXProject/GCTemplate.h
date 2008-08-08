#ifndef GCTEMPLATE_HEADER
#define GCTEMPLATE_HEADER

#include "GOComponent.h"

class GCTemplate
{
public:
	GCTemplate(){}
	virtual ~GCTemplate() = 0 {}

	virtual const GOComponent::goc_id_type& componentId() const = 0;
	virtual const GOComponent::goc_id_type& familyId() const = 0;

	virtual GOComponent* makeComponent() = 0;
};

#endif