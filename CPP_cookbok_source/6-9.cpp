#include <iostream>
#include <string>
#include <hash_map>

using namespace std;

class Session
{
    /* ... */
};

// Make reading easier with a typedef
typedef hash_map<string, Session*> SessionHashMap;

class SessionManager
{

public:
    SessionManager() : sessionMap_(500) {}    // Initialize hash table
    // with 500 buckets
    ~SessionManager()
    {
        for (SessionHashMap::iterator p = sessionMap_.begin();
             p != sessionMap_.end(); ++p)
            delete(*p).second;  // Destroy the Session object
    }

    Session* addSession(const string& login)
    {
        Session* p = NULL;

        if (!(p = getSession(login))) {
            p = new Session();
            sessionMap_[login] = p; // Assign the new session with
        }                          // operator[]

        return (p);
    }
    Session* getSession(const string& login)
    {
        return (sessionMap_[login]);
    }
    // ...

private:
    SessionHashMap sessionMap_;
};
