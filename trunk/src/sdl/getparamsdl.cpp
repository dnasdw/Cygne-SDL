#include "include/getparamsdl.h"
#include <fstream>
#include <string>

int getparam(const char *file, const char *param)
{
	FILE *fin; // using FILE because gcc 2.95.3(BeOS) has big problems with fstream files.
	int i;
	string need;
	
	fin = fopen(file, "rb");
	if(!fin) {
		cout << "error: " << file << " does not exist" << endl;
		return(-1);
	}
	
	if(param == NULL) {
		cout << "error: invalid parameter" << endl;
		return(-1);
	}
	
	fseek(fin, 0, SEEK_END);
    i = ftell(fin);
    fseek(fin, 0, SEEK_SET);

	char *p = new char[i+1];
	p[i] = '\n';
	
	fread(p, i, 1, fin);
	
	need = p;
	
	string::size_type findpos = need.find(param);
		
		if(findpos == string::npos) {
			cout << "error: no such parameter" << endl;
			return(-1);
		}
		
		string::size_type lastPos = need.find_first_not_of(" ", findpos); 
		
		if(lastPos == string::npos) {
			cout << "error: no such delimiter" << endl;
			return(-1);
		}
		
		string::size_type pos = need.find_first_of(" ", lastPos);
		
		if(pos == string::npos) {
			cout << "error: no such delimiter" << endl;
			return(-1);
		}
		
		string::size_type pos2 = need.find_first_of("\n", pos);
		
		if(pos2 == string::npos) {
			cout << "error: no such delimiter" << endl;
			return(-1);
		}
		
		string s = need.substr(pos, pos2-pos);
		
		delete p;
		fclose(fin);
	return(atoi(s.c_str()));
}
