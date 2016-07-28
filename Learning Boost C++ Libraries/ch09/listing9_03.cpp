#include <boost/filesystem.hpp>
#include <iostream>
#include <cassert>
namespace fs = boost::filesystem;

void printPathParts(const fs::path& p1)
{
    std::cout << "For path: " << p1.string() << '\n';

    if (p1.is_relative()) {
        std::cout << "\tPath is relative\n";
    } else {
        assert(p1.is_absolute());
        std::cout << "\tPath is absolute\n";
    }

    if (p1.has_root_name())
        std::cout << "Root name: "
                  << p1.root_name().string() << '\n';

    if (p1.has_root_directory())
        std::cout << "Root directory: "
                  << p1.root_directory().string() << '\n';

    if (p1.has_root_path())
        std::cout << "Root path: "
                  << p1.root_path().string() << '\n';

    if (p1.has_parent_path())
        std::cout << "Parent path: "
                  << p1.parent_path().string() << '\n';

    if (p1.has_relative_path())
        std::cout << "Relative path: "
                  << p1.relative_path().string() << '\n';

    if (p1.has_filename())
        std::cout << "File name: "
                  << p1.filename().string() << '\n';

    if (p1.has_extension())
        std::cout << "Extension: "
                  << p1.extension().string() << '\n';

    if (p1.has_stem())
        std::cout << "Stem: " << p1.stem().string() << '\n';

    std::cout << '\n';
}

int main()
{
    printPathParts("");                     // no components
    printPathParts("E:\\DATA\\books.txt");  // all components
    printPathParts("/root/favs.txt");       // no root name
    printPathParts("\\DATA\\books.txt");    // Windows, relative
    printPathParts("boost");               // no rootdir, no extn
    printPathParts(".boost");               // no stem, only extn
    printPathParts("..");                   // no extension
    printPathParts(".");                    // no extension
    printPathParts("/opt/boost/");          // file name == .
}

