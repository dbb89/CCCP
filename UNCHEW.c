/*
 * UNCHEW.c
 * by Daniel Bruce Burns
 *
 * A program that decrypts EXAMPLE.svg, an illustration of
 * the Consciously Creative Chaos Project applied to cyber-
 * security.
 *
 * This program uses a simple XOR cipher, but there are
 * four sets of 'teeth' that might be XORed depending
 * on whether the key encounters a 00, 01, 10, or 11 as
 * it is parsed.
 *
 * The strength of the encryption is quite good for an XOR
 * cipher, especially if the values of the 'teeth' are kept
 * secret. If they are known a sort of reverse-brute force
 * method of attack would reveal the key.
 *
 * This program was written in Sublime Text.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

//File pointers
FILE *key;	//RandomNumbers.bin is used as the key
FILE *fin;	//EXAMPLE.svg.chewed is used as the file to be unchewed 
FILE *fot;	//Unchewed.svg is the decrypted file

//Helper variables
static unsigned char mask[] = {128, 64, 32, 16, 8, 4, 2, 1};
static int lockchunk = 64;

int main(int ac, char** av)
{
	char *lock = malloc(lockchunk + 1);

	key = fopen("RandomNumbers", "rb");		//read binary
	fin = fopen("EXAMPLE.svg.chewed", "rb");		//read binary
	fot = fopen("Unchewed.svg", "w+");//write

	//Read entire file to be encrypted into buffer bite.
	fseek(fin, 0, SEEK_END);
	long fsize = ftell(fin);
	//printf("Size of EXAMPLE.svg = %ld\n", fsize);
	fseek(fin, 0, SEEK_SET);

	char *bite = malloc(fsize + 1);
	fread(bite, 1, fsize, fin);
	fclose(fin);

	bite[fsize] = 0; //set eof

	int i, j, k;

	k=0;
	while (k != fsize)
	{
		fread(lock, 1, lockchunk, key);
		lock[lockchunk] = 0;

		for(i = 0; i < lockchunk; i++)
		{
			for(j = 0; j < 4; j++)
			{
				if (lock[i] & mask[j*2])
				{		//Four sets of 'teeth' for XOR:
					if (lock[i] & mask[j*2+1])
						bite[k] ^= 0xA5;	//10100101
					else
						bite[k] ^= 0x5A;	//01011010
				} else
				{
					if (lock[i] & mask[j*2+1])
						bite[k] ^= 0xAA;	//10101010
					else
						bite[k] ^= 0x55;	//01010101
				}
				k++;
				if (k == fsize)
					break;
			}
			if (k == fsize)
				break;
		}
		//printf("k = %d\n", k);

	}

	fwrite(bite, fsize, 1, fot);
	fclose(fot);
	fclose(key);

	return 0;
}

