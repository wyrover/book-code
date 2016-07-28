#include <cstdio>
#include <string>

using namespace std;

int main()
{
	string bottles = "bottles of beer";
	char on_wall[99];
	for (int i = 99; i > 0; --i) {
		snprintf(on_wall, sizeof(on_wall), "%s on the wall", bottles.c_str());
		printf("%d %s, %d %s.\n", i, on_wall, i, bottles.c_str());
		printf("Take one down, pass it around, %d %s.\n", i - 1, on_wall);
	}
}
