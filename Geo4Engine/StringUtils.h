#pragma once

#include <string>
#include <ostream>
#include <sstream>
#include <windows.h>
#include "math/HVector2.h"
#include "math/HVector3.h"
#include "math/HVector4.h"
#include "math/HMatrix4.h"
#include "math/HQuaternion.h"

using namespace std;

class Utils
{
public:
	static std::string GetLineFromString(const std::string& data, unsigned int offset=0){
		std::string ret("");
		if(data.empty() || offset>data.size())return ret;

		for(unsigned int i=offset; i<data.size(); i++){
			if(data[i] == '\n')break;
			else ret += data[i];
		}
		return ret;
	}

	//CRC-8 - based on the CRC8 formulas by Dallas/Maxim
	//code released under the therms of the GNU GPL 3.0 license
	static byte CRC8(const byte *data, byte len) {
		byte crc = 0x00;
		while (len--) {
			byte extract = *data++;
			for (byte tempI = 8; tempI; tempI--) {
				byte sum = (crc ^ extract) & 0x01;
				crc >>= 1;
				if (sum) {
					crc ^= 0x8C;
				}
				extract >>= 1;
			}
		}
		return crc;
	}

	//CRC-8 - based on the CRC8 formulas by Dallas/Maxim
	//code released under the therms of the GNU GPL 3.0 license
	static int iCRC8(const int *iData, int length) {

		unsigned char* data = new unsigned char[length];
		for (int i = 0; i < length; i++) {
			int val = iData[i];
			if (val < 0)val = 0;
			if (val > 255)val = 255;
			data[i] = (unsigned char)val;
			cout << (int)data[i]<<" "<<val <<", "<< iData[i] << endl;

		}
		byte crc = 0x00;
		byte len = (byte)length;
		byte counter = 0;
		while (len--) {
			byte extract = data[counter];//*data++;
			counter++;
			for (byte tempI = 8; tempI; tempI--) {
				byte sum = (crc ^ extract) & 0x01;
				crc >>= 1;
				if (sum) {
					crc ^= 0x8C;
				}
				extract >>= 1;
			}
		}
		
		//delete[] data;

		return (int)crc;
	}


static Vector2 StringToVector2(std::string& str){
	Vector2 value;
	std::string sval;
	int a=0;
	for(unsigned int i=0; i<str.size();++i){
	
		if(str[i]!=',' && str[i]!='\b' && str[i]!='\n'){
			sval+=str[i];				
		}else{
			if(!sval.empty() && a<2){
				value[a] = Real(atof(sval.c_str()));
				a++;
				sval="";
			}
		}
	}
	if(!sval.empty() && a<2){value[a] = Real(atof(sval.c_str()));}
	return value;
}


static Vector3 StringToVector3(std::string& str){
	Vector3 value(0,0,0);
	std::string sval;
	int a=0;
	for(unsigned int i=0; i<str.size();++i){
	
		if(str[i]!=',' && str[i]!='\b' && str[i]!='\n'){
			sval+=str[i];				
		}else{
			if(!sval.empty() && a<3){
				value[a] = Real(atof(sval.c_str()));
				//cout<<"atof "<<value[a]<<endl;
				//Math::clamptozero(value[a]);
				a++;
				sval="";
			}
		}
	}
	if(!sval.empty() && a<3){value[a] = Real(atof(sval.c_str()));}
	return value;
}
static Vector4 StringToVector4(std::string& str){
	Vector4 value;
	std::string sval;
	int a=0;
	for(unsigned int i=0; i<str.size();++i){
	
		if(str[i]!=',' && str[i]!='\b' && str[i]!='\n'){
			sval+=str[i];				
		}else{
			if(!sval.empty() && a<4){
				value[a] = Real(atof(sval.c_str()));
				a++;
				sval="";
			}
		}
	}
	if(!sval.empty() && a<4){value[a] = Real(atof(sval.c_str()));}
	return value;
}


static Matrix4 StringToMatrix4(std::string& str){

	/*
	
			Vector3 r0,r1,r2,r3;
				bone_data->getValueVector3("row0",r0);
				bone_data->getValueVector3("row1",r1);
				bone_data->getValueVector3("row2",r2);
				bone_data->getValueVector3("row3",r3);
				bone.absolute_transformation = 	
	Matrix4(	r0.x, r1.x, r2.x, r3.x,
					r0.y, r1.y, r2.y, r3.y,
					r0.z, r1.z, r2.z, r3.z,
					0, 0, 0, 1	);
	*/
//	cout<<str<<endl;
	float value[12];
	memset(value,0,12 * sizeof(float));
	std::string sval;

	Matrix4 m;
	int a=0;
	for(unsigned int i=0; i<str.size();++i){
		if(str[i]!=',' && str[i]!='\b' && str[i]!='\n'){
			sval+=str[i];				
		}else{
			if(!sval.empty() && a<12){
				value[a] = Real(atof(sval.c_str()));
				//m._m[a] = Real(atof(sval.c_str()));
				//cout<<value[a]<<endl;	
				a++;
				sval="";
			}
		}
	}
	if(!sval.empty() && a<12){value[a] = Real(atof(sval.c_str()));}
	
//	return m;

	return Matrix4(	value[0], value[3], value[6], value[9],
					value[1], value[4], value[7], value[10],
					value[2], value[5], value[8], value[11],
					0, 0, 0, 1	);
}

/////////////////////////////////////////////////////////////////////////////////////
// izvadiishanas precizitaates jautaajums?
// skat sprintf(buf, "%.6f,%.6f,%.6f", v.x, v.y, v.z);

static std::string QuaternionToString(Quaternion v){
	std::stringstream sr;
	sr<<v.x<<","<<v.y<<","<<v.z<<","<<v.w<<'\0';
	return std::string(sr.str());
}

static std::string VectorToString(Vector2 v){
	std::stringstream sr;
	sr<<v.x<<","<<v.y;
	return std::string(sr.str()+"\0");
}
static std::string VectorToString(Vector3 v){

	char buf[40];
	sprintf_s(buf, "%.6f,%.6f,%.6f\0", v.x, v.y, v.z);
	return std::string(buf);
/*
	std::stringstream sr;
	sr<<v.x<<","<<v.y<<","<<v.z<<'\0';
	return std::string(sr.str());
	*/
}
static std::string VectorToString(Vector4 v){
	char buf[40];
	sprintf_s(buf, "%.6f,%.6f,%.6f,%.6f\0", v.x, v.y, v.z, v.w);
	return std::string(buf);
}

static std::string IntToString(int i){
	std::stringstream sr;
	//sr<<i<<'\0';
	sr<<i;
	std::string ret(sr.str()+"\0");
	
	//sr.clear();
	return ret;
}

static std::string FloatToString(float f,int decDigits = 2){
	std::stringstream sr;
	//sr<<f<<'\0';
	sr.precision(decDigits);
	sr<<f;
	return std::string(sr.str()+"\0");
}

static std::string ReplaceStr(std::string str, std::string search, std::string replace)
{
    string::size_type pos = 0;
    while ( (pos = str.find(search, pos)) != string::npos ) {
        str.replace( pos, search.size(), replace );
        pos++;
    }
	return str;
}


static int Explode(char separator, const std::string& input, std::vector<std::string>& out) 
{
	std::string::size_type substring_start(0);
	std::string::size_type separator_index(0);
	while(std::string::npos != separator_index)
	{
		separator_index = input.find(separator, substring_start);
		out.push_back(input.substr(substring_start, separator_index - substring_start));
		substring_start = separator_index + 1; 
	}
	return (int)out.size();
}

static std::string EatDir(std::string str)
{
	if(str[str.size()-1] == '/')str[str.size()-1] = ' ';
	if(str.empty())return "";
	std::string::size_type first = str.find_last_of('/')+1;
	return str.substr(0,first);
}

static std::string GetFileExt(std::string str)
{
	if(str.empty())return "";
	std::string::size_type first = str.find_last_of('.')+1;
	std::string::size_type size = str.size()-first;
	return str.substr(first,size);
}

};


inline std::ostream& RED(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
                FOREGROUND_RED|FOREGROUND_INTENSITY);
    return s;
}
inline std::ostream& GREEN(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN|FOREGROUND_INTENSITY);
    return s;
}

inline std::ostream& WHITE(std::ostream &s)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout, 
       FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
    return s;
}

constexpr unsigned int hashStr(const char *s, unsigned int h = 0x811C9DC5){
	return !*s
		? h
		: hashStr(
			s + 1,
			static_cast<unsigned int>(
			(h ^ *s) * static_cast<unsigned long long>(0x01000193)));
}
