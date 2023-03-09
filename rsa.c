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
long rsa_algo(int key, int mod, long msg){
  long res=1;
  for (long i=0; i<key; i++){
    res=(res*msg)%mod;
  }
  return res;
}
