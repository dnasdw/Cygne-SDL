#include "include/getparamsdl.h"
#define C_PARSER
#ifndef C_PARSER
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
#else
extern "C"{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *cfgfile=NULL;

int initparam(const char *filename)
{
    FILE *fin;
    int i;
    fin = fopen(filename, "rb");
    if(fin==NULL) {
	printf("No config file \n");
	return(-1);
    }

    fseek(fin, 0, SEEK_END);
    i = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    cfgfile= (char*)malloc(sizeof(char)*(i+1));
    if(cfgfile==NULL) 
    {
	printf("Not enough memory\n");
	return(-1);
    }
    
    cfgfile[i] = '\n';
    fread(cfgfile, i, 1, fin);
    fclose(fin);
    return 0;
}


int getparam(const char *file, const char *param)
{
	FILE *fin; 
	char *need,buf[5];
	int i;
// that isn't clean but is enough to avoid other changes
	if ((cfgfile==NULL)&&(initparam(file)<0))return (-1);
	
	need = strstr(cfgfile,param);
		
	if(need == NULL) 
	{
	printf("No \"%s\" present\n",param);
	return(-1);
	}

	if (strlen(need)<strlen(param)+2) return (-1);

	need+=strlen(param);
	if(*need != ' ') 
	{
	printf("error: no such delimiter %c \n",need[0]);
	return(-1);
	}
	i=strpbrk(need,"\n")-need;
	printf("param %s value strlen %d",param,i);
	strncpy(buf,need,i);
	buf[i]='\0';
	printf("value %s, atoi %d \n",buf,atoi(buf));
	if (i>4) return (-1);
	return (atoi(buf));
}
}
#endif