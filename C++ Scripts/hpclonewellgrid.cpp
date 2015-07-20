/*
 * hpclonewellgrid.cpp
 *
 *  Created on: 15 de Abr de 2011
 *      Author: pedro.correia
 */

#include <algorithm>
#include "iostream"
#include <new>
#include "fstream"
#include "unistd.h"
#include "errno.h"
#include "math.h"
#include <ctype.h>

using namespace std;

int check_number_of_columns(char path[1024],int header_size){ // Function that returns the number of columns in file.
	// Function internal variables:
	FILE *hard_data_file; // FILE variable since we need one to actually work over the file.
	int header=0; // This is the header counter starting from zero.
	int columns=0; // This is the columns counter starting from 0.
	char c,d; // This is the character variable since our file reading is done char by char and not by string.
	int counter=0; // Loop counter for character count.

	// Opening input path to FILE declaration. If null than a debug error is printed.
	if ((hard_data_file=fopen(path,"r"))==NULL){
			cout << "ERRO: Can't find hard data file while checking number of columns. Check hard data file path.\n";
			return 0; // returning 0 means the program is over since the hard data file was not found.
		}

	// Passing header with user input header_size variable. When the 'c' variable equal a '\n' meaning end of
	// file symbol adds one to the header counter until it reaches all header rows. From then on we only have
	// real numeric data.
	while (header<header_size){
		c=fgetc(hard_data_file); // fgetc receives char by char from file instead of string or stream.
		if (c=='\n'){header=header+1;} // When the char is equal to '\n' then we've reached a new row.
	}

	// Counting the number of columns. It only does this for the first row (its assumed the rest is similar).
	// Since it works char by char instead of string with two working variables ('a' and 'b') it can detect
	// when a whitespace is followed by a non-whitespace, therefore meaning a column was found. The only
	// exception is when the there's no whitespace before the first column. If so it also counts a column.
	c='a'; // Since the 'c' variable was last a '\n' (end of line symbol) i've given it a random value so the
	       // the next loop may start.
	while (c!='\n'){ // May use 0x0A but its possible the code 0x0A only works in windows. Change if in UNIX (try 0x0D).
		c=fgetc(hard_data_file); // Still gathering char by char from file.
		if (counter>0){ // To every character except the first.
			if (c!=9 && d==9){ // If 'c' is not whitespace and 'd' is then add 1 to columns.
				columns=columns+1;
			}
			if (c!=32 && d==32 && c!='\n'){ // If 'c' is not tab and 'd' is then add 1 to columns (also prevents the case of whitespace after a number in the final column).
				columns=columns+1;
			}
		}
		else { // Only to the first character of the row.
			if (c!=9 || c!=32) columns=columns+1; // if first char is different from whitespace or tab add 1 to columns.
			counter=counter+1; // This is only important when counter is 0 but we still need to add.
		}
		d=c;
	}
	fclose(hard_data_file); // Closing FILE variable since has no purpose.
	return columns; // Returning columns as function result.
}

int check_number_of_rows(char path[1024],int header_size){ // Function that returns the number of rows in file.
	// Function internal variables:
	FILE *hard_data_file; // FILE variable since we need one to actually work over the file.
	int header=0; // This is the header counter starting from zero.
	int rows=0; // This is the rows counter starting from 0.
	char c; // This is the character variable since our file reading is done char by char and not by string.

	// Opening input path to FILE declaration. If null than a debug error is printed.
	if ((hard_data_file=fopen(path,"r"))==NULL){
			cout << "ERROR: Can't find hard data file while checking number of rows. Check hard data file path.\n";
			return 0; // returning 0 means the program is over since the hard data file was not found.
		}

	// Passing header with user input header_size variable. When the 'c' variable equal a '\n' meaning end of
	// file symbol adds one to the header counter until it reaches all header rows. From then on we only have
	// real numeric data.
	while (header<header_size){
		c=fgetc(hard_data_file); // fgetc receives char by char from file instead of string or stream.
		if (c=='\n'){header=header+1;} // When the char is equal to '\n' then we've reached a new row.
	}

	while (!feof(hard_data_file)){ // While does not reach end of file symbol (eof or feof)...
		c=fgetc(hard_data_file); // Still gathering char by char from file.
		if (c=='\n'){rows=rows+1;} // When it encounters the '\n' symbol meaning end of line adds 1 to rows.
	}
	fclose(hard_data_file); // Closing FILE variable since has no purpose.
	return rows; // Returning rows as function result.
}

float **load_hard_data(char path[1024],int rows,int columns,int header_size=0){ // Function that returns hard data from file to multidimensional array.
	// Function internal variables:
	FILE *hard_data_file; // FILE variable since we need one to actually work over the file.
	char c='a'; // This is the character variable since our file reading is done char by char and not by string.
	int lines=0; // Loop counter for number of rows.
	int column; // Loop counter for number of columns.
	int h=0; // Loop counter for header size.
	// Opening input path to FILE declaration. If null than a debug error is printed.
	if ((hard_data_file=fopen(path,"r"))==NULL){
		cout << "ERROR: Can't find hard data file while loading data into memory. Check hard data file path.\n";
		return 0; // returning 0 means the program is over since the hard data file was not found.
	}

	// Allocating multi-dimensional array (e.g. array of arrays, type float).
	float **hard_data;
	hard_data = new (nothrow) float*[rows];
	int i;
	for (i = 0; i < rows; ++i){hard_data[i]=new (nothrow) float[columns];}

	// Two loops (rows and columns) to attribute the values on the file to the bi-dimensional array.
	// But first lets skip the header:
	while (h<header_size){
			c=fgetc(hard_data_file);
			if (c=='\n'){h=h+1;}
		}

	// Loop to read all rows over all columns in the file.
	while (lines<=rows){
		column=0;
		while (column<columns){
			if (column==columns-1){
				fscanf(hard_data_file,"%f\n",&hard_data[lines][column]);
			}
			else {
				fscanf(hard_data_file,"%f",&hard_data[lines][column]);
			}
			column=column+1;
		}
		lines=lines+1;
	}
	// From now on we can call the array as a multidimensional, meaning [i][j].
	fclose(hard_data_file);
	return hard_data;
}


float *load_single_grid_data(char path[1024], int rows, int header_size=0){
	// Function internal variables:
	FILE *grid_data_file; // FILE variable since we need one to actually work over the file.
	char c='a'; // This is the character variable since our file reading is done char by char and not by string.
	int h=0; // Header counter for getting over this rows.
	int i=0; // Data counter to retrieve all values from file.

	// Opening input path to FILE declaration. If null than a debug error is printed.
	if ((grid_data_file=fopen(path,"r"))==NULL){
		cout << "ERROR: Can't find single grid data file while attempting to retrieve data. Check single grid data file path.\n";
		return 0; // returning 0 means the program is over since the hard data file was not found.
	}
	// Getting over header rows.
	while (h<header_size){
		c=fgetc(grid_data_file);
		if (c=='\n'){h=h+1;}
	}
	// Allocating the single array for that considering the input number of rows (expected to not to consider  header rows).
	float *grid_data;
	grid_data = new (nothrow) float[rows];
	// Retrieving information from file to single array.
	while (i< rows){
		fscanf(grid_data_file,"%f\n",&grid_data[i]);
		i++;
	}
	// Returning the single array as function result.
	return grid_data;
}

int gslib_write_multi_array_to_file(char path[1024],float **array,int rows,int columns){
	FILE *output_file; // FILE variable for outputting information to file.
	output_file=fopen(path,"w"); // Opening file to FILE variable. No error checking, take caution.

	int i; // Write loop counter for number of rows.
	int j; // Write loop counter for number of columns.

	fprintf(output_file,"gslib_file_format\n");
	fprintf(output_file,"%d\n",columns);
	for (i=0;i<columns;i++){
		fprintf(output_file,"variable%d\n",i+1);
	}
	for (i=0; i < rows; i++) { // Writing information block by block to file considering all columns.
			for (j=0; j < columns; j++) {
				if (j==columns-1){ // If its the final column we need to chang row.
					fprintf(output_file,"%f\n",array[i][j]);
				}
				else { // If any other we remain writing with only tab separator.
					fprintf(output_file,"%f\t",array[i][j]);
				}
			}
	}
	fclose(output_file); // Closing file since we don't need it anymore.
	return 0; // Returning 0 meaning function has completed its purpose.
}

int simple_write_multi_array_to_file(char path[1024],float **array,int rows,int columns){
	FILE *output_file; // FILE variable for outputting information to file.
	output_file=fopen(path,"w"); // Opening file to FILE variable. No error checking, take caution.
	int i; // Write loop counter for number of rows.
	int j; // Write loop counter for number of columns.
	for (i=0; i < rows; i++) { // Writing information block by block to file considering all columns.
			for (j=0; j < columns; j++) {
				if (j==columns-1){ // If its the final column we need to chang row.
					fprintf(output_file,"%f\n",array[i][j]);
				}
				else { // If any other we remain writing with only tab separator.
					fprintf(output_file,"%f\t",array[i][j]);
				}
			}
	}
	fclose(output_file); // Closing file since we don't need it anymore.
	return 0; // Returning 0 meaning function has completed its purpose.
}

int main()
{
	cout << "******************************************************\n";
	cout << "Starting clone well from grid algorithm.\n";
	cout << "******************************************************\n";

	char conf[256]="gridclonewell.pin";

	char filename[1024];
	// Caminho e nome do ficheiro de saida.
	char filename2[1024];
	// Abrir ficheiro de entrada e ficheiro de saida.
	char filename3[1024];
	//FILE *inputfile;
	FILE *fileconf;

	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	cout << cwd << endl;

	if ((fileconf=fopen(conf,"r"))==NULL){
		cout << "Can't find CONF file: " << conf << "\n";
		return 1;
	}

	int x,y,z;
	int xs,ys,zs;
	float cx,cy,cz;
	int header1,header2;
	int wellx,welly,wellz;
	float nodata;
	fscanf(fileconf,"%[^\n]s\n",filename);
	fscanf(fileconf,"%s\n",filename2);
	fscanf(fileconf,"%[^\n]s\n",filename2);
	fscanf(fileconf,"%s\n",filename3);
	fscanf(fileconf,"%[^\n]s\n",filename3);
	fscanf(fileconf,"%d\n",&x);
	fscanf(fileconf,"%d\n",&y);
	fscanf(fileconf,"%d\n",&z);
	fscanf(fileconf,"%d\n",&xs);
	fscanf(fileconf,"%d\n",&ys);
	fscanf(fileconf,"%d\n",&zs);
	fscanf(fileconf,"%f\n",&cx);
	fscanf(fileconf,"%f\n",&cy);
	fscanf(fileconf,"%f\n",&cz);
	fscanf(fileconf,"%d\n",&header1);
	fscanf(fileconf,"%d\n",&header2);
	fscanf(fileconf,"%d\n",&wellx);
	fscanf(fileconf,"%d\n",&welly);
	fscanf(fileconf,"%d\n",&wellz);
	fscanf(fileconf,"%f\n",&nodata);
	fclose(fileconf);

	cout << "Input file:  " << filename << "\n";
	cout << "Output file: " << filename2 << "\n";
	cout << "Cloning from file: " << filename3 << "\n";
	cout << "******************************************************\n";
	cout << "Grid header size is: " << header1 << endl;
	cout << "Number of blocks in X: " << x << endl;
	cout << "Number of blocks in Y: " << y << endl;
	cout << "Number of blocks in Z: " << z << endl;
	cout << "Block size in X: " << xs << endl;
	cout << "Block size in Y: " << ys << endl;
	cout << "Block size in Z: " << zs << endl;
	cout << "Initial coordinate in X: " << cx << endl;
	cout << "Initial coordinate in Y: " << cy << endl;
	cout << "Initial coordinate in Z: " << cz << endl;
	cout << "Loading grid data into-memory... ";
	float *grid_data;
	grid_data=load_single_grid_data(filename,x*y*z,header1);
	cout << "Finished.\n";
	cout << "******************************************************\n";
	cout << "Well data header size is: " << header2 << endl;
	cout << "Column for X coordinate: " << wellx << endl;
	cout << "Column for Y coordinate: " << welly << endl;
	cout << "Column for Z coordinate: " << wellz << endl;
	cout << "Loading well data into-memory... ";
	int rows,columns;
	rows=check_number_of_rows(filename3,header2);
	columns=check_number_of_columns(filename3,header2);
	float **well_data;
	well_data=load_hard_data(filename3,rows,columns,header2);
	//cout << well_data[10][0] << " ; " << rows << endl;
	cout << "Finished.\n";
	cout << "******************************************************\n";
	cout << "Testing coordinates for localization ... ";
	int *xcoord,*ycoord,*zcoord;
	int counter=0;
	int i;
	for (i=0 ; i < rows ; i++){
		if (well_data[i][wellx-1]!=nodata && well_data[i][welly-1]!=nodata && well_data[i][wellz-1]!=nodata){
			if (well_data[i][wellx-1]-cx>0 && well_data[i][welly-1]-cy>0 && well_data[i][wellz-1]-cz>0){
				if (well_data[i][wellx-1]<=cx+x*xs && well_data[i][welly-1]<=cy+y*ys && well_data[i][wellz-1]<=cz+z*zs){
					counter=counter+1;
				}
			}
		}
	}
	if (counter==0){cout << "ERROR: No point in file seems to be inside the grid. Check your parameters.\n"; return 0;}
	xcoord = new(nothrow) int[counter];
	ycoord = new(nothrow) int[counter];
	zcoord = new(nothrow) int[counter];
	/*float **hard_data;
	hard_data = new (nothrow) float*[rows];
	int i;
	for (i = 0; i < rows; ++i){hard_data[i]=new (nothrow) float[columns];}*/
	int counter2=0;
	for (i=0 ; i < rows ; i++){
		if (well_data[i][wellx-1]!=nodata && well_data[i][welly-1]!=nodata && well_data[i][wellz-1]!=nodata){
			if (well_data[i][wellx-1]-cx>0 && well_data[i][welly-1]-cy>0 && well_data[i][wellz-1]-cz>0){
				if (well_data[i][wellx-1]<=cx+x*xs && well_data[i][welly-1]<=cy+y*ys && well_data[i][wellz-1]<=cz+z*zs){
					//cout << well_data[i][wellx-1] << " ; " << (well_data[i][wellx-1]-cx)/xs << endl;
					xcoord[counter2]=int((well_data[i][wellx-1]-cx)/xs);
					ycoord[counter2]=int((well_data[i][welly-1]-cy)/ys);
					zcoord[counter2]=int((well_data[i][wellz-1]-cz)/zs);
					//cout << xcoord[counter2]<< endl;
					counter2=counter2+1;
				}
			}
		}
	}
	//cout << counter << " ; " << xcoord[9] << endl;
	cout << "Finished.\n";
	cout << "Retrieving data from grid ... ";
	int j;
	float **clone_data;
	clone_data = new (nothrow) float*[counter];
	for (i = 0 ; i < counter; ++i){clone_data[i]=new (nothrow) float[4];}
	for (i = 0 ; i < counter ; i++){
		for (j=0 ; j < 4 ; j++){
			if (j==0){clone_data[i][j]=xcoord[i]*(xs)+cx;}
			else if (j==1){clone_data[i][j]=ycoord[i]*(ys)+cy;}
			else if (j==2){clone_data[i][j]=zcoord[i]*(zs)+cz;}
			else if (j==3){clone_data[i][j]=grid_data[(zcoord[i]-1)*x*y+(ycoord[i]-1)*x+(xcoord[i]-1)];}
		}
	}
	cout << "Finished.\n";
	cout << "Writing result to file ... ";
	gslib_write_multi_array_to_file(filename2,clone_data,counter,4);
	cout << "Finished.\n";
	return 0;
}
