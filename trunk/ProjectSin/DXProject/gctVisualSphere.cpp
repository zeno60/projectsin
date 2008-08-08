#include "gctVisualSphere.h"

GOComponent::goc_id_type gctVisualSphere::mComponentId = "gocVisualSphere";
GOComponent::goc_id_type gctVisualSphere::mFamilyId = "gocVisualSphere";

void gctVisualSphere::RegisterTemplate()
{
	GCTManager::getInstance()->registerTemplate(gctVisualSphere::CreateMe);
}

GCTemplate* gctVisualSphere::CreateMe()
{
	return new gctVisualSphere;
}