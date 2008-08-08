#include "GCTemplate.h"
#include <list>

class GOTemplate
{
public:
	typedef std::list<GCTemplate*> gct_list_t;;
	
	~GOTemplate();

	void clear();

	const std::string& name() const { return mName; }
	void setName(const std::string& name) { mName = name; }

	gct_list_t& getTemplates() { return mTemplates; }

	void addGCTemplate(GCTemplate* gcTemplate);
	GCTemplate* getGCTemplate(const GOComponent::goc_id_type& id) const;

	GOTemplate(const std::string& name):mName(name){ }

protected:
	
	std::string mName;
	gct_list_t mTemplates;
};