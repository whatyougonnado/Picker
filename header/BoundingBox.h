#pragma once

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <array>

class BoundingBox {
public:
	struct Margin {
		std::array<int, 4> value_container;
		int& left = value_container[0];
		int& right = value_container[1];
		int& top = value_container[2];
		int& bottom = value_container[3];
	};

	//before --//eliminate margin//--> after
	struct ImageInfo {
		//image to array: row first
		unsigned char *before_data;
		int before_width;
		int before_height;
		int n_channels;

		unsigned char *after_data;
		int after_width;
		int after_height;
		Margin margin;
	};
	
	BoundingBox();
	BoundingBox(const std::string& imagename);
	~BoundingBox();

	int saveTexBounded(const std::string& save_imagename, bool isPNG=true);
	int setTexData(const std::string& imagename);

private:
	std::shared_ptr<ImageInfo> image_info_;

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
	int saveTexData_(const std::string& imagename);
};