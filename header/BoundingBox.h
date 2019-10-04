#pragma once

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <array>

#include <GeneralUtility.h>

class BoundingBox {
public:
    struct Margin {
        std::array<int, 4> value_container;
        int& left = value_container[0];
        int& right = value_container[1];
        int& top = value_container[2];
        int& bottom = value_container[3];
    };
    //if you use default constructor, you should run setTexData(...)
    BoundingBox();
    BoundingBox(const std::string& imagename);
    ~BoundingBox();

    int saveTexBounded(const std::string& save_imagename);
    //if you want to change texture, use this function
    int setTexData(const std::string& imagename);

private:
    std::shared_ptr<mg::ImageInfo> image_info_;
    Margin margin_;
    //set texture of the BoundingBox
    //true: can read and set 'imagename'
    //false: cannot read 'imagename'
    bool checkFileExist_(const char * filename);

    void init_();

    //if line is all black, consider line as padding line
    void getMargin_(std::array<int, 3> lead_pixel);
    void removePadding_();

    //imageinfo before
    void setTexInfoBefore_();
    //imageinfo after
    void setTexInfoBounded_();
};