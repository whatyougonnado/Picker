#pragma once

#include <Eigen/Dense>

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <stb/stb_image_write.h>
#include <stb/stb_image.h>

#include <iostream>
#include <memory>

class Picking {
public:
	std::string color_csv_full_path_ = "D:/Data/GigaKorea/scans_obj/mman_coloring.txt";
	static bool fileExist(const std::string& full_path);

	struct TextureInfo {
		unsigned char *data;
		int width;
		int height;
		int n_channels;

	};

	Picking(Eigen::MatrixXd verts, Eigen::MatrixXi faces, Eigen::Vector4d viewport);
	~Picking();

	//param: overwrite (default: false)
	static int makeTexture(const std::string& color_csv_full_path, const std::string& save_texture_full_path, bool overwrite = false);
	void run();

private:
	Picking();
	void getCenterVertex_();
	void getVisualizeFaces_();

	void readTex_(const std::string & imagename);
	void readPallet_(const std::string& map_full_path = "D:/Data/GigaKorea/scans_obj/color_map.png", const std::string& color_full_path = "D:/Data/GigaKorea/scans_obj/mman_coloring.txt");
	char* readPicture(std::string full_path);
	void getFacesColor_(std::string origin_full_path);

	template <typename T>
	void setSharedPtr(std::shared_ptr<T>& original_ptr, std::shared_ptr<T>& temp_ptr);


	Eigen::MatrixXd verts_;
	Eigen::MatrixXd center_face_;
	Eigen::MatrixXi faces_;

	const Eigen::Vector3d projectionplane_normal_ = Eigen::Vector3d(0,0,-1);

	//ex. 1024 x 1024
	Eigen::Vector4d viewport_;
	Eigen::MatrixXi visualize_faces_;
	Eigen::MatrixXi faces_color_;

	TextureInfo pallet_info_;
};