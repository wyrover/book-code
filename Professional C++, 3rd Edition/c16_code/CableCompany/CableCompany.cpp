#include "CableCompany.h"
#include <stdexcept>

using namespace std;

void CableCompany::addPackage(const string& packageName,
                              const bitset<kNumChannels>& channels)
{
    // Just make a key/value pair and insert it into the packages map.
    mPackages.insert({ packageName, channels });            // C++11
    //mPackages.insert(make_pair(packageName, channels));   // Pre-C++11
}

void CableCompany::removePackage(const string& packageName)
{
    // Just erase the package from the package map
    mPackages.erase(packageName);
}

void CableCompany::newCustomer(const string& name, const string& package)
{
    // Get a reference to the specified package.
    auto it = mPackages.find(package);

    if (it == end(mPackages)) {
        // That package doesn't exist. Throw an exception.
        throw out_of_range("Invalid package");
    }

    // Create the account with the bitset representing that package.
    // Note that 'it' refers to a name/bitset pair. The bitset is the
    // second field.
    auto result = mCustomers.insert({ name, it->second });

    if (!result.second) {
        // Customer was already in the database. Nothing changed.
        throw invalid_argument("Duplicate customer");
    }
}

void CableCompany::newCustomer(const string& name,
                               const bitset<kNumChannels>& channels)
{
    // Just add the customer/channels pair to the customers map.
    auto result = mCustomers.insert({ name, channels });

    if (!result.second) {
        // Customer was already in the database. Nothing changed.
        throw invalid_argument("Duplicate customer");
    }
}

void CableCompany::addChannel(const string& name, int channel)
{
    // Find a reference to the customer.
    auto it = mCustomers.find(name);

    if (it != end(mCustomers)) {
        // We found this customer; set the channel.
        // Note that 'it' is a reference to a name/bitset pair.
        // The bitset is the second field.
        it->second.set(channel);
    } else {
        throw invalid_argument("Unknown customer");
    }
}

void CableCompany::removeChannel(const string& name, int channel)
{
    // Find a reference to the customer.
    auto it = mCustomers.find(name);

    if (it != end(mCustomers)) {
        // We found this customer; remove the channel.
        // Note that 'it' is a reference to a name/bitset pair.
        // The bitset is the second field.
        it->second.reset(channel);
    } else {
        throw invalid_argument("Unknown customer");
    }
}

void CableCompany::addPackageToCustomer(const string& name, const string& package)
{
    // Find the package.
    auto itPack = mPackages.find(package);

    if (itPack == end(mPackages)) {
        // That package doesn't exist. Throw an exception.
        throw out_of_range("Invalid package");
    }

    // Find the customer.
    auto itCust = mCustomers.find(name);

    if (itCust != end(mCustomers)) {
        // Or-in the package to the customers existing channels.
        // Note that the iterators are references to name/bitset pairs.
        // The bitset is the second field.
        itCust->second |= itPack->second;
    } else {
        throw invalid_argument("Unknown customer");
    }
}

void CableCompany::deleteCustomer(const string& name)
{
    // Remove the customer with this name
    mCustomers.erase(name);
}

bitset<kNumChannels>& CableCompany::getCustomerChannels(const string& name)
{
    // Find the customer
    auto it = mCustomers.find(name);

    if (it != end(mCustomers)) {
        // Found it.
        // Note that 'it' is a reference to a name/bitset pair.
        // The bitset is the second field.
        return it->second;
    }

    // Didn't find it. Throw an exception.
    throw invalid_argument("Unknown customer");
}
