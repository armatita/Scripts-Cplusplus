/*
 * bender.cpp
 *
 *  Created on: 6 de Nov de 2010
 *      Author: Pedro Correia
 */

#include <iostream>
#include <fstream>
#include <new>

using namespace std;


int main() {
	cout << "********************************************************\n";
	cout << "Bender for bending and unbending off to 3-dimensional\n";
	cout << "grids. You can also make the bending map from a horizon.\n";
	cout << "This version is code dependent and not interactive with\n";
	cout << "the user. Please modify only the desired paths for grid\n";
	cout << "choice and respective parameters.\n";
	cout << "********************************************************\n\n";

	// Number of blocks in original grid and bending map (x and y):
	int x=1380;
	int y=993;
	int z=133;
	int zef=133;

	// No-Data value:
	float nodata=-999.25;



	// Paths and file names of bending map, original and final output grid:
	char map_path[256]="C:\\Documents and Settings\\Administrator\\Desktop\\Final\\output\\Short_GALP_bendmap.PRN";
	char grid_path[256]="C:\\Documents and Settings\\Administrator\\Desktop\\Final\\output\\resmedia.out";
	char final_path[256]="C:\\Documents and Settings\\Administrator\\Desktop\\Final\\output\\bended.prn";

	// Final grid specifications:
	// int upper=5;
	// int zsize=108;


	// Program variables.
	// Array variables for bending map, input grid and output grid.
	float *map;
	float *grid;


	// FILE variables for bending map, input grid and output grid.
	FILE *mapfile;
	FILE *gridfile;
	FILE *finalfile;


	float valor;
	int max;
	cout << "**********************INPUT FILES***********************\n";
	cout << "********************************************************\n";

	// Opening bending map file.
	mapfile=fopen(map_path,"r");
	// Opening error management.
	if (mapfile==NULL){
			cout << "1) Bending map FILE NOT FOUND.\n";
			return 1;
		}
	else cout << "1) Bending map file found. ";

	// Allocating bending map.
	map=new(nothrow)float[x*y];
	// Allocation error management.
	if (map==NULL){
		cout << "Bending map ALLOCATION FAILED.\n";
		return 1;
	}
	else cout << "Bending map successfully allocated.\n";
	// Passing information in file to memory.
	for (int n=0; n<x*y; n++) {
		fscanf(mapfile,"%f\n",&valor);
		map[n]=valor;
		if (n==0) max=valor;
		if (valor>max) max=valor;
	}
	//cout << max << "NOva" << endl;
	// Opening grid file.
	gridfile=fopen(grid_path,"r");
	// Opening error management.
	if (gridfile==NULL){
			cout << "2) Grid FILE NOT FOUND.\n";
			return 1;
		}
	else cout << "2) Grid file found. ";

	// Allocating grid.
	grid=new(nothrow)float[x*y*z];
	// Allocation error management.
	if (grid==NULL){
		cout << "Grid ALLOCATION FAILED.\n";
		return 1;
	}
	else cout << "Grid successfully allocated.\n";
	// Passing information in grid file to memory.
	for (int n=0; n<x*y*z; n++) {fscanf(gridfile,"%f\n",&valor); grid[n]=valor;}
	cout << "********************************************************\n\n";

	cout << "*********************OUTPUT FILE************************\n";
	cout << "********************************************************\n";
	// Calculating z number of columns for unbended grid.

	// Opening unbended grid file to write.
	finalfile=fopen(final_path,"w");
	// Opening error management.
	if (finalfile==NULL){
			cout << "3) Output FAILED TO GENERATE.\n";
			return 1;
		}
	else {
		cout << "3) Output file generated.\n";
		cout << "3) OUTPUT GRID SIZE is X: " << x << " Y: " << y << " Z: " << z << "\n";
	}

	cout << "********************************************************\n\n";

	cout << "********************Bending grid************************\n";
	cout << "********************************************************\n";

	cout << "4) Calculating...";
	//int l1=zz/5;
	//int l2=2*(zz/5);
	//int l3=3*(zz/5);
	//int l4=4*(zz/5);

	int yc=0;
	int xc;
	int zc;
	int absvalue;
	int camada=x*y;
	while (yc<y) {
		xc=0;
		while (xc<x) {
			zc=0;
			while (zc<z) {
				if (zc<z-map[x*yc+xc])
				{
					absvalue=map[x*yc+xc];
					grid[camada*zc+x*yc+xc]=grid[camada*(absvalue+zc)+x*yc+xc];
				}
				else
				{
					grid[camada*zc+x*yc+xc]=nodata;
				}
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
	fprintf(finalfile,"%i_%i_%i\n1\nvariable\n",x,y,z);
	for (int n=0; n<x*y*zef; n++) { // suposed to be zz
		fprintf(finalfile,"%f\n",grid[n]);
	}
	delete[] map;
	delete[] grid;
	return 0;
}
