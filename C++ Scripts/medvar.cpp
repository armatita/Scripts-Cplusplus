/*
 * medvar.cpp
 *
 *  Created on: 26 de Set de 2011
 *      Author: pedro.correia
 */

#include "iostream"
#include "new"
#include "fstream"
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

using namespace std;

int main()
{
	cout << "******************************************************\n";
	cout << "Starting mean and variance algorithm.\n";
	cout << "******************************************************\n";

	float valor;
	char conf[256]="medvar.conf";
	char filename[256];
	char filename2[256]="media.prn";
	char filename3[256]="variancia.prn";

	FILE *inputfile;
	FILE *outputfile;
	FILE *outputfile2;
	FILE *fileconf;

	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	cout << cwd << endl;

	int xi;
	int yi;
	int zi;
	int start,finish;
	float nodata;
	char name[128];
	char ext[4];

	if ((fileconf=fopen(conf,"r"))==NULL){
				cout << "Can't find CONF file: " << conf << "\n";
				return 1;
			}

	fscanf(fileconf,"%d;%d;%d\n",&xi,&yi,&zi);
	fscanf(fileconf,"%f\n",&nodata);
	fscanf(fileconf,"%d\n",&start);
	fscanf(fileconf,"%d\n",&finish);
	fscanf(fileconf,"%s\n",name);
	fscanf(fileconf,"%s\n",ext);

	cout << nodata;
	float *original;
	float *original2;
	original = new (nothrow) float[xi*yi*zi];
	original2 = new (nothrow) float[xi*yi*zi];
	int f=0;
	while (f<xi*yi*zi){
		original[f]=0;
		original2[f]=0;
		f=f+1;
	}
	int j;
	int i=start;
	while (i<=finish){
		sprintf(filename,"%s%i%s",name,i,ext);
		if ((inputfile=fopen(filename,"r"))==NULL){
				cout << "Failed " << filename << "\n";
				return 1;
			}
		j=0;
		while (j<xi*yi*zi){
			fscanf(inputfile,"%f\n",&valor);
			if (valor!=nodata){
				original[j]=original[j]+valor;
				//cout << original[j];
			}
			else {
				original[j]=nodata;
			}
			j=j+1;
			}
			fclose(inputfile);
			i=i+1;
		}

	outputfile=fopen(filename2,"w");
	i=0;
	while (i<xi*yi*zi){
		if (original[i]!=nodata){
			fprintf(outputfile,"%f\n",original[i]/(finish-start+1));
			original[i]=original[i]/(finish-start+1);
		}
		else {
			fprintf(outputfile,"%f\n",nodata);
		}
		i=i+1;
	}
	fclose(outputfile);

	i=start;
	while (i<=finish){
		sprintf(filename,"%s%i%s",name,i,ext);
			if ((inputfile=fopen(filename,"r"))==NULL){
					cout << "Failed " << filename << "\n";
					return 1;
				}
			j=0;
			while (j<xi*yi*zi){
				fscanf(inputfile,"%f\n",&valor);
				if (valor!=nodata){
					original2[j]=original2[j]+pow(original[j]-valor,2);
					//cout << valor << ";"  << pow(original[j]-valor,2) << original[j] << endl;
				}
				else {
					original2[j]=nodata;
				}
				j=j+1;
			}
			fclose(inputfile);
			i=i+1;
		}

	outputfile2=fopen(filename3,"w");
	i=0;
	while (i<xi*yi*zi){
		if (original[i]!=nodata){
			fprintf(outputfile2,"%f\n",original2[i]/(finish-start+1));
		}
		else{
			fprintf(outputfile2,"%f\n",nodata);
		}
		i=i+1;
	}
	fclose(outputfile2);
	cout << "Finished";
}
