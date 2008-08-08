#ifndef GCTMANAGER_H
#define GCTMANAGER_H

#include <map>
#include "GCTemplate.h"
#include "GOComponent.h"

class GCTManager
{
public:
	typedef std::map<const GOComponent::goc_id_type, GCTemplate*> gct_table_type;
	typedef GCTemplate* (*gctCreationMethod)(void);

	static GCTManager* getInstance();
	void registerTemplate(GCTemplate* gct);
	void registerTemplate(gctCreationMethod gctCreateMe);
	void clear();
	GOComponent* createGOC(GOComponent::goc_id_type &compid);

private:
	gct_table_type mTemplates;
	static GCTManager* mInstance;
};

#endif