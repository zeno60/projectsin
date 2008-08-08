#include "GOTManager.h"

GOTManager* GOTManager::mInstance = NULL;

GOTManager::~GOTManager()
{
}

GOTManager* GOTManager::getInstance()
{
	if(!mInstance) mInstance = new GOTManager();
	return mInstance;
}


void GOTManager::clear()
{
	got_list_t::iterator it = mTemplates.begin();
	for(it; it != mTemplates.end(); it++)
	{
		(*it)->clear();
		delete *it;
	}
}

void GOTManager::registerTemplate(GOTemplate* got)
{
	mTemplates.push_back(got);
}

GameObject* GOTManager::createGO(std::string tName)
{
	got_list_t::iterator it = mTemplates.begin();
	for(it; it != mTemplates.end(); it++)
	{
		if((*it)->name() == tName)
		{
			GameObject* go = new GameObject(tName);
			GOTemplate::gct_list_t components = (*it)->getTemplates();
			GOTemplate::gct_list_t::iterator i = components.begin();
			for(i; i != components.end(); i++)
			{
				go->setGOComponent((*i)->makeComponent());
			}
			return go;
		}
	}
	return NULL;
}