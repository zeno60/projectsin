#include "GCTManager.h"

GCTManager* GCTManager::mInstance = NULL;

GCTManager* GCTManager::getInstance()
{
	if(!mInstance) mInstance = new GCTManager();
	return mInstance;
}

void GCTManager::clear()
{
	gct_table_type::iterator it;
	for(it = mTemplates.begin(); it != mTemplates.end(); it++)
	{
		delete it->second;
	}
	mTemplates.clear();
}

void GCTManager::registerTemplate(GCTemplate* gct)
{
	mTemplates[gct->componentId()] = gct;
}

void GCTManager::registerTemplate(gctCreationMethod gctCreateMe)
{
	GCTemplate* gct = gctCreateMe();
	registerTemplate(gct);
}

GOComponent* GCTManager::createGOC(GOComponent::goc_id_type &compid)
{
	return mTemplates[compid]->makeComponent();
}