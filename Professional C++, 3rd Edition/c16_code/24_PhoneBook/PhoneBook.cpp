#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

template<class T>
void printMap(const T& m)
{
    for (auto& p : m) {
        cout << p.first << " (Phone: " << p.second << ")" << endl;
    }

    cout << "-------" << endl;
}

int main()
{
    // Create a hash table.
    unordered_map<string, string> um = {
        { "Marc G.", "123-456789" },
        { "Scott K.", "654-987321" }
    };
    printMap(um);
    // Add/remove some phone numbers.
    um.insert(make_pair("John D.", "321-987654"));
    um["Johan G."] = "963-258147";
    um["Freddy K."] = "999-256256";
    um.erase("Freddy K.");
    printMap(um);
    // Find the bucket index for a specific key.
    int bucket = um.bucket("Marc G.");
    cout << "Marc G. is in bucket " << bucket
         << " which contains the following "
         << um.bucket_size(bucket) << " elements:" << endl;
    // Get begin and end iterators for the elements in this bucket.
    // 'auto' is being used here. The compiler will derive the type
    // of both iterators as unordered_map<string, string>::const_local_iterator
    auto liter = um.cbegin(bucket);
    auto literEnd = um.cend(bucket);

    while (liter != literEnd) {
        cout << "\t" << liter->first << " (Phone: " << liter->second << ")" << endl;
        ++liter;
    }

    cout << "-------" << endl;
    // Print some statistics about the hash table
    cout << "There are " << um.bucket_count() << " buckets." << endl;
    cout << "Average number of elements in a bucket is " << um.load_factor() << endl;
    return 0;
}
