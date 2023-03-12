#define p_def 1009
#define q_def 1109
#define e_def 1019
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "send.h"

int check_prime(int n){
  if(n<2){
    return 1;
  }
  int div=0;
  for(int i=2; i<n-1; i++){
    if(n%i==0){
      div++;
    }
  }
  return div;
}

int get_prime(int diff_a, int diff_b){
  srand(time(NULL));
  int p=rand()%1000+1000;
  while(p==diff_a || p==diff_b || check_prime(p)){
    p+=1;
  }
  return p;
}

int exists(const char *fname){
  FILE *file;
  if ((file = fopen(fname, "r")))
  {
    fclose(file);
    return 1;
  }
  return 0;
}

/*
  get_n()
  returns the modulus for rsa
*/
int get_n(){
  int n=0;
  FILE* rptr;
  if(exists("pubgyb")){
    rptr=fopen("pubgyb", "r");
  } else if(exists("prvgyb")){
    rptr=fopen("prvgyb", "r");
  } else {
    return n;
  }
  fscanf(rptr, "%d", &n);
  getc(rptr);
  fscanf(rptr, "%d", &n);
  fclose(rptr);
  return n;
}

/*
  get_e()
  returns the encryption exponent for rsa
*/
int get_e(){
  int e=0;
  if(exists("pubgyb")){
    FILE* rptr=fopen("pubgyb", "r");
    fscanf(rptr, "%d", &e);
    fclose(rptr);
    return e;
  } else {
    return e;
  }
}

/*
  get_d()
  returns the decryption exponent for rsa
*/
int get_d(){
  int d=0;
  if(exists("prvgyb")){
    FILE* rptr=fopen("prvgyb", "r");
    fscanf(rptr, "%d", &d);
    fclose(rptr);
    return d;
  } else {
    return d;
  }
}

/*
  rsa(int key, int mod, long msg)
  takes the exponent (e or d), the modulus and the message to encrypt/decrypt as parameters
  returns the encrypted/decrypted message
*/
long rsa_algo(int key, int mod, long msg){
  long res=1;
  for (long i=0; i<key; i++){
    res=(res*msg)%mod;
  }
  return res;
}

int key_ok(int n, int e, int d){
  long test=123;
  test=(int)rsa_algo(e, n, test);
  test=(int)rsa_algo(d, n, test);
  return test!=123;
}

int gen_keys(){
  int n;
  int e;
  int d;
  do{
    int p=get_prime(0, 0);
    int q=get_prime(p, 0);
    n=p*q;
    e=get_prime(p, q);
    int phi=(p-1)*(q-1);
    d=2;
    while ((d*e)%phi!=1){
      d++;
    }
  } while (key_ok(n, e, d));
  FILE* wptr=fopen("pubgyb", "w");
  fprintf(wptr, "%d:%d", e, n);
  fclose(wptr);
  send_key(d,n);
  return check_key(d,n);
}
