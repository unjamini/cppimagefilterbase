#include <iostream>
#include "png_toolkit.h"
#include "filterSeq.h"

int main( int argc, char *argv[] )
{
    // toolkit filter_name base_pic_name sudent_tool student_pic_name limitPix limitMSE
    // toolkit near test images!

    try
    {
        if (argc != 4)
            throw "Not enough arguments";

        png_image img;
        img.load(argv[2]);
        FilterSeq fltSeq(argv[1]);
        fltSeq.applyFilters(img);
        img.save(argv[3]);
    }
    catch (const char *str)
    {
        std::cout << "Error: " << str << std::endl;
        return 1;
    }
    return 0;
}
