#include "randpool.h"
#include <rsa.h>
#include "hex.h"
#include "files.h"
#include <iostream>
using namespace CryptoPP;
using namespace std;


#pragma comment(lib, "cryptlib.lib")

void GenerateRSAKey(unsigned int keyLength, const char *privFilename, const char *pubFilename, const char *seed);
string RSAEncryptString(const char *pubFilename, const char *seed, const char *message);
string RSADecryptString(const char *privFilename, const char *ciphertext);

