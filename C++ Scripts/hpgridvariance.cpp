/*
 * hpgridvariance.cpp
 *
 *  Created on: 10 de Jan de 2011
 *      Author: Pedro Correia
 */

#include <iostream>
#include <fstream>
#include <new>
#include "stdlib.h"
#include "unistd.h"
#include "errno.h"
#include "math.h"

using namespace std;


int main()
{
	cout << "******************************************************\n";
	cout << "Starting variance from simulations algorithm.\n";
	cout << "******************************************************\n";
    //printf("Starting downscaling.\n\n\n");
	float valor;
	float valor2;
	// CONF FILE.
	char conf[256]="gridvariance.pin"; //C:\\Documents and Settings\\Administrator\\Desktop\\eclipse\\workspace\\hpgriddown\\Release\\griddown.pin";
	// Caminho e nome do ficheiro de entrada.
	char filename[256];
	// Caminho e nome do ficheiro de saida.
	char filename2[256];
	// Abrir ficheiro de entrada e ficheiro de saida.
	char filename3[256];
	char input[256];
	char ext[256];
	FILE *inputfile;
	FILE *inputfile2;
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
	int nums;
	int header;
	float nodata;

	fscanf(fileconf,"%[^\n]s\n",filename);
	fscanf(fileconf,"%s\n",filename2);
	fscanf(fileconf,"%[^\n]s\n",filename2);
	fscanf(fileconf,"%s\n",filename3);
	fscanf(fileconf,"%[^\n]s\n",filename3);
	fscanf(fileconf,"%s\n",input);
	fscanf(fileconf,"%s\n",ext);
	fscanf(fileconf,"%d\n",&nums);
	fscanf(fileconf,"%d\n",&xi);
	fscanf(fileconf,"%d\n",&yi);
	fscanf(fileconf,"%d\n",&zi);
	fscanf(fileconf,"%d\n",&header);
	fscanf(fileconf,"%f\n",&nodata);

	cout << "Input directory:  " << filename << "\n";
	cout << "Output file: " << filename2 << "\n";
	cout << "******************************************************\n";
	cout << "1) Xo:  " << xi << "  Yo:  " << yi << "  Zo:  " << zi << "\n";
	cout << "******************************************************\n";
	cout << "Size of header:  " << header << "\n";
	cout << "No-Data value:   " << nodata << "\n";
	cout << "******************************************************\n";
	outputfile=fopen(filename2,"w");
	fprintf(outputfile,"%i_%i_%i\n",xi,yi,zi);
	fprintf(outputfile,"%i\n",1);
	fprintf(outputfile,"Variance_from_simulations\n");
	int j;
	char path[256];
	float *original;
	//float *meangrid;
	original = new (nothrow) float[xi*yi*zi];
	//meangrid = new (nothrow) float[xi*yi*zi];
	if ((inputfile2=fopen(filename3,"r"))==NULL){
							cout << "Failed " << filename3 << "\n";
							return 1;
						}
	char lixo[256];
	int h=0;
	while(h<header){fscanf(inputfile2,"%s\n",lixo);h=h+1;}
	j=1;
	//int lixoid;
	while (j<=nums){
		int i=0;
		h=0;
		sprintf(path,"%s%s%i.%s",filename,input,j,ext);
		cout<<path<<"\n";
		//cin >> lixoid;
		if ((inputfile=fopen(path,"r"))==NULL){
						cout << "Failed " << filename << "\n";
						return 1;
					}
		while(h<header){fscanf(inputfile,"%s\n",lixo);h=h+1;}
		//cout << "A) Reading input file:" << j << "\n");
		while(i<xi*yi*zi) {
			fscanf(inputfile,"%f\n",&valor);
			fscanf(inputfile2,"%f\n",&valor2);
			if (j==1){
				original[i]=pow((valor2-valor),2);}
			else if (j==nums){
				original[i]=(original[i]+pow((valor2-valor),2))/nums;
				fprintf(outputfile,"%f\n",original[i]);
			}
			else{
				original[i]=original[i]+pow((valor2-valor),2);
			}
			i=i+1;
		}
	j=j+1;
	}

	cout << "******************************************************\n";
	printf("Finished.\n");
}
