#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define p 1009
#define q 1109
#define e 1019

/*
  get_n()
  returns the modulus for rsa
*/
int get_n(){
  return p*q;
}

/*
  get_e()
  returns the encryption exponent for rsa
*/
int get_e(){
  return e;
}

/*
  get_d()
  returns the decryption exponent for rsa
*/
int get_d(){
  int phi=(p-1)*(q-1);
  int d=2;
  while ((d*e)%phi!=1){
    d++;
  }
  return d;
}

/*
  rsa(int key, int mod, long msg)
  takes the exponent (e or d), the modulus and the message to encrypt/decrypt as parameters
  returns the encrypted/decrypted message
*/
long rsa(int key, int mod, long msg){
  long res=1;
  for (long i=0; i<key; i++){
    res=(res*msg)%mod;
  }
  return res;
}

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
  encrypt(char* path)
  takes the path to the file to encrypt as a parameter
  returns whether the original file was successfully removed or not
*/
void encrypt(char* path){
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
		fprintf(wptr, "%ldf", rsa(enc, mod, (long)c));
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

/*
  decrypt(char* path)
  takes the path to the file to decrypt as a parameter
  returns whether the encrypted file was successfully removed or not
*/
void decrypt(char * path){
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
	  putc((int)rsa(dec, mod, number), wptr);
    c = getc(rptr);
	}
	
	fclose(rptr);
	fclose(wptr);
  remove(bygpath);
	free(bygpath);
	free(gybpath);
}

/*
  exists(const char *fname)
  takes the path to the file to check as a parameter
  returns whether the file exists or not
*/
int exists(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void main(){
  char path[100];
  printf("please write the path to the file:\n");
  scanf("%s", path);
  char* ext = strrchr(path, '.');
  if (!exists(path)) {
    printf("error finding file: %s\n", path);
    exit(1);
  } else {
    if(!ext){
      printf("there is no extension\n");
      ext=".NULL";
    } else {
      printf("extension is %s\n", ext + 1);
    }
    if(strcmp("gyb", ext+1)){
      printf("encrypting: %s", path);
      encrypt(path);
    }else{
      path[strlen(path)-4]='\0';
      printf("decrypting: %s", path);
      decrypt(path);
    }
  }
  printf("\n");
}
