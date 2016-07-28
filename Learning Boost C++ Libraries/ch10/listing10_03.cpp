#include <boost/thread.hpp>
#include <boost/chrono/duration.hpp>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <boost/move/move.hpp>

void doStuff(const std::string& name)
{
    std::stringstream sout;
    sout << "[name=" << name << "]"
         << "[id=" << boost::this_thread::get_id() << "]"
         << " doing work\n";
    std::cout << sout.str();
    boost::this_thread::sleep_for(boost::chrono::seconds(2));
}

int main()
{
    typedef std::map<boost::thread::id, boost::thread> threadmap;
    threadmap tmap;
    std::vector<std::string> tnames{ "thread1", "thread2",
                                     "thread3", "thread4", "thread5" };

    for (auto name : tnames) {
        boost::thread thr(doStuff, name);
        tmap[thr.get_id()] = boost::move(thr);
    }

    for (auto& thrdEntry : tmap) {
        thrdEntry.second.join();
        std::cout << thrdEntry.first << " returned\n";
    }
}

