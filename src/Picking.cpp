#include <Picking.h>

Picking::Picking(Eigen::MatrixXd verts, Eigen::MatrixXi faces, Eigen::Vector4d viewport): verts_(verts), faces_(faces), viewport_(viewport){
}
Picking::~Picking() {
}

void Picking::makeTexture(const std::string& color_csv_full_path, const std::string& save_texture_full_path) {
	std::vector<int> color;
	std::string line;
	std::ifstream in_file;
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
}

void Picking::run() {
	getCenterVertex_();
	getVisualizeFaces_();
}

void Picking::getCenterVertex_() {
	
	center_face_ = Eigen::MatrixXd::Zero(verts_.rows(), 3);

	for (auto i = 0; i < center_face_.rows(); ++i) {
		int face_vert_idx[3] = {faces_(i,0), faces_(i,1), faces_(i,2)};
		for (auto k = 0; k < 3; ++k) {
			center_face_(i, 0) += verts_(face_vert_idx[k], 0);
			center_face_(i, 1) += verts_(face_vert_idx[k], 1);
			center_face_(i, 2) += verts_(face_vert_idx[k], 2);
		}
	}

	center_face_ /= 3.0;
}

void Picking::getVisualizeFaces_() {
	Eigen::MatrixXd cache_pixel;
	cache_pixel = Eigen::MatrixXd::Ones(1024, 1024);
	cache_pixel *= -1.0;
	visualize_faces_ = Eigen::MatrixXi::Ones(1024, 1024);
	visualize_faces_ *= -1;

	for (auto face_idx = 0; face_idx < center_face_.rows(); ++face_idx) {
		int local_x = (int)(center_face_(face_idx, 0) + 0.5);
		int local_y = (int)(center_face_(face_idx, 1) + 0.5);
		double local_z = center_face_(face_idx, 2);
		
		double& cache_pixel_z = cache_pixel(local_x, local_y);

		if (cache_pixel_z < local_z) {
			cache_pixel_z = local_z;
			visualize_faces_(local_x, local_y) = face_idx;
			std::cout << local_x << ", " << local_y << ": " << face_idx << std::endl;
		}
	}
}

void Picking::readPicture(std::string full_path) {

}

void Picking::getFacesColor_(std::string origin_full_path) {

}

void Picking::writeParsingTexture(std::string full_path) {

}