/*
 * hpremovewhitespace.cpp
 *
 *  Created on: 31 de Mar de 2011
 *      Author: pedro.correia
 */

#include <algorithm>
#include "iostream"
#include <new>
#include "fstream"
#include "unistd.h"
#include "errno.h"
#include "math.h"
#include <ctype.h>

using namespace std;

int main()
{
	cout << "******************************************************\n";
	cout << "Starting remove whitespace algorithm.\n";
	cout << "******************************************************\n";

	char conf[256]="removewhitespace.pin";

	char filename[1024];
	// Caminho e nome do ficheiro de saida.
	char filename2[1024];
	// Abrir ficheiro de entrada e ficheiro de saida.
	//FILE *inputfile;
	FILE *outputfile;
	FILE *inputfile;
	FILE *fileconf;

	int header;
	char sep;

	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	cout << cwd << endl;

	if ((fileconf=fopen(conf,"r"))==NULL){
		cout << "Can't find CONF file: " << conf << "\n";
		return 1;
	}


	fscanf(fileconf,"%[^\n]s\n",filename);
	fscanf(fileconf,"%s\n",filename2);
	fscanf(fileconf,"%[^\n]s\n",filename2);
	fscanf(fileconf,"%s\n",&sep);
	fscanf(fileconf,"%d\n",&header);

	if (header==0){header=-1;}

	cout << "Input file:  " << filename << "\n";
	cout << "Output file: " << filename2 << "\n";
	cout << "******************************************************\n";
	cout << "Loading input file to read. ";
	if ((inputfile=fopen(filename,"r"))==NULL){
		cout << "Can't find input file: " << filename << "\n";
		return 1;
	}
	cout << "Finished.\n";

	if (header == -1){ cout << "Removing all whitespaces from file... ";}
	else { cout << "Removing whitespaces until row: " << header << " ... " << endl;}

	outputfile=fopen(filename2,"w");

	char c;
	int value=0;
	int rows=0;
	while (!feof(inputfile)){
		c=fgetc(inputfile);
		if (c=='\n'){
			rows=rows+1;
			if (rows==header){value=1;}
		}
		if (c==9 || c==32){
			if (value==0){c=sep;}
		}
		if (feof(inputfile)){break;} //c==152
		fprintf(outputfile,"%c",c);
	}

	cout << "Finished.\n";
	return 0;
}


