/*
 * tosegy.cpp
 *
 *  Created on: 17 de Nov de 2010
 *      Author: Administrator
 */

// WriteSEGY.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <string.h>

#include <algorithm> //required for std::swap

#define XLENGHT 635


#define ByteSwap5(x) ByteSwap((unsigned char *) &x,sizeof(x))

using namespace std;

void ByteSwap(unsigned char * b, int n)
{
	// change from big endian to little endian format
	register int i = 0;
	register int j = n-1;
	while (i<j)
	{
		std::swap(b[i], b[j]);
		i++, j--;
	}
}

void ibm2ieee(float* input, int swap)
{
	typedef unsigned char byte;
	typedef unsigned long ulng;

	byte  *cbuf,expp,tem,sign;
	ulng  *umantis,expll; //,signl;
//	ulng  *usignl;
	long *mantis;
	int  shift;

	cbuf=(byte*)&input[0];        /* assign address of input to char array */
	umantis=(ulng*)&input[0];     /* two differnt points to the same spot  */
	mantis =(long*)&input[0];     /* signned & unsigned                    */

	if(swap)
	{
		/* now byte reverce for PC use if swap true (bif endian to little endian) */
		tem=cbuf[0];
		cbuf[0]=cbuf[3];
		cbuf[3]=tem;

		tem=cbuf[2];
		cbuf[2]=cbuf[1];
		cbuf[1]=tem;
	}

	/* start extraction information from number */

	expp=*mantis>>24;     /* get expo fro upper byte      */
	*mantis=(*mantis)<<8; /* shift off upper byte         */
	shift=1;              /* set a counter to 1           */
	while(*mantis>0 && shift<23) /* start of shifting data*/
	{
		*mantis=*mantis<<1;
		shift++;
	} /* shift until a 1 in msb */

	*mantis=*mantis<<1; /* need one more shift to get implied one bit */
	sign=expp & 0x80;   /* set sign to msb of exponent            */
	expp=expp & 0x7F;   /* kill sign bit                          */

	if(expp!=0)        /* don't do anymore if zero exponent       */
	{
		expp=expp-64;   /* compute what shift was (old exponent)*/
		*umantis=*umantis>>9; /* MOST IMPORTANT an UNSIGNED shift back down */
		expll=0x7F+(expp*4-shift); /* add in excess 172 */

		/* now mantissa is correctly aligned, now create the other two pairs */
		/* needed the extended sign word and the exponent word               */

		expll=expll<<23;        /* shift exponent up */

		/* combine them into a floating point IEEE format !     */

		if(sign) *umantis=expll | *mantis | 0x80000000;
		else     *umantis=expll | *mantis; /* set or don't set sign bit */
	}
}



//void ieee2ibm(float *input)
//{
///*define several different class vars pointing to input area*/
//unsigned char *cbuf,exp,tem;
//unsigned long *umantis;
//long expl;
//unsigned long usignl;
//
//umantis=(long*)&input[0];
//cbuf=(char *)&input[0];
///*algo in comments step by step*/
//usignl=*umantis & 0x80000000; /*get and save sign bit.*/
//exp = (*umantis >>23); /*get and save exponent.*/
///*or in the implied 1 bit in location 24.*/
//*umantis=((*umantis|0x00800000))&0x00ffffff; //mask off exponent &
//sign
//exp=(exp-127)+1; /*subtract excess 127 and add one for the implied one
//bit.*/
//
///*now adjust exponent to base 16 and also umantis.
//this is done by shifting umantis exp%4 times. Then dividing exp by 4
//if exp is already a multiple of 4 no shifts required.
//*/
//if(exp%4)
//{
////*umantis=*umantis >> ( exp % 4);
//*umantis=*umantis >> (4 - (exp%4));
///*if additional shift needed then need to add 4 to new exponent.*/
//exp=exp+4;
//}
//
//exp=exp >> 2; /*divide exp by 4 to convert to base 16.*/
//exp=exp+64; /*make exponent excess 64 and exponent is ready*/
//expl=exp; /*create long exponent*/
//expl=(expl << 24)&0x7f000000; /*mask all but exponent and shift into
//location*/
///*or components together. and we have an IBM floating pointer.*/
//*umantis= *umantis | expl | usignl;
///*finally interchange the bytes to make it ready for main-framing.
//now byte reverse them for PC use if swap flag set.*/
////if (swap)
//// {
//// tem=cbuf[0]; cbuf[0]=cbuf[3];cbuf[3]=tem; /*1 and 4 done*/
//// tem=cbuf[1]; cbuf[1]=cbuf[2]; cbuf[2]=tem; /*2 and 3 done*/
//// }
//}





void main()
{
//***************************************************************************************
// *** Variaveis a mudar ***

	char out[100] = "//Dc-server/acrep/GALP/DELIVERABLES/ASCII/AI_5_a=100_unbended/AI_5_media_WIN_unbended.tsi"; //SY_5_4.tsi";
	char sgy[200] = "//Dc-server/acrep/GALP/DELIVERABLES/SEGY/AI_5_a=100_unbended/AI_5_media_WIN_unbended.tsi"; //SY_5_4.sgy";
	int trace_code = -1 ; // =1 para seismic data (amplitudes), =-1 para outros

	char reservatorio[80] = "galp_1380x993x351_2255x6836x4334";
	char tipo_de_cubo[80] = "cube_media";
	char tempo_inicial[80] = "4334";

	int inlines = 1380;//460;//681;// 156; // marlimSul corresponde a 681 e bu_hasa a 156
	int xlines =  993;//331;//351;// 237;
	int samples = 351;//351;//831; //75;

	int in_step = 1;//4;//3;
	int x_step =  1;//2;//1;
	int resolucao = 4;//1; //4;

	int Oinline = 2255;//5781; // 5617;
	int Oxline = 6836;//3299; // 10400;
	int Otime = 4334;//2690;// 1332;

	int Oxcoord =  6109916.33;//384963;//731350;
	int Oycoord = 7115607.04;//7510042; //2608174;

	double in_increment_x = 19.37284989;//25; //5.258088;//25; // relacao entre os inlines e as coordenadas para cada inline (x)
	double in_increment_y = 0;//0; //-6.75714; //0;
	double x_increment_x = 0;//0; //-3.37831;//0;  // relacao entre os xlines e as coordenadas para cada xline (y)
	double x_increment_y = -1.930413306;//-25; //-10.5171; //8.333333;

//********************************************************************************************
//********************************************************************************************
// *** Codigo ***


	FILE *inputfile;
	FILE *outputfile;
	char lixo1[100];
	int lixo2;
	float *cube, *cube2;
	int x, y, z;
	float valor;
	int ind;

	char fheader[3200];
	char bheader[400];
	char theader[240];
	char tdata[4];

	if( (outputfile = fopen( sgy, "wb" )) == NULL )
	{
		return 0;
	}

	if( (inputfile = fopen( out, "rb" )) == NULL )
	{
		return 0;
	}

//	fscanf (inputfile, "%s %d %d %d %d\n", &lixo1, &lixo2, &lixo2, &lixo2, &lixo2);
	fscanf (inputfile, "%s\n", &lixo1);
	fscanf (inputfile, "%d", &lixo2);
	fscanf (inputfile, "%s\n", &lixo1);
	//fscanf (inputfile, "%s\n", &lixo1);
	//fscanf (inputfile, "%d", &lixo2);
	//fscanf (inputfile, "%s\n", &lixo1);
	//fscanf (inputfile, "%s\n", &lixo1);
	//fscanf (inputfile, "%d", &lixo2);
	//fscanf (inputfile, "%s\n", &lixo1);

	if ((cube = (float *) malloc(sizeof(float) * inlines * xlines * samples)) == 0) {
	printf( "%s\n", "Unable to allocate cube ");
	}

	printf( "%s", "Reading the ASCII Cube ");

	for (z = 0 ; z < samples; z++)
	{
		for (y = 0; y < xlines; y++)
		{
			for ( x = 0; x < inlines; x++)
			{
				fscanf (inputfile, "%f", &valor);

//				ind = ((z) * inlines * xlines) + ((y) * xlines) + x;
				ind = (y * inlines * samples) + (x * samples) + z;

				ByteSwap((unsigned char *) (&valor), sizeof(float));
				cube[ind] = valor;
			}
		}
	}

	fclose(inputfile);

	printf( "%s/n", "Finished ");

	printf( "%s", "Writing the SEY ");

	//File Header
	memset( fheader, ' ', 3200 );

	sprintf( fheader,      reservatorio);
	sprintf( fheader +80,  "*************************************************");
	sprintf( fheader +160, tipo_de_cubo);
	sprintf( fheader +240, "*************************************************");
	sprintf( fheader +320, "The Initial Time is: (ms)");
	sprintf( fheader +400, tempo_inicial);

	fwrite (fheader, sizeof(char), 3200, outputfile);

	// Bin Header
	int b;


	*((int *)bheader +0) = 0;
	ByteSwap((unsigned char * )(&Oxline), sizeof(int));
	*((int *)bheader +1) = Oxline;
	*((int *)bheader +2) = Oxline;
	*((short *)bheader +6) = 0;
	*((short *)bheader +7) = 0;
	ByteSwap((unsigned char * )(&Oxline), sizeof(int));

	resolucao=resolucao*1000;
	ByteSwap((unsigned char * )(&resolucao), sizeof(short));
	*((short *)bheader +8) = resolucao; //Sample Interval      Start:    17   Number Bytes:     2
	*((short *)bheader +9) = resolucao;

	ByteSwap((unsigned char * )(&samples), sizeof(short));
	*((short *)bheader +10) = samples; //Numbeior samples       Start:    21   Number Bytes:     2
	*((short *)bheader +11) = samples;

	short sample_format=5;
	ByteSwap((unsigned char * )(&sample_format), sizeof(short));
	*((short *)bheader +12) = sample_format; //Sample format        Start:    25   Number Bytes:     2


		for(b = 13; b <= 26; b++)
		{
			*((short *)bheader +b) = 0;
		}
	*((short *)bheader +27) = 1; //Distance Units       Start:    55   Number Bytes:     2
		for(b = 28; b <= 29; b++)
		{
			*((short *)bheader +b) = 0;
		}

	fwrite (bheader, sizeof(char), 400, outputfile);



	// *********  Traces  *********
	int t;
	int n_trace = 0 ;

	for (y = 0; y < xlines; y++)
	{
		for ( x = 0; x < inlines; x++)
		{
			n_trace = n_trace + 1 ;

			// Trace Header
			ByteSwap((unsigned char * )(&n_trace), sizeof(int));
			*((int *)theader +0) = n_trace; //Trace number            Start:     1   Number Bytes:     4
			*((int *)theader +1) = n_trace;                                // dos bytes 5 aos 8
			ByteSwap((unsigned char * )(&n_trace), sizeof(int));

			int d_inl=Oxline + (y*x_step);

			int d_clin=Oinline + (x*in_step);

			ByteSwap((unsigned char * )(&d_inl), sizeof(int));
			ByteSwap((unsigned char * )(&d_clin), sizeof(int));

			*((int *)theader +2) = d_clin; //3D Inline            Start:     9   Number Bytes:     4

			*((int *)theader +3) = d_inl; //3D Crossline         Start:    13   Number Bytes:     4 13-16
			*((int *)theader +4) = d_inl; //3D Crossline         Start:    13   Number Bytes:     4 17-20
			*((int *)theader +5) = d_inl; //3D Crossline         Start:    13   Number Bytes:     4 21-24
			*((int *)theader +6) = d_inl; //3D Crossline         Start:    13   Number Bytes:     4 25-28

			//for(t = 4; t <= 6; t++)
				//{
				//	*((int *)theader +t) = 0;
				//}
			*((short*)theader +14) = trace_code; //Trace code           Start:    29   Number Bytes:     2
				for(t = 15; t <= 18; t++)
				{
					*((short *)theader +t) = 0;
				}
				for(t = 9; t <= 13; t++)
				{
					*((int *)theader +t) = 0;
				}
			*((int*)theader +14) = 0;//Datum                Start:    53   Number Bytes:     4
				for(t = 15; t <= 17; t++)
				{
					*((int *)theader +t) = 0;
				}

				int a=(Oxcoord + (x * in_step * in_increment_x) + (y * x_step * in_increment_y));
				int b=(Oycoord + (y * x_step * x_increment_y) + (x * in_step * x_increment_x));
				int c=(Oxcoord + (x * in_step * in_increment_x) + (y * x_step * in_increment_y));
				int d=(Oycoord + (y * x_step * x_increment_y) + (x * in_step * x_increment_x));
				ByteSwap((unsigned char *)(&a), sizeof(int));
				ByteSwap((unsigned char *)(&b), sizeof(int));
				ByteSwap((unsigned char *)(&c), sizeof(int));
				ByteSwap((unsigned char *)(&d), sizeof(int));

			*((int*)theader +18) = (int)(a); //3D X Coordinate      Start:    73   Number Bytes:     4
			*((int*)theader +19) = (int)(b); //3D Y Coordinate      Start:    77   Number Bytes:     4
			*((int*)theader +20) = (int)(c); //3D X Coordinate      Start:    73   Number Bytes:     4
			*((int*)theader +21) = (int)(d); //3D Y Coordinate      Start:    77   Number Bytes:     4



			//for(t = 20; t <= 21; t++)
				//{
				//	*((int *)theader +t) = 0;
				//}

			int cood_code=1;
			ByteSwap((unsigned char *)(&cood_code), sizeof(short));
			*((short*)theader +44) = cood_code;//Coordinate Units Code Start:    89   Number Bytes:     2
				for(t = 45; t <= 53; t++)
				{
					*((short *)theader + t) = 0;
				}
			ByteSwap((unsigned char *)(&cood_code), sizeof(short));
			ByteSwap((unsigned char *)(&Otime), sizeof(short));
			*((short*)theader +54) = Otime;//Delay Recording time     Start:  108    Number Bytes:     2
				for(t = 55; t <= 56; t++)
				{
					*((short *)theader + t) = 0;
				}
			ByteSwap((unsigned char *)(&Otime), sizeof(short));
			*((short*)theader +57) = samples;//Number of samples in this trace    Start:    115   Number Bytes:     2
			*((short*)theader +58) = resolucao ;//Sample interval in ms for this trace     Start:    117   Number Bytes:     2
				for(t = 59; t <= 119; t++)
				{
					*((short *)theader + t) = 0;
				}

			//ByteSwap((unsigned char * )(&theader +54), sizeof(short));


			fwrite (theader, sizeof(char), 240, outputfile);

			// Trace Data

			ByteSwap((unsigned char * )(&samples), sizeof(short));
			for (z = 0 ; z < samples; z++)
			{
				ind = (y * inlines * samples) + (x * samples) + z;
				*((float *)tdata +0) = cube[ind];
				fwrite (tdata, sizeof(float), 1, outputfile);

			}
			ByteSwap((unsigned char * )(&samples), sizeof(short));

		}
	}

    free (cube);

	fclose(outputfile);

	printf( "%s/n", "Finished ");

	//return 0;
}



