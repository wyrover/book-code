 1 bool byDescendingISBN(const Book& lhs, const Book& rhs)
 2 {  return lhs.isbn > rhs.isbn; }
 3 
 4 ...
 5 std::vector<Book> books;
 6 ...
 7 std::sort(books.begin(), books.end(), byDescendingISBN);

