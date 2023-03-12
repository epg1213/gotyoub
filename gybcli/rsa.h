#ifndef rsa
#define rsa

int gen_keys();
int exists(const char *fname);
int get_n();
int get_e();
int get_d();
long rsa_algo(int key, int mod, long msg);

#endif