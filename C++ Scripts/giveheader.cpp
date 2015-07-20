/*
 * giveheader.cpp
 *
 *  Created on: 20 de Dez de 2010
 *      Author: Administrator
 */

#include <iostream>
#include <fstream>
#include <new>

using namespace std;


int main() {

	int x=1380;
	int y=993;
	int z=108;
	int sizeofgrid=x*y*z;

	char input_path[256]="C:\\Documents and Settings\\Administrator\\Desktop\\Testes\\AI_5_media_WIN.tsi";
	char output_path[256]="C:\\Documents and Settings\\Administrator\\Desktop\\Testes\\AI_5_media_sgems.tsi";
	char variable[256]="variable";

	FILE *inputfile;
	FILE *outputfile;

	float valor;
	inputfile=fopen(input_path,"r");
	outputfile=fopen(output_path,"w");
	if (inputfile==NULL){cout << "1) INPUT FILE NOT FOUND.\n";return 1;}
	else cout << "1) Input file found. ";

	fprintf(outputfile,"%i_%i_%i\n",x,y,z);
	fprintf(outputfile,"%i\n",1);
	fprintf(outputfile,"%s\n",variable);
	for (int n=0; n<sizeofgrid; n++) {
		fscanf(inputfile,"%f\n",&valor);
		fprintf(outputfile,"%f\n",valor);
	}
	cout << "Finished";
	return 0;
}
