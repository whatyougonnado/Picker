#pragma once

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <stb/stb_image_resize.h>
#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <array>
#include <map>

#include <algorithm> // for_each

#include <algorithm>
#include <GeneralMesh.h>

#include <fstream>
#include <sstream>

using namespace std;

class PictureComparison {
public:
	PictureComparison(int face_size, const std::string & tex_pic_name, const std::string & tex_faceid_name);
	~PictureComparison();

	static std::vector<std::string> ssplit(std::string input, char criteria = ' ');
	void setColorTable(const std::string& full_path);
	int decodeNormalizedNumeralSystem(glm::ivec3 val, int from = 255); // base is decimal
	void computeIdColor();

	vector<string> getFaceColor() { return face_color_; }
	map<string, array<unsigned char, 3>> getColorTable() const { return color_table_; };
	map<string, array<float, 3>> getColorTableDouble() const;

private:
	int face_size_;
	int sort_size_;
	GeneralMesh::TextureInfo tex_picture_, tex_faceid_;

	map<string, array<unsigned char, 3>> color_table_;
	
	int* id_table_;
	vector<map<string, int>> face_color_counter_;
	vector<string> face_color_;

	std::string getSynchronizedColorName_(unsigned char r, unsigned char g, unsigned char b);

	void initFaceColorTable_();
	void extractFaceColor_();
};