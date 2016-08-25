#include <iostream>
#include <set>
#include <map>
#include <string>

using namespace std;

typedef set<string> SetStr;
typedef map<string, SetStr*> MapStrSetStr;

// Dummy database class
class DBConn
{
public:
    void beginTxn() {}
    void endTxn() {}
    void execSql(string& sql) {}
};

class SimpleTxnLog
{

public:
    SimpleTxnLog() {}
    ~SimpleTxnLog()
    {
        purge();
    }

    // Add an SQL statement to the list
    void addTxn(const string& id,
                const string& sql)
    {
        SetStr* pSet = log_[id];      // This creates the entry for

        if (pSet == NULL) {           // this id if it isn't there
            pSet = new SetStr();
            log_[id] = pSet;
        }

        pSet->insert(sql);
    }

    // Apply the SQL statements to the database, one transaction
    // at a time
    void apply()
    {
        for (MapStrSetStr::iterator p = log_.begin();
             p != log_.end(); ++p) {
            conn_->beginTxn();

            // Remember that a map iterator actually refers to an object
            // of pair<Key,Val>.  The set pointer is stored in p->second.
            for (SetStr::iterator pSql = p->second->begin();
                 pSql != p->second->end(); ++pSql) {
                string s = *pSql;
                conn_->execSql(s);
                cout << "Executing SQL: " << s << endl;
            }

            conn_->endTxn();
            delete p->second;
        }

        log_.clear();
    }

    void purge()
    {
        for (MapStrSetStr::iterator p = log_.begin();
             p != log_.end(); ++p)
            delete p->second;

        log_.clear();
    }
    // ...

private:
    MapStrSetStr log_;
    DBConn* conn_;
};
