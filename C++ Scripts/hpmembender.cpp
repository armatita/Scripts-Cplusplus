/*
 * hpmembender.cpp
 *
 *  Created on: 29 de Nov de 2010
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
	cout << "Starting bending algorithm.\n";
	cout << "******************************************************\n";
	float valor;
	char conf[256]="membender.pin"; //C:\\Documents and Settings\\Administrator\\Desktop\\eclipse\\workspace\\hpgriddown\\Release\\griddown.pin";
	char filename[256];
	char filename2[256];
	char filename3[256];

	FILE *inputfile;
	FILE *inputmap;
	FILE *outputfile;
	FILE *fileconf;

	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	cout << cwd << endl;

	float *map;
	float *grid;
	float *final;
	int xi;
	int yi;
	int zi;
	int header;
	int end;
	float nodata;

	if ((fileconf=fopen(conf,"r"))==NULL){
			cout << "Can't find CONF file: " << conf << "\n";
			return 1;
		}

	fscanf(fileconf,"%[^\n]s\n",filename);
	fscanf(fileconf,"%s\n",filename2);
	fscanf(fileconf,"%[^\n]s\n",filename2);
	fscanf(fileconf,"%s\n",filename3);
	fscanf(fileconf,"%[^\n]s\n",filename3);
	fscanf(fileconf,"%d\n",&xi);
	fscanf(fileconf,"%d\n",&yi);
	fscanf(fileconf,"%d\n",&zi);
	fscanf(fileconf,"%d\n",&end);
	fscanf(fileconf,"%d\n",&header);
	fscanf(fileconf,"%f\n",&nodata);

	int max;
	cout << "**********************INPUT FILES***********************\n";
	cout << "********************************************************\n";

	// Opening bending map file.
	inputmap=fopen(filename3,"r");
	// Opening error management.
	if (inputmap==NULL){
			cout << "1) Bending map FILE NOT FOUND.\n";
			return 1;
		}
	else cout << "1) Bending map file found. ";

	// Allocating bending map.
	map=new(nothrow)float[xi*yi];
	// Allocation error management.
	if (map==NULL){
		cout << "Bending map ALLOCATION FAILED.\n";
		return 1;
	}
	else cout << "Bending map successfully allocated.\n";
	// Passing information in file to memory.
	char lixo[256];
	fscanf(inputmap,"%s\n",lixo);
	fscanf(inputmap,"%s\n",lixo);
	fscanf(inputmap,"%s\n",lixo);
	for (int n=0; n<xi*yi; n++) {
		fscanf(inputmap,"%f\n",&valor);
		map[n]=valor;
		if (n==0) max=valor;
		if (valor>max) max=valor;
		}

	// Opening grid file.
	inputfile=fopen(filename,"r");
	// Opening error management.
	if (inputfile==NULL){
			cout << "2) Grid FILE NOT FOUND.\n";
			return 1;
		}
	else cout << "2) Grid file found. ";

	// Allocating grid.
	grid=new(nothrow)float[xi*yi*zi];
	// Allocation error management.
	if (grid==NULL){
		cout << "Grid ALLOCATION FAILED.\n";
		return 1;
	}
	else cout << "Grid successfully allocated.\n";
	// Passing information in grid file to memory.
	int h=0;
	while(h<header){fscanf(inputfile,"%s\n",lixo);h=h+1;}
	for (int n=0; n<xi*yi*zi; n++) {fscanf(inputfile,"%f\n",&valor); grid[n]=valor;}

	cout << "********************************************************\n\n";

	cout << "*********************OUTPUT FILE************************\n";
	cout << "********************************************************\n";
	// Calculating z number of columns for unbended grid.
	int zz=zi+max;

	// Opening unbended grid file to write.
	outputfile=fopen(filename2,"w");
	// Opening error management.
	if (outputfile==NULL){
			cout << "3) Output FAILED TO GENERATE.\n";
			return 1;
		}
	else {
		cout << "3) Output file generated.\n";
		cout << "3) OUTPUT GRID SIZE is X: " << xi << " Y: " << yi << " Z: " << zz << "\n";
	}
	// Allocating unbended grid.
	final=new(nothrow)float[xi*yi*zz];
	// Allocation error management.
	if (final==NULL){
		cout << "3) Output ALLOCATION FAILED.\n";
		return 1;
	}
	else cout << "3) Output successfully allocated.\n";
	cout << "********************************************************\n\n";

	cout << "********************Bending grid************************\n";
	cout << "********************************************************\n";
	cout << "4) Atributing No-Data values to output grid.\n";
	for (int n=0; n<xi*yi*zz; n++) {final[n]=nodata;}

	cout << "4) Calculating...";
	int yc=0;
	int xc;
	int zc;
	int absvalue;
	while (yc<yi) {
		xc=0;
		while (xc<xi) {
			zc=0;
			while (zc<zi) {
				absvalue=map[xi*yc+xc]/20;
				final[xi*yi*(absvalue+zc)+xi*yc+xc]=grid[xi*yi*zc+xi*yc+xc];
				zc=zc+1;
			}
			xc=xc+1;
		}
		yc=yc+1;
	}
	cout << " Bending complete.\n";
	cout << "********************************************************\n\n";

	cout << "*****************Writing bended grid********************\n";
	cout << "********************************************************\n";
	fprintf(outputfile,"%i_%i_%i\n1\nvariable\n",xi,yi,end);
	for (int n=0; n<xi*yi*end; n++) { // suposed to be zz
		fprintf(outputfile,"%f\n",final[n]);
	}
	delete[] map;
	delete[] grid;
	delete[] final;
	cout << "******************************************************\n";
	printf("Finished.\n");
	return 0;
	}

