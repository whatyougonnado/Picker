#include "PictureComparison.h"

PictureComparison::PictureComparison(int face_size, const std::string & tex_pic_name, const std::string & tex_faceid_name): face_size_(face_size)
{
	GeneralMesh myGeneralMesh;
	myGeneralMesh.readTex(tex_picture_, tex_pic_name);
	myGeneralMesh.readTex(tex_faceid_, tex_faceid_name);

	if ((tex_picture_.width != tex_faceid_.width) || (tex_picture_.height != tex_faceid_.height)) {
		std::cout << "PictureComparison( ... ) has different size pictures" << std::endl;
		exit(1);
	}

	face_color_table_ = new map<string, int>[face_size];
}

PictureComparison::~PictureComparison() {
	for (auto k = 0; k < face_size_; ++k) {
		delete[] face_color_table_;
	}

	delete[] face_color_table_;
}

std::vector<std::string> PictureComparison::ssplit(std::string input, char criteria) {

	std::vector<std::string> ret;
	std::istringstream ss(input);
	std::string token;

	while (std::getline(ss, token, criteria)) {
		ret.push_back(token);
	}

	return ret;
}

void PictureComparison::setColorTable(const std::string& full_path)
{
	std::vector<std::string> token_list;
	string in_line;
	ifstream in;
	in.open(full_path);
	
	while (getline(in,in_line)) {
		token_list = ssplit(in_line, ',');
		color_table_.insert(pair<string, array<unsigned char, 3>>{token_list[0], array<unsigned char, 3>{
			(unsigned char)atoi(token_list[1].c_str()), (unsigned char)atoi(token_list[2].c_str()), (unsigned char)atoi(token_list[3].c_str())}});
	}
	in.close();

	initFaceColorTable_();
}

int PictureComparison::decodeNormalizedNumeralSystem(glm::vec3 val, int from)
{
	int id;
	id = val[0] * from * from + val[1] * from + val[2];

	return id;
}

void PictureComparison::computeIdColor()
{
	std::string color_name;
	glm::vec3 color, encoded_id;
	int id, full_size;
	
	full_size = tex_picture_.width * tex_picture_.height * 3; //r,g,b == 3

	for (int k = 0; k < full_size; k += 3) {
		for (int l = 0; l < 3; ++l) {
			color[l] = tex_picture_.data[k + l];
			encoded_id[l] = tex_faceid_.data[k + l];
		}

		color_name = getSynchronizedColorName_(color[0], color[1], color[2]);
		
		try {
			if (color_name == "ERROR") {
				//cout << "void PictureComparison::computeIdColor() ERROR: There is color not included in color table" << endl;
				throw k;
			}
		}
		catch(int expn){
			//cout << "Error at " << expn << endl;
			//cout << (int)color[0] << ", " << (int)color[1] << ", " << (int)color[2] << endl;
			color_name = "unknown";
		}
		
		id = decodeNormalizedNumeralSystem(glm::vec3(encoded_id));
		++face_color_table_[id][color_name];

	}
}

std::string PictureComparison::getSynchronizedColorName_(unsigned char r, unsigned char g, unsigned char b)
{
	std::string ret;
	ret = "ERROR"; // default

	all_of(color_table_.begin(), color_table_.end(), [&](std::pair<string, array<unsigned char, 3>> color_chip) {
		auto& colors = color_chip.second;
		
		if ((colors[0] == r) && (colors[1] == g) && (colors[2] == b)) {
			ret = color_chip.first;
			return false;
		}
		return true;
	});

	return ret;
}

void PictureComparison::initFaceColorTable_()
{
	string color_name;

	for (auto k = 0; k < face_size_; ++k) {
		for_each(color_table_.begin(), color_table_.end(), [&](std::pair<string, array<unsigned char, 3>>&& color_chip) {
			auto& colors = color_chip.second;
			color_name = color_chip.first;
			face_color_table_[k].insert(pair<string, int>{color_name, 0});
			});
	}
}
