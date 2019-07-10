#include "ResourceManager.h"
#include <iostream>

using namespace std;
//#include <algorithm>
//#include "StringUtils.h"
//#include "TextureTGA.h"
//#include "Font.h"
//#include "Mesh.h"
//#include "AnimatedMesh.h"
//#include "GPUProgram.h"
//#include "Sound.h"
//bool (ResourceManager::*Function_Instantiation_TextureTGA)(TextureTGAHandle&, string name) = &ResourceManager::Get<TextureTGAHandle>;
//bool (ResourceManager::*Function_Instantiation_Font)(FontHandle&, string name) = &ResourceManager::Get<FontHandle>;

/*
bool (ResourceManager::*Function_Instantiation_TextureDDS)(TextureDDSHandle&, string name) = &ResourceManager::Get<TextureDDSHandle>;
bool (ResourceManager::*Function_Instantiation_TextureTGA)(TextureTGAHandle&, string name) = &ResourceManager::Get<TextureTGAHandle>;
bool (ResourceManager::*Function_Instantiation_Mesh)(MeshHandle&, string name) = &ResourceManager::Get<MeshHandle>;
//bool (ResourceManager::*Function_Instantiation_AnimMesh)(SkeletonMeshHandle&, string name) = &ResourceManager::Get<SkeletonMeshHandle>;
//bool (ResourceManager::*Function_Instantiation_GPUProgram)(GPUProgramHandle&, string name) = &ResourceManager::Get<GPUProgramHandle>;
//bool (ResourceManager::*Function_Instantiation_Sound)(SoundHandle&, string name) = &ResourceManager::Get<SoundHandle>;
//bool (ResourceManager::*Function_Instantiation_VBO)(VBOMeshHandle&, string name) = &ResourceManager::Get<VBOMeshHandle>;

template<class T>
bool ResourceManager::Get(T& hand, std::string name)
{		
	if(hand.isValid())hand.Invalidate();
	
	list<Resource*>::iterator pos = find_if (resources.begin(), resources.end(), Resource(name));  
	if(pos!=resources.end()){ //found, return 1

		hand.Validate(this, reinterpret_cast<T::DATA_TYPE*>(*pos));
		(*pos)->refcount++;
		return 1;
	}else{	//not found, load
		//construct object from extension
		string ext = Utils::GetFileExt(name);
		std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
		Resource* res = factory.ConstructObjectFromName(ext);
		if(!res){
			cout<<"Could not find resource constructor associated with '"<<ext<<"'"<<endl;
			return 0;
		}
		if(res->Load(name)){
				res->setName(name);
				resources.push_back(res);
				hand.Validate(this,  reinterpret_cast<T::DATA_TYPE*>(res));
				res->refcount++;
				return 1;
		}else{	
			cout<<"Could not load resource '"<<name<<"'"<<endl;
			delete res;	
			return 0;
		}
	}
	return 0;
}
*/
void ResourceManager::PrintDebug()
{	
	cout<<"-----------------------------------"<<endl;
	list<Resource*>::iterator pos = resources.begin();
	while(pos!=resources.end()){
		cout<<(*pos)->getName()<<" ref:"<<(*pos)->refcount<<endl;
		pos++;
	}
}


