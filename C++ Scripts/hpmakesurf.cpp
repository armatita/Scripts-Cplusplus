/*
 * hpmakesurf.cpp
 *
 *  Created on: 27 de Abr de 2011
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

int estimate_surface_blocks(float *surface,int gx,int gy,int gxs,int gys,float gxf,float gyf,float **horizon,int rows,int xcol,int ycol, int zcol,float nodata){
	int i,j,s;
	char a;
	float value,dist;
	float sumvalue=0;
	float sumdist=0;
	float localx,localy,appex;
	for (i=0; i<gx ; i++){
		for (j=0 ; j<gx ; j++){
			if (surface[gx*i+j]==nodata){
				localx=j*gxs+gxf+gxs/2;
				localy=i*gys+gyf+gys/2;
				for (s=0 ; s<rows ; s++){
					appex=pow((horizon[s][ycol-1]-localy),2)+pow((horizon[s][xcol-1]-localx),2);
					//cout << appex << endl;
					dist=1/appex;
					value=(horizon[s][zcol-1])*dist;
					sumdist=sumdist+dist;
					sumvalue=sumvalue+value;
				}
				surface[gx*i+j]=sumvalue/sumdist;
				//cout << sumvalue << " ; " << sumdist << " ; " << surface[gx*i+j] << endl;
				//cin >> a;
			}
		}
		//cout << "\nStep " << i+1 << " of " << gy << " finished.\n";
	}
	return 0;
}

int from_horizon_to_surface(float *surface,int gx,int gy,int gxs,int gys,float gxf,float gyf,float **horizon,int rows,int xcol,int ycol, int zcol,float nodata){
	int i;
	int j;
	int s;
	int count;
	float sum;
	float localx,localy;
	for (i=0; i<gy ; i++){
		for (j=0 ; j<gx ; j++){
			localx=j*gxs+gxf;
			localy=i*gys+gyf;
			//cout << "Localx is: " << localx << " and Localy is: " << localy << endl;
			count=0;
			sum=0;
			for (s=0 ; s<rows ; s++){
				if (horizon[s][xcol-1]>=localx && horizon[s][xcol-1]<(localx+gxs)){
					if (horizon[s][ycol-1]>=localy && horizon[s][ycol-1]<(localy+gys)){
						sum = sum + horizon[s][zcol-1];
						count = count + 1;
					}
				}
			}
			if (count==0){surface[gx*i+j]=nodata;}
			else{surface[gx*i+j]=sum/count;}
		}
		cout << "\nStep " << i+1 << " of " << gy << " finished.\n";
	}
	return 0;
}

int check_number_of_columns(char path[1024],int header_size){ // Function that returns the number of columns in file.
	// Function internal variables:
	FILE *hard_data_file; // FILE variable since we need one to actually work over the file.
	int header=0; // This is the header counter starting from zero.
	int columns=0; // This is the columns counter starting from 0.
	char c,d; // This is the character variable since our file reading is done char by char and not by string.
	int counter=0; // Loop counter for character count.

	// Opening input path to FILE declaration. If null than a debug error is printed.
	if ((hard_data_file=fopen(path,"r"))==NULL){
			cout << "Can't find hard data file while checking number of columns. Check hard data file path.\n";
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
			cout << "Can't find hard data file while checking number of rows. Check hard data file path.\n";
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
		cout << "Can't find hard data file while checking GSLIB header for number of columns. Check hard data file path.\n";
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

int surface_write_single_array_to_file(char path[1024],float *array,int rows,int gx,int gy, int gxs,int gys,float gxf,float gyf,float nodata){
	FILE *output_file; // FILE variable for outputting information to file.
	int j;
	int count=0;
	float first,max,min;
	int sign=0;
	for (j=0; j<rows ; j++){
		if (array[j]==nodata){sign=1;}
		if (array[j]!=nodata && count==0){
			count=1;
			first=array[j];
			max=array[j];
			min=array[j];
		}
		else{
			if (array[j]>max && array[j]!=nodata){
				max=array[j];
			}
			if (array[j]<min && array[j]!=nodata){
				min=array[j];
			}
		}
	}
	output_file=fopen(path,"w"); // Opening file to FILE variable. No error checking, take caution.
	fprintf(output_file,"%i_%i_%i_%i_%f_%f_%f_%f_%f_%f\n",gx,gy,gxs,gys,gxf,gyf,first,max,min,nodata);
	fprintf(output_file,"1\n");
	if (sign==0){fprintf(output_file,"depth\n");}
	else {fprintf(output_file,"depth_with_no_data\n");}
	int i; // Write loop counter for number of rows.
	while (i<rows){ // Writing information block by block to file.
		fprintf(output_file,"%f\n",array[i]);
		i++;
	}
	fclose(output_file); // Closing file since we don't need it anymore.
	return 0; // Returning 0 meaning function has completed its purpose.
}


int simple_write_single_array_to_file(char path[1024],float *array,int rows){
	FILE *output_file; // FILE variable for outputting information to file.
	output_file=fopen(path,"w"); // Opening file to FILE variable. No error checking, take caution.
	int i; // Write loop counter for number of rows.
	while (i<rows){ // Writing information block by block to file.
		fprintf(output_file,"%f\n",array[i]);
		i++;
	}
	fclose(output_file); // Closing file since we don't need it anymore.
	return 0; // Returning 0 meaning function has completed its purpose.
}

int main()
{
	cout << "******************************************************\n";
	cout << "Starting make surface algorithm.\n";
	cout << "******************************************************\n";

	char conf[256]="makesurf.pin";

	char filename[1024];
	// Caminho e nome do ficheiro de saida.
	char filename2[1024];
	// Abrir ficheiro de entrada e ficheiro de saida.
	//FILE *inputfile;
	FILE *fileconf;

	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	cout << cwd << endl;

	int header;
	int gx,gy,gxs,gys;
	float gxf,gyf;
	int x;
	int y;
	int z;
	float nodata;

	if ((fileconf=fopen(conf,"r"))==NULL){
		cout << "Can't find CONF file: " << conf << "\n";
		return 1;
	}
	fscanf(fileconf,"%[^\n]s\n",filename);
	fscanf(fileconf,"%s\n",filename2);
	fscanf(fileconf,"%[^\n]s\n",filename2);
	fscanf(fileconf,"%d\n",&header);
	fscanf(fileconf,"%d\n",&gx);
	fscanf(fileconf,"%d\n",&gy);
	fscanf(fileconf,"%d\n",&gxs);
	fscanf(fileconf,"%d\n",&gys);
	fscanf(fileconf,"%f\n",&gxf);
	fscanf(fileconf,"%f\n",&gyf);
	fscanf(fileconf,"%d\n",&x);
	fscanf(fileconf,"%d\n",&y);
	fscanf(fileconf,"%d\n",&z);
	fscanf(fileconf,"%f\n",&nodata);

	cout << "Input file:  " << filename << "\n";
	cout << "Output file: " << filename2 << "\n";
	cout << "****************Horizon specifications****************\n";
	cout << "X coordinate column is: " << x << endl;
	cout << "Y coordinate column is: " << y << endl;
	cout << "Depth column is: " << z << endl;
	cout << "Loading data into-memory... ";
	float **data;
	int rows;
	int columns;
	columns=check_number_of_columns(filename,header);
	rows=check_number_of_rows(filename,header);
	data=load_hard_data(filename,rows,columns,header);
	cout << "Finished.\n";
	cout << "File has " << columns << " columns and " << rows << " rows.\n";
	cout << "****************Grid specifications*******************\n";
	cout << "Surface grid has " << gx << " number of blocks in X.\n";
	cout << "Surface grid has " << gy << " number of blocks in Y.\n";
	cout << "Surface grid has block size of " << gxs << " in X.\n";
	cout << "Surface grid has block size of " << gys << " in Y.\n";
	cout << "First X coordinate in surface grid is " << gxf << ".\n";
	cout << "First Y coordinate in surface grid is " << gyf << ".\n";
	cout << "Allocating surface grid in memory... ";
	float *surface;
	surface = new (nothrow) float[gx*gy];
	cout << "Finished.\n";
	cout << "Surface grid has 1 column and " << gx*gy << " rows.\n";
	cout << "**************Building surface data*******************\n";
	cout << "Inserting horizon data onto grid... ";
	from_horizon_to_surface(surface,gx,gy,gxs,gys,gxf,gyf,data,rows,x,y,z,nodata);
	cout << "Finished.\n";
	//cout << "Estimating remaining blocks... ";
	//estimate_surface_blocks(surface,gx,gy,gxs,gys,gxf,gyf,data,rows,x,y,z,nodata);
	//cout << "Finished.\n";
	cout << "Writing to file...";
	surface_write_single_array_to_file(filename2,surface,gx*gy,gx,gy,gxs,gys,gxf,gyf,nodata);
	//simple_write_single_array_to_file(filename2,surface,gx*gy);
	cout << "Finished.\n";

	return 0;
}
