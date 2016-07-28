#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/assign/ptr_list_inserter.hpp>
#include <boost/assign/ptr_map_inserter.hpp>
#include <boost/assign/ptr_list_of.hpp>
#include <string>
#include <iostream>

using namespace boost::assign;

struct WorkShift {
    WorkShift(double start = 9.30, double end = 17.30)
        : start_(start), end_(end)
    {}

    double start_, end_;
};

std::ostream& operator<<(std::ostream& os, const WorkShift& ws)
{
    return os << "[" << ws.start_ << " till " << ws.end_ << "]";
}

int main()
{
    boost::ptr_vector<WorkShift> shifts = ptr_list_of<WorkShift>
                                          (6.00, 14.00)();
    ptr_push_back(shifts)(14.00, 22.00)(22.00, 6.00);
    boost::ptr_map<std::string, WorkShift> shiftMap;
    ptr_map_insert(shiftMap)("morning", 6.00, 14.00)("day")
    ("afternoon", 14.00, 22.00)("night", 22.00, 6.00);

    for (const auto& entry : shiftMap) {
        std::cout << entry.first << " " << shiftMap.at(entry.first)
                  << '\n';
    }
}

