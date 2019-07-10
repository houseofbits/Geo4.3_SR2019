#ifndef _RESOURCEMANAGER_H_INCLUDED_
#define _RESOURCEMANAGER_H_INCLUDED_

#include <string>
#include <iostream>
#include <list>
#include <vector>

//using namespace std;

#include "ObjectFactory.h"
///////////////////////////////////////////////////////////////////////////////
//
//
class ResourceManager;
template<class T>
class ResourceHandle;

class Resource
{
friend class ResourceManager;
//friend class ResourceHandle;
public:
	Resource():name(""), refcount(0), resourceManager(0){}
	virtual ~Resource(){}

	virtual bool	Load(std::string name){return 0;}
	virtual void	Unload(){}

	virtual std::string		getExtensionStr(unsigned int n=0){return "";}
	virtual unsigned int	getNumExt(){return 0;}

	void					setName(std::string n){name = n;}
	std::string				getName(){return name;}
	bool operator () (Resource* obj){ return obj->name == name; } 

	Resource(std::string n):name(n), refcount(0), resourceManager(0){}
	std::string			name;
	int					refcount;
	ResourceManager*	resourceManager;
};

template<class T>
class ResourceHandle
{
friend class ResourceManager;
public:
	ResourceHandle():res(&T::m_DefaultInstance), mgr(0){}
	~ResourceHandle(){
		Invalidate();
	}
	
	typedef T DATA_TYPE;	//Resource class
	
	bool	isValid(){
		return (mgr!=0);
	}
	
	T* operator->(){
		if(!isValid())
			return &T::m_DefaultInstance;
		return res;
	}
	
	T* operator () (){ 		
		if(!isValid())
			return &T::m_DefaultInstance;
		return res;} 

	std::string		getName(){
		if(isValid())return res->getName();

		return "";
	}

//private:
	void			Invalidate(){	

		if(isValid()){
			res->refcount--;
			if(res->refcount<=0){
				mgr->Remove(res);
			}
		}
		mgr=0;
	}
	void			Validate(ResourceManager* m, T* r){mgr = m; res = r;}
	T*				res;
	ResourceManager*	mgr;
};


template<class T>
class ObjectFactory;

class ResourceManager 
{
public:
	ResourceManager():factory(),resources(){}

	template<class T>
		bool	Get(T& hand, std::string name){
			if(hand.isValid())hand.Invalidate();

				std::list<Resource*>::iterator pos = find_if (resources.begin(), resources.end(), Resource(name));  
				if(pos!=resources.end()){ //found, return 1

					hand.Validate(this, reinterpret_cast<T::DATA_TYPE*>(*pos));
					(*pos)->refcount++;
					return 1;
				}else{	//not found, load
					//construct object from extension
					std::string ext = Utils::GetFileExt(name);
					std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
					Resource* res = factory.ConstructObjectFromName(ext);
					if(!res){
						cout<<"Could not find resource constructor associated with '"<<ext<<"'"<<endl;
						return 0;
					}
					res->resourceManager = this;
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
	
	void	Remove(Resource* res){
		resources.remove(res);
		res->Unload();
		delete res;		
	}	

	template<class T>
	void	RegisterResourceType(T t){
		unsigned int num = t.getNumExt();
		if(num>1){
			for(unsigned int i=0; i<num; i++){
				factory.RegisterObject(t, t.getExtenionStr(i));	
			}
		}else{
			factory.RegisterObject(t, t.getExtenionStr());	
		}
	}
	
	void	PrintDebug();	

	ObjectFactory<Resource>		factory;
	std::list<Resource*>				resources;
};
/**

///////////////////////////////////////////////////////////////////////////////
//
//

//predefs
template<class T, class F>class Resource;
template<class T>class ResourceManager;


//handle
template<class T, class F>
class ResourceHandle
{
public:
	typedef T DATA_TYPE;
	typedef F INDEX_TYPE;
	typedef ResourceHandle<T,F> ReH;

	friend ResourceManager<ReH>;
	friend Resource<T, F>;
public:
	ResourceHandle():m_pResource(0)//, m_pManager(0)
	{}
	~ResourceHandle(){
	//	cout<<"~ResourceHandle"<<endl;
		if(isValid()){
			//shiit
			//m_pManager->
			m_pResource->DeleteHandle(this);
		}
	}


	//copy constructor
	ResourceHandle(ResourceHandle& res){
		if(res.isValid()){
			m_pResource = res.m_pResource;
		//	m_pManager = res.m_pManager;
			m_pResource->InsertHandle(this);
		}
	}


	//
	ResourceHandle<T, F>& operator=(ResourceHandle<T, F>& res){
		if(res.isValid()){
			m_pResource = res.m_pResource;
			m_pResource->InsertHandle(this);
		}
		return *this;
	}
	
	

	bool	isValid(){
		if(m_pResource)return 1;
		return 0;
	}

	void	Invalidate(){
		m_pResource=0;
	}
	
	T* operator->(){
		if(isValid()){
			return &m_pResource->GetData();
		}else{
			////////////////////////////////////////////////////////////////////////////
			//	dizaina probleema: peec idejas sheit buutu jaaatgriezh defaultais objekts
			//	tjipa ja buutu pieejams menedzera ptr tad: return mgr->default_obj;
			//	bet resurss nedrtiikst satureet mgr pointeri lai saglabaatu neatkariibu, taapeec ir vajadziigs cits veids
			//	kaa piekljuut defaultajam objektam.	Varbuut peec peec singletona principa kad preksh objekta defaultaas veertiibas 
			//	tiek defineeta instance. 
			return &T::m_pDefaultInstance;
		}
	}

private:
	ResourceHandle(Resource<T, F>* type) : m_pResource(type){	}
	Resource<T, F>*			m_pResource;
}; 


//resource
//note: this class is acessible only by ResourceManager, some day will make it nested

template<class T, class F>
class Resource              //...ouuu, dont touch me, you pervert...
{
	typedef ResourceHandle<T,F> ReH;

	friend ResourceManager<ReH>;
	friend ResourceHandle<T,F>;

private:
//	Resource() : m_HandleRegister(), m_ResourceData(), m_Index(){}
	Resource(T* dta, F index, ResourceManager<ReH>* rmgr) : m_ResourceData(dta), m_Index(index), m_pManager(rmgr){}
	virtual ~Resource(){
		delete m_ResourceData;
		for(list<ResourceHandle<T, F>*>::iterator i=m_HandleRegister.begin();i!=m_HandleRegister.end(); i++){
			InvalidateHandle(*i);
		}
		m_HandleRegister.clear();
	}

	void				InsertHandle(ResourceHandle<T, F>* handle){

		list<ResourceHandle<T, F>*>::iterator result=m_HandleRegister.begin();
			while(result!=m_HandleRegister.end()){
				if((*result)==handle)break;
				result++;
			}
			if(result==m_HandleRegister.end()){
			//cout<<"InsertHandle "<<handle<<endl;
	//	if(m_HandleRegister.find(handle)==m_HandleRegister.end()){
			m_HandleRegister.push_front(handle);
		}
	}

	void				DeleteHandle(ResourceHandle<T,F>* handle){
		m_HandleRegister.remove(handle);
	//	cout<<"DeleteHandle "<<m_Index<<endl;
		if(m_HandleRegister.empty()){
		//	cout<<"delete resource: "<<m_Index<<endl;
			//kill myself
			m_pManager->_RemoveResource(this);
		}
	}
	
	void				InvalidateHandle(ResourceHandle<T, F>* handle){
		if(handle){	handle->Invalidate();}	
	}

	bool operator== (const F& f){
		return (m_Index == f);
	}

	T&				GetData(){
		return *m_ResourceData;
	}
	
	void	PrintDebug(){
		cout<<"...data: "<<m_ResourceData<<endl;
		cout<<"...index: "<<m_Index<<endl;
		cout<<"...manager: "<<m_pManager<<endl;
			list<ResourceHandle<T, F>*>::iterator result=m_HandleRegister.begin();
			while(result!=m_HandleRegister.end()){
				
				cout<<"....handle: "<<(*result)<<endl;

				result++;
			}
	
	}

	//resurss nevar eksisteet bez shiim veertiibaam, ja kaada no shiim veertiibaam ir tuksha tad kaut kur ir notikusi 
	//kljuuda 

	list<ResourceHandle<T, F>*>	m_HandleRegister;
	//resursam jaazin kam vinsh pieder!
	ResourceManager<ReH>*		m_pManager;
	T*							m_ResourceData;	
	F							m_Index;
};

//remember: it takes only ResourceHandle as template type, otherwise it all will be screwed up. 
template<class T>
class ResourceManager
{
	typedef Resource<typename T::DATA_TYPE, typename T::INDEX_TYPE> ResourceType;

	friend	ResourceType;
public:

	void	CreateNewResource(typename T::DATA_TYPE* dta, typename T::INDEX_TYPE index){
		//mekleet vai resurss ar shaadiem datiem jau neeksistee
		m_ResourceList.push_front(new ResourceType(dta, index, this));
	}

	bool GetHandle(typename T::INDEX_TYPE index, T*& ptr)
	{
		list<ResourceType*>::iterator result=m_ResourceList.begin();
			while(result!=m_ResourceList.end()){
				if(*(*result)==index)break;
				result++;
			}
			if(result!=0 && result!=m_ResourceList.end()){
			//	cout<<"create handle"<<endl;
				if(ptr!=0){
				//	delete ptr;
				}	
				//	cout<<"GetHandle*"<<endl;
				//	ptr = _CreateHandleToResource((*result));
					_CreateHandleToResource((*result), *ptr);
					return 1;
			}
			return 0;
	}

	bool GetHandle(typename T::INDEX_TYPE index, T& ptr)
	{
		list<ResourceType*>::iterator result=m_ResourceList.begin();
			while(result!=m_ResourceList.end()){
				if(*(*result)==index)break;
				result++;
			}
			if(result!=m_ResourceList.end()){

				//	ptr = *(*result);
				//	cout<<"GetHandle"<<endl;
					_CreateHandleToResource((*result), ptr);
					return 1;
			}
			return 0;
	}

	//acess operator
	T* operator[](const typename T::INDEX_TYPE& index){
		T* ptr=0;
		bool res = GetHandle(index, ptr);
		if(res){
			return ptr;
		}
		return 0;
	}
	
	void	DebugPrint(){
		cout<<"-------------------------------------------------"<<endl;
		cout<<"manager: "<<this<<endl;
		list<ResourceType*>::iterator result=m_ResourceList.begin();
			while(result!=m_ResourceList.end()){
			cout<<"-------------------------------------------------"<<endl;
				cout<<"resource begin: "<<(*result)<<endl;	
				(*result)->PrintDebug();

 
				result++;
			}
	}

private:
	void	_RemoveResource(ResourceType* res){
	//	cout<<"_RemoveResource "<<res<<endl;
		m_ResourceList.remove(res);
		delete res;
	}

	void	_CreateHandleToResource(ResourceType* res, T& handle){
		if(res){
			//crap over here
		//	T* handle = new T(res);
			handle.m_pResource = res;
		//	cout<<"_CreateHandleToResource"<<endl;
			res->InsertHandle(&handle);
		//	return handle;
		}
		return;
	}

	list<ResourceType*>	m_ResourceList;
};

/**/



#endif