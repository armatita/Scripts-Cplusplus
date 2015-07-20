/*
 * insertheader.cpp
 *
 *  Created on: 19 de Mar de 2011
 *      Author: pedro.correia
 */

#include "iostream"
#include "new"
#include "fstream"
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"

using namespace std;

int main()
{
	cout << "******************************************************\n";
	cout << "Starting insert header algorithm.\n";
	cout << "******************************************************\n";
	char conf[256]="insertheader.pin"; //C:\\Documents and Settings\\Administrator\\Desktop\\eclipse\\workspace\\hpgriddown\\Release\\griddown.pin";
	char filename[256];
	char filename2[256];

	FILE *inputfile;
	FILE *outputfile;
	FILE *fileconf;

	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	cout << cwd << endl;

	char line[1024];
	char var[1024];
	char value[1024];
	if ((fileconf=fopen(conf,"r"))==NULL){
				cout << "Can't find CONF file: " << conf << "\n";
				return 1;
	}
	fscanf(fileconf,"%[^\n]s\n",filename);
	fscanf(fileconf,"%s\n",filename2);
	fscanf(fileconf,"%[^\n]s\n",filename2);
	fscanf(fileconf,"%s\n",line);
	fscanf(fileconf,"%s\n",var);

	cout << "Input file:  " << filename << "\n";
	cout << "Output file: " << filename2 << "\n";

	cout << "******************************************************\n";
	cout << "1) First line in header:  " << line << "\n";
	cout << "2) Second line in header:  " << var << "\n";
	cout << "******************************************************\n";

	if ((inputfile=fopen(filename,"r"))==NULL){
				cout << "Failed " << filename << "\n";
				return 1;
			}
	outputfile=fopen(filename2,"w");
	fprintf(outputfile,"%s\n",line);
	fprintf(outputfile,"1\n");
	fprintf(outputfile,"%s\n",var);

	while (fscanf(inputfile,"%s\n",value)!=EOF){
			fprintf(outputfile,"%s\n",value);
	}
	cout << "******************************************************\n";
	printf("Finished.\n");
	}
