#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

#define COMMON_FREQ 0.17110

using namespace std;
using std::string;
using std::size_t;

struct Key {
	string	key;
	float	error;
};

bool is_alpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

vector<map<char, float> > get_frequencies(const string& content, size_t l)
{
	vector<map<char, float> > rval(l);
	size_t content_length = content.length();

	for (size_t n = 0; n < l; n++)
		for (char i = 'a'; i <= 'z'; i++)
			rval[n][i] = 0;

	for (size_t i = 0; i < content_length; i++)
		rval[i % l][content[i]]++;
		
	for (size_t n = 0; n < l; n++)
		for (char i = 'a'; i <= 'z'; i++)
			rval[n][i] = rval[n][i] / (content_length / l);

	return rval;
}

float get_max(map<char, float> mapp) {
	map<char, float>::iterator it1 = mapp.begin();
	map<char, float>::iterator it2 = mapp.end();
	float max = it1->second;
	while (it1 != it2)
	{
		if (it1->second > max)
			max = it1->second;
		it1++;
	}
	return max;
}

float get_error_mean(vector<map<char, float> >& freq){
	float error = 0;
	size_t freq_size = freq.size();
	for (size_t i = 0; i < freq_size; i++)
		error += abs(get_max(freq[i]) - COMMON_FREQ);
	return error / freq_size;
}

char get_letter(map<char, float>& from) {
	const string alphabet = "abcdefghijklmnopqrstuvwxyz";
	map<char, float>::iterator beg = from.begin();
	map<char, float>::iterator end = from.end();
	char letter = beg->first;
	while (beg != end)
	{
		if (from[beg->first] > from[letter])
			letter = beg->first;
		beg++;
	}
	int diff = letter - 'e';
	if (diff < 0)
		diff += 'z' - 'a' + 1;
	return alphabet.c_str()[diff];
}

string get_combination(vector<map<char, float> >& freq) {
	size_t freq_size = freq.size();
	string tmp;
	for (size_t n = 0; n < freq_size; n++)
		tmp += get_letter(freq[n]);
	return tmp;
}

string decode_helper(const string& encoded, const string& key) {
	string rval;
	size_t len = key.length();
	size_t encoded_length = encoded.length();
	const string alphabet = "abcdefghijklmnopqrstuvwxyz";

	for (size_t n = 0; n < encoded_length; n++)
	{
		char offset = key[n % len] - 'a';
		if (encoded[n] - offset < 'a')
		{
			char overflow = 'a' - encoded[n] + offset;
			rval += alphabet[26 - overflow];
		}
		else
			rval += encoded[n] - offset;
	}
	return rval;
}

void decode(const string& content_raw, const string& content_lower_decoded, string& output) {
	size_t j = 0;
	size_t content_size = content_raw.size();
	for (size_t n = 0 ; n < content_size; n++)
	{
		if (is_alpha(content_raw[n]))
		{
			if (content_raw[n] >= 'a')
				output += tolower(content_lower_decoded[j++]);
			else
				output += toupper(content_lower_decoded[j++]);
		}
		else
			output += content_raw[n];
	}
}

Key findCandidate(const string& cypher, size_t l) {
	Key rval;

	vector<map<char, float> > freq = get_frequencies(cypher, l);
	rval.error	= get_error_mean(freq);
	rval.key	= get_combination(freq);

	return rval;
}

string read_file(const char *filename, string& content_lower){
	ifstream file(filename);
	ostringstream sstr;
	sstr << file.rdbuf();
	string content = sstr.str();
	for (size_t n = 0; n < content.size(); n++)
		if (is_alpha(content[n]))
			content_lower += tolower(content[n]);
	return content;
}

int main(int ac, char** av)
{
	if (ac != 4)
	{
		cerr << "arguments are: file_to_decrypt max_key_len output_filename" << endl;
		return 1;
	}
	string content_lower;
	string content	= read_file(av[1], content_lower);
	int l			= atoi(av[2]);
	Key best;
	best.error = 100;

	for (int i = 1; i <= l; i++)
	{
		Key tmp = findCandidate(content_lower, i);
		if (tmp.error < best.error)
			best = tmp;
	}
	string intermediate = decode_helper(content_lower, best.key);
	string final;
	decode(content, intermediate, final);

	ofstream output(av[3]);
	output << final;

	return 0;
}