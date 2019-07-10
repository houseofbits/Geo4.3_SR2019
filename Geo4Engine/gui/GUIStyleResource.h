#pragma once
#include "ResourceManager.h"
#include <string>

class GUIStyleResource :
	public Resource
{
public:
	GUIStyleResource();
	virtual ~GUIStyleResource();

	bool    Load(std::string);
	void	Unload() { }

	std::string		getExtenionStr(unsigned int n = 0) { return "cfo"; }
	unsigned int	getNumExt() { return 1; }

	typedef std::pair<string, GUIStyle*> StyleEntry;

	GUIStyle& get(string);

	vector<StyleEntry> styles;

	static GUIStyleResource m_DefaultInstance;

	static GUIStyle defaultStyle;
};

typedef ResourceHandle<GUIStyleResource> GUIStyleResourceHandle;