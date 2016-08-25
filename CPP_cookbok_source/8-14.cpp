#include <iostream>

using namespace std;

class Score
{
public:
    Score() : score_(0) {}
    Score(int i) : score_(i) {}

    Score& operator++()    // prefix
    {
        ++score_;
        return (*this);
    }
    const Score operator++(int)   // postfix
    {
        Score tmp(*this);
        ++(*this); // Take advantage of the prefix operator
        return (tmp);
    }
    Score& operator--()
    {
        --score_;
        return (*this);
    }
    const Score operator--(int x)
    {
        Score tmp(*this);
        --(*this);
        return (tmp);
    }
    int getScore() const
    {
        return (score_);
    }

private:
    int score_;
};

int main()
{
    Score player1(50);
    player1++;
    ++player1; // score_ = 52
    cout << "Score = " << player1.getScore() << '\n';
    (--player1)--; // score_ = 50
    cout << "Score = " << player1.getScore() << '\n';
}