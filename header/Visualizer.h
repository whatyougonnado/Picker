#pragma once

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <array>

#include <PictureComparison.h>

#include <GeneralUtility.h>

class Visualizer {
public:
    Visualizer();
    Visualizer(const std::string& imagename);
    ~Visualizer();

    bool saveTexVisualize(const std::string & reference_file_path, const std::string& save_imagename);
    int setTexData(const std::string& full_path);

    //get table from 8bit png file
    void setReferenceTable_(const std::string& full_path);

private:
    std::shared_ptr<mg::ImageInfo> image_info_;

    void init_();

    //if line is all black, consider line as padding line
    void changeColor_();

    //imageinfo after
    void setTexInfo_();

    std::vector<std::array<int,3> > table_;
};