/*
 * hpfindcommas.cpp
 *
 *  Created on: 4 de Abr de 2011
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
	cout << "Starting find and replace commas by dots algorithm.\n";
	cout << "******************************************************\n";

	char conf[256]="findcommas.pin";

	char filename[1024];
	// Caminho e nome do ficheiro de saida.
	char filename2[1024];
	// Abrir ficheiro de entrada e ficheiro de saida.
	//FILE *inputfile;
	FILE *inputfile;
	FILE *outputfile;
	FILE *fileconf;
	char c;
	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	cout << cwd << endl;

	if ((fileconf=fopen(conf,"r"))==NULL){
		cout << "Can't find CONF file: " << conf << "\n";
		return 1;
	}

	int header;
	/*int i=0;
	while (c!='\n'){
		c=fgetc(fileconf);
		if (c!='\n'){
			filename[i]=c;
		}
		i++;
	}
	c='a';
	i=0;
	while (c!='\n'){
		c=fgetc(fileconf);
		if (c!='\n'){
			filename2[i]=c;
		}
		i++;
	}*/
	fscanf(fileconf,"%[^\n]s\n",filename);
	fscanf(fileconf,"%s\n",filename2);
	fscanf(fileconf,"%[^\n]s\n",filename2);
	fscanf(fileconf,"%d\n",&header);

	cout << "Input file:  " << filename << "\n";
	cout << "Output file: " << filename2 << "\n";
	cout << "Header size: " << header << endl;
	cout << "******************************************************\n";
	cout << "Replacing commas by dots to all rows next to header. ";
	int h=0;
	inputfile=fopen(filename,"r");
	outputfile=fopen(filename2,"w");
	while (h<header){
		c=fgetc(inputfile); // fgetc receives char by char from file instead of string or stream.
		fprintf(outputfile,"%c",c);
		if (c=='\n'){h=h+1;} // When the char is equal to '\n' then we've reached a new row.
	}
	while (!feof(inputfile)){ // While does not reach end of file symbol (eof or feof)...
		c=fgetc(inputfile); // Still gathering char by char from file.
		if (feof(inputfile)){break;}
		else if(c==','){c='.';}
		fprintf(outputfile,"%c",c);
	}
	fclose(inputfile);

	cout << "Finished.\n";
	return 0;
}
