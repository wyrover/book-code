#include <iostream>
#include <istream>
#include <fstream>
#include <string>

using namespace std;

class Employee
{
    friend ostream& operator<<              // These have to be friends
    (ostream& out, const Employee& emp); // so they can access
    friend istream& operator>>              // nonpublic members
    (istream& in, Employee& emp);

public:
    Employee() {}
    ~Employee() {}

    void setFirstName(const string& name)
    {
        firstName_ = name;
    }
    void setLastName(const string& name)
    {
        lastName_ = name;
    }

private:
    string firstName_;
    string lastName_;
};

// Send an Employee object to an ostream...
ostream& operator<<(ostream& out, const Employee& emp)
{
    out << emp.firstName_ << endl;
    out << emp.lastName_ << endl;
    return (out);
}

// Read an Employee object from a stream
istream& operator>>(istream& in, Employee& emp)
{
    in >> emp.firstName_;
    in >> emp.lastName_;
    return (in);
}

int main()
{
    Employee emp;
    string first = "William";
    string last = "Shatner";
    emp.setFirstName(first);
    emp.setLastName(last);
    ofstream out("tmp\\emp.txt");

    if (!out) {
        cerr << "Unable to open output file.\n";
        exit(EXIT_FAILURE);
    }

    out << emp;  // Write the Emp to the file
    out.close();
    ifstream in("tmp\\emp.txt");

    if (!in) {
        cerr << "Unable to open input file.\n";
        exit(EXIT_FAILURE);
    }

    Employee emp2;
    in >> emp2;  // Read the file into an empty object
    in.close();
    cout << emp2;
}