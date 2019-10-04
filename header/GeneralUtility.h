#pragma once


#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include <string>
#include <array>
#include <vector>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include <direct.h>//WINDOWS HEADER

namespace mg {
    enum ImageExtension {
        BMP,
        JPG,
        PNG,
        UNKNOWN
    };

    struct ImageInfo {
        //image to array: row first
        unsigned char *before_data;
        int before_width;
        int before_height;
        int before_n_channels;

        unsigned char *after_data;
        int after_width;
        int after_height;
        int after_n_channels;
    };
    //if full_path extension is included in ImageExtension,
    // output will be ImageExtension::BMP, ImageExtension::JPG or ImageExtension::PNG.
    //if not, output is ImageExtension::UNKNOWN
    ImageExtension getType(const std::string& full_path);
    //get tokenized output based on criteria(ex. space, colon etc.)
    std::vector<std::string> ssplit(std::string input, char criteria);
    //read from the first line
    std::vector< std::vector<std::string> > readCSV(const std::string& full_path);
    std::vector <std::string> readTxt(const std::string& full_path);
    bool saveTexData(std::shared_ptr<ImageInfo> image_info, const std::string& full_path);
    bool writeTxt(std::vector<std::string> cache_list, const std::string& full_path);
    //change only extension name, not change inner construction
    std::string convertExtension(std::string filename, std::string extension);


    //set texture of the BoundingBox
    //true: can read and set 'imagename'
    //false: cannot read 'imagename'
    bool checkFileExist(const char *filename);
    //recognize file using extension info.
    //ex)input: C:/a/b/c/mytextfile.txt
    //   output: C:/a/b/c/
    std::string getDir(std::string full_path);
    //can work on Windows version only
    void mkDir(std::string full_path);

}