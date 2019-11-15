#include <iostream>
#include "png_toolkit.h"
#include "filterSeq.h"

int main( int argc, char *argv[] )
{
    // toolkit filter_name base_pic_name sudent_tool student_pic_name limitPix limitMSE
    // toolkit near test images!
    std::string configName = argv[1];
    std::string fileIn = argv[2];
    std::string fileOut = argv[3];
    png_image img;
    img.load(fileIn);
    FilterSeq fltSeq(configName);
    fltSeq.applyFilters(img);
    img.save(fileOut);
    return 0;
}
