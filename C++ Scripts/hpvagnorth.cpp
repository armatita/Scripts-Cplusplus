/*
 * hpvagnorth.cpp
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
	cout << "Starting grid variogram north direction algorithm.\n";
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
	FILE *outputfile3;
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
	char nfile[256]="north";
	sprintf(strapp,"%s%s.pin",filename2,nfile);
	outputfile3=fopen(strapp,"w");

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

	float *north;
	north=new(nothrow)float[yi];
	i=0; while (i<yi){north[i]=0;i=i+1;}
	int *northcounter;
	northcounter=new(nothrow)int[yi];
	i=0; while (i<yi){northcounter[i]=0;i=i+1;}

	cout << "B) Generating North-South grid variogram." << "\n";
	int x=0;
	int y=0;
	int z=0;
	int app=0;
	while (z<zi){
		x=0;
		while (x<xi){
			y=0;
			while (y<yi){
				app=y;
				while (app<yi){
					if (original[xi*yi*z+app*xi+x]!=nodata  && original[xi*yi*z+y*xi+x]!=nodata) {
					north[abs(app-y)]=north[abs(app-y)]+pow((original[xi*yi*z+y*xi+x]-original[xi*yi*z+app*xi+x]),2);
					northcounter[abs(app-y)]=northcounter[abs(app-y)]+1;}
					app=app+1;
				}
				y=y+1;
			}
			x=x+1;
		}
		z=z+1;
	}
	fprintf(outputfile3,"North-South\n");
	fprintf(outputfile3,"2\n");
	fprintf(outputfile3,"Distance\n");
	fprintf(outputfile3,"Variogram\n");
	fprintf(outputfile3,"Counter\n");

	i=1;
	int step=ys;
	while (i<yi){
		fprintf(outputfile3,"%i     %f     %i\n",step,(north[i]/(2*northcounter[i])),northcounter[i]);
		step=step+ys;
		i=i+1;
	}


	cout << "******************************************************\n";
	printf("Finished.\n");
}
