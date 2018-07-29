/*
 * DOCTOR_ENT.c
 * by Daniel Bruce Burns
 *
 * A program that creates EXAMPLE.svg, an illustration of
 * the Consciously Creative Chaos Project applied to graphic
 * design.
 *
 * This program was written in Sublime Text.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

//GLOBALS\\

//File Pointers
FILE *fin; //Input:  RandomNumbers
FILE *fot; //Output: EXAMPLE.svg

//Helper Variables
static int steps = 64;
static float ratio = 1.13;
static bool even = true;
static float xy[] = {0.0, 0.0};

//Starting Positions
static float north[] = {350, 350};
static float south[] = {350, 350};
static float east[]  = {350, 350};
static float west[]  = {350, 350};

//Track Last Colour (1=red, 2=yellow, 3=green, 4=purple, 0=none)
static int last_north = 0;
static int last_south = 0;
static int last_east  = 0;
static int last_west  = 0;

//FUNCTIONS\\

//Used to prevent using the same colour twice in a row in the same direction.
static bool colour_safe(int colour, int dir)
{
	if      (dir == 0 && last_north == colour)
		return false;
	else if (dir == 1 && last_south == colour)
		return false;
	else if (dir == 2 && last_east  == colour)
		return false;
	else if (dir == 3 && last_west  == colour)
		return false;
	else
		return true;
}

//Used to increase the ratio exponentially
static float power(float rat, int step)
{
	int i = 0;
	float result = rat;
	for(; i < step; i++)
		result *= rat;

	return result;
}

//Set x and y variables depending on direction. Some ifs for graphic style.
static void set_xy(int step, int dir)
{
	if (dir == 0)
	{
		if (even)
			north[0] += power(ratio, step);
		else
			north[0] -= power(ratio, step);
		north[1] -= power(ratio, step);
		xy[0] = north[0];
		xy[1] = north[1];

	} else if (dir == 1)
	{
		if (even)
			south[0] -= power(ratio, step);
		else
			south[0] += power(ratio, step);
		south[1] += power(ratio, step);
		xy[0] = south[0];
		xy[1] = south[1];

	} else if (dir == 2)
	{
		if (even)
			east[1] += power(ratio, step);
		else
			east[1] -= power(ratio, step);
		east[0] += power(ratio, step);
		xy[0] = east[0];
		xy[1] = east[1];

	} else
	{
		if (even)
			west[1] -= power(ratio, step);
		else
			west[1] += power(ratio, step);
		west[0] -= power(ratio, step);
		xy[0] = west[0];
		xy[1] = west[1];

	}
}

//Indicate which colour was last used in a particular direction with globals.
static void set_last(int colour, int dir)
{
	if (dir == 0)
		last_north = colour;
	else if (dir == 1)
		last_south = colour;
	else if (dir == 2)
		last_east  = colour;
	else
		last_west  = colour;
}

//Forward Declarations
static void add_purple(int step, int dir);
static void add_green(int step, int dir);

//Add red circles
static void add_red(int step, int dir)
{
	if (!colour_safe(1, dir))
		add_purple(step, dir);
	else {
		set_xy(step, dir);
		fprintf(fot, "<circle fill=\"#DD0000\" cx=\"%.3f\" cy=\"%.3f\" r=\"%.3f\"/>\n", 
															xy[0], xy[1], power(ratio, step));
		set_last(1, dir);
	}
}

//Add yellow circles
static void add_yellow(int step, int dir)
{
	if (!colour_safe(2, dir))
		add_green(step, dir);
	else {
		set_xy(step, dir);
		fprintf(fot, "<circle fill=\"#DDDD00\" cx=\"%.3f\" cy=\"%.3f\" r=\"%.3f\"/>\n", 
															xy[0], xy[1], power(ratio, step));
		set_last(2, dir);
	}
}

//Add green circles
static void add_green(int step, int dir)
{
	if (!colour_safe(3, dir))
		add_yellow(step, dir);
	else {
		set_xy(step, dir);
		fprintf(fot, "<circle fill=\"#00DD00\" cx=\"%.3f\" cy=\"%.3f\" r=\"%.3f\"/>\n", 
															xy[0], xy[1], power(ratio, step));
		set_last(3, dir);
	}
}

//Add purple circles
static void add_purple(int step, int dir)
{
	if (!colour_safe(4, dir))
		add_red(step, dir);
	else {
		set_xy(step, dir);
		fprintf(fot, "<circle fill=\"#AA00AA\" cx=\"%.3f\" cy=\"%.3f\" r=\"%.3f\"/>\n", 
															xy[0], xy[1], power(ratio, step));
		set_last(4, dir);
	}
}

//MAIN\\

//Reads from RandomNumbers :: 00=Red, 01=Yellow, 10=Green, 11=Purple
int main(int ac, char** av)
{
	static unsigned char mask[] = {128, 64, 32, 16, 8, 4, 2, 1};

	fin = fopen("RandomNumbers", "rb"); //read binary
	fot = fopen("EXAMPLE.svg", "w");	//write

	//Print SVG Header
	fprintf(fot, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	fprintf(fot, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
	fprintf(fot, "<svg version=\"1.1\" id=\"Your_Design\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\"\n");
	fprintf(fot, "y=\"0px\" width=\"700\" height=\"700\" viewBox=\"0 0 700 700\" enable-background=\"new 0 0 700 700\" xml:space=\"preserve\">\n");
	fprintf(fot, "<rect fill=\"#000000\" width=\"700\" height=\"700\"/>\n");

	char *bite = malloc(steps + 1);
	fread(bite, 1, steps, fin);
	fread(bite, 1, steps, fin); //Read twice to see second window of
	fclose(fin);				//random information.
	bite[steps] = 0;

	int i,j;

	for(i = 0; i < steps; i++)
	{
		for(j = 0; j < 4; j++)
		{
			if (bite[i] & mask[j*2])
			{
				if (bite[i] & mask[j*2+1])
					add_purple(i, j);
				else
					add_green(i, j);
			} else
			{
				if (bite[i] & mask[j*2+1])
					add_yellow(i, j);
				else
					add_red(i, j);
			}
		}

		even = (i%2 == 0);
	}

	fprintf(fot, "</svg>");

	return 0;
}