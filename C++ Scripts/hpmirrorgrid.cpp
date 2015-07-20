/*
 * hpmirrorgrid.cpp
 *
 *  Created on: 12 de Nov de 2010
 *      Author: Administrator
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
	cout << "Starting mirror grid algorithm.\n";
	cout << "******************************************************\n";
	float valor;
	char conf[256]="mirrorgrid.pin"; //C:\\Documents and Settings\\Administrator\\Desktop\\eclipse\\workspace\\hpgriddown\\Release\\griddown.pin";
	char filename[256];
	char filename2[256];

	FILE *inputfile;
	FILE *outputfile;
	FILE *fileconf;

	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	cout << cwd << endl;

	int xi;
	int yi;
	int zi;
	int header;
	float nodata;

	if ((fileconf=fopen(conf,"r"))==NULL){
			cout << "Can't find CONF file: " << conf << "\n";
			return 1;
		}

	fscanf(fileconf,"%[^\n]s\n",filename);
	fscanf(fileconf,"%s\n",filename2);
	fscanf(fileconf,"%[^\n]s\n",filename2);
	fscanf(fileconf,"%d\n",&xi);
	fscanf(fileconf,"%d\n",&yi);
	fscanf(fileconf,"%d\n",&zi);
	fscanf(fileconf,"%d\n",&header);
	fscanf(fileconf,"%f\n",&nodata);

	cout << "Input file:  " << filename << "\n";
	cout << "Output file: " << filename2 << "\n";

	cout << "******************************************************\n";
	cout << "1) Xo:  " << xi << "  Yo:  " << yi << "  Zo:  " << zi << "\n";
	cout << "******************************************************\n";
	cout << "Size of header:  " << header << "\n";
	cout << "No-Data value:   " << nodata << "\n";
	cout << "******************************************************\n";

	if ((inputfile=fopen(filename,"r"))==NULL){
			cout << "Failed " << filename << "\n";
			return 1;
		}
	outputfile=fopen(filename2,"w");

	fprintf(outputfile,"%i_%i_%i\n",xi,yi,zi);
	fprintf(outputfile,"%i\n",1);
	fprintf(outputfile,"variable\n");

	float *original;
	original = new (nothrow) float[xi*yi*zi];

	int i=0;
	int h=0;
	float max;
	float min;
	char lixo[256];
	while(h<header){fscanf(inputfile,"%s\n",lixo);h=h+1;}
	printf("A) Reading input file.\n");
	while(i<xi*yi*zi) {
		fscanf(inputfile,"%f\n",&valor);
		if (i==0) {
			max=valor;
			min=valor;
		}
		else {
			if (valor>max && valor!=nodata) {max=valor;}
			if (valor<min && valor!=nodata) {min=valor;}
		}
		original[i]=valor;
		i=i+1;
	}
	float middle=(max-min)/2+min;
	cout << "B) MAXIMUM is  " << max << "\n";
	cout << "C) MINIMUM is  " << min << "\n";
	cout << "D) Mirroring at  " << middle << "\n";
	cout << "E) Generating mirror image.";
	i=0;
	while(i<xi*yi*zi)
		{
		if (original[i]!=nodata){
			if(original[i]<=middle) {valor=abs(middle-original[i])+middle;}
			else {valor=middle-abs(middle-original[i]);}
		}
		else {valor=nodata;}
		fprintf(outputfile,"%f\n",valor);
		i=i+1;
		}
	cout << "******************************************************\n";
	printf("Finished.\n");
}
