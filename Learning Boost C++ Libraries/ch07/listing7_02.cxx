 1 ...
 2 struct CompareBooks
 3 {
 4   bool operator()(const Book& b1, const Book& b2) const {
 5     return (b1.author < b2.author)
 6            || (b1.author == b2.author 
 7                && b1.title < b2.title);
 8   }
 9 };
10
11 ...
12 std::vector<Book> books;
13 ...
14 std::sort(books.begin(), books.end(), CompareBooks());

