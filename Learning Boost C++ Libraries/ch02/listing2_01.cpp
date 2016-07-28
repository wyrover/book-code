#include <iostream>
#include <string>
#include <map>
#include <boost/optional.hpp>

typedef std::map<std::string, std::string> artiste_album_map;

artiste_album_map latest_albums;

boost::optional<std::string> find_latest_album_of(
    const std::string& artiste_name)
{
    auto iter = latest_albums.find(artiste_name);

    if (iter != latest_albums.end()) {
        return iter->second;
    } else {
        return boost::none;
    }
}

int main()
{
    latest_albums["Korn"] = "";
    latest_albums["The Clash"] = "London Calling";
    latest_albums["David Gilmour"] = "On An Island";
    std::string artistes[3] {"Korn", "The Clash", "David Gilmour"};

    for (const auto& artiste : artistes) {
        boost::optional<std::string> album =
            find_latest_album_of(artiste);

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
