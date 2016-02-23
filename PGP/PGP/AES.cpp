#include "stdafx.h"
#include "AES.h"

BYTE key[CryptoPP::AES::DEFAULT_KEYLENGTH], iv[CryptoPP::AES::BLOCKSIZE];
//unsigned char key[] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08, 0x01,0x02, 0x03,0x04,0x05,0x06,0x07,0x08 };//AES::DEFAULT_KEYLENGTH  
void initKV(string key_temp)
{
	//memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);//自动生成随机秘钥
	//memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

	//char tmpK[] = "1234567890123456"; //AES秘钥
	char tmpIV[] = "1234567890123456"; //初始向量
	for (int j = 0; j < CryptoPP::AES::DEFAULT_KEYLENGTH; ++j)
	{
	key[j] = key_temp[j];
	}

	for (int i = 0; i < CryptoPP::AES::BLOCKSIZE; ++i)
	{
	iv[i] = tmpIV[i];
	}

}

string encrypt(string plainText)
{
	string cipherText;

	//  
	CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);
	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(cipherText));
	stfEncryptor.Put(reinterpret_cast<const unsigned char*>(plainText.c_str()), plainText.length() + 1);
	stfEncryptor.MessageEnd();

	string cipherTextHex;
	for (int i = 0; i < cipherText.size(); i++)
	{
		char ch[3] = { 0 };
		sprintf_s(ch, "%02x", static_cast<byte>(cipherText[i]));
		cipherTextHex += ch;
	}

	return cipherTextHex;
}



void writeCipher(string output)
{
	ofstream out("cipher.data");
	out.write(output.c_str(), output.length());
	out.close();

	//cout << "writeCipher finish " << endl << endl;
}

string decrypt(string cipherTextHex)
{
	string cipherText;
	string decryptedText;

	int i = 0;
	while (true)
	{
		char c;
		int x;
		stringstream ss;
		ss << hex << cipherTextHex.substr(i, 2).c_str();
		ss >> x;
		c = (char)x;
		cipherText += c;
		if (i >= cipherTextHex.length() - 2)break;
		i += 2;
	}

	//  
	CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);
	CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decryptedText));
	stfDecryptor.Put(reinterpret_cast<const unsigned char*>(cipherText.c_str()), cipherText.size());

	stfDecryptor.MessageEnd();

	return decryptedText;
}

string readCipher()//返回解密后的数据
{
	ifstream in("cipher.data");

	string line;
	string decryptedText;
	while (getline(in, line))
	{
		if (line.length() > 1)
		{
			decryptedText += decrypt(line) + "\n";
		}
		line.clear();
	}

	//cout << "readCipher finish " << endl;
	in.close();

	return decryptedText;
}

string read()//直接返回数据，并截断文字
{
	ifstream in("cipher.data");

	string line;
	string decryptedText;
	while (getline(in, line))
	{
		if (line.length() > 1)
		{
			decryptedText += line + "\n";
		}
		line.clear();
	}

	//cout << "readCipher finish " << endl;
	in.close();

	return decryptedText;
}

/*---------------加密

int main()
{
string text = "hello zhuzhu dashen !";
cout<<"text : "<<text<<endl;

initKV();
string cipherHex = encrypt(text);
cout<<"cipher : "<<cipherHex<<endl;
writeCipher(cipherHex);

return 0;
}
*/
/*---------------解密
int main()
{
	initKV();
	string text = readCipher();
	cout << "text : " << text << endl;
	return 0;
}*/