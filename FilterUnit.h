//
// Created by Evgenia on 10.11.2019.
//

#ifndef STUDENT_TOOLKIT_FILTERUNIT_H
#define STUDENT_TOOLKIT_FILTERUNIT_H


#include "png_toolkit.h"
#include <cmath>
#include <memory.h>
#include <cstring>
#include <algorithm>


struct Zone {
    int top;
    int bottom;
    int left;
    int right;
};



class FilterUnit {
public:
    FilterUnit(int top, int bottom, int left, int right);
    virtual void applyFilter(image_data& imgData) = 0;
    virtual ~FilterUnit();
protected:
    int top_p;
    int bottom_p;
    int left_p;
    int right_p;
};


class RedFilter: public FilterUnit{
public:
    RedFilter(int top, int bottom, int left, int right) : FilterUnit(top, bottom, left, right)  {};
    void applyFilter(image_data& imgData) override;
};



class BlurFilter: public FilterUnit{
public:
    BlurFilter(int top, int bottom, int left, int right) : FilterUnit(top, bottom, left, right)  {};
    void applyFilter(image_data& imgData) override;
};


class ThresholdFilter: public FilterUnit{
public:
    ThresholdFilter(int top, int bottom, int left, int right) : FilterUnit(top, bottom, left, right)  {};
    void applyFilter(image_data& imgData) override;
};



class EdgeFilter: public FilterUnit{
public:
    EdgeFilter(int top, int bottom, int left, int right) : FilterUnit(top, bottom, left, right)  {};
    void applyFilter(image_data& imgData) override;
};

#endif //STUDENT_TOOLKIT_FILTERUNIT_H
