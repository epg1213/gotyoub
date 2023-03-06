#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

/*
  encrypt(char* path)
  takes the path to the file to encrypt as a parameter
  returns whether the original file was successfully removed or not
*/
int encrypt(char* path){
	int mod=get_n();
	int enc=get_e();
	
	FILE* rptr=fopen(path, "r");
	
	char* new_path=malloc(sizeof(char)*(strlen(path)+4));
	strcpy(new_path, path);
	strcat(new_path, ".gyb");
	FILE* wptr=fopen(new_path, "w");
	
	char c = getc(rptr);
	while (c!=EOF)
	{
    char* conv=malloc(sizeof(long));
    sprintf(conv, "%d", c);
		fprintf(wptr, "%ldf", rsa(enc, mod, atol(conv)));
    free(conv);
    c = getc(rptr);
	}
	free(new_path);
	fclose(rptr);
	fclose(wptr);
	return remove(path);
}

/*
  decrypt(char* path)
  takes the path to the file to decrypt as a parameter
  returns whether the encrypted file was successfully removed or not
*/
int decrypt(char * path){
	int mod=get_n();
	int dec=get_d();

	path[strlen(path)-4]='\0';
	FILE* wptr=fopen(path, "w");
	
  path[strlen(path)]='.';
	FILE* rptr=fopen(path, "r");
	
	char c = 'f';
	while (c=='f')
	{
	  long number;
	  if (fscanf(rptr, "%ld", &number)!=1)
	    break;
	  fprintf(wptr, "%c", (int)rsa(dec, mod, number));
    c = getc(rptr);
	}
	fclose(rptr);
	fclose(wptr);
	return remove(path);
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
      printf("decrypting: %s", path);
      decrypt(path);
    }
  }
  printf("\n");
}

