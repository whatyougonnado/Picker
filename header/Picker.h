#pragma once

#include <Eigen/Dense>
#include <igl/per_face_normals.h>
#include <memory>

#include <igl/writeOBJ.h>

class Picker {
public:
	Picker(Eigen::MatrixXd MVP, Eigen::MatrixXd verts_normalized, Eigen::MatrixXi faces, Eigen::Vector3d projectionplane_normal);
	~Picker();
private:
	Picker();
	void normalizedPlaneNormal_();
	void projectedVertices_();
	void projetedVerticesNormal_();

	void Picker::backfaceCulling_();

	//void getColor(double preprocessed_x, double preprocessed_y, int )
	Eigen::MatrixXd MVP_;

	Eigen::MatrixXd verts_normalized_;
	Eigen::MatrixXd verts_projected_;
	Eigen::MatrixXd faces_projected_normals_;

	Eigen::MatrixXi faces_;
	Eigen::MatrixXi faces_front_;

	Eigen::Vector3d projectionplane_normal_;
	
	std::shared_ptr<bool> cache_front_face;
	
};