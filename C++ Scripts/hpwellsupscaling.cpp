/*
 * hpwellsupscaling.cpp
 *
 *  Created on: 30 de Mar de 2011
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
			if (c!=9 && d==9 && c!='\n'){ // If 'c' is not whitespace and 'd' is then add 1 to columns.
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

float upcoord(float **hard_data,int c,int ci,float inf,float sup,int rows,float nodata){
	int i=0;
	int count=0;
	float result=0;
	while (i<rows){
		if (hard_data[i][c-1]!=nodata){
			if (hard_data[i][c-1]>=inf && hard_data[i][c-1]<sup){
				result=result+hard_data[i][ci-1];
				count++;
			}
		}
		i++;
	}
	if (count==0){return nodata;}
	return result/count;
}

float upscale(float **hard_data,int c,int ci,int op,float inf,float sup,int rows,float nodata){
	int i=0;
	int count=0;
	float result=0;
	float max;
	float min;
	int cc=0;
	if (op==0){
		while (i<rows){
			if (hard_data[i][c-1]!=nodata){
				if (hard_data[i][c-1]>=inf && hard_data[i][c-1]<sup){
					if (hard_data[i][ci]!=nodata){
						result=result+hard_data[i][ci];
						count++;
					}
				}
			}
			i++;
		}
		if (count==0){return nodata;}
		return (result/count);
	}
	else if (op==1){
		while (i<rows){
			if (hard_data[i][c-1]!=nodata){
				if (hard_data[i][c-1]>=inf && hard_data[i][c-1]<sup){
					if (hard_data[i][ci]!=nodata){
						if (cc==0){max=hard_data[i][ci];cc++;count++;}
						else {
							if (hard_data[i][ci]>max){max=hard_data[i][ci];count++;}
						}
					}
				}
			}
			i++;
		}
		if (count==0){return nodata;}
		return max;
	}
	else if (op==2){
		while (i<rows){
			if (hard_data[i][c-1]!=nodata){
				if (hard_data[i][c-1]>=inf && hard_data[i][c-1]<sup){
					if (hard_data[i][ci]!=nodata){
						if (cc==0){min=hard_data[i][ci];cc++;count++;}
						else {
							if (hard_data[i][ci]<min){min=hard_data[i][ci];}
							count++;
						}
					}
				}
			}
			i++;
		}
		if (count==0){return nodata;}
		return min;
	}
	return nodata;
}

int main()
{
	cout << "******************************************************\n";
	cout << "Starting well upscale algorithm.\n";
	cout << "******************************************************\n";

	char conf[256]="wellupscale.pin";

	char filename[1024];
	// Caminho e nome do ficheiro de saida.
	char filename2[1024];
	// Abrir ficheiro de entrada e ficheiro de saida.
	//FILE *inputfile;
	FILE *outputfile;
	FILE *fileconf;

	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	cout << cwd << endl;

	if ((fileconf=fopen(conf,"r"))==NULL){
		cout << "Can't find CONF file: " << conf << "\n";
		return 1;
	}

	// Número da coluna de cada uma das coordenadas.
	int xi;
	int yi;
	int zi;

	// Step ou passo do upscale.
	int step;

	// Tamanho do header e valor no-data.
	int header;
	float nodata;

	// Operador de upscaling.
	int op;

	fscanf(fileconf,"%[^\n]s\n",filename);
	fscanf(fileconf,"%s\n",filename2);
	fscanf(fileconf,"%[^\n]s\n",filename2);
	fscanf(fileconf,"%d_%d_%d_%d_%d_%d\n",&xi,&yi,&zi,&step,&op,&header);
	fscanf(fileconf,"%f\n",&nodata);
	cout << "Input file:  " << filename << "\n";
	cout << "Output file: " << filename2 << "\n";
	cout << "******************************************************\n";
	cout << "X coordinate column is: " << xi << endl;
	cout << "Y coordinate column is: " << yi << endl;
	cout << "Z-Depth coordinate column is: " << zi << endl;
	cout << "Header size is: " << header << endl;
	cout << "No data value is: "  << nodata << endl;
	cout << "******************************************************\n";
	cout << "Checking for true number of columns and rows in file. ";
	int number_of_columns;
	int number_of_rows;
	number_of_columns=check_number_of_columns(filename,header);
	number_of_rows=check_number_of_rows(filename,header);
	cout << "Finished.\n";
	cout << "True number of columns is: " << number_of_columns << endl;
	cout << "True number of rows is: " << number_of_rows << endl;
	if (xi>number_of_rows){cout << "ERROR: The column number for X coordinate you gave is superior to the actual number of the columns in file. Check the parameters." << endl; return 0;}
	if (yi>number_of_rows){cout << "ERROR: The column number for Y coordinate you gave is superior to the actual number of the columns in file. Check the parameters." << endl; return 0;}
	if (zi>number_of_rows){cout << "ERROR: The column number for Z coordinate you gave is superior to the actual number of the columns in file. Check the parameters." << endl; return 0;}
	cout << "******************************************************\n";
	cout << "Loading data into memory. ";
	float **hard_data;
	hard_data=load_hard_data(filename,number_of_rows,number_of_columns,header);
	cout << "Finished.\n";
	cout << "Retrieving minimum and maximum depth values. ";
	int i;
	float min; // Precisamos de saber o minimo e máximo de profundidade no ficheiro considerado.
	float max;
	int ccc=0;
	for (i=0; i<number_of_rows ; i++){
		if (hard_data[i][zi-1]!=nodata){
			if (ccc==0) { min=hard_data[i][zi-1]; max=hard_data[i][zi-1];ccc++;}
			else {
				if (hard_data[i][zi-1]<min){min=hard_data[i][zi-1];}
				if (hard_data[i][zi-1]>max){max=hard_data[i][zi-1];}
			}
		}
	}
	cout << "Finished.\n";
	cout << "Minimum depth is: " << min << endl;
	cout << "Maximum depth is: " << max << endl;
	if (op==0){cout << "Operator for upscale is: Mean\n";}
	else if (op==1){cout << "Operator for upscale is: Maximum\n";}
	else if (op==2){cout << "Operator for upscale is: Minimum\n";}
	cout << "******************************************************\n";
	cout << "Proceeding with the file upscale and writing new data. ";
	outputfile=fopen(filename2,"w");

	fprintf(outputfile,"Well_upscaled_at_step_%d\n",step);
	fprintf(outputfile,"%d\n",number_of_columns);
	int c=0;
	int vname=1;
	while (c<number_of_columns){
		if (c+1==xi){fprintf(outputfile,"X\n");}
		else if (c+1==yi){fprintf(outputfile,"Y\n");}
		else if (c+1==zi){fprintf(outputfile,"Z_Depth\n");}
		else {fprintf(outputfile,"Variable%d\n",vname);vname=vname+1;}
		c=c+1;
	}

	int r;
	float value;
	float inf;
	float sup;
	inf = min;
	sup = min+step;

	while (inf < max){
		r=0;
		while (r<number_of_columns){
			if (r==xi-1){value=upcoord(hard_data,zi,xi,inf,sup,number_of_rows,nodata);}
			else if (r==yi-1){value=upcoord(hard_data,zi,yi,inf,sup,number_of_rows,nodata);}
			else if (r==zi-1){value=(sup-inf)/2+inf;}
			else {value=upscale(hard_data,zi,r,op,inf,sup,number_of_rows,nodata);}
			if (r==number_of_columns-1){fprintf(outputfile,"%f\n",value);}
			else {fprintf(outputfile,"%f\t",value);}
			r=r+1;
		}
		inf = sup;
		sup = sup + step;
	}
	cout << "Finished.\n";

	return 0;
}

