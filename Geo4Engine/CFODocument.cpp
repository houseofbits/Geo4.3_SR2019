/********************************************************************
	created:	2004/11/27
	created:	27:11:2004   12:45	
	copyright:  Copyright (c) 2003-2004 Krists Pudzens (hobitz@zb.lv)
    author:		Hobits

	purpose:	Konfiguraacijas failu nolasiishana

*********************************************************************/

#include "CFODocument.h"
//#include "PFSFile.h"

using namespace std;

bool CFODocument::SaveFile(const string file)
{
	
//PFSFile* f = new PFSFile(file, PFSFile::F_WRITE);


	FILE *f = 0;
	fopen_s(&f, file.c_str(), "w");
	if (!f)return 0;

	string str;
	
	Generate(str);
	
//	f->Write((void*)str.c_str(), (int)str.size(), 1);
	fputs(str.c_str(), f);
	
	//delete f;	//close filehandle
fclose( f );

return 1;
} 

bool CFODocument::ReadFile(const string file)
{
/**
bool ret=0;
	PFSFile* f = new PFSFile(file, PFSFile::F_READ);
	if(f->ReadAll()>0){
		uint sz = f->GetSize();//sizeof(f->GetDataBuffer());
		string dta="";
		for(uint i=0; i<sz;++i){
			dta += f->GetDataBuffer()[i];
		}
		//string data((char*)f->GetDataBuffer());

		ret = Parse(dta);			
	}else cout<<"CFO Document '"<<file<<"' not found"<<endl;
	delete f;
	
/**/
	FILE *f = 0;
		
	fopen_s(&f, file.c_str(), "r");
   
	if (!f) {
		cout << "CFO Document '" << file << "' not found" << endl;
		return 0;
	}

	long length = 0;
	fseek( f, 0, SEEK_END );
	length = ftell( f);
	fseek( f, 0, SEEK_SET );

	if ( length == 0 )
	{
		fclose( f );
	return false;
	}

	string data;

	data.reserve(length);

	const int BUF_SIZE = 2048;
	char buf[BUF_SIZE];

	while( fgets( buf, BUF_SIZE, f ) )
	{
		data += buf;
	}

	fclose( f );


	return Parse(data);
	/**/
//	return ret;
}

bool CFODocument::Parse(string buff)
{

	if(!_EreaseComents(buff)){
		return 0;
	}
	_mName = "Document";
	
	if(_TestScopes(buff)){
		
		if(_ParseRecursive(buff,0) == buff.size()){
			_GenerateInheritanceRecursive();
			return 1;
		}
		else
		{
			return 0;
		}

	}
	else	{
		cout<<mError<<endl;
		return 0;
	}

return 1;
}

bool CFODocument::_TestScopes(const string &buff)
{
	int open=0;
	int close=0;

	for(uint i=0;i<buff.size();++i){
		if(buff[i]=='{')open++;
		if(buff[i]=='}')close++;
	}
	if(open!=close)	{
		mError = "Parse failed: Missing scopes";
		return 0;
	}
	
return 1;
}

void CFONode::_ParseNameInheritage(std::string& name, std::string& inhName) {
	std::string::size_type substring_start(0);
	std::string::size_type separator_index(0);
	separator_index = name.find(':', substring_start);
	if (separator_index < name.size()) {
		inhName = name.substr(separator_index + 1);
		name = name.substr(substring_start, separator_index - substring_start);
	}	
}

uint CFONode::_ParseRecursive(string& buff,int start)
{		
	string line = "";
	string name = "";
	string inherit_name = "";
	string param = "";

	CFONode*	child=0;

	for(uint i=start;i<buff.size();++i)
	{
		//if(buff[i]!='\n')cout<<buff[i];


	//	if(buff[i]==' ' || buff[i]=='\n' || buff[i]=='\\' || buff[i]=='\t'){}
		//else
//		if(buff[i]!='\b' && buff[i]!='\n' && buff[i]!='\\' && buff[i]!='\t')
		//{
		
			
	
			if(buff[i]=='{')
			{	
				child = new CFONode("","",this);	
				
				//cout << "name: " << name << endl;

				//TODO parse inherited params

				if(!name.empty()){
					_ParseNameInheritage(name, inherit_name);
					child->_mName = name;
					if (!inherit_name.empty())child->_mInheritedName = inherit_name;
					name = "";
					inherit_name = "";
				}
				
				i = child->_ParseRecursive(buff,i+1);
				
				_InsertNode(child);
				//child = 0;
				//mNodes.push_back(child);
			}else if(buff[i]=='}')
			{	
				if(!line.empty())
				{
					_InsertNode(new CFONode(line,"", this));
				//	mNodes.push_back(new CFGNode(line,""));	
				}
				return i;
			}
			else{
				if(buff[i]=='=' && !line.empty())
				{	
					name = line;
					line = "";
					
				}
				if(buff[i]==';' && !line.empty())
				{
					param = line;
					
					//cout << name << " = " << param << endl;

					if(!name.empty()){

						if (name == "include") {
							CFODocument* doc = new CFODocument();
							if (doc->ReadFile(param)) {
								for (unsigned int i = 0; i < doc->_mNodes.size(); i++){
									doc->_mNodes[i]->_mParentNode = this;
									_InsertNode(doc->_mNodes[i]);
								}
								doc->_mNodes.clear();
							}
							delete doc;
						}
						else {
							_InsertNode(new CFONode(name, param, this));
							//mNodes.push_back(new CFGNode(name,param));
						}
						name = "";
						param = "";
					}

					line = "";
				}
				else if(buff[i]!='=' && buff[i]!=';'){
					line += buff[i];
				}
			}
		//}
	
	
	}

	return (unsigned int)buff.size();
}

void PShift(uint sh)
{
	for(uint i=0;i<sh;++i){
		cout<<" ";
	}
}

void  CFONode::_GenerateInheritanceRecursive() {
	
	if (!_mInheritedName.empty() && _mParentNode) {
		CFONode* n = _mParentNode->GetFirstChild(_mInheritedName);
//		cout <<this<< " Extended from " <<n<<" : "<< _mInheritedName << endl;
		if (n)_mInheritedNode = n;
		else _mInheritedName = "";
	}
	for (uint i = 0; i < _mNodes.size(); ++i)
		_mNodes[i]->_GenerateInheritanceRecursive();
}

void CFONode::RecursiveDebugOutput(int shift)
{	
	if(!_mName.empty()){
		PShift(shift);
		cout<<"node: "<<_mName;
	}
	else
	{
		PShift(shift);
		cout<<"node: unnamed";
	}
	
	if(!_mValue.empty()){
		cout<<", value: "<<_mValue<<endl;
	}else{
		cout<<endl;
	}

//	cout<<_mParentNode<<endl;

	for(uint i=0; i<_mNodes.size();++i){

		_mNodes[i]->RecursiveDebugOutput(shift+1);
	}
}

bool CFODocument::_EreaseComents(string& buff)
{
string	str = "";
bool	eatline=0;
int 	eattext = 0;

int im1=0;
int im2=0;

	for(uint i=0;i<buff.size();i++)
	{	
		//if(buff[i]=='\r')buff[i]='\0';

		if(i+1 <= (unsigned int)buff.size()){
			im1 = i+1;
		}else{		im1 = i;}
		
		if(i+2 <= (unsigned int)buff.size()){
			im2 = i+2;
		}else{		im2 = i;}

		if(buff[i]=='/' && buff[im1]=='/')
		{
			eatline=1; 
		}
		if(buff[i]=='/' && buff[im1]=='*')
		{
			eattext += 1;
		}

		if(eattext && buff[i]=='*' && buff[im1]=='/')
		{
			eattext -= 1; 
			i = im2;
		}

		if(eatline && buff[i]=='\n'){
			eatline=0;
		}
		
		if(!eatline && eattext==0 && buff[i]!='\r')
		{
			str += buff[i];
		}
	}
	buff = str;
		
	str="";
	int 	eatspaces = 1;
	bool	quoteopen=0;

	for(uint i=0;i<buff.size();i++)
	{	
		im1 = (i+1 <= (unsigned int)buff.size()) ? (i+1) : i;
		im2 = (i-1>=0) ? (i-1) : i;

		if(buff[i]=='\\' && buff[im1]=='\"'){
			str+='"';
			i++;
		}else{

			if(buff[i]=='"' && buff[im2]!='\\')quoteopen=!quoteopen;
			
			eatspaces = (quoteopen)? 0:1;

			if(eatspaces==0  && buff[i]!='\"'){
				str += buff[i];
			}else if(buff[i]!=' ' && buff[i]!='\n' && buff[i]!='\\' && buff[i]!='\t' && buff[i]!='\"'){
				str += buff[i];
			}
		}
	}
	buff = str;

return 1;
}

CFONode* CFONode::GetFirstChild()
{	
	if(_mNodes.empty())return 0; 
	CFONode* node = *_mNodes.begin();
	if(node)
	{
		return node;
	}

return 0;
}

CFONode* CFONode::GetNextNode()
{		
return _FindNextNode(this);
}

CFONode* CFONode::GetFirstChild(string name)
{
	for (uint i=0; i<_mNodes.size();++i)
	{
		if(_mNodes[i]->_mName == name)
		{
			return _mNodes[i];
		}
	}
	//If node is not found, get inherited node
	if (_mInheritedNode) {
		return _mInheritedNode->GetFirstChild(name);
	}
return 0;
}

void CFONode::_InsertNode(CFONode* node)
{
	if(node)
	{
		
		node->_mThisId = (int)_mNodes.size();
		_mNodes.push_back(node);
	//	mNodeIterator = &mNodes.end();
	}
}

CFONode* CFONode::_FindNextNode(CFONode* node)
{

if(_mParentNode && node)
{
//	if(_mThisId>=0)
//	{
		if(unsigned(_mThisId+1) < (unsigned int)_mParentNode->_mNodes.size())
		{
			return _mParentNode->_mNodes[((unsigned int)_mThisId + 1)];
		}

//	}
/*	TCFGNodeList::iterator iter;
	bool ret=0;
	
	if(*mNodeIterator!=mParentNode->mNodes.end())
	{
		return **mNodeIterator+1;
	}
	/*
	for (iter=mParentNode->mNodes.begin(); iter!=mParentNode->mNodes.end();iter++)
	{
		if(ret){
			return *iter;
		}
		if(node==*iter){
			ret=1;
		}
	}*/
}

return 0;
}

void CFONode::_Print(string& str)
{
	if(!_mName.empty()){
		str += _mName;
	}
	if(!_mValue.empty()){
		str+="="+_mValue+";\n";
	}
}

void CFONode::_GenerateRecursive(string& str, string indent)
{
//	if(_mNodes.empty() && _mValue.empty())return;

	str+=indent;
	_Print(str);
	if(_mNodes.size()>0 && _mValue.empty()){
		if(_mName.empty())str += "{";
		else str += "={\n";

		for(uint i=0; i<_mNodes.size();i++){
			_mNodes[i]->_GenerateRecursive(str, indent+"\t");	
		}
		if(_mName.empty())str = str + "}\n";
		else str = str + indent+"}\n";
	}
}	

bool CFODocument::Generate(string& str)
{	
	if(_mNodes.size()>0){
		string indent="";
		for(uint i=0; i<_mNodes.size();i++){
			_mNodes[i]->_GenerateRecursive(str, indent);	
		}
	}
	return 0;
}

CFONode* CFONode::CreateChildNode()
{
	CFONode* node = new CFONode("","",this);
	_InsertNode(node);
return node;
}

CFONode* CFONode::CreateChildNode(string name)
{
	CFONode* node = new CFONode(name,"",this);
	_InsertNode(node);
return node;
}

CFONode* CFONode::CreateChildNode(string name, string value)
{
	CFONode* node = new CFONode(name,value,this);
	_InsertNode(node);
return node;
}

CFONode*	CFONode::CreateRawDataNode(std::string data_string)
{
	CFONode* node = new CFONode(data_string, "", this);
	node->_mRawData = 1;
	_InsertNode(node);
return node;
}

void CFODocument::PrintDebug()
{
	_PrintDebug(this, 0);
}
void CFODocument::_PrintDebug(CFONode* parent, int indent)
{
	for(int i=0; i<indent; i++)cout<<" ";
	cout<<parent->GetName()<<" = "<<parent->GetValue()<<endl;
	if(parent->_mNodes.empty())return;
	CFONode* node = parent->GetFirstChild();
	while(node){
		_PrintDebug(node, indent+1);
		node=node->GetNextNode();
	}
}