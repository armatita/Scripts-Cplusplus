/*
 * hpwellgrid.cpp
 *
 *  Created on: 22 de Nov de 2010
 *      Author: Pedro Correia
 */

#include "iostream"
#include "new"
#include "fstream"
#include "math.h"
#include "unistd.h"
#include "errno.h"
#include "stdlib.h"

using namespace std;

int main()
{
	cout << "******************************************************\n";
	cout << "Starting grid well retrieve algorithm.\n";
	cout << "******************************************************\n";
	float valor;
	// CONF FILE.
	char conf[256]="gridwell.pin"; //C:\\Documents and Settings\\Administrator\\Desktop\\eclipse\\workspace\\hpgriddown\\Release\\griddown.pin";
	// WELL FILE
	char well[256]="coordwell.pin";
	// Caminho e nome do ficheiro de entrada.
	char filename[256];
	// Caminho e nome do ficheiro de saida.
	char filename2[256];
	// Abrir ficheiro de entrada e ficheiro de saida.
	FILE *inputfile;
	FILE *outputfile;
	FILE *fileconf;
	FILE *filewell;

	char cwd[1024];
		getcwd(cwd,sizeof(cwd));
		cout << cwd << endl;

	if ((fileconf=fopen(conf,"r"))==NULL){
				cout << "Can't find CONF file: " << conf << "\n";
				return 1;
			}

	if ((filewell=fopen(well,"r"))==NULL){
				cout << "Can't find grid well file: " << well << "\n";
				return 1;
			}

	// VARIAVEIS DE INPUT
	// Tamanho da grid original (xi,yi,zi)
	int xi;
	int yi;
	int zi;
	int xs;
	int ys;
	int zs;
	int xo;
	int yo;
	int zo;
	int header;
	float nodata;

	fscanf(fileconf,"%[^\n]s\n",filename);
	fscanf(fileconf,"%s\n",filename2);
	fscanf(fileconf,"%[^\n]s\n",filename2);
	fscanf(fileconf,"%d\n",&xi);
	fscanf(fileconf,"%d\n",&yi);
	fscanf(fileconf,"%d\n",&zi);
	fscanf(fileconf,"%d\n",&xs);
	fscanf(fileconf,"%d\n",&ys);
	fscanf(fileconf,"%d\n",&zs);
	fscanf(fileconf,"%d\n",&xo);
	fscanf(fileconf,"%d\n",&yo);
	fscanf(fileconf,"%d\n",&zo);
	fscanf(fileconf,"%d\n",&header);
	fscanf(fileconf,"%f\n",&nodata);

	int number;
	fscanf(filewell,"%i\n",&number);
	int j=0;
	int *localx;
	int *localy;
	localx = new (nothrow) int[number];
	localy = new (nothrow) int[number];
	int valx;
	int valy;
	while (j<number){
		fscanf(filewell,"%i %i\n",&valx,&valy);
		localx[j]=valx;
		localy[j]=valy;
		j=j+1;
	}

	cout << "Input file:  " << filename << "\n";
	cout << "Output file: " << filename2 << "\n";
	cout << "******************************************************\n";
	cout << "1) Xo:  " << xi << "  Yo:  " << yi << "  Zo:  " << zi << "\n";
	cout << "******************************************************\n";
	cout << "Size of header:  " << header << "\n";
	cout << "No-Data value:   " << nodata << "\n";
	cout << "Number of wells: " << number << "\n";
	cout << "******************************************************\n";

	if ((inputfile=fopen(filename,"r"))==NULL){
				cout << "Failed " << filename << "\n";
				return 1;
			}

	outputfile=fopen(filename2,"w");

	fprintf(outputfile,"%i_%i_%i\n",xs,ys,zs);
	fprintf(outputfile,"%i\n",4);
	fprintf(outputfile,"X\n");
	fprintf(outputfile,"Y\n");
	fprintf(outputfile,"Z\n");
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

	cout << "B) Generating well file from grid." << "\n";
	int k=0;
	int z;
	while (k<number){
		z=0;
		while (z<zi){
			fprintf(outputfile,"%i     %i     %i     %f\n",localx[k]*xs+xo,localy[k]*ys+yo,(z+1)*zs+zo,original[xi*yi*z+xi*localy[k]+localx[k]]);
			z=z+1;
		}
		k=k+1;
	}

	cout << "******************************************************\n";
	printf("Finished.\n");
}
