/*
 * hpgridup.cpp
 *
 *  Created on: 15 de Nov de 2010
 *      Author: Administrator
 */

#include "iostream"
#include "new"
#include "fstream"
#include "math.h"
#include "unistd.h"
#include "errno.h"

using namespace std;

int gridcut (float *griddata, float *gridradius,int maxx, int minx, int maxy, int miny, int maxz, int minz, float nodata, int xf, int yf, int zf) {
	int i=minz;
	int ii;
	int iii;
	int counter=0;
	while (i<maxz) {
		ii=miny;
		while (ii<maxy) {
			iii=minx;
			while (iii<maxx) {
				if ((i<0)|(i>zf-1)|(ii<0)|(ii>yf-1)|(iii<0)|(iii>xf-1))
					{gridradius[counter]=nodata;}
				else
					{gridradius[counter]=griddata[i*xf*yf+ii*xf+iii];}
				iii=iii+1;
				counter=counter+1;
			}
			ii=ii+1;
		}
		i=i+1;
	}

	return 0;
}

float opmean(float *gridradius, int radx, int rady, int radz, int radiusx, int radiusy, int radiusz, float nodata) {
	float ave=0;
	int counter=0;
	int z=0;
	int x;
	int y;
	while (z<radz) {
		y=0;
		while (y<rady) {
			x=0;
			while (x<radx) {
				if (gridradius[z*radx*rady+y*radx+x]!=nodata) {
					ave=ave+gridradius[z*radx*rady+y*radx+x];
					counter=counter+1;
					}
				x=x+1;
			}
			y=y+1;
		}
		z=z+1;
	}
	if (ave==0) {return nodata;}
	return(ave/counter);
}

float opmax(float *gridradius, int radx, int rady, int radz, int radiusx, int radiusy, int radiusz, float nodata) {
	float max;
	int z=0;
	int x;
	int y;
	int counter=0;
	while (z<radz) {
		y=0;
		while (y<rady) {
			x=0;
			while (x<radx) {
				if (gridradius[z*radx*rady+y*radx+x]!=nodata) {
					if (counter==0){max=gridradius[z*radx*rady+y*radx+x];counter=counter+1;}
					else {
						if (max<gridradius[z*radx*rady+y*radx+x]) {
							max=gridradius[z*radx*rady+y*radx+x];
						}
					}
					}
				x=x+1;
			}
			y=y+1;
		}
		z=z+1;
	}
	return(max);
}

float opmin(float *gridradius, int radx, int rady, int radz, int radiusx, int radiusy, int radiusz, float nodata) {
	float min;
	int z=0;
	int x;
	int y;
	int counter=0;
	while (z<radz) {
		y=0;
		while (y<rady) {
			x=0;
			while (x<radx) {
				if (gridradius[z*radx*rady+y*radx+x]!=nodata) {
					if (counter==0){min=gridradius[z*radx*rady+y*radx+x];counter=counter+1;}
					else {
						if (min>gridradius[z*radx*rady+y*radx+x]) {
							min=gridradius[z*radx*rady+y*radx+x];
						}
					}
					}
				x=x+1;
			}
			y=y+1;
		}
		z=z+1;
	}
	return(min);
}

int main()
{
	cout << "******************************************************\n";
	cout << "Starting upscaling algorithm.\n";
	cout << "******************************************************\n";
    //printf("Starting downscaling.\n\n\n");
	float valor;
	// CONF FILE.
	char conf[256]="gridup.pin"; //C:\\Documents and Settings\\Administrator\\Desktop\\eclipse\\workspace\\hpgriddown\\Release\\griddown.pin";
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

	// Passo para cada um dos eixos (passo onde se encontram os dados da grid original na final, stepx, stepy, stepz)
	int stepx;
	int stepy;
	int stepz;

	// Primeiro valor para cada um dos eixos.
	int firstx;
	int firsty;
	int firstz;

	// Tipo de interpolação 1) nearest value, 2) inverse weighted distance, 3) kriging, 4) Tricubic
	int tipo;

	// Valores no-data a serem aplicados (e nao lidos, nodata)
	float nodata;

	// Raio de procura para a interpolacao dos blocos na grid final para cada um dos eixos (radiusx, radiusy, radiusz).
	int radiusx;
	int radiusy;
	int radiusz;
	int header;

	// Coordenadas do primeiro ponto da grid.

	fscanf(fileconf,"%[^\n]s\n",filename);
	fscanf(fileconf,"%s\n",filename2);
	fscanf(fileconf,"%[^\n]s\n",filename2);
	fscanf(fileconf,"%d\n",&xi);
	fscanf(fileconf,"%d\n",&yi);
	fscanf(fileconf,"%d\n",&zi);
	fscanf(fileconf,"%d\n",&stepx);
	fscanf(fileconf,"%d\n",&stepy);
	fscanf(fileconf,"%d\n",&stepz);
	fscanf(fileconf,"%d\n",&firstx);
	fscanf(fileconf,"%d\n",&firsty);
	fscanf(fileconf,"%d\n",&firstz);
	fscanf(fileconf,"%d\n",&radiusx);
	fscanf(fileconf,"%d\n",&radiusy);
	fscanf(fileconf,"%d\n",&radiusz);
	fscanf(fileconf,"%d\n",&header);
	fscanf(fileconf,"%d\n",&tipo);
	fscanf(fileconf,"%f\n",&nodata);

	cout << "Input file:  " << filename << "\n";
	cout << "Output file: " << filename2 << "\n";
	cout << "******************************************************\n";
	cout << "1) Xo:  " << xi << "  Yo:  " << yi << "  Zo:  " << zi << "\n";
	cout << "5) RADIUS in X:  " << radiusx << "  RADIUS in Y:  " << radiusy << "  RADIUS in Z:  " << radiusz << "\n";
	cout << "******************************************************\n";
	cout << "Size of header:  " << header << "\n";
	cout << "No-Data value:   " << nodata << "\n";

	if (tipo==0) { cout << "Upscaling operator:  " <<tipo<< " meaning picking" << ".\n";}
	else if (tipo==1) {cout << "Upscaling operator:  " <<tipo<< " meaning arithmetic mean"<< ".\n";}
	else if (tipo==2) {cout << "Upscaling operator:  " <<tipo<< " meaning maximum"<< ".\n";}
	else if (tipo==3) {cout << "Upscaling operator:  " <<tipo<< " meaning minimum"<< ".\n";}

	if ((inputfile=fopen(filename,"r"))==NULL){
			cout << "Failed " << filename << "\n";
			return 1;
		}
	cout << "******************************************************\n";
	outputfile=fopen(filename2,"w");
	int xf=xi/stepx;
	int yf=yi/stepy;
	int zf=zi/stepz;
	// A escrever o cabecalho do ficheiro de saida.
	fprintf(outputfile,"%i_%i_%i\n",xf,yf,zf);
	fprintf(outputfile,"%i\n",1);
	fprintf(outputfile,"variable\n");

	// VARIAVEIS INTERNAS DO PROGRAMA
	// Variaveis limite para a recolha dos blocos que vao servir para a interpolacao em cada um dos eixos (maxx,minx,maxy,
	// miny,maxz,minz) ---> NAO SAO PARA EDICAO PELO UTILIZADOR
	int maxx;
	int minx;
	int maxy;
	int miny;
	int maxz;
	int minz;
	int radx=radiusx*2;
	int rady=radiusy*2;
	int radz=radiusz*2;
	//printf("%i_%i_%i_%i\n\n",radx,rady,radz,radx*rady*radz);

	// Alocacao dos arrays necessarios para a interpolacao.
	// original -> Alocacao da grid original (input).
	// final -> Alocacao da grid final (output).
	// griddata -> Alocacao da grid final apenas com os dados da original atribuidos (interno).
	// gridradius -> Alocacao da grid de procura que vai ser gerada a partir da griddata (interno).
	// --> NAO SAO PARA EDICAO PELO UTILIZADOR
	float *original;
	float *gridradius;
	original = new (nothrow) float[xi*yi*zi]; //(float*)malloc(sizeof(float)*xi*yi*zi);
	gridradius = new (nothrow) float[(radx)*(rady)*(radz)]; //(float*)malloc(sizeof(float)*(radx)*(rady)*(radz));

	// Leitura do ficheiro de input para a grid alocada em original.
	int h=0;
	char lixo[256];
	while(h<header){fscanf(inputfile,"%s\n",lixo);h=h+1;}

	int i=0;
	printf("A) Reading input file.\n");
	while(i<xi*yi*zi)
	{
		fscanf(inputfile,"%f\n",&valor);
		original[i]=valor;
		i=i+1;
	}

	// Interpolacao dos blocos da grid final.
	printf("B) Calculating blocks.\n");
	i=firstz;
	int ii;
	int iii;
	int index;
	while (i<zi){
		ii=firsty;
		while (ii<yi){
			iii=firstx;
			while (iii<xi){
				if (tipo==0) {
					fprintf(outputfile,"%f\n",original[i*xi*yi+ii*xi+iii]);
					iii=iii+stepx;
				}
				else if (tipo==1) {
					index=i*xf*yf+ii*xf+iii;
					maxx=iii+radiusx;
					minx=iii-radiusx;
					maxy=ii+radiusy;
					miny=ii-radiusy;
					maxz=i+radiusz;
					minz=i-radiusz;
					gridcut(original,gridradius,maxx,minx,maxy,miny,maxz,minz,nodata,xi,yi,zi);
					valor=opmean(gridradius,radx,rady,radz,radiusx,radiusy,radiusz,nodata);
					fprintf(outputfile,"%f\n",valor);
					iii=iii+stepx;
				}
				else if (tipo==2) {
					index=i*xf*yf+ii*xf+iii;
					maxx=iii+radiusx;
					minx=iii-radiusx;
					maxy=ii+radiusy;
					miny=ii-radiusy;
					maxz=i+radiusz;
					minz=i-radiusz;
					gridcut(original,gridradius,maxx,minx,maxy,miny,maxz,minz,nodata,xf,yf,zf);
					valor=opmax(gridradius,radx,rady,radz,radiusx,radiusy,radiusz,nodata);
					fprintf(outputfile,"%f\n",valor);
					iii=iii+stepx;
				}
				else if (tipo==3) {
					index=i*xf*yf+ii*xf+iii;
					maxx=iii+radiusx;
					minx=iii-radiusx;
					maxy=ii+radiusy;
					miny=ii-radiusy;
					maxz=i+radiusz;
					minz=i-radiusz;
					gridcut(original,gridradius,maxx,minx,maxy,miny,maxz,minz,nodata,xf,yf,zf);
					valor=opmin(gridradius,radx,rady,radz,radiusx,radiusy,radiusz,nodata);
					fprintf(outputfile,"%f\n",valor);
					iii=iii+stepx;
				}
			}
			ii=ii+stepy;
		}
		i=i+stepz;
	}
	delete[] original;
	delete[] gridradius;
	cout << "******************************************************\n";
	printf("Finished.\n");
	return 0;

	}
