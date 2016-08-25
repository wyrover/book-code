#include <iostream>
#include <string>

using namespace std;

class Employer
{
    friend ostream& operator<<              // This has to be a friend
    (ostream& out, const Employer& empr); // so it can access non-
public:                                    // public members
    Employer() {}
    ~Employer() {}

    void setName(const string& name)
    {
        name_ = name;
    }
private:
    string name_;
};

class Employee
{
    friend ostream& operator<<
    (ostream& out, const Employee& obj);
public:
    Employee() : empr_(NULL) {}
    ~Employee()
    {
        if (empr_) delete empr_;
    }

    void setFirstName(const string& name)
    {
        firstName_ = name;
    }
    void setLastName(const string& name)
    {
        lastName_ = name;
    }
    void setEmployer(Employer& empr)
    {
        empr_ = &empr;
    }
    const Employer* getEmployer() const
    {
        return (empr_);
    }

private:
    string firstName_;
    string lastName_;
    Employer* empr_;
};

// Allow us to send Employer objects to an ostream...
ostream& operator<<(ostream& out, const Employer& empr)
{
    out << empr.name_ << endl;
    return (out);
}

// Allow us to send Employee objects to an ostream...
ostream& operator<<(ostream& out, const Employee& emp)
{
    out << emp.firstName_ << endl;
    out << emp.lastName_ << endl;

    if (emp.empr_)
        out << *emp.empr_ << endl;

    return (out);
}

int main()
{
    Employee emp;
    string first = "William";
    string last = "Shatner";
    Employer empr;
    string name = "Enterprise";
    empr.setName(name);
    emp.setFirstName(first);
    emp.setLastName(last);
    emp.setEmployer(empr);
    cout << emp;  // Write to the stream
}