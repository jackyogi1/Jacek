/*
 * zadanie.c
 *
 *  Created on: 28 kwi 2022
 *      Author: Jacek
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <inttypes.h>
#include <math.h>

//___________________________________
 #define uruchamianie_w_eclipse
//___________________________________

	#ifndef uruchamianie_w_eclipse
	#define SCIEZKA "colors.txt"
	#endif

	#ifdef uruchamianie_w_eclipse
	#define SCIEZKA "C:\workspace_console_apps\PROG_CHALLANGE_2022\zadanie\colors.txt"
	#endif



//-------------red[0] = MSB----------------
typedef enum
{
	MIX,
	LOWEST,
	HIGHEST,
	MIX_SATURATE
}modes_t;

typedef enum
{
	FALSE = 0,
	TRUE = 1
}boolean_t;

typedef struct
{
	uint8_t red[3];
	uint8_t green[3];
	uint8_t blue[3];
	uint8_t alpha[3];
}color_read_t;
typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
}color_t;
typedef struct
{
	float red;
	float green;
	float blue;
}saturation_t;

static modes_t mode = MIX;

void print_struct (color_read_t);
void check_cli (void);
uint8_t ascii_2_uint8 (uint8_t input, uint8_t n);
void color (void);
color_t compute_result(color_t color, color_t last_color);
void print_color(color_t color);
color_t copy_color(color_t color);
float max (saturation_t color);
float min (saturation_t color);

int main(void)
{
	color();
	return(0);
}

void color (void)
{
	   FILE *fp;
	   fpos_t dlugosc;
	   char c;


	   uint8_t num_tmp=0;
	   color_t color ={0,0,0,0};
	   color_t last_color ={0,0,0,0};
	   color_t result ={0,0,0,0};
	   uint8_t char_cnt=0,num_cnt=0,row_cnt = 0;

	   check_cli();

	   if ((fp=fopen("colors.txt", "r+"))==NULL)
	   {
	     printf ("Nie mogê otworzyæ pliku colors.txt do odczytu!\n");
	     exit(1);
	   }

	   else
	   {
		   fseek (fp, 0, SEEK_END); /* ustawiamy wskaŸnik na koniec pliku */
		   fgetpos (fp, &dlugosc);
		   fseek (fp, 0, SEEK_SET); /* ustawiamy wskaŸnik na poczatek pliku */

	  	   while (fscanf(fp,"%c",&c)!=EOF)
	  	   {

	  		 //printf("\ntekst= %c",c);

	  		 if(c==',')
	  		 {
	  			 char_cnt=0;
	  			 num_cnt++;
	  		 }

	  		 num_tmp = (uint8_t)c;
	  		 if((num_tmp>=48) && (num_tmp<=57))
	  		 {
	  			 num_tmp = num_tmp - 48;

		  		 switch(num_cnt)
		  		 {
					 case 0:
					 {

						color.red = (color.red + ascii_2_uint8(num_tmp,char_cnt));
						break;
					 }
					 case 1:
					 {

						color.green = (color.green + ascii_2_uint8(num_tmp,char_cnt));
						break;
					 }
					 case 2:
					 {

						color.blue = (color.blue + ascii_2_uint8(num_tmp,char_cnt));
						break;
					 }
					 case 3:
					 {

						color.alpha = (color.alpha + ascii_2_uint8(num_tmp,char_cnt));
						if(char_cnt>1)
							{
							//odczytana ostatia wartoœc z linii
							num_cnt=0;
							char_cnt = 0;
							if((row_cnt != 0) || (mode != MIX)) result = compute_result(color,last_color);
							row_cnt++;
							print_color(color);
							printf("\n");
							//printf("\nostatni= %c",c);
							//printf("\ncolor = ");print_color(color);
							//printf("\nlast_color = ");print_color(last_color);
							//printf("\nresult = ");print_color(result);

							last_color = copy_color(color);

							color.red = 0;
							color.green = 0;
							color.blue = 0;
							color.alpha = 0;
							}
						break;
					 }
		  		 }
		  		 char_cnt++;
	  		 }
	  		 else
	  		 {
	  			char_cnt=0;
	  		 }
	  	   }

	  	   printf("\n\nresult:\n");
	  	   print_color(result);
	  	 // printf("\n\nlast_color:\n");
	  	 // print_color(last_color);
	  	  printf("\nlicznik_wierszy = %d",row_cnt);
	  	   fclose (fp); /* zamknij plik */
	   }
}

void print_struct (color_read_t color)
{
	uint8_t i=0;
	printf("\nred     = ");
	for( i=0;i<3;i++) printf(" %d",color.red[i]);
	printf("\ngreen   = ");
	for( i=0;i<3;i++) printf(" %d",color.green[i]);
	printf("\nblue    = ");
	for( i=0;i<3;i++) printf(" %d",color.blue[i]);
	printf("\nalpha   = ");
	for( i=0;i<3;i++) printf(" %d",color.alpha[i]);
}

void check_cli (void)
{
	char str[20];
	char *strptr;
	char keyword_1[]={"mode"};
	char keyword_2[]={"-m"};
	char keyword_3[]={"mix"};
	char keyword_4[]={"lowest"};
	char keyword_5[]={"highest"};
	char keyword_6[]={"mix-saturate"};


	scanf(" %s",str);
	if ((0 == strncmp (str,keyword_1,(strlen(keyword_1)))) || (0 == strncmp (str,keyword_2,(strlen(keyword_2)))))
	{
		printf("Mode = ");

		strptr = strstr(str,keyword_3);
		if(strptr != NULL)
		{
			printf("mix");
			mode = MIX;
		}


		strptr = strstr(str,keyword_4);
		if(strptr != NULL)
		{
			printf("lowest");
			mode = LOWEST;
		}


		strptr = strstr(str,keyword_5);
		if(strptr != NULL)
		{
			printf("highest");
			mode = HIGHEST;
		}


		strptr = strstr(str,keyword_6);
		if(strptr != NULL)
		{
			printf("mix-saturate");
			mode = MIX_SATURATE;
		}

	}
}

uint8_t ascii_2_uint8 (uint8_t input, uint8_t n)
{
	uint8_t result = 0;
	switch(n)
	{
		case 0:
		{
			result = (input * 100);
			break;
		}
		case 1:
		{
			result = (input * 10);
			break;
		}
		case 2:
		{
			result = input;
			break;
		}
	}
	return(result);
}

color_t compute_result(color_t color, color_t last_color)
{
	static color_t result = {0,0,0,0};
	static boolean_t first_run = FALSE;

	switch(mode)
	{
		case MIX:
		{
			if((result.red > 0) || (result.green > 0) || (result.blue > 0) || (result.alpha > 0))
			{
				result.red = (uint8_t)((int)(result.red + color.red) / 2);
				result.green = (uint8_t)((int)(result.green + color.green) / 2);
				result.blue = (uint8_t)((int)(result.blue + color.blue) / 2);
				result.alpha = (uint8_t)((int)(result.alpha + color.alpha) / 2);
			}
			else
			{
				result.red = ((last_color.red + color.red) / 2);
				result.green = ((last_color.green + color.green) / 2);
				result.blue = ((last_color.blue + color.blue) / 2);
				result.alpha = ((last_color.alpha + color.alpha) / 2);
			}
			break;
		}
		case LOWEST:
		{
			if(first_run == FALSE)
			{
				first_run = TRUE;
				result.red = 255;
				result.green = 255;
				result.blue = 255;
				result.alpha = 255;
			}
			if (result.red > color.red) result.red = color.red;
			if (result.green > color.green) result.green = color.green;
			if (result.blue > color.blue) result.blue = color.blue;
			if (result.alpha > color.alpha) result.alpha = color.alpha;
			break;
		}
		case HIGHEST:
		{
			if (result.red < color.red) result.red = color.red;
			if (result.green < color.green) result.green = color.green;
			if (result.blue < color.blue) result.blue = color.blue;
			if (result.alpha < color.alpha) result.alpha = color.alpha;
			break;
		}
		case MIX_SATURATE:
		{
			result.alpha =	(uint8_t)((int)(color.red + color.green + color.blue)) / 3;
			break;
		}
	}
	return (result);
}

void print_color(color_t color)
{
	float hue = 0;
	float saturation = 0;
	float lightness = 0;
	saturation_t prim;

	printf("\n red = ");
	printf(" %d",color.red);
	printf(" green = ");
	printf(" %d",color.green);
	printf(" blue = ");
	printf(" %d",color.blue);
	printf(" alpha = ");
	printf(" %d",color.alpha);
	printf(" hex = ");
	printf(" #%x%x%x%x",color.red,color.green,color.blue,color.alpha);
	hue = ((float)color.red + (float)color.green + (float)color.blue) / 2.125; // Dla zakresu 0-360 (max = 255+255+255=765 / 360 =2,125 )
	printf(" hue = ");
	printf(" %f",hue);

	prim.red = ((float)color.red / 255);
	prim.green = ((float)color.green / 255);
	prim.blue = ((float)color.blue / 255);

	saturation = (max(prim) - min(prim));
	printf(" saturation = ");
	printf(" %f",saturation);

	lightness = ((max(prim) - min(prim)) / 2);
	printf(" lightness = ");
	printf(" %f",lightness);
}

color_t copy_color(color_t color)
{
	color_t result;
	result.red = color.red;
	result.green = color.green;
	result.blue = color.blue;
	result.alpha = color.alpha;
	return(result);
}

float max (saturation_t color)
{
	float result = 0;
	if(color.red > result) result = color.red;
	if(color.green > result) result = color.green;
	if(color.blue > result) result = color.blue;
	return (result);
}

float min (saturation_t color)
{
	float result = 255;
	if(color.red < result) result = color.red;
	if(color.green < result) result = color.green;
	if(color.blue < result) result = color.blue;
	return (result);
}
