/********************************************************************
	created:	2004/11/27
	created:	27:11:2004   12:45	
	copyright:  Copyright (c) 2003-2019 Krists Pudzens (hobitz@zb.lv)
    author:		Hobitz

	purpose:	Konfiguraacijas failu nolasiishana

	edited 2005/11/05: Korekti tiek nolasiits teksts objekta parametros.
	            Tukshumu dzeeshana ir paarnesta uz _EreaseComents
	edited 2005/11/12:	Paardeeveeti objekti uz CFO (Configuration Object)
	edited 2019/03/30:	CFO failu inklûdoðana ar [include]

	todo:	
	bool setParamT(string name, T value, bool create=1);
	return: true if exists, false if does not exist or created

	bool getParamT(string name, T& value);
	return: true if exists, false otherwise

	Unicode charakteri

	TODO paramu extendoðana, topParam = thisParam->parent->firstChild(inheritedParam);
		topParam={
			overwriteTop=0;
			randomStuff=1;
		}
		someParam:topParam={
			overwriteTop=1;
		}


*********************************************************************
	
	skriptu sintakse:

		node=
		{
			subnode=
			{
				subparam=pamparam;
			}

			param=1.55;
		}

		globalparam=tralalaa;

*********************************************************************/
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <list>
#include <vector>
#include "math/HVector2.h"
#include "math/HVector3.h"
#include "math/HMatrix4.h"
#include "StringUtils.h"

#ifndef uint
#define uint unsigned int
#endif

class CFODocument;					

class CFONode
{
public:
	CFONode() : _mRawData(0), _mInheritedName(""),  _mName(""), _mParentNode(0), _mValue(""), _mNodes(), _mThisId(-1), _mInheritedNode(0){}
	CFONode(std::string name, std::string param) : _mRawData(0), _mInheritedName(""), _mName(name), _mParentNode(0), _mValue(param), _mNodes(), _mThisId(-1), _mInheritedNode(0) {}
	CFONode(std::string name, std::string param, CFONode* parent) : _mRawData(0),_mName(name), _mInheritedName(""), _mParentNode(parent), _mValue(param), _mNodes(), _mThisId(-1), _mInheritedNode(0) {}
	virtual ~CFONode(){
		for (uint i=0;i<_mNodes.size();++i){delete _mNodes[i];}
		_mNodes.clear();
	}
private:
	friend class CFODocument;
	typedef std::list<CFONode*> TCFONodeList;

public:
	inline std::string			GetValue(){
		return _mValue;
	}
	inline std::string			GetName(){
		return _mName;
	}
	CFONode*		GetFirstChild(const std::string);			//sanjem pirmo beernu listaa ar doto vaardu
	CFONode*		GetFirstChild();						//sanjem pirmo beernu listaa
	CFONode*		GetNextNode();							//sanjem naakamo elementu aiz dotaa
	CFONode*		CreateChildNode();		
	CFONode*		CreateChildNode(std::string name);
	CFONode*		CreateChildNode(std::string name, std::string value);
	CFONode*		CreateRawDataNode(std::string data_string);

	inline void		SetValue(const std::string str){
		_mValue = str;
	}
	inline	void	SetName(const std::string str){
		_mName = str;
	}

	void			RecursiveDebugOutput(int);
	
	bool		setValueVector2(const std::string name, Vector2& val){
		CFONode* node = GetFirstChild(name);
		if(node){
			node->SetValue(Utils::VectorToString(val));
			return true;
		}
		return false;
	}

	bool				getValueBool(const std::string name, bool& val){
		CFONode* node = GetFirstChild(name);
		if(node){
			val =  (atoi(node->GetValue().c_str())>0)?true:false;
			return true;
		}
		return false;
	}

	bool				getValueInt(const std::string name, int& val){
		CFONode* node = GetFirstChild(name);
		if(node){
			val =  (int)atoi(node->GetValue().c_str());
			return true;
		}
		return false;
	}
	bool				getValueInt(const std::string name, unsigned int& val){
		CFONode* node = GetFirstChild(name);
		if(node){
			val =  (unsigned int)atoi(node->GetValue().c_str());
			return true;
		}
		return false;
	}
	bool	getValueFloat(const std::string name, float& val){
		CFONode* node = GetFirstChild(name);
		if(node){
			val = (float)atof(node->GetValue().c_str());
			return true;
		}
		return false;
	}
	bool	getValueString(const std::string name, std::string& val){
		CFONode* node = GetFirstChild(name);
		if(node){
			val = node->GetValue();
			return true;
		}
		return false;
	}
	bool	getValueVector2(const std::string name, Vector2& val){
		CFONode* node = GetFirstChild(name);
		if(node){
			val =  Utils::StringToVector2(node->GetValue());
			return true;
		}
		return false;
	}
	bool	getValueVector3(const std::string name, Vector3& val){
		CFONode* node = GetFirstChild(name);
		if(node){
			val =  Utils::StringToVector3(node->GetValue());
			return true;
		}
		return false;
	}
	bool	getValueVector4(const std::string name, Vector4& val){
		CFONode* node = GetFirstChild(name);
		if(node){
			val =  Utils::StringToVector4(node->GetValue());
			return true;
		}
		return false;
	}
	bool	getValueMatrix4(const std::string name, Matrix4& val){
		CFONode* node = GetFirstChild(name);
		if(node){
			CFONode* value = node->GetFirstChild();
			//std::cout<<value->GetName()<<std::endl;
			if(value){
				Matrix4 m4 = Utils::StringToMatrix4(value->GetName());			
				val = m4;  
			}
			return true;
		}
		return false;
	}
private:
	uint			_ParseRecursive(std::string&,int);
	void			_GenerateRecursive(std::string&,std::string);
	void			_GenerateInheritanceRecursive();
	void			_InsertNode(CFONode* node);
	CFONode*		_FindNextNode(CFONode*);
	void			_Print(std::string&);

	void			_ParseNameInheritage(std::string&, std::string&);

	std::string			_mInheritedName;
	std::string			_mName;									
	std::string			_mValue;
	
	CFONode*			_mInheritedNode;

	std::vector<CFONode*>	_mNodes;
	int						_mThisId;
	CFONode*				_mParentNode;

	bool				_mRawData;
};

class CFODocument : public CFONode
{
public:
	CFODocument() : mError(""), CFONode(){

	}
	CFODocument(std::string filename) : mError(""), CFONode(){
		ReadFile(filename);
	}
	~CFODocument(){
		mError.erase();
	}
	bool	ReadFile(const std::string);							//izveido dokumentu no CFile 
	bool	SaveFile(const std::string);

	bool	Parse(std::string);									//konstruee dokumentu no teksta
	bool	Generate(std::string&);
	

	void	PrintDebug();
    
private:
	void	_PrintDebug(CFONode*, int);
	bool	_EreaseComents(std::string&);						//dzeesh komentaarus no bufereetaa teksta
	bool	_TestScopes(const std::string&);						

	std::string  mError;
};

