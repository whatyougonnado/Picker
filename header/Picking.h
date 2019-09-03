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
#include <array>
#include <map>
#include <limits>

#include <igl/unproject_onto_mesh.h>
#include <igl/unproject_ray.h>

class Picking {
public:
	enum XYZ {
		x = 0, y, z
	};
	std::string color_csv_full_path_ = "D:/Data/GigaKorea/scans_obj/mman_coloring.txt";
	static bool fileExist(const std::string& full_path);

	struct TextureInfo {
		unsigned char *data;
		int width;
		int height;
		int n_channels;

	};

	Picking(const Eigen::MatrixXd& verts, const Eigen::MatrixXi& faces);
	~Picking();

	//param: overwrite (default: false)
	static int makeTexture(const std::string& color_csv_full_path, const std::string& save_texture_full_path, bool overwrite = false);
	void run(const std::string& full_path);

private:
	Picking();
	void init_();

	void Picking::computeCenterVertex_(Eigen::MatrixXd& cache, const Eigen::MatrixXi& faces);
	bool findIntersection(int main_face_idx);
	bool isFormer(double _z, int face_idx);
	Eigen::Vector3d computeBaryCentricCoord_(Eigen::Vector2d input, int face_idx);
	void computeVisualizeFaces_();
	void computeColorInfo_();

	void readTex_(const std::string & imagename);
	void readPallet_(const std::string& map_full_path = "D:/Data/GigaKorea/scans_obj/color_map.png", const std::string& color_full_path = "D:/Data/GigaKorea/scans_obj/mman_coloring.txt");
	char* readPicture(std::string full_path);
	void computeFacesColor_(std::string origin_full_path);

	template <typename T>
	void setSharedPtr(std::shared_ptr<T>& original_ptr, std::shared_ptr<T>& temp_ptr);

	Eigen::RowVectorXi getTexColor_(int row, int col);

	Eigen::MatrixXd verts_;
	Eigen::MatrixXd center_faces_;
	Eigen::MatrixXi faces_;

	const Eigen::Vector3d projectionplane_normal_ = Eigen::Vector3d(0,0,-1);

	//ex. 1024 x 1024
	Eigen::MatrixXd visual_center_faces_;
	Eigen::MatrixXi visualize_faces_;
	std::map<std::string, std::vector<int>> color_visual_faces_;
	//Eigen::VectorXi ;

	struct DCache_{
		bool init;
		Eigen::Vector2d v0, v1, v2;
		double d00, d01, d11, denom;
	};
	std::vector<DCache_> d_cache_;

	TextureInfo pallet_info_;
};