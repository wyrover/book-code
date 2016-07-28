#include <boost/array.hpp>
#include <boost/flyweight.hpp>
#include <boost/flyweight/key_value.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace boost::flyweights;

class texture
{
public:
	texture(const std::string& filename) : filename(filename)
	{
		std::cout << "loaded " << filename << " file" << std::endl;
	}

	texture(const texture& x) : filename(x.filename)
	{
		std::cout << "texture[" << filename << "] copied" << std::endl;
	}

	~texture()
	{
		std::cout << "unloaded " << filename << std::endl;
	}

	const std::string& get_filename() const { return filename; }

private:
	std::string filename;
};

struct texture_filename_extractor
{
	const std::string& operator()(const texture& x) const {
		return x.get_filename();
	}
};

typedef flyweight<
	key_value<std::string, texture, texture_filename_extractor>
> texture_flyweight;

int main()
{
	const char* texture_filenames[] = {
		"grass.texture", "sand.texture", "water.texture", "wood.texture",
		"granite.texture", "cotton.texture", "concrete.texture", "carpet.texture"
	};

	const int num_texture_filenames = sizeof(texture_filenames) / sizeof(texture_filenames[0]);

	std::cout << "creating flyweights...\n" << std::endl;

	std::vector<texture_flyweight> textures;
	for (int i = 0; i < 50000; ++i) {
		textures.push_back(texture_flyweight(texture_filenames[std::rand()%num_texture_filenames]));
	}

	for (int j = 0; j < 50000; ++j) {
		textures.push_back(texture_flyweight(textures[std::rand()%textures.size()].get()));
	}

	std::cout << "\n" << textures.size() << " flyweights created\n" << std::endl;

	return 0;
}

