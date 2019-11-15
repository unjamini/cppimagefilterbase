#ifndef STUDENT_TOOLKIT_FILTERSEQ_H
#define STUDENT_TOOLKIT_FILTERSEQ_H


#include <string>
#include <vector>
#include "png_toolkit.h"
#include "FilterUnit.h"


class FilterSeq
{
public:
    explicit FilterSeq(std::string config) ;
    void applyFilters(png_image& pic);

private:
    void parseConfig(std::string const &configName);
    std::vector<FilterUnit*> filterSeq;
};


#endif //STUDENT_TOOLKIT_FILTERSEQ_H
