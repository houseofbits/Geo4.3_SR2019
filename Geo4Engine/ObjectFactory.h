#pragma once

#include <string>
#include <map>
#include <vector>
#include <algorithm>

template<class G>
class ObjectFactory  
{
private:
	class _VObjectConstructor{		//virtuaals konstruktors
		public:	virtual G*	Construct(){return 0;};
	};

	template<class T>				//konstruktors kas straadaa ar reaaliem objektiem
	class _RObjectConstructor : public _VObjectConstructor{
	public: G*	Construct(){return new T();};
	};

	class find_by_name 
	{ 
	private: 
		std::string name; 
	public: 
		find_by_name(std::string n) : name(n) {} 
		bool operator () (const std::pair<std::string, _VObjectConstructor*> &v) {return v.first==name;} 

	};
	typedef std::pair<std::string, _VObjectConstructor*> TObjNameReg;

public:
	ObjectFactory(){}
	virtual ~ObjectFactory(){}
	
	//vajag: paarbaude vai taads tips jau neeksistee!
	template<class T>
	void			RegisterObject(T object, std::string name){
		//	m_GConstructors.insert(std::map<std::string, _VObjectConstructor*>::value_type(name, new _RObjectConstructor<T>()));

			TObjNameReg objreg(name, new _RObjectConstructor<T>());
			mv_GConstructors.push_back(objreg);
	}	

	G* 	ConstructObjectFromName(std::string name){
		if(mv_GConstructors.empty())return 0;
		
		std::vector<TObjNameReg>::iterator pos = find_if (mv_GConstructors.begin(), mv_GConstructors.end(), find_by_name(name));   
		
		//if(/*pos==mv_GConstructors.end() && */(*pos).first==name || pos!=mv_GConstructors.end()){
		if(pos!=mv_GConstructors.end()){
			//cout<<&pos<<endl;
			return (*pos).second->Construct();
		}else return 0;
		


	//	if(m_GConstructors[name]==0)return 0;
	//	cout<<name<<endl;
	//	return m_GConstructors[name]->Construct();
	}

private: 

	//std::map<std::string, _VObjectConstructor*>	m_GConstructors;	//regjistrs
	

	std::vector<TObjNameReg> mv_GConstructors;
};
