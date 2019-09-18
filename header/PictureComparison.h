#pragma once

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <array>

#include <algorithm>
#include <GeneralMesh/GeneralMesh.h>

using namespace std;

class PictureComparison {
public:
	PictureComparison(int face_size, const std::string & tex_pic_name, const std::string & tex_faceid_name);
	~PictureComparison();
private:
	void setColorTable_();
	void setIdTable_();
	int decodeNormalizedNumeralSystem(glm::vec3 val, int from); // base is decimal
	int face_size_;
	GeneralMesh::TextureInfo tex_picture_, tex_faceid_;
	vector<array<float, 3>> color_table_;

	int* id_table_;
	int** face_color_table_;
};