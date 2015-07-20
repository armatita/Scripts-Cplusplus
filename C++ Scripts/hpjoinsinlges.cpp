/*
 * hpjoinsinlges.cpp
 *
 *  Created on: 2 de Abr de 2011
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
	cout << "Starting join single columned files algorithm.\n";
	cout << "******************************************************\n";

	char conf[256]="singlecolumned.pin";

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
	int many;
	int i=0;
	int j=0;
	int h;
	fscanf(fileconf,"%d\n",&header);
	while (c!='\n'){
		c=fgetc(fileconf);
		if (c!='\n'){
			filename2[i]=c;
		}
		i++;
	}

	outputfile=fopen(filename2,"w");
	fscanf(fileconf,"%d\n",&many);
	fprintf(outputfile,"Joined_%d_files_in_one_singled_columned\n1\nvariable\n",many);
	while (j<many){
		i=0;
		c='a';
		while (c!='\n'){
			c=fgetc(fileconf);
			if (c!='\n'){
				filename[i]=c;
				i++;
			}
		}
		while (i<1024){
			filename[i]=NULL;
			i++;
		}
		cout << "Joining file " << filename <<" ("<< j+1 << " from " << many <<")."<< endl;
		inputfile=fopen(filename,"r");
		h=0;
		while (h<header){
			c=fgetc(inputfile); // fgetc receives char by char from file instead of string or stream.
			if (c=='\n'){h=h+1;} // When the char is equal to '\n' then we've reached a new row.
		}
		while (!feof(inputfile)){ // While does not reach end of file symbol (eof or feof)...
			c=fgetc(inputfile); // Still gathering char by char from file.
			if (feof(inputfile)){break;}
			fprintf(outputfile,"%c",c);
		}
		fclose(inputfile);
		j++;
	}

	cout << "Finished.\n";
	return 0;
}

