#include <iostream>
#include <cstdlib>
#include <ctime>
#include <set>
#include <boost/thread.hpp>

struct player {
    int id;
    // other fields
    bool operator < (const player& that) const
    {
        return id < that.id;
    }
};

std::set<player> armed, unarmed; // A, U
boost::mutex amtx, umtx;

auto a2u = [&](int playerId)
{
    boost::lock_guard<boost::mutex> lka(amtx);
    auto it = armed.find(player{playerId});

    if (it != armed.end()) {
        auto plyr = *it;
        boost::unique_lock<boost::mutex> lku(umtx);
        unarmed.insert(plyr);
        lku.unlock();
        armed.erase(it);
    }
};

auto u2a = [&](int playerId)
{
    boost::lock_guard<boost::mutex> lku(umtx);
    auto it = unarmed.find(player{playerId});

    if (it != unarmed.end()) {
        auto plyr = *it;
        boost::unique_lock<boost::mutex> lka(amtx);
        armed.insert(plyr);
        lka.unlock();
        unarmed.erase(it);
    }
};

void onAmmoExhausted(int playerId)   // event callback
{
    boost::thread exhausted(a2u, playerId);
    exhausted.detach();
}

void onAmmoReplenished(int playerId)   // event callback
{
    boost::thread replenished(a2u, playerId);
    replenished.detach();
}

int main()
{
}
