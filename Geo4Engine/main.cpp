#include "Geo4.h"
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{	
	
#ifndef _DEBUG
	ofstream file;
	file.open("logfile.log");
	streambuf* sbuf = cout.rdbuf();
	cout.rdbuf(file.rdbuf());
#endif

	cout << RED << "GEO4.2 Engine - SR2019 Seven Steps Animator" << endl;
	cout << "Copyright: Krists Pudzens (c) 2007 - 2019" << endl;
	cout << GREEN << "========================================" << WHITE << endl;

	SceneManager manager("conf.cfo");

	while (manager.process()) {	}

	return 0;
}