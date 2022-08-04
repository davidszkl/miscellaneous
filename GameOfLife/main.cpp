#include <iostream>
#include <unistd.h>
#include <fstream>
#include <signal.h>
#include <random>
#include <time.h>

using namespace std;
#define SIZE 100*40

struct field {
	char alive;
	size_t neighbors;
};

void init_file(char **alive) {
	ifstream ss("config");
	std::string tmp;
	std::string tmp2;
	for (size_t n = 0; n < 40; n++)
	{
		ss >> tmp;
		tmp2 += tmp;
		tmp2[tmp2.size() - 1] =  '\n';
	}
	*alive = (char *)tmp2.c_str();
}

void init_random(char **alive) {
	srand(time(NULL));
	std::string tmp2;
	std::string tmp;
	for (size_t n = 0; n < 40; n++)
	{
		tmp.clear();
		for (size_t j = 0; j < 100; j++)
		{
			if (rand() % 100 > 50)
				tmp += '@';
			else
				tmp += '.';
		}
		tmp[tmp.size() - 1] = '\n';
		tmp2 += tmp;
	}
	*alive = (char *)tmp2.c_str();
}

void check_rules(char *alive, char *neighbours, size_t n) {
	if (n - 101 > 0 && alive[n - 101] == '@')
		neighbours[n]++;
	if (n - 100 > 0 && alive[n - 100] == '@')
		neighbours[n]++;
	if (n - 99 > 0 && alive[n - 99] == '@')
		neighbours[n]++;
	if (n - 1 > 0 && alive[n - 1] == '@')
		neighbours[n]++;
	if (n + 1 > 0 && alive[n + 1] == '@')
		neighbours[n]++;
	if (n + 99 < SIZE && alive[n + 99] == '@')
		neighbours[n]++;
	if (n + 100 < SIZE && alive[n + 100] == '@')
		neighbours[n]++;
	if (n + 101 < SIZE && alive[n + 101] == '@')
		neighbours[n]++;
}

void update_rules(char *alive, char* neighbours, size_t n) {
	if (alive[n] == '.' && neighbours[n] == 3)
		alive[n] = '@';
	else if (alive[n] == '@' && (neighbours[n] < 2 || neighbours[n] > 3))
		alive[n] = '.';
}

void update(char *alive, char *neighbours) {
	for (size_t n = 0; n < 100 * 40; n++)
		check_rules(alive, neighbours,n);
	for (size_t n = 0; n < 100 * 40; n++)
		update_rules(alive, neighbours, n);
}

int main(int argc, char **argv)
{
	useconds_t sleep_time;
	char *alive;
	char neighbours[100 * 40] = {0};
	switch (argc) {
		case 2:
			sleep_time = atoi(argv[1]);
			init_random(&alive);
			break;
		case 3:
			sleep_time = atoi(argv[1]);
			init_file(&alive);
			break;
		case 4:
			cerr << "too many arguments" << endl;
			return 1;
		default :
			sleep_time = 100000;
			init_random(&alive);
	}
	cout << alive;
	while(1)
	{
		update(alive, neighbours);
		cout << alive;
		memset(neighbours, 0, sizeof(neighbours));
		usleep(sleep_time);
	}
	return 0;
}