#include <Picking.h>

Picking::Picking(const Eigen::MatrixXd& verts, const Eigen::MatrixXi& faces): verts_(verts), faces_(faces){
	init_();
}
Picking::~Picking() {
}

void Picking::init_() {
	stbi_flip_vertically_on_write(false);
	stbi_set_flip_vertically_on_load(false);

	d_cache_.resize(faces_.rows());
}
void Picking::run(const std::string& full_path) {
	readPallet_(full_path, color_csv_full_path_);
	computeVisualizeFaces_();
	computeColorInfo_();
}
bool Picking::fileExist(const std::string& full_path) {
	std::ifstream f(full_path.c_str());
	if (f.good()) {
		f.close();
		return true;
	}
	else {
		f.close();
		return false;
	}
}

int Picking::makeTexture(const std::string& color_csv_full_path, const std::string& save_texture_full_path, bool overwrite) {
	std::vector<int> color;
	std::string line;
	std::ifstream in_file;

	if (fileExist(save_texture_full_path) && (!overwrite))
	{
		return 0;
	}

	in_file.open(color_csv_full_path);
	while (std::getline(in_file, line)) {
		unsigned int token_idx = 0;
		std::stringstream s_in_file;
		s_in_file.str(line);
		while (std::getline(s_in_file, line, ',')) {
			if ((line.size() > 0) && (line[0] == '#')) {
				break;
			}
			if (token_idx++ == 0) {
				continue;
			}
			color.push_back(std::stoi(line));
		}
	}
	in_file.close();

	const int WIDTH = 1024;
	const int HEIGHT = 1024;
	const int LINE_WIDTH = 40;
	const int CHANNEL_NUM = 3;
	uint8_t* data = new uint8_t[WIDTH * HEIGHT * 3];

	int cnt = 0;

	for (int k = 0; k < (color.size() / 3); ++k) {
		uint8_t r = color[k * 3 + 0];
		uint8_t g = color[k * 3 + 1];
		uint8_t b = color[k * 3 + 2];
		uint8_t color[3] = { r, g, b };
		for (int j = 0; j < WIDTH; ++j) {
			for (int i = k * LINE_WIDTH; i < (k + 1) * LINE_WIDTH; ++i) {
				for (int c = 0; c < 3; ++c) {
					data[i * WIDTH * 3 + j * 3 + c] = color[c];
					++cnt;
				}
			}
		}
	}

	stbi_write_png(save_texture_full_path.c_str(), WIDTH, HEIGHT, CHANNEL_NUM, data, WIDTH * CHANNEL_NUM);

	delete[] data;

	return 1;
}

void Picking::computeCenterVertex_(Eigen::MatrixXd& cache, const Eigen::MatrixXi& faces) {
	cache = Eigen::MatrixXd::Zero(faces_.rows(), 3);

	for (auto i = 0; i < cache.rows(); ++i) {
		int face_vert_idx[3] = {faces_(i,x), faces_(i,y), faces_(i,z)};
		for (auto k = 0; k < 3; ++k) {
			cache(i, x) += verts_(face_vert_idx[k], x);
			cache(i, y) += verts_(face_vert_idx[k], y);
			cache(i, z) += verts_(face_vert_idx[k], z);
		}
	}

	cache /= 3.0;
}

bool Picking::findIntersection(int main_face_idx) {
	double _z;
	Eigen::Vector2d _center;
	Eigen::Vector3d _bary;
	_center(0) = center_faces_(main_face_idx, 0);
	_center(1) = center_faces_(main_face_idx, 1);
	_z = center_faces_(main_face_idx, 2);

	//find more bigger z
	for (auto face_idx = 0; face_idx < faces_.rows(); ++face_idx) {
		//std::cout << "face_idx: " << face_idx << std::endl;
		if ((face_idx == main_face_idx) || (isFormer(_z, face_idx))) {
			continue;
		}

		_bary = computeBaryCentricCoord_(_center, face_idx);
		if (((0 < _bary(0)) && (_bary(0) < 1)) && ((0 < _bary(1)) && (_bary(1) < 1)) && ((0 < _bary(2)) && (_bary(2) < 1))) {
			return true;
		}
	}

	return false;
}

bool Picking::isFormer(double _z, int face_idx) {
	std::array<double, 3> _vert_z;
	//point a, b, c
	for (auto i = 0; i < 3; ++i) {
		_vert_z[i] = verts_(faces_(face_idx, i), 2);
	}
	if ((_z >= _vert_z[0]) && (_z >= _vert_z[1]) && (_z >= _vert_z[2])) {
		return true;
	}

	return false;
}

Eigen::Vector3d Picking::computeBaryCentricCoord_(Eigen::Vector2d input, int face_idx) {
	double elips = std::numeric_limits<double>::epsilon();
	std::string func_name = "Eigen::Vector2d Picking::computeBaryCentricCoord_(Eigen::Vector2d input, int face_idx)";
	Eigen::Vector3d bary;

	std::array<Eigen::Vector2d, 3> _vert;
	//point a, b, c
	for (auto i = 0; i < 3; ++i) {
		//x,y
		for (auto j = 0; j < 2; ++j) {
			_vert[i](j) = verts_(faces_(face_idx, i), j);
		}
	}

	Eigen::Vector2d v0, v1, v2;
	v0 = _vert[1] - _vert[0];
	v1 = _vert[2] - _vert[0];
	v2 = input - _vert[0];
	double d00 = v0.dot(v0);
	double d01 = v0.dot(v1);
	double d11 = v1.dot(v1);
	double d20 = v2.dot(v0);
	double d21 = v2.dot(v1);
	double denom = d00 * d11 - d01 * d01;
	
	if (( -elips <denom) && (denom <elips)) {
		//std::cout << func_name << ": ERROR: denom is zero" << std::endl;
		
		bary(1) = -1.0;
		bary(2) = -2.0;
		bary(0) = 1.0 - bary(1) - bary(2);

		return bary;
		//exit(1);
	}
	
	bary(1) = (d11 * d20 - d01 * d21) / denom;
	bary(2) = (d00 * d21 - d01 * d20) / denom;
	bary(0) = 1.0 - bary(1) - bary(2);

	return bary;
}

void Picking::computeVisualizeFaces_() {
	int cnt;

	computeCenterVertex_(center_faces_, faces_);
	
	visualize_faces_.resize(faces_.rows(), 3);

	cnt = 0;
	for (int face_idx = 0; face_idx < faces_.rows(); ++face_idx) {
		if (!findIntersection(face_idx)) {
			visualize_faces_(cnt++) = faces_(face_idx);
		}
	}

	visualize_faces_.conservativeResize(cnt, 3);
}
void Picking::computeColorInfo_() {
	computeCenterVertex_(visual_center_faces_, visualize_faces_);

	for (auto k = 0; k < visualize_faces_.rows(); ++k) {
		int pixel_x = (int)(visual_center_faces_(k, 0) + 0.50);
		int pixel_y = (int)(visual_center_faces_(k, 1) + 0.50);
		Eigen::RowVectorXi pixel_color= getTexColor_(pixel_x, pixel_y);

		std::string map_idx(1, (char)(pixel_color(0)));
		map_idx += (char)(pixel_color(1));
		map_idx += (char)(pixel_color(2));
		color_visual_faces_[map_idx].push_back(k);
	}
}

void Picking::readPallet_(const std::string& map_full_path, const std::string& color_full_path) {
	if (!fileExist(map_full_path)) {
		std::cout << "Picking::readPallet_: Make a new Pallet(" << color_full_path << ")" << std::endl;
		makeTexture(color_full_path, map_full_path);
	}

	readTex_(map_full_path);
}

void Picking::readTex_(const std::string& full_path) {
	if (!fileExist(full_path)) {
		std::cout << "Picking::readTex_: ERROR, File is not exist(" << full_path << ")" << std::endl;
		//exit(1);
	}
	else {
		pallet_info_.data = stbi_load(full_path.c_str(), &(pallet_info_.width), &(pallet_info_.height), &(pallet_info_.n_channels), 0);
	}
}

void Picking::computeFacesColor_(std::string origin_full_path) {

}

template <typename T>
void Picking::setSharedPtr(std::shared_ptr<T>& original_ptr, std::shared_ptr<T>& temp_ptr) {
	swap(original_ptr, temp_ptr);
}

Eigen::RowVectorXi Picking::getTexColor_(int row, int col) {
	std::string function_name = "Eigen::RowVectorXd Picking::getTexColor_(int row, int col, int n_channel)";
	Eigen::RowVectorXi ret;

	if ((row >= pallet_info_.width) || (col >= pallet_info_.height) || (row < 0) || (col < 0)) {
		std::cout << function_name << ": ERROR: OUT OF RANGE" << std::endl;
		//exit(1);
	}
	
	ret.resize(pallet_info_.n_channels);
	for (int k = 0; k < pallet_info_.n_channels; ++k) {
		ret(k) = pallet_info_.data[row * pallet_info_.width * pallet_info_.n_channels + col * pallet_info_.n_channels + k];
	}

	return ret;
}
