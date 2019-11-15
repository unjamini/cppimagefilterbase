#ifndef PNG_TOOLKIT_H
#define PNG_TOOLKIT_H

#include <string>
#include <map>
//#include <memory>
#include "stb_image.h"

struct image_data
{
    stbi_uc *pixels;
    int w, h;
    int compPerPixel;
};

struct image_intn
{
    int *intn_val;
    int w, h;
};


class png_image
{
public:
    enum class Error {
        WrongSize,
        WrongFormat,
        Ok
    };

    png_image();
    ~png_image();
    bool load( std::string const &pictureName );
    bool save( std::string const &pictureName );
    void set_data(image_data newData);
    image_data getPixelData( void ) const;

private:
    image_data imgData;
};

#endif // PNG_TOOLKIT_H
