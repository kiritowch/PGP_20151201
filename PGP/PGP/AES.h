#pragma once

#include <stdio.h>  

#include <iostream>  
#include <fstream>  
#include <sstream>  

#include <aes.h>  
#include <filters.h>  
#include <modes.h>  

using namespace std;
#pragma comment(lib, "cryptlib.lib")

//void initKV();
void initKV(string key_temp);
string encrypt(string plainText);
void writeCipher(string output);
string decrypt(string cipherTextHex);
string readCipher();
string read();

