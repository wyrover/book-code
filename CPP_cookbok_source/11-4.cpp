#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

struct ChessPlayer {
    ChessPlayer(const char* name, int rating)
        : name_(name), rating_(rating)
    { }
    const char* name_;
    int rating_;
};

struct IsWeakerPlayer {
    bool operator()(const ChessPlayer& x, const ChessPlayer& y)
    {
        return x.rating_ < y.rating_;
    }
};

int main()
{
    ChessPlayer kasparov("Garry Kasparov", 2805);
    ChessPlayer anand("Viswanathan Anand ", 2788);
    ChessPlayer topalov("Veselin Topalov", 2788);
    vector<ChessPlayer> v;
    v.push_back(kasparov);
    v.push_back(anand);
    v.push_back(topalov);
    cout << "the best player is ";
    cout << max_element(v.begin(), v.end(), IsWeakerPlayer())->name_;
    cout << endl;
}