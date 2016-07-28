#include <boost/scoped_array.hpp>
#include <fstream>

typedef unsigned char byte;

byte *img_rotate(byte*, size_t size, double angle)
{
    return nullptr;
}

byte *rotateImage(const std::string &imgFile, double angle,
                  size_t& sz)
{
    // open the file for reading
    std::ifstream imgStrm(imgFile, std::ios::binary);

    if (imgStrm) {
        imgStrm.seekg(0, std::ios::end);
        sz = imgStrm.tellg();            // determine file size
        imgStrm.seekg(0);
        // allocate buffer and read
        boost::scoped_array<byte> img(new byte[sz]);
        // read the image contents
        imgStrm.read(reinterpret_cast<char*>(img.get()), sz);
        byte first = img[0];  // indexed access
        return img_rotate(img.get(), sz, angle);
    }

    sz = 0;
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc > 1) {
        size_t size = 0;
        rotateImage(argv[1], 3.14159259 / 2, size);
    }
}
