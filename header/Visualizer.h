#pragma once

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <array>

#include <PictureComparison.h>

class Visualizer {
public:
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
	
	Visualizer();
	Visualizer(const std::string& imagename);
	~Visualizer();

	static ImageExtension getType(const std::string& full_path);
	static std::vector< std::vector<std::string> > readCSV(const std::string& full_path);
	static std::vector <std::string> readFile(const std::string& full_path);

	bool saveTexVisualize(const std::string & reference_file_path, const std::string& save_imagename);
	int setTexData(const std::string& full_path);

	//get table from 8bit png file
	void setReferenceTable_(const std::string& full_path);

private:
	std::shared_ptr<ImageInfo> image_info_;

	//set texture of the BoundingBox
	//true: can read and set 'imagename'
	//false: cannot read 'imagename'
	bool checkFileExist_(const char * filename);

	void init_();

	//if line is all black, consider line as padding line
	void changeColor_();

	//imageinfo before
	void setTexInfoBefore_();
	//imageinfo after
	void setTexInfoVisualize_();
	bool saveTexData_(const std::string& save_imagename);

	std::vector<array<int,3>> table_;
};