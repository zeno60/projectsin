#include "GOTemplate.h"
#include <iostream>

GOTemplate::~GOTemplate()
{
}

void GOTemplate::clear()
{
	gct_list_t::iterator it = mTemplates.begin();
	for(it; it != mTemplates.end(); ++it) delete *it;
}

void GOTemplate::addGCTemplate(GCTemplate* gcTemplate)
{
	mTemplates.push_back(gcTemplate);
}

GCTemplate* GOTemplate::getGCTemplate(const GOComponent::goc_id_type &id) const
{
	gct_list_t::const_iterator it = mTemplates.begin();
	for(it; it != mTemplates.end(); ++it) if((*it)->componentId() == id) return *it;

	return NULL;
}