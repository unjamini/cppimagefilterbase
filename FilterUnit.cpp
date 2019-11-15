//
// Created by Evgenia on 10.11.2019.
//

#include <iostream>
#include "FilterUnit.h"


image_intn Intensity(image_data& imgData)
{
    image_intn intensity;
    intensity.intn_val = new int[imgData.h * imgData.w];
    for (int i = 0; i < imgData.h ; ++i) {
        for (int j = 0; j < imgData.w; ++j) {
            double r = imgData.pixels[(i * imgData.w + j) * imgData.compPerPixel];
            double g = imgData.pixels[(i * imgData.w + j) * imgData.compPerPixel + 1];
            double b = imgData.pixels[(i * imgData.w + j) * imgData.compPerPixel + 2];
            intensity.intn_val[i * imgData.w + j] = floor(0.3 * r + 0.6 * g + 0.1 * b);
        }
    }
    return intensity;
}


void BW (image_data& imgData) {
    for (int i = 0; i < imgData.h ; ++i) {
        for (int j = 0; j < imgData.w; ++j) {
            int coord = (i * imgData.w + j) * imgData.compPerPixel;
            int mean = ceil(imgData.pixels[coord] + imgData.pixels[coord + 1] + imgData.pixels[coord + 2] / 3);
            imgData.pixels[coord] = imgData.pixels[coord + 1] = imgData.pixels[coord + 2] = mean;
        }
    }
}



void conv (image_data& imgData, float** kernel) {
    
}

FilterUnit::FilterUnit(int top, int bottom, int left, int right)
{
    top_p = top;
    bottom_p = bottom;
    left_p = left;
    right_p = right;
}


void RedFilter::applyFilter(image_data imgData)
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

int sum3_3 (stbi_uc* pixels, int x, int y, image_data& imgData, int channel) {
    int result = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result += pixels[((x + i) * imgData.w + y + j) * imgData.compPerPixel  + channel];
        }
    }
    return result / 9;
}


int sum2_2 (stbi_uc* pixels, int x, int y, image_data& imgData, int channel) {
    int result = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result += pixels[((x + i) * imgData.w + y + j) * imgData.compPerPixel  + channel];
        }
    }
    return result / 4;
}


int sum2_3 (stbi_uc* pixels, int x, int y, image_data& imgData, int channel) {
    int result = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            result += pixels[((x + i) * imgData.w + y + j) * imgData.compPerPixel  + channel];
        }
    }
    return result / 6;
}


int sum3_2 (stbi_uc* pixels, int x, int y, image_data& imgData, int channel) {
    int result = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            result += pixels[((x + i) * imgData.w + y + j) * imgData.compPerPixel  + channel];
        }
    }
    return result / 6;
}

void BlurFilter::applyFilter(image_data imgData)
{
    int top = top_p? imgData.h / top_p : 0;
    int bottom = bottom_p? imgData.h / bottom_p : 0;
    int left = left_p? imgData.w / left_p : 0;
    int right = right_p? imgData.w / right_p : 0;
    auto* nPixels = new stbi_uc[imgData.h * imgData.w * imgData.compPerPixel];
    memcpy(nPixels, imgData.pixels, imgData.h * imgData.w * imgData.compPerPixel);
    for (int channel = 0; channel < 3; ++channel) {
        //заполняем внутренние
        for (int i = top; i < bottom - 2; ++i) {
            for (int j = left; j < right - 2; ++j) {
                imgData.pixels[((i + 1) * imgData.w + j + 1) * imgData.compPerPixel  + channel] = sum3_3(nPixels, i, j, imgData, channel);
            }
        }

        //заполняем края (4 краевых пикселя)
        imgData.pixels[(top * imgData.w + left) * imgData.compPerPixel  + channel] = sum2_2(nPixels, top, left, imgData, channel);
        imgData.pixels[((bottom - 1) * imgData.w + left) * imgData.compPerPixel  + channel] = sum2_2(nPixels, bottom - 2, left, imgData, channel);
        imgData.pixels[(top * imgData.w + right - 1) * imgData.compPerPixel  + channel] = sum2_2(nPixels, top, right - 2, imgData, channel);
        imgData.pixels[((bottom - 1) * imgData.w + right - 1) * imgData.compPerPixel  + channel] = sum2_2(nPixels, bottom - 2, right - 2, imgData, channel);

        //верхняя и нижняя полоса
        for (int j = left + 1; j < right - 1; j++){
            imgData.pixels[(top * imgData.w + j) * imgData.compPerPixel  + channel] = sum2_3(nPixels, top, j, imgData, channel);
            imgData.pixels[((bottom - 1) * imgData.w + j) * imgData.compPerPixel  + channel] = sum2_3(nPixels, bottom - 2, j, imgData, channel);
        }
        //левая и правая полоса
        for (int i = top + 1; i < bottom - 1; i++ ) {
            imgData.pixels[(i * imgData.w + left) * imgData.compPerPixel  + channel] = sum3_2(nPixels, i, left, imgData, channel);
            imgData.pixels[(i * imgData.w + right - 1) * imgData.compPerPixel  + channel] = sum3_2(nPixels, i, right - 2, imgData, channel);
        }
    }
    delete[] nPixels;
}


void ThresholdFilter::applyFilter(image_data imgData)
{

}

void EdgeFilter::applyFilter(image_data imgData)
{

}