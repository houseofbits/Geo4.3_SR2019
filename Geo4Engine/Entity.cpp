#include "Geo4.h"
//#include "CFODocument.h"

using namespace std;

unsigned int Entity::GLOBAL_OBJECT_TYPE_COUNTER = 0;

void Entity::Deserialize(CFONode* node)
{	
	node->getValueString("name", m_Name);
	m_HashName = hashStr(m_Name.c_str());
}

SceneManager*	Entity::getSceneManager()
{
	Entity* ent = getRootObject();
	if(ent->getTypeId() == SceneManager::type_id){
		return (SceneManager*)ent;
	}
	return 0;
}

void	Entity::SaveObjects(CFONode* node, bool include_parent)
{
	SceneManager* mgr = getSceneManager();	
	if(!mgr || !node)return;
	if(include_parent){
		CFONode* n = node->CreateChildNode(getTypename(), "");
		Serialize(n);
		_RecursiveSerializeObject(n, this, mgr);
	}else{
		_RecursiveSerializeObject(node, this, mgr);
	}
}

void	Entity::CreateObjects(CFONode* node)
{	
//	cout<<"Entity::createobjects"<<endl;
	SceneManager* mgr = getSceneManager();	
	if(!mgr || !node)return;
	TEntityArray	obj_init_array;
	//cout<<"Scene Mgr "<<mgr<<endl;
	string classname = node->GetName();
	Entity* obj = mgr->object_factory.ConstructObjectFromName(classname);
	if(obj){
		//cout<<"Root object "<<obj<<endl;
		obj->setParent(this);
		addChildObject(obj);
		obj->Deserialize(node);
		//cout<<"Deserialize "<<obj<<endl;
		//save created objects for initiation
		obj_init_array.push_back(obj);
		//cout<<"'"<<classname<<"'"<<endl;	
		_RecursiveDeserializeChilds(node, obj, mgr, &obj_init_array);
	//}else if(classname=="require"){	//Obsolette
	//	_RecursiveDeserializeChilds(&CFODocument(node->GetValue()), this, mgr, &obj_init_array);
	}else{
		//cout<<"Child object"<<endl;
		_RecursiveDeserializeChilds(node, this, mgr, &obj_init_array);
	}
//	cout<<"Start init"<<endl;
	_RecursiveInitialiseChilds(this, mgr, &obj_init_array);
	//_RecursiveSortByZIndex();

	//TODO Send [Object created] event
}

void	Entity::_RecursiveDeserializeChilds(CFONode* node, Entity* parent, SceneManager* mgr, TEntityArray* init_a)
{
//	cout<<"Entity::_RecursiveDeserializeChilds "<<parent<<endl;
//	parent->Deserialize(node, &mgr->resource_manager);
	if(!node)return;
	CFONode* object = node->GetFirstChild();
	while(object){			
		string classname = object->GetName();
		Entity* obj = mgr->object_factory.ConstructObjectFromName(classname);
		if(obj){
			obj->setParent(parent);
			parent->addChildObject(obj);
			obj->Deserialize(object);
			//save created objects for initiation
			if(init_a)init_a->push_back(obj);
			string name = "";
			if(object->getValueString("name", name))
				cout<<"'"<<classname<<"' : '"<< name <<"'"<<endl;
			else
				cout << "'" << classname << "'"<< endl;
			_RecursiveDeserializeChilds(object, obj, mgr, init_a);
		}else if(classname=="require"){
			_RecursiveDeserializeChilds(&CFODocument(object->GetValue()), parent, mgr, init_a);
		}
		object = object->GetNextNode();
	}
}

void	Entity::_RecursiveSerializeObject(CFONode* node, Entity* parent, SceneManager* mgr)
{
	if(!node)return;
	if(!parent || parent->m_Childs.empty())return;
	list<Entity*>::iterator pos = parent->m_Childs.begin();
	while(pos!=parent->m_Childs.end())
	{	
		CFONode* n = node->CreateChildNode((*pos)->getTypename(), "");
		(*pos)->Serialize(n);
		_RecursiveSerializeObject(n, (*pos), mgr);
		pos++;	
	}
}

void	Entity::_RecursiveInitialiseChilds(Entity* parent, SceneManager* mgr, TEntityArray* init_a)
{	
//	cout<<getTypename()<<endl;

	//init only specified objects
	if(init_a){	
		for(unsigned int i=0; i<init_a->size(); i++){
		//	cout<<" Init "<<(*init_a)[i]<<endl;
			(*init_a)[i]->Initialise(&mgr->event_dispatcher, &mgr->resource_manager);
		}
		return;
	}
	//else init all subobjects
	if(!parent || parent->m_Childs.empty())return;
	list<Entity*>::iterator pos = parent->m_Childs.begin();
	while(pos!=parent->m_Childs.end())
	{
		(*pos)->Initialise(&mgr->event_dispatcher, &mgr->resource_manager);
		_RecursiveInitialiseChilds((*pos), mgr);
		pos++;	
	}
}

void	Entity::setName(std::string name)
{
	m_Name = name; 
	m_HashName = hashStr(m_Name.c_str());
}