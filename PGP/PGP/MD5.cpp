#include "stdafx.h"
#include"MD5.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1 

string md5_string(const string & message)
{
	string digest;
	MD5 md5;
	StringSource(message, true,
		new HashFilter(md5, new HexEncoder(new StringSink(digest))));
	return digest;
}
/*
int main()
{
	string test = "1234";
	cout << md5_string(test);
	system("pause");
	return 0;
}*/