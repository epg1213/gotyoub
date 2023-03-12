#include "rsa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void gyb_to_byg(char* gybpath, char* bygpath){
    FILE* rptr=fopen(gybpath, "rb");
    FILE* wptr=fopen(bygpath, "w");
    
    int i=getc(rptr);
    while (i!=EOF){
        char* c=malloc(2);
        sprintf(c, "%02x", i);
    if (c[0]<58 || 101<c[0] ){
        putc(c[0], wptr);
    }
    if (c[1]<58 || 101<c[1] ){
        putc(c[1], wptr);
    }
    i=getc(rptr);
    free(c);
    }
    
    fclose(rptr);
    fclose(wptr);
}

/*
  decrypt(char* path)
  takes the path to the file to decrypt as a parameter
  returns whether the encrypted file was successfully removed or not
*/
void decrypt_file(char * path){
	char* bygpath=malloc(strlen(path)+4);
	strcpy(bygpath, path);
	strcat(bygpath, ".byg");

	char* gybpath=malloc(strlen(path)+4);
	strcpy(gybpath, path);
	strcat(gybpath, ".gyb");

    gyb_to_byg(gybpath, bygpath);
    rename(gybpath, path);

	int mod=get_n();
	int dec=get_d();

	FILE* rptr=fopen(bygpath, "r");
	FILE* wptr=fopen(path, "w");

	char c = 'f';
	while (c=='f')
	{
        long number;
        if (fscanf(rptr, "%ld", &number)!=1)
            break;
        putc((int)rsa_algo(dec, mod, number), wptr);
        c = getc(rptr);
	}

	fclose(rptr);
	fclose(wptr);
    remove(bygpath);
	free(bygpath);
	free(gybpath);
}
