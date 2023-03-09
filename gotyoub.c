#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recursive.h"

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
    rec(path, &decrypt_file);
  } else {
    rec(path, &encrypt_file);
  }
}
