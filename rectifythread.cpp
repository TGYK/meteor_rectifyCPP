#include "rectifythread.h"

void RectifyThread::run(){
    //Make temporary working img to push pixels onto
    QRgb start_pixel, end_pixel, working_pixel;
    working_pixel = 0;
    int column_rectified;
    double target_column;
    int delta;
    unsigned int working_color= 0;
    unsigned int start_color = 0;
    unsigned int end_color = 0;
    //First pass, center to right side
    for(int row = start_row; row < end_row; row++){
        //Push R, G, B onto the start_pixel vector from original_pixels at halfway point of original image
        start_pixel = original_pixels.pixel((original_pixels.width() / 2), row);
        //Calculate the current working column at halfway point of rectified image
        column_rectified = static_cast<int>(rectified_width / 2);
        //Calculate the target of widening
        target_column = column_rectified;
        for(int column_original = static_cast<int>((original_pixels.width() / 2)); column_original < original_pixels.width(); column_original++){ //From center to the right edge of the original picture
            target_column += correction_factor[column_original]; //Add correction factor for column
            end_pixel = original_pixels.pixel(column_original, row);
            delta = static_cast<int>(target_column) - column_rectified; //Calculate the difference between the target column and current column
            for(int i = 0; i < delta; i++){ //For each pixel between the current column of the original and the target column of the rectified..
                working_color = (unsigned int)start_pixel >> 24; //Alpha value.. We don't mess with transparency, so we'll use the original
                working_pixel = working_pixel | working_color; // Shift into the pixel ARGB value

                start_color = (unsigned int)start_pixel >> 16 & 255; //Red color value of start pixel
                end_color = (unsigned int)end_pixel >> 16 & 255; //Red color value of end pixel
                working_color = ((start_color * (delta - i) + end_color * i) / delta); // Linearly interpolate red value
                working_pixel = working_pixel << 8 | working_color; // Shift into the pixel ARGB value

                start_color = (unsigned int)start_pixel >> 8 & 255; //Green color value of start pixel
                end_color = (unsigned int)end_pixel >> 8 & 255; //Green color value of end pixel
                working_color = ((start_color * (delta - i) + end_color * i) / delta); // Linearly interpolate green value
                working_pixel = working_pixel << 8 | working_color; // Shift into the pixel ARGB value

                start_color = (unsigned int)start_pixel & 255; //Blue color value of start pixel
                end_color = (unsigned int)end_pixel & 255; //Blue color value of end pixel
                working_color = ((start_color * (delta - i) + end_color * i) / delta); // Linearly interpolate blue value
                working_pixel = working_pixel << 8 | working_color; // Shift into the pixel ARGB value

                rectified_pixels->setPixel(column_rectified, row, working_pixel);

                column_rectified++;
            }
            start_pixel = end_pixel;
        }
        //Second pass, center to left side
        start_pixel = original_pixels.pixel((original_pixels.width() / 2), row);
        column_rectified = static_cast<int>(rectified_width / 2);
        target_column = column_rectified;
        for(int column_original = static_cast<int>(original_pixels.width() / 2) -1; column_original > -1; column_original--){ //From center to left edge of the original picture
            target_column -= correction_factor[column_original]; //Remove correction factor for column
            end_pixel = original_pixels.pixel(column_original, row);
            delta = column_rectified - static_cast<int>(target_column);
            for(int i = 0; i < delta; i++){
                working_color = (unsigned int)start_pixel >> 24; //Alpha value.. We don't mess with transparency, so we'll use the original
                working_pixel = working_pixel | working_color; // Shift into the pixel ARGB value

                start_color = (unsigned int)start_pixel >> 16 & 255; //Red color value of start pixel
                end_color = (unsigned int)end_pixel >> 16 & 255; //Red color value of end pixel
                working_color = ((start_color * (delta - i) + end_color * i) / delta); // Linearly interpolate red value
                working_pixel = working_pixel << 8 | working_color; // Shift into the pixel ARGB value

                start_color = (unsigned int)start_pixel >> 8 & 255; //Green color value of start pixel
                end_color = (unsigned int)end_pixel >> 8 & 255; //Green color value of end pixel
                working_color = ((start_color * (delta - i) + end_color * i) / delta); // Linearly interpolate green value
                working_pixel = working_pixel << 8 | working_color; // Shift into the pixel ARGB value

                start_color = (unsigned int)start_pixel & 255; //Blue color value of start pixel
                end_color = (unsigned int)end_pixel & 255; //Blue color value of end pixel
                working_color = ((start_color * (delta - i) + end_color * i) / delta); // Linearly interpolate blue value
                working_pixel = working_pixel << 8 | working_color; // Shift into the pixel ARGB value
                rectified_pixels->setPixel(column_rectified, row, working_pixel);
                column_rectified--;
            }
            start_pixel = end_pixel;
        }
    }
    return;
}
