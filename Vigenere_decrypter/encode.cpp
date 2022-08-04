#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

bool is_alpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void encode(string& encode, string& to, const string& key){
	int len = key.length();
	const char alphabet[27] = "abcdefghijklmnopqrstuvwxyz";
	string encode_lower;
	size_t encode_size = encode.size();
	for (size_t n = 0; n < encode_size; n++)
		if (is_alpha(encode[n]))
			encode_lower += tolower(encode[n]);
	size_t j = 0;
	for (size_t n = 0; n < encode_size; n++)
	{
		if (!is_alpha(encode[n]))
		{
			to += encode[n];
			continue;
		}
		int offset = key[j % len] - 'a';
		if (encode_lower[j] + offset > 'z')
		{
			int overflow = encode_lower[j] + offset - 'z' - 1;
			to += alphabet[overflow];
		}
		else
			to += encode_lower[j] + offset;
		j++;
	}
}

// void decode(const string& encoded, const string& key){
// 	size_t len = key.size();
// 	size_t encoded_length = encoded.length();
// 	string rval;
// 	string alphabet = "abcdefghijklmnopqrstuvwxyz";
// 	size_t j = 0;
// 	for (size_t n = 0; n < encoded_length; n++)
// 	{
// 		if (!is_alpha(encoded[n]))
// 		{
// 			rval += encoded[n];
// 			continue;
// 		}
// 		int offset = key[j % len] - 'a';
// 		if (encoded[n] - offset < 'a')
// 		{
// 			int overflow = 'a' - encoded[n] + offset;
// 			rval += alphabet[26 - overflow];
// 		}
// 		else
// 			rval += encoded[n] - offset;
// 		j++;
// 	}
// 	cout << rval << endl;
// }

int main(int ac, char** av)
{
	if (ac != 3)
	{
		cerr << "arguments are: text_to_encrypt(french) encryption_key" << endl;
		return 1;
	}
	ifstream file(av[1]);
	ostringstream sstr;
	sstr << file.rdbuf();
	string content = sstr.str();

	string key(av[2]);
	string encoded;
	encode(content, encoded, key);

	ofstream ofile("secrect.txt");
	ofile << encoded << endl;
	ofile.close();

	return 0;
}