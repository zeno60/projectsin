#include "GCTTest.h"
#include "GCTManager.h"

GOComponent::goc_id_type GCTTest::mComponentId = "GOCTest";
GOComponent::goc_id_type GCTTest::mFamilyId = "GOCTest";

void GCTTest::RegisterTemplate()
{
	GCTManager::getInstance()->registerTemplate(GCTTest::CreateMe);
}

GCTemplate* GCTTest::CreateMe()
{
	return new GCTTest;
}