//
// Created by Evgenia on 10.11.2019.
//

#include <iostream>
#include <vector>
#include<functional>
#include "FilterUnit.h"




void BW (image_data& imgData, int top, int bottom, int left, int right) {
    for (int i = top; i < bottom ; ++i) {
        for (int j = left; j < right; ++j) {
            int coord = (i * imgData.w + j) * imgData.compPerPixel;
            int mean = floor((3 * imgData.pixels[coord] + 6 * imgData.pixels[coord + 1] + imgData.pixels[coord + 2]) / 10);
            imgData.pixels[coord] = imgData.pixels[coord + 1] = imgData.pixels[coord + 2] = mean;
        }
    }
}



FilterUnit::FilterUnit(int top, int bottom, int left, int right)
{
    top_p = top;
    bottom_p = bottom;
    left_p = left;
    right_p = right;
}

FilterUnit::~FilterUnit() = default;


void RedFilter::applyFilter(image_data& imgData)
{
    int top = top_p? imgData.h / top_p : 0;
    int bottom = bottom_p? imgData.h / bottom_p : 0;
    int left = left_p? imgData.w / left_p : 0;
    int right = right_p? imgData.w / right_p : 0;
    for (int i = top; i < bottom ; ++i) {
        for (int j = left; j < right; ++j) {
            for (int channel = 0; channel < imgData.compPerPixel; ++channel) {
                imgData.pixels[(i * imgData.w + j) * imgData.compPerPixel  + channel] = (channel && channel != 3)? 0x00 : 0xFF;
            }
        }
    }

}

int sum3_3 (struct Zone& zone, stbi_uc const* pixels, int x, int y, image_data& imgData, int channel, int central_w = 1, int border_w = 1, int kern_size=3) {
    int border = kern_size / 2;
    int result = 0;
    for (int i = x - border; i <= x + border; i++) {
        for (int j = y - border; j <= y + border; j++) {
            if (i < zone.top || i >= zone.bottom || j < zone.left || j >= zone.right)
                continue;
            int coeff = (i == x && j == y)? central_w : border_w;
            result += coeff * pixels[(i * imgData.w + j) * imgData.compPerPixel  + channel];
        }
    }
    return result;
}




void BlurFilter::applyFilter(image_data& imgData)
{
    int top = top_p? imgData.h / top_p : 0;
    int bottom = bottom_p? imgData.h / bottom_p : 0;
    int left = left_p? imgData.w / left_p : 0;
    int right = right_p? imgData.w / right_p : 0;
    auto* nPixels = new stbi_uc[imgData.h * imgData.w * imgData.compPerPixel];
    memcpy(nPixels, imgData.pixels, imgData.h * imgData.w * imgData.compPerPixel);
    for (int channel = 0; channel < 3; ++channel) {
        //заполняем внутренние
        for (int i = top; i < bottom; ++i) {
            for (int j = left; j < right; ++j) {
                Zone zone = {top, bottom, left, right};
                imgData.pixels[(i * imgData.w + j) * imgData.compPerPixel  + channel] = sum3_3(zone, nPixels, i, j, imgData, channel) / 9;
            }
        }
    }
    delete[] nPixels;
}


void ThresProcess(int top, int left, image_data& imgData, int zone_bottom, int zone_right)
{
    std::vector<stbi_uc*> pixel_v;
    stbi_uc* pixels = imgData.pixels;
    for (int i = top; i < fmin(zone_bottom, top + 5); i++) {
        for (int j = left; j < fmin(zone_right, left + 5); j++) {
            pixel_v.push_back(&pixels[(i * imgData.w + j) * imgData.compPerPixel]);
        }
    }
    std::sort(pixel_v.begin(), pixel_v.end(), [](const stbi_uc* a, const stbi_uc* b) -> bool
    {
        return *a < *b;
    });
    for (int i = 0; i < int(pixel_v.size() - 13); i++) {
        *pixel_v[i] = 0;
    }
    for (int i = top; i < fmin(zone_bottom, top + 5); i++) {
        for (int j = left; j < fmin(zone_right, left + 5); j++) {
            pixels[(i * imgData.w + j) * imgData.compPerPixel + 1] = pixels[(i * imgData.w + j) * imgData.compPerPixel + 2] = pixels[(i * imgData.w + j) * imgData.compPerPixel];
        }
    }
}



void ThresholdFilter::applyFilter(image_data& imgData)
{
    int top = top_p? imgData.h / top_p : 0;
    int bottom = bottom_p? imgData.h / bottom_p : 0;
    int left = left_p? imgData.w / left_p : 0;
    int right = right_p? imgData.w / right_p : 0;
    BW(imgData, top, bottom, left, right);
    int h_n = (bottom - top) / 5;
    int w_n = (right - left) / 5;
    for (int h_counter = 0; h_counter < h_n + 1; h_counter++) {
        for (int w_counter = 0; w_counter < w_n + 1; w_counter++) {
            ThresProcess(top + 5 * h_counter, left + 5 * w_counter, imgData, bottom, right);
        }
    }
}



void EdgeFilter::applyFilter(image_data& imgData)
{
    int top = top_p? imgData.h / top_p : 0;
    int bottom = bottom_p? imgData.h / bottom_p : 0;
    int left = left_p? imgData.w / left_p : 0;
    int right = right_p? imgData.w / right_p : 0;
    BW(imgData, top, bottom, left, right);
    auto* nPixels = new stbi_uc[imgData.h * imgData.w * imgData.compPerPixel];
    memcpy(nPixels, imgData.pixels, imgData.h * imgData.w * imgData.compPerPixel);
    for (int channel = 0; channel < 3; ++channel) {
        //заполняем внутренние
        for (int i = top; i < bottom; ++i) {
            for (int j = left; j < right; ++j) {
                Zone zone = {top, bottom, left, right};
                imgData.pixels[(i * imgData.w + j) * imgData.compPerPixel  + channel] = fmax(0, fmin(255, sum3_3(zone, nPixels, i, j, imgData, channel, 9, -1)));
            }
        }
    }
    delete[] nPixels;
}