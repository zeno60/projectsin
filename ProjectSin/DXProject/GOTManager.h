#ifndef GOTMANAGER_H
#define GOTMANAGER_H

#include "GameObject.h"
#include "GOTemplate.h"

class GOTManager
{
public:
	typedef std::list<GOTemplate*> got_list_t;

	~GOTManager();

	static GOTManager* getInstance();
	void registerTemplate(GOTemplate* got);
	void clear();
	GameObject* createGO(std::string tName);

private:
	got_list_t mTemplates;
	static GOTManager* mInstance;
};

#endif