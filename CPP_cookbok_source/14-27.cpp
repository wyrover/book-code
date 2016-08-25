#include <fstream>
#include <boost/archive/xml_oarchive.hpp> // Archive for writing XML
#include <boost/archive/xml_iarchive.hpp> // Archive for reading XML
#include <boost/serialization/vector.hpp> // machinery for serializing
#include "animal.hpp"                     // std::vector

int main()
{
    using namespace std;
    using namespace boost::archive;       // namespace for archives
    using namespace boost::serialization; // namespace for make_nvp

    try {
        // Populate list of animals
        vector<Animal> animalList;
        animalList.push_back(
            Animal("Herby", "elephant", "1992-04-23",
                   Contact("Dr. Hal Brown", "(801)595-9627"),
                   Contact("Bob Fisk", "(801)881-2260")));
        animalList.push_back(
            Animal("Sheldon", "parrot", "1998-09-30",
                   Contact("Dr. Kevin Wilson", "(801)466-6498"),
                   Contact("Eli Wendel", "(801)929-2506")));
        animalList.push_back(
            Animal("Dippy", "penguin", "2001-06-08",
                   Contact("Dr. Barbara Swayne", "(801)459-7746"),
                   Contact("Ben Waxman", "(801)882-3549")));
        // Construct XML output archive and serialize list
        ofstream       fout("animals.xml");
        xml_oarchive   oa(fout);
        oa << make_nvp("animalList", animalList);
        fout.close();
        // Construct XML intput archive and deserialize list
        ifstream        fin("animals.xml");
        xml_iarchive    ia(fin);
        vector<Animal>  animalListCopy;
        ia >> make_nvp("animalList", animalListCopy);
        fin.close();

        if (animalListCopy != animalList) {
            cout << "XML serialization failed\n";
            return EXIT_FAILURE;
        }
    } catch (const exception& e) {
        cout << e.what() << "\n";
        return EXIT_FAILURE;
    }
}