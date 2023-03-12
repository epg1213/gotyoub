#include <stdio.h>

void send_key(int d, int n){
    printf("sending private key...\n");
    FILE* wptr=fopen(".prvgyb", "w");
    fprintf(wptr, "%d:%d", d, n);
    fclose(wptr);
}

int check_key(int d, int n){
    printf("checking private key...\n");
    return 0;
}
