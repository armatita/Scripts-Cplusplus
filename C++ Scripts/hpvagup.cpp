/*
 * hpvagup.cpp
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
	cout << "Starting variogram algorithm for vertical direction.\n";
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
	FILE *outputfile;
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
	char vfile[256]="vertical";
	sprintf(strapp,"%s%s.pin",filename2,vfile);
	outputfile=fopen(strapp,"w");

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

	cout << "B) Generating Vertical grid variogram." << "\n";
	float *vert;
	vert=new(nothrow)float[zi];
	i=0; while (i<zi){vert[i]=0;i=i+1;}
	int *vertcounter;
	vertcounter=new(nothrow)int[zi];
	i=0; while (i<zi){vertcounter[i]=0;i=i+1;}
	int x=0;
	int y=0;
	int z;
	int app;
	while (x<xi){
		y=0;
		while (y<yi){
			z=0;
			while (z<zi){
				app=z;
				while (app<zi){
					if (original[xi*yi*app+y*xi+x]!=nodata) {
					vert[abs(app-z)]=vert[abs(app-z)]+pow((original[xi*yi*z+y*xi+x]-original[xi*yi*app+y*xi+x]),2);
					vertcounter[abs(app-z)]=vertcounter[abs(app-z)]+1;}
					app=app+1;
				}
				z=z+1;
			}
			y=y+1;
		}
		x=x+1;
	}

	fprintf(outputfile,"Vertical\n");
	fprintf(outputfile,"3\n");
	fprintf(outputfile,"Distance\n");
	fprintf(outputfile,"Variogram\n");
	fprintf(outputfile,"Counter\n");
	/*i=1;
	while (i<zi) {
		vertcounter[i]=100000-(i-1)*2500;
		i=i+1;
	}*/
	i=1;
	int step=zs;
	while (i<zi){
		fprintf(outputfile,"%i     %f     %i\n",step,(vert[i]/(2*vertcounter[i])),vertcounter[i]);
		step=step+zs;
		i=i+1;
	}

	cout << "******************************************************\n";
	printf("Finished.\n");
}
