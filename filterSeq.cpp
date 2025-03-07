//
// Created by Evgenia on 03.11.2019.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include "filterSeq.h"
#include "png_toolkit.h"

using namespace std;


//не забыть почистить память



FilterSeq::FilterSeq(std::string config)
{
    parseConfig(config);
}




void FilterSeq::parseConfig(std::string const &configName)
{
    std::string line;
    std::ifstream confStream(configName);
    if (confStream.is_open()) {
        while (getline(confStream, line)) {
            if (!line.empty()) {
                std::istringstream iss(line);
                std::vector<std::string> parsed_line((std::istream_iterator<std::string>(iss)),
                                                     std::istream_iterator<std::string>());
                if (parsed_line.size() < 5)
                    continue;
                if (parsed_line[0] == "Red") {
                    FilterUnit* flt = new RedFilter(stoi(parsed_line[1]), stoi(parsed_line[3]), stoi(parsed_line[2]), stoi(parsed_line[4]));
                    filterSeq.push_back(flt);
                }
                else if (parsed_line[0] == "Threshold") {
                    FilterUnit* flt = new ThresholdFilter(stoi(parsed_line[1]), stoi(parsed_line[3]), stoi(parsed_line[2]), stoi(parsed_line[4]));
                    filterSeq.push_back(flt);
                }
                else if (parsed_line[0] == "Edge") {
                    FilterUnit* flt = new EdgeFilter(stoi(parsed_line[1]), stoi(parsed_line[3]), stoi(parsed_line[2]), stoi(parsed_line[4]));
                    filterSeq.push_back(flt);
                }
                else if (parsed_line[0] == "Blur") {
                    FilterUnit* flt = new BlurFilter(stoi(parsed_line[1]), stoi(parsed_line[3]), stoi(parsed_line[2]), stoi(parsed_line[4]));
                    filterSeq.push_back(flt);
                }
            }
        }
        confStream.close();
    }
}


void FilterSeq::applyFilters(png_image &pic)
{
    image_data imgData = pic.getPixelData();
    for (FilterUnit* filter: filterSeq) {
        filter->applyFilter(imgData);
    }
    pic.set_data(imgData);
}

FilterSeq::~FilterSeq()
{
    for (FilterUnit* filter: filterSeq) {
        delete filter;
    }
}
