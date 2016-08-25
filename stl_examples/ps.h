// ps.h
// File used in "Anagram Finding" examples in Chapters 13 & 14.
struct PS : public pair<vector<char>, string> { 
  PS() : pair<vector<char>,string>(vector<char>(), string()) 
    { }

  PS(const string& s) : pair<vector<char>, string>(s, s) {
    sort(first.begin(), first.end());
  }
 
  operator string() const { return second; }
};

struct FirstLess : public binary_function<PS, PS, bool> {
  bool operator()(const PS& p, const PS& q) const 
  { 
    return p.first < q.first; 
  }
} firstLess;

struct FirstEqual : public binary_function<PS, PS, bool> {
  bool operator()(const PS& p, const PS& q) const 
  { 
    return p.first == q.first; 
  }
} firstEqual;

