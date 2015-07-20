/*
 * downscaler.cpp
 *
 *  Created on: 9 de Nov de 2010
 *      Author: Pedro Correia
 */

#include "iostream"
#include "new"
#include "fstream"
#include "math.h"

using namespace std;

int nodataarray (float *griddata,float nodata, int total){
	int i=0;
	while (i<total){
		griddata[i]=nodata;
		i=i+1;
	}
	return 0;
}

int pointgrid (float *griddata, float *original, int xf, int yf, int zf, int stepx, int stepy, int stepz,int firstx,int firsty, int firstz) {
	int i=firstz;
	int ii;
	int iii;
	int counter=0;
	while (i<zf){
		ii=firsty;
		while (ii<yf){
			iii=firstx;
			while (iii<xf){
				griddata[i*xf*yf+xf*ii+iii]=original[counter];
				iii=iii+stepx;
				counter=counter+1;
			}
			ii=ii+stepy;
		}
		i=i+stepz;
	}
	i=0;

	return 0;
}

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

float interpolate (float *gridradius, int radx, int rady, int radz, int radiusx, int radiusy, int radiusz, float nodata, int potence) {
	int z=0;
	int y;
	int x;
	int xo=radiusx;
	int yo=radiusy;
	int zo=radiusz;
	float sqdist;
	float sumdist=0;
	float value;
	float appex;
	float appex2;
	float sumvalue=0;
	while (z<radz) {
		y=0;
		while (y<rady) {
			x=0;
			while (x<radx) {
				if (gridradius[z*radx*rady+y*radx+x]!=nodata) {
					sqdist=sqrt(pow(float(x-xo),2)+pow(float(y-yo),2)+pow(float(z-zo),2));
					sqdist=pow(sqdist,potence);
					appex2=1/sqdist;
					sumdist=sumdist+appex2;
					value=gridradius[z*radx*rady+y*radx+x];
					appex=value/sqdist;
					sumvalue=sumvalue+appex;
				}
				x=x+1;
			}
			y=y+1;
		}
		z=z+1;
	}
	float result=sumvalue/(sumdist);
	return (result);
}

float nearest (float *gridradius, int radx, int rady, int radz, int radiusx, int radiusy, int radiusz, float nodata) {
	int z=0;
	int y;
	int x;
	int xo=radiusx;
	int yo=radiusy;
	int zo=radiusz;
	float sqdist;
	float appexdist;
	int counter=0;
	float result;
	while (z<radz) {
		y=0;
		while (y<rady) {
			x=0;
			while (x<radx) {
				if (gridradius[z*radx*rady+y*radx+x]!=nodata) {
					sqdist=sqrt(pow(float(x-xo),2)+pow(float(y-yo),2)+pow(float(z-zo),2));
					counter=counter+1;
					if (counter==1) {result=gridradius[z*radx*rady+y*radx+x];appexdist=sqdist;}
					else{
						if (sqdist<appexdist) {result=gridradius[z*radx*rady+y*radx+x];appexdist=sqdist;}
					}
				}
				x=x+1;
			}
			y=y+1;
		}
		z=z+1;
	}
	return (result);
}

int main()
{
	cout << "******************************************************\n";
	cout << "Starting downscaling algorithm.\n";
	cout << "******************************************************\n";
    //printf("Starting downscaling.\n\n\n");
	float valor;
	// Caminho e nome do ficheiro de entrada.
	char filename[256]="C:\\dadosbihist\\testehorizontes\\testeIA2.out";
	// Caminho e nome do ficheiro de saida.
	char filename2[256]="C:\\dadosbihist\\testehorizontes\\testeIAinterp2.out";
	// Abrir ficheiro de entrada e ficheiro de saida.
	FILE *inputfile;
	FILE *outputfile;

	if ((inputfile=fopen(filename,"r"))==NULL){
		return 1;
	}

	outputfile=fopen(filename2,"w");

	// VARIAVEIS DE INPUT
	// Tamanho da grid original (xi,yi,zi)
	int xi=50;
	int yi=50;
	int zi=41;

	// Tamanho da grid final (xf,yf,zf)
	int xf=150;
	int yf=150;
	int zf=41;

	// Passo para cada um dos eixos (passo onde se encontram os dados da grid original na final, stepx, stepy, stepz)
	int stepx=3;
	int stepy=3;
	int stepz=1;

	// Primeiro valor para cada um dos eixos.
	int firstx=0;
	int firsty=0;
	int firstz=0;

	// Tipo de interpolação 1) nearest value, 2) inverse weighted distance, 3) kriging, 4) Tricubic
	int tipo=2;

	// Valores no-data a serem aplicados (e nao lidos, nodata)
	float nodata=-999.25;
	// potencia do IPD.
	int potence=9;

	// Raio de procura para a interpolacao dos blocos na grid final para cada um dos eixos (radiusx, radiusy, radiusz).
	int radiusx=5;
	int radiusy=5;
	int radiusz=1;

	// Coordenadas do primeiro ponto da grid.
	int xcoord=2255;
	int ycoord=6836;
	int zcoord=0;

	// A escrever o cabecalho do ficheiro de saida.
	fprintf(outputfile,"%i_%i_%i__%i_%i_%i\n",xcoord,ycoord,zcoord,xf,yf,zf);
	fprintf(outputfile,"%i\n",1);
	fprintf(outputfile,"ia\n");

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
	float *final;
	float *griddata;
	float *gridradius;
	original = new (nothrow) float[xi*yi*zi]; //(float*)malloc(sizeof(float)*xi*yi*zi);
	final = new (nothrow) float[xf*yf*zf]; //(float*)malloc(sizeof(float)*xf*yf*zf);
	griddata = new (nothrow) float[xf*yf*zf]; //(float*)malloc(sizeof(float)*xf*yf*zf);
	gridradius = new (nothrow) float[(radx)*(rady)*(radz)]; //(float*)malloc(sizeof(float)*(radx)*(rady)*(radz));

	// Leitura do ficheiro de input para a grid alocada em original.
	int i=0;
	printf("A ler o ficheiro de input.\n\n\n");
	while(i<xi*yi*zi)
	{
		fscanf(inputfile,"%f\n",&valor);
		original[i]=valor;
		i=i+1;
	}

	// Atribuicao de valores no-data a todos os elementos da griddata.
	printf("A gerar grid de calculo.\n\n\n");
	int total=xf*yf*zf;
	nodataarray(griddata,nodata,total);

	// ANALISE: printf("Valor: %f.\n\n",griddata[922284]);
	// Atribuicao dos valores da grid original na grid griddata.
	printf("A atribuir os valores da grid original a grid de calculo.\n\n\n");
	pointgrid(griddata,original,xf,yf,zf,stepx,stepy,stepz,firstx,firsty,firstz);

	/*int f=0;
	while(f<100000)
		{
			fprintf(outputfile,"%f\n",griddata[f]);
			//if (i>922280) {printf("%i_%f\n\n",i,griddata[i]);}
			f=f+1;
		}*/

	// Interpolacao dos blocos da grid final.
	printf("A interpolar os blocos da grid final.\n\n\n");
	i=0;
	int ii;
	int iii;
	int index;
	while (i<zf){
		ii=0;
		while (ii<yf){
			iii=0;
			while (iii<xf){
					index=i*xf*yf+ii*xf+iii;
					maxx=iii+radiusx;
					minx=iii-radiusx;
					maxy=ii+radiusy;
					miny=ii-radiusy;
					maxz=i+radiusz;
					minz=i-radiusz;
					gridcut(griddata,gridradius,maxx,minx,maxy,miny,maxz,minz,nodata,xf,yf,zf);
					if (griddata[i*xf*yf+ii*xf+iii]==nodata){
						if (tipo==2) {
							final[i*xf*yf+ii*xf+iii]=interpolate(gridradius,radx,rady,radz,radiusx,radiusy,radiusz,nodata,potence);}
						else if (tipo==1) {
							final[i*xf*yf+ii*xf+iii]=nearest(gridradius,radx,rady,radz,radiusx,radiusy,radiusz,nodata);}
						}
					else if (griddata[i*xf*yf+ii*xf+iii]!=nodata) {
						final[i*xf*yf+ii*xf+iii]=griddata[i*xf*yf+ii*xf+iii];
					}
					fprintf(outputfile,"%f\n",final[i*xf*yf+ii*xf+iii]);
					iii=iii+1;
			}
			ii=ii+1;
		}
		i=i+1;
	}
	delete[] original;
	delete[] griddata;
	delete[] gridradius;
	delete[] final;
	printf("Terminado.\n\n\n");

	return 0;

	}
