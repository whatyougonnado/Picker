#include "PictureComparison.h"

PictureComparison::PictureComparison(int face_size, const std::string & tex_pic_name, const std::string & tex_faceid_name): face_size_(face_size)
{
	GeneralMesh myGeneralMesh;
	myGeneralMesh.readTex(tex_picture_, tex_pic_name);
	myGeneralMesh.readTex(tex_faceid_, tex_faceid_name);

	//if ((tex_picture_.width != tex_faceid_.width) || (tex_picture_.height != tex_faceid_.height)) {
	//	std::cout << "PictureComparison( ... ) has different size pictures" << std::endl;
	//	exit(1);
	//}

	face_color_table_ = new int*[face_size];

	for (auto k = 0; k < face_size; ++k) {
		face_color_table_[k] = new int[21];
		for (auto l = 0; l < 21; ++l) {
			face_color_table_[k][l] = 0;
		}
	}

	setColorTable_();
}

PictureComparison::~PictureComparison() {
	for (auto k = 0; k < face_size_; ++k) {
		delete[] face_color_table_[k];
	}

	delete[] face_color_table_;
}

void PictureComparison::setColorTable_()
{
	int full_size = tex_picture_.width * tex_picture_.height * 3; //r,g,b == 3
	for (int k = 0; k < full_size; k += 3) {
		////face id
		//if (tex_faceid_.data[k + 0] + tex_faceid_.data[k + 1] + tex_faceid_.data[k + 2] > 0) {
		//	std::cout << decodeNormalizedNumeralSystem(glm::vec3{ tex_faceid_.data[k + 0] ,tex_faceid_.data[k + 1],tex_faceid_.data[k + 2] }, 255) << std::endl;

		//}
		int cnt = 0;
		float r, g, b;
		r = tex_picture_.data[k + 0];
		g = tex_picture_.data[k + 1];
		b = tex_picture_.data[k + 2];

		bool alreadyExist = false;

		for (vector<array<float, 3>>::iterator& iter = color_table_.begin(); iter != color_table_.end(); ++iter) {
			if (((*iter)[0] == r) && ((*iter)[1] == g) && ((*iter)[2] == b)) {
				alreadyExist = true;
				break;
			}
		}
		if (!alreadyExist) {
			std::cout << "pixel: " << k / 1024 << ", " << k % 1024 << std::endl;
			std::cout << r << ", " << g << ", " << b << std::endl;
			color_table_.push_back(array<float, 3>{r, g, b});
		}
	}
}

void PictureComparison::setIdTable_() {

}

int PictureComparison::decodeNormalizedNumeralSystem(glm::vec3 val, int from)
{
	int id;
	id = val[0] * from * from + val[1] * from + val[2];

	return id;
}

  