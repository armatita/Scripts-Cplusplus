/*
 * hpslicegrid.cpp
 *
 *  Created on: 15 de Nov de 2010
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
	cout << "Starting slice grid algorithm.\n";
	cout << "******************************************************\n";
	float valor;
	char conf[256]="slicegrid.pin"; //C:\\Documents and Settings\\Administrator\\Desktop\\eclipse\\workspace\\hpgriddown\\Release\\griddown.pin";
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
	int xo;
	int yo;
	int zo;
	int xf;
	int yf;
	int zf;
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
	fscanf(fileconf,"%d\n",&xo);
	fscanf(fileconf,"%d\n",&yo);
	fscanf(fileconf,"%d\n",&zo);
	fscanf(fileconf,"%d\n",&xf);
	fscanf(fileconf,"%d\n",&yf);
	fscanf(fileconf,"%d\n",&zf);
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

	fprintf(outputfile,"%i_%i_%i\n",xf-xo+1,yf-yo+1,zf-zo+1);
	fprintf(outputfile,"%i\n",1);
	fprintf(outputfile,"variable\n");

	float *original;
	original = new (nothrow) float[xi*yi*zi];

	int i=0;
	int h=0;
	char lixo[256];
	while(h<header){fscanf(inputfile,"%s\n",lixo);h=h+1;}
	printf("A) Reading input file.\n");
	while(i<xi*yi*zi) {
		fscanf(inputfile,"%f\n",&valor);
		original[i]=valor;
		i=i+1;
	}
	cout << "B) SIZE in X is  " << xf-xo+1 << "\n";
	cout << "C) SIZE in Y is  " << yf-yo+1 << "\n";
	cout << "D) SIZE in Z is  " << zf-zo+1 << "\n";
	cout << "E) Generating slice grid." << "\n";
	i=0;
	int v;
	int camada=xi*yi;
	int j=zo-1;
	while (j<zf){
		v=j*camada+xi*(yo-1)+xo-1;
		while (v<(yf)*xi+xo-1+j*camada) {
			i=0;
			while (i<(xf-xo+1)) {
				fprintf(outputfile,"%f\n",original[v+i]);
				i=i+1;
			}
			v=v+xi;
		}
		j=j+1;
	}
	cout << "******************************************************\n";
	printf("Finished.\n");
}
