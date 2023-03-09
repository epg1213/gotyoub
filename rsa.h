#ifndef rsa
#define rsa

int get_n();
int get_e();
int get_d();
long rsa_algo(int key, int mod, long msg);

#endif