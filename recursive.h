#ifndef recursive
#define recursive

int is_regular_file(const char *path);
void rec(const char *dname, void (*encdec)(char*));
void encrypt_file(char * path);
void decrypt_file(char * path);

#endif