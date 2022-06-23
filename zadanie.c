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
#include <stdbool.h>

//#define SCIEZKA "colors.txt"
#define SCIEZKA "C:\workspace_console_apps\PROG_CHALLANGE_2022\zadanie\colors.txt"
#define ZERO_CHAR	48U
#define NINE_CHAR	57U

#define FILE_OFFSET_ZERO	0LU
#define FIRST_CHAR	0U
#define SECOND_CHAR	1U
#define FIRST_ROW	0U

#define CLI_MAX_STRING_LEN 20

typedef enum
{
	MIX = 0,
	LOWEST = 1,
	HIGHEST = 2,
	MIX_SATURATE = 3
}modes_t;

typedef enum
{
	RED = 0,
	GREEN = 1,
	BLUE = 2,
	ALPHA = 3
}num_cnt_t;

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

void check_cli (void);
uint8_t ascii_to_uint8 (uint8_t input, uint8_t n);
void color (void);
color_t compute_result(color_t color, color_t last_color);
void print_color(color_t color);
color_t copy_color(color_t color);
float max (saturation_t color);
float min (saturation_t color);

int main(void)
{
	color();
	return(0U);
}

void color (void)
{
	   FILE *fp;
	   fpos_t file_len;
	   char char_read;

	   uint8_t num_tmp = 0U;
	   color_t color = {0U,0U,0U,0U};
	   color_t last_color = {0U,0U,0U,0U};
	   color_t result = {0U,0U,0U,0U};
	   uint8_t char_cnt = FIRST_CHAR;
	   num_cnt_t num_cnt = RED;
	   uint8_t row_cnt = FIRST_ROW;

	   check_cli();

	   if ((fp=fopen("colors.txt", "r+")) == NULL)
	   {
	     printf ("Nie mogê otworzyæ pliku colors.txt do odczytu!\n");
	     exit(1U);
	   }

	   else
	   {
		   fgetpos (fp, &file_len);
		   fseek (fp, FILE_OFFSET_ZERO, SEEK_SET);

	  	   while (fscanf(fp,"%c",&char_read) != EOF)
	  	   {
	  		 if(char_read == ',')
	  		 {
	  			 char_cnt = FIRST_CHAR;
	  			 num_cnt ++;
	  		 }

	  		 num_tmp = (uint8_t)char_read;
	  		 if((num_tmp >= ZERO_CHAR) && (num_tmp <= NINE_CHAR))
	  		 {
	  			 num_tmp = num_tmp - ZERO_CHAR;

		  		 switch(num_cnt)
		  		 {
					 case RED:
					 {
						color.red = (color.red + ascii_to_uint8(num_tmp,char_cnt));
						break;
					 }
					 case GREEN:
					 {
						color.green = (color.green + ascii_to_uint8(num_tmp,char_cnt));
						break;
					 }
					 case BLUE:
					 {
						color.blue = (color.blue + ascii_to_uint8(num_tmp,char_cnt));
						break;
					 }
					 case ALPHA:
					 {
						color.alpha = (color.alpha + ascii_to_uint8(num_tmp,char_cnt));
						if(char_cnt > SECOND_CHAR)
							{
							num_cnt = RED;
							char_cnt = FIRST_CHAR;
							if((row_cnt != FIRST_ROW) || (mode != MIX)) result = compute_result(color,last_color);
							row_cnt ++;
							print_color(color);
							printf("\n");

							last_color = copy_color(color);

							color.red = 0U;
							color.green = 0U;
							color.blue = 0U;
							color.alpha = 0U;
							}
						break;
					 }
					 default:
					 {
						 color.red = (color.red + ascii_to_uint8(num_tmp,char_cnt));
						 break;
					 }
		  		 }
		  		 char_cnt ++;
	  		 }
	  		 else
	  		 {
	  			char_cnt = FIRST_CHAR;
	  		 }
	  	   }

			printf("\n\nresult:\n");
			print_color(result);
			printf("\nlicznik_wierszy = %d",row_cnt);
			fclose (fp);
	   }
}

void check_cli (void)
{
	char str[CLI_MAX_STRING_LEN];
	char *strptr;
	scanf(" %s",str);
	if ((0U == strncmp (str,"mode",(strlen("mode")))) || (0U == strncmp (str,"-m",(strlen("-m")))))
	{
		printf("Mode = ");

		strptr = strstr(str,"mix");
		if(strptr != NULL)
		{
			printf("mix");
			mode = MIX;
		}

		strptr = strstr(str,"lowest");
		if(strptr != NULL)
		{
			printf("lowest");
			mode = LOWEST;
		}

		strptr = strstr(str,"highest");
		if(strptr != NULL)
		{
			printf("highest");
			mode = HIGHEST;
		}

		strptr = strstr(str,"mix-saturate");
		if(strptr != NULL)
		{
			printf("mix-saturate");
			mode = MIX_SATURATE;
		}
	}
}

uint8_t ascii_to_uint8 (uint8_t input, uint8_t n)
{
	uint8_t result = 0U;
	switch(n)
	{
		case 0:
		{
			result = (input * 100U);
			break;
		}
		case 1:
		{
			result = (input * 10U);
			break;
		}
		case 2:
		{
			result = input;
			break;
		}
		default:
		{
			result = (input * 100U);
			break;
		}

	}
	return(result);
}

color_t compute_result(color_t color, color_t last_color)
{
	static color_t result = {0U,0U,0U,0U};
	static bool first_run = false;

	switch(mode)
	{
		case MIX:
		{
			if((result.red > 0U) || (result.green > 0U) || (result.blue > 0U) || (result.alpha > 0U))
			{
				result.red = (uint8_t)((int)(result.red + color.red) / 2U);
				result.green = (uint8_t)((int)(result.green + color.green) / 2U);
				result.blue = (uint8_t)((int)(result.blue + color.blue) / 2U);
				result.alpha = (uint8_t)((int)(result.alpha + color.alpha) / 2U);
			}
			else
			{
				result.red = ((last_color.red + color.red) / 2U);
				result.green = ((last_color.green + color.green) / 2U);
				result.blue = ((last_color.blue + color.blue) / 2U);
				result.alpha = ((last_color.alpha + color.alpha) / 2U);
			}
			break;
		}
		case LOWEST:
		{
			if(first_run == false)
			{
				first_run = true;
				result.red = 255U;
				result.green = 255U;
				result.blue = 255U;
				result.alpha = 255U;
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
			result.alpha =	(uint8_t)((int)(color.red + color.green + color.blue)) / 3U;
			break;
		}
		default:
		{
			if((result.red > 0U) || (result.green > 0U) || (result.blue > 0U) || (result.alpha > 0U))
			{
				result.red = (uint8_t)((int)(result.red + color.red) / 2U);
				result.green = (uint8_t)((int)(result.green + color.green) / 2U);
				result.blue = (uint8_t)((int)(result.blue + color.blue) / 2U);
				result.alpha = (uint8_t)((int)(result.alpha + color.alpha) / 2U);
			}
			else
			{
				result.red = ((last_color.red + color.red) / 2U);
				result.green = ((last_color.green + color.green) / 2U);
				result.blue = ((last_color.blue + color.blue) / 2U);
				result.alpha = ((last_color.alpha + color.alpha) / 2U);
			}
			break;
		}
	}
	return (result);
}

void print_color(color_t color)
{
	float hue = 0.0f;
	float saturation = 0.0f;
	float lightness = 0.0f;
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
	hue = ((float)color.red + (float)color.green + (float)color.blue) / 2.125f; // Dla zakresu 0-360 (max = 255+255+255=765 / 360 =2,125 )
	printf(" hue = ");
	printf(" %f",hue);

	prim.red = ((float)color.red / 255.0f);
	prim.green = ((float)color.green / 255.0f);
	prim.blue = ((float)color.blue / 255.0f);

	saturation = (max(prim) - min(prim));
	printf(" saturation = ");
	printf(" %f",saturation);

	lightness = ((max(prim) - min(prim)) / 2.0f);
	printf(" lightness = ");
	printf(" %f",lightness);
}

color_t copy_color(color_t color)
{
	color_t result = {0U,0U,0U,0U};
	result.red = color.red;
	result.green = color.green;
	result.blue = color.blue;
	result.alpha = color.alpha;
	return(result);
}

float max (saturation_t color)
{
	float result = 0.0f;
	if(color.red > result) result = color.red;
	if(color.green > result) result = color.green;
	if(color.blue > result) result = color.blue;
	return (result);
}

float min (saturation_t color)
{
	float result = 255.0f;
	if(color.red < result) result = color.red;
	if(color.green < result) result = color.green;
	if(color.blue < result) result = color.blue;
	return (result);
}
