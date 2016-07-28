#include <iostream>
#include <string>
#include <map>
#include <boost/optional.hpp>

template <typename C>
boost::optional<typename C::mapped_type>
lookup(const C& dict, const typename C::key_type& key)
{
    typename C::const_iterator it = dict.find(key);

    if (it != dict.end()) {
        return it->second;
    } else {
        return boost::none;
    }
}

typedef std::map<std::string, std::string> artiste_album_map;

artiste_album_map latest_albums;

int main()
{
    latest_albums["Korn"] = "";
    latest_albums["The Clash"] = "London Calling";
    latest_albums["David Gilmour"] = "On An Island";
    std::string artistes[3] {"Korn", "The Clash", "David Gilmour"};

    for (const auto& artiste : artistes) {
        boost::optional<std::string> album =
            lookup(latest_albums, artiste);

        if (album) {
            std::cout << "The last album from " << artiste;

            if (album->empty()) {
                std::cout << " is untitled\n";
            } else {
                std::cout << " is named " << *album << '\n';
            }
        } else {
            std::cout << "No information on albums from "
                      << artiste << '\n';
        }
    }
}
