#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <array>
#include "stb_image_write.h"
#include "png_toolkit.h"

png_toolkit::png_toolkit()
{
}

png_toolkit::~png_toolkit()
{
    stbi_image_free(imgData.pixels);
}

bool png_toolkit::load( const std::string &pictureName )
{
    imgData.pixels = stbi_load(pictureName.c_str(), &imgData.w, &imgData.h, &imgData.compPerPixel, 0);
    return imgData.pixels != nullptr;
}

bool png_toolkit::save( const std::string &pictureName )
{
    return stbi_write_png(pictureName.c_str(),
                   imgData.w, imgData.h,
                   imgData.compPerPixel,
                          imgData.pixels, 0) != 0;
}


void png_toolkit::halfRedFilter()
{
    for (int i = imgData.h / 2; i < imgData.h ; i++) {
            for (int j = 0; j < imgData.w; j++) {
                for (int channel = 0; channel < imgData.compPerPixel; channel++) {
                imgData.pixels[(i * imgData.w + j) * imgData.compPerPixel  + channel] = channel? 0x00 : 0xFF;
            }
        }
    }

}

image_data png_toolkit::getPixelData( void ) const
{
    return imgData;
}
