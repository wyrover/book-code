#include <boost/container/stable_vector.hpp>
#include <cassert>
#include <string>

int main()
{
    const char *cloud_names[] = {"cumulus", "cirrus", "stratus",
                                 "cumulonimbus", "cirrostratus", "cirrocumulus",
                                 "altocumulus", "altostratus"
                                };
    boost::container::stable_vector<std::string> clouds;
    clouds.reserve(4);
    clouds.resize(4);   // To circumvent a bug in Boost 1.54
    size_t name_count = sizeof(cloud_names) / sizeof(const char*);
    size_t capacity = clouds.capacity();
    size_t i = 0;

    for (i = 0; i < name_count && i < capacity; ++i) {
        clouds[i] = cloud_names[i];
    }

    auto first = clouds.begin();

    for (; i < name_count; ++i) {
        clouds.push_back(cloud_names[i]);
    }

    auto sixth = clouds.begin() + 5;
    // 1 erase @4
    clouds.erase(clouds.begin() + 4);
    // 2 inserts @3
    clouds.insert(clouds.begin() + 3, "stratocumulus");
    clouds.insert(clouds.begin() + 3, "nimbostratus");
    assert(*first == cloud_names[0]);
    assert(sixth == clouds.begin() + 6); // not +5
    assert(*sixth == cloud_names[5]);
}

