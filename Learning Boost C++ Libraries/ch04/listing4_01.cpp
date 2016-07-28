#include <string>
#include <algorithm>
#include <cassert>
#include <cctype>

int main()
{
    std::string song = "Green-tinted sixties mind";
    std::transform(song.begin(), song.end(), song.begin(),
                   ::toupper);
    assert(song == "GREEN-TINTED SIXTIES MIND");
}

