/*
 * hpvageast.cpp
 *
 *  Created on: 19 de Nov de 2010
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
	cout << "Starting grid variogram for east direction algorithm.\n";
	cout << "******************************************************\n";
    //printf("Starting downscaling.\n\n\n");
	float valor;
	// CONF FILE.
	char conf[256]="gridvag.pin"; //C:\\Documents and Settings\\Administrator\\Desktop\\eclipse\\workspace\\hpgriddown\\Release\\griddown.pin";
	// Caminho e nome do ficheiro de entrada.
	char filename[256];
	// Caminho e nome do ficheiro de saida.
	char filename2[256];
	// Abrir ficheiro de entrada e ficheiro de saida.
	FILE *inputfile;
	FILE *outputfile2;
	FILE *fileconf;

	char cwd[1024];
		getcwd(cwd,sizeof(cwd));
		cout << cwd << endl;

	if ((fileconf=fopen(conf,"r"))==NULL){
				cout << "Can't find CONF file: " << conf << "\n";
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
	char strapp[256];
	char efile[256]="east";
	sprintf(strapp,"%s%s.pin",filename2,efile);
	outputfile2=fopen(strapp,"w");

	/*fprintf(outputfile,"%i_%i_%i\n",xi,yi,zi);
	fprintf(outputfile,"%i\n",1);
	fprintf(outputfile,"variable\n");*/

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

	float *east;
	east=new(nothrow)float[xi];
	i=0; while (i<xi){east[i]=0;i=i+1;}

	int *eastcounter;
	eastcounter=new(nothrow)int[xi];
	i=0; while (i<xi){eastcounter[i]=0;i=i+1;}

	cout << "C) Generating East-West grid variogram." << "\n";
	int x=0;
	int y=0;
	int z=0;
	int app=0;
	while (z<zi){
		y=0;
		while (y<yi){
			x=0;
			while (x<xi){
				app=x;
				while (app<xi){
					if (original[xi*yi*z+y*xi+app]!=nodata) {
					east[abs(app-x)]=east[abs(app-x)]+pow((original[xi*yi*z+y*xi+x]-original[xi*yi*z+y*xi+app]),2);
					eastcounter[abs(app-x)]=eastcounter[abs(app-x)]+1;}
					app=app+1;
				}
				x=x+1;
			}
			y=y+1;
		}
		z=z+1;
	}

	fprintf(outputfile2,"East-West\n");
	fprintf(outputfile2,"2\n");
	fprintf(outputfile2,"Distance\n");
	fprintf(outputfile2,"Variogram\n");
	fprintf(outputfile2,"Counter\n");

	i=1;
	int step=xs;
	while (i<xi){
		fprintf(outputfile2,"%i     %f     %i\n",step,(east[i]/(2*eastcounter[i])),eastcounter[i]);
		step=step+xs;
		i=i+1;
	}

	cout << "******************************************************\n";
	printf("Finished.\n");
}
