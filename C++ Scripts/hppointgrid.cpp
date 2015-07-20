/*
 * hppointgrid.cpp
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
	cout << "Starting from grid to point data algorithm.\n";
	cout << "******************************************************\n";
	float valor;
	char conf[256]="pointgrid.pin"; //C:\\Documents and Settings\\Administrator\\Desktop\\eclipse\\workspace\\hpgriddown\\Release\\griddown.pin";
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

	cout << "Input file:  " << filename << "\n";
	cout << "Output file: " << filename2 << "\n";

	cout << "******************************************************\n";
	cout << "1) Xo:  " << xi << "  Yo:  " << yi << "  Zo:  " << zi << "\n";
	cout << "******************************************************\n";
	cout << "Size of header:  " << header << "\n";
	cout << "******************************************************\n";

	if ((inputfile=fopen(filename,"r"))==NULL){
			cout << "Failed " << filename << "\n";
			return 1;
		}
	outputfile=fopen(filename2,"w");

	fprintf(outputfile,"point_data_%i_%i_%i\n",xi,yi,zi);
	fprintf(outputfile,"%i\n",4);
	fprintf(outputfile,"X Coordinate\n");
	fprintf(outputfile,"Y Coordinate\n");
	fprintf(outputfile,"Z Coordinate\n");
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
	cout << "B) SIZE in X is  " << xi << "\n";
	cout << "C) SIZE in Y is  " << yi << "\n";
	cout << "D) SIZE in Z is  " << zi << "\n";
	cout << "E) Generating point-data grid." << "\n";
	int z=0;
	while (z<zi){
		int y=0;
		while (y<yi){
			int x=0;
			while (x<xi){
				fprintf(outputfile,"%i    %i    %i    %f\n",xo+xf*x,yo+yf*y,zo+zf*z,original[z*xi*yi+xi*y+x]);
				x=x+1;
			}
			y=y+1;
		}
		z=z+1;
	}
	cout << "******************************************************\n";
	printf("Finished.\n");
}
