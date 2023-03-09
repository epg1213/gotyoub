#include "rsa.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int getInt(char a, char b){
    if(a>57){
        a-=87;
    } else {
        a-=48;
    }
    if(b>57){
        b-=87;
    } else {
        b-=48;
    }
    return 16*a+b;
}

void byg_to_gyb(char* bygpath, char* gybpath){
    srand(time(NULL));
    FILE* rptr=fopen(bygpath, "r");
    FILE* wptr=fopen(gybpath, "wb");
    char a=' ';
    char i=getc(rptr);
    int random;
    char c;
    while(i!=EOF){
        random=rand()%16;
        if(random<5){
            c=97+random;
        } else {
            c=i;
            i=getc(rptr);
        }
        if(a==' '){
            a=c;
        } else {
            putc(getInt(a, c), wptr);
            a=' ';
        }
    }
    if(a!=' '){
        random=rand()%5;
        c=97+random;
        putc(getInt(a, c), wptr);
    }

    fclose(rptr);
    fclose(wptr);
}

/*
  encrypt(char* path)
  takes the path to the file to encrypt as a parameter
  returns whether the original file was successfully removed or not
*/
void encrypt_file(char* path){
    char* bygpath=malloc(strlen(path)+4);
	strcpy(bygpath, path);
	strcat(bygpath, ".byg");

	char* gybpath=malloc(strlen(path)+4);
	strcpy(gybpath, path);
	strcat(gybpath, ".gyb");

	int mod=get_n();
	int enc=get_e();
	
	FILE* rptr=fopen(path, "rb");
	FILE* wptr=fopen(bygpath, "w");
	
	int c = getc(rptr);
	while (c!=EOF)
	{
		fprintf(wptr, "%ldf", rsa_algo(enc, mod, (long)c));
        c = getc(rptr);
	}
	
	fclose(rptr);
	fclose(wptr);
    rename(path, gybpath);
    byg_to_gyb(bygpath, gybpath);
    remove(bygpath);
	free(bygpath);
	free(gybpath);
}
