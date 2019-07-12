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
	~BoundingBox();

	//set texture of the Picker
	//true: can read and set 'imagename'
	//false: cannot read 'imagename'
	bool setTex(const std::string& imagename);
	bool checkFileExist_(const char * filename);
	int saveModifiedTex(const std::string& save_imagename);
	//getModifiedTex_();
	void removePadding_(int min_margin_left = 0, int min_margin_right = 0, int min_margin_top = 0, int min_margin_bottom = 0);
private:
	std::shared_ptr<ImageInfo> image_info_;
};