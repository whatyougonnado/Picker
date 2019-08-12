#pragma once

#include <Eigen/Dense>
#include <igl/per_face_normals.h>
#include <memory>

#include <igl/writeOBJ.h>

#include <string>

class OBJController {
public:
	OBJController(Eigen::Matrix4d MVP, Eigen::MatrixXd verts_normalized, Eigen::MatrixXi faces,
		Eigen::Vector3d projectionplane_normal, Eigen::Vector4d viewport);
	~OBJController();

	Eigen::Matrix4d getViewPortMatrix();
	void saveWindowFittedOutput(std::string full_path);
	void receiveOBJ(Eigen::MatrixXd& verts_at_window, Eigen::MatrixXi& faces_front) const;
private:
	OBJController();
	void HomogeneousMultiplication(const Eigen::Matrix4d& left_matrix, const Eigen::MatrixXd& cache, Eigen::MatrixXd& output);
	void projectedVertices_();
	void projetedVerticesNormal_();
	void screenizedVertices_();
	void backfaceCulling_();

	//void getColor(double preprocessed_x, double preprocessed_y, int )
	Eigen::Matrix4d MVP_;

	Eigen::MatrixXd verts_normalized_;
	Eigen::MatrixXd verts_projected_;
	Eigen::MatrixXd verts_at_window_;
	Eigen::MatrixXd faces_projected_normals_;

	Eigen::MatrixXi faces_;
	Eigen::MatrixXi faces_front_;

	Eigen::Vector3d projectionplane_normal_;

	Eigen::Vector4d viewport_;
	
	std::shared_ptr<bool> cache_front_face;
	
};