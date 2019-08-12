#pragma once

#include <Eigen/Dense>

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <stb/stb_image_write.h>
#include <stb/stb_image.h>

#include <iostream>

class Picking {
public:
	Picking(Eigen::MatrixXd verts, Eigen::MatrixXi faces, Eigen::Vector4d viewport);
	~Picking();

	static void makeTexture(const std::string& color_csv_full_path, const std::string& save_texture_full_path);
	void run();

private:
	Picking();
	void getCenterVertex_();
	void getVisualizeFaces_();

	void readPicture(std::string full_path);
	void getFacesColor_(std::string origin_full_path);

	//manually create
	void writeParsingTexture(std::string full_path);

	Eigen::MatrixXd verts_;
	Eigen::MatrixXd center_face_;
	Eigen::MatrixXi faces_;

	const Eigen::Vector3d projectionplane_normal_ = Eigen::Vector3d(0,0,-1);

	//ex. 1024 x 1024
	Eigen::Vector4d viewport_;
	Eigen::MatrixXi visualize_faces_;
	Eigen::MatrixXi faces_color_;
};