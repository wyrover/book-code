#define BOOST_VARIANT_NO_FULL_RECURSIVE_VARIANT_SUPPORT
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <boost/variant.hpp>
#include <algorithm>

struct JSONNullType {};

typedef boost::make_recursive_variant <
std::string,
    double,
    bool,
    JSONNullType,
    std::map<std::string,
    boost::recursive_variant_>,
    std::vector<boost::recursive_variant_>
    >::type JSONValue;

typedef std::vector<JSONValue> JSONArray;
typedef std::map<std::string, JSONValue> JSONObject;

void printArrElem(const JSONValue& val);
void printObjAttr(const JSONObject::value_type& val);

std::string escapeStr(const std::string& str)
{
    /* logic for escaping embedded quotes in str */
    return str;
}

struct JSONPrintVisitor : public boost::static_visitor<void> {
    void operator()(const std::string& str) const
    {
        std::cout << '"' << escapeStr(str) << '"';
    }

    void operator()(const JSONNullType&) const
    {
        std::cout << "null";
    }

    template <typename T>
    void operator()(const T& value) const
    {
        std::cout << std::boolalpha << value;
    }

    void operator()(const JSONArray& arr) const
    {
        std::cout << '[';

        if (!arr.empty()) {
            boost::apply_visitor(*this, arr[0]);
            std::for_each(arr.begin() + 1, arr.end(), printArrElem);
        }

        std::cout << "\n";
    }

    void operator()(const JSONObject& object) const
    {
        std::cout << '{';

        if (!object.empty()) {
            const auto& kv_pair = *(object.begin());
            std::cout << '"' << escapeStr(kv_pair.first) << '"';
            std::cout << ':';
            boost::apply_visitor(*this, kv_pair.second);
            auto it = object.begin();
            std::for_each(++it, object.end(), printObjAttr);
        }

        std::cout << '}';
    }

};

void printArrElem(const JSONValue& val)
{
    std::cout << ',';
    boost::apply_visitor(JSONPrintVisitor(), val);
}

void printObjAttr(const JSONObject::value_type& val)
{
    std::cout << ',';
    std::cout << '"' << escapeStr(val.first) << '"';
    std::cout << ':';
    boost::apply_visitor(JSONPrintVisitor(), val.second);
}

int main()
{}
