#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
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

int is_regular_file(const char *path)
{
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISREG(path_stat.st_mode);
}

void rec(const char *dname, void (*encdec)(char*))
{
  DIR *d;
  struct dirent *dir;
  d = opendir(dname);
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if(strcmp(dir->d_name,".") && strcmp(dir->d_name,"..") && strcmp(dir->d_name,"gotyoub")){
        char* fullpath=malloc(strlen(dname)+strlen(dir->d_name)+1);
        strcpy(fullpath, dname);
        strcat(fullpath, "/");
        strcat(fullpath, dir->d_name);
        if(is_regular_file(fullpath)){
          printf("encrypting:%s\n", fullpath);
          if (encdec==&decrypt){
            fullpath[strlen(fullpath)-4]='\0';
          }
          encdec(fullpath);
        } else {
          printf(" entering :%s\n", fullpath);
          rec(fullpath, encdec);
          printf(" exiting  :%s\n", fullpath);
        }
        
        free(fullpath);
      }
    }
    closedir(d);
  }
}

void main(int argc, char* argv[]){
  int dec=0;
  if (argc==2 && (!strcmp(argv[1], "-e") || !strcmp(argv[1], "--encrypt"))){
    printf("Encrypring mode enabled.\n");
  } else if (argc==2 && (!strcmp(argv[1], "-d") || !strcmp(argv[1], "--decrypt"))){
    printf("Decrypring mode enabled.\n");
    dec=1;
  } else {
    printf("Usage:\ngotyoub [-e|--encrypt] :recursively encrypts a directory\ngotyoub [-d|--decrypt] :recursively decrypts a directory\n");
    exit(0);
  }
  printf("Please enter the path to the direcrory:\ngyb!>");
  char path[100]="NULL";
  scanf("%s", path);
  
  while(path[0]!='/' || !exists(path) || is_regular_file(path)){
    printf("Only valid absolute path to directories are allowed.\ngyb!>");
    scanf("%s", path);
  }
  if (dec){
    rec(path, &decrypt);
  } else {
    rec(path, &encrypt);
  }
}
