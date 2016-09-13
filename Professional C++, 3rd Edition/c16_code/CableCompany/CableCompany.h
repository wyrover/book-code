#pragma once

#include <cstddef>
#include <bitset>
#include <map>
#include <string>

const size_t kNumChannels = 10;

class CableCompany
{
public:
    // Adds the package with the specified channels to the database.
    void addPackage(const std::string& packageName,
                    const std::bitset<kNumChannels>& channels);

    // Removes the specified package from the database
    void removePackage(const std::string& packageName);

    // Adds customer to database with initial channels found in package.
    // Throws out_of_range if the package name is invalid.
    // Throws invalid_argument if the customer is unknown.
    void newCustomer(const std::string& name, const std::string& package);

    // Adds customer to database with given initial channels.
    // Throws invalid_argument if the customer is unknown.
    void newCustomer(const std::string& name, const std::bitset<kNumChannels>& channels);

    // Adds the channel to the customers profile.
    // Throws invalid_argument if the customer is unknown.
    void addChannel(const std::string& name, int channel);

    // Removes the channel from the customers profile.
    // Throws invalid_argument if the customer is unknown.
    void removeChannel(const std::string& name, int channel);

    // Adds the specified package to the customers profile.
    // Throws out_of_range if the package name is invalid.
    // Throws invalid_argument if the customer is unknown.
    void addPackageToCustomer(const std::string& name, const std::string& package);

    // Removes the specified customer from the database.
    void deleteCustomer(const std::string& name);

    // Retrieves the channels to which a customer subscribes.
    // Throws invalid_argument if the customer is unknown.
    std::bitset<kNumChannels>& getCustomerChannels(const std::string& name);

private:
    typedef std::map<std::string, std::bitset<kNumChannels>> MapType;
    MapType mPackages, mCustomers;
};
