//#include "stdafx.h"
#include <md5.h>
#include <iostream>
using namespace CryptoPP;
using namespace std;
#pragma comment (lib,"cryptlib.lib")
#include <hex.h>
#include <files.h>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1 

string md5_string(const string & message);