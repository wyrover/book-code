#include <vector>
#include <string>
#include <boost/thread.hpp>
#include <boost/optional.hpp>
#include <boost/unordered/unordered_map.hpp>

struct book_t {
    std::string title;
    std::string author;
    int edition;
};

class LibraryCatalog
{
public:
    typedef boost::unordered_map<std::string, book_t> map_type;
    typedef std::vector<book_t> booklist_t;

    boost::optional<book_t> find_book(const std::string& title)
    const
    {
        boost::shared_lock<boost::shared_mutex> rdlock(mtx);
        auto it = catalog.find(title);

        if (it != catalog.end()) {
            return it->second;
        }

        rdlock.unlock();
        return boost::none;
    }

    booklist_t find_books(const std::vector<std::string>& titles)
    const
    {
        booklist_t result;

        for (auto title : titles) {
            auto book = find_book(title);

            if (book) {
                result.push_back(book.get());
            }
        }

        return result;
    }

    bool add_book(const book_t& book)
    {
        boost::unique_lock<boost::shared_mutex> wrlock(mtx);
        auto it = catalog.find(book.title);

        if (it == catalog.end()) {
            catalog[book.title] = book;
            return true;
        } else if (it->second.edition < book.edition) {
            it->second = book;
            return true;
        }

        return false;
    }

    bool remove_book(const std::string& title)
    {
        boost::unique_lock<boost::shared_mutex> wrlock(mtx);
        return catalog.erase(title);
    }

private:
    map_type catalog;
    mutable boost::shared_mutex mtx;
};

int main()
{
}
