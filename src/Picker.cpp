#include <Picker.h>

Picker::Picker(Eigen::MatrixXd MVP, Eigen::MatrixXd verts_normalized, Eigen::MatrixXi faces, Eigen::Vector3d projectionplane_normal):
	MVP_(MVP), verts_normalized_(verts_normalized), verts_projected_(verts_normalized), faces_(faces), projectionplane_normal_(projectionplane_normal){
	std::shared_ptr<bool> ptr(new bool[faces_.rows()], std::default_delete<bool[]>());
	std::swap(cache_front_face, ptr);

	projectedVertices();
	projetedVerticesNormal();
	backfaceCulling();
}

Picker::~Picker() {
}

void Picker::projectedVertices() {
	Eigen::MatrixXd padding, homogenous;
	padding.resize(verts_normalized_.rows(), 1);	
	homogenous.resize(verts_normalized_.rows(), verts_normalized_.cols() + 1);

	padding = Eigen::MatrixXd::Zero(verts_normalized_.rows(), 1);
	homogenous << verts_normalized_, padding;

	verts_projected_ = MVP_ * homogenous.transpose();
	Eigen::MatrixXd tw = verts_projected_.transpose();
	MVP_ = MVP_.transpose();
}

void Picker::projetedVerticesNormal() {
	igl::per_vertex_normals(verts_projected_, faces_, verts_projected_normals_);
}

void Picker::backfaceCulling() {
	int size_face = faces_.rows();

	for (auto i = 0; i < size_face; ++i) {
		Eigen::RowVector3d verts_projected_normal = verts_projected_normals_.row(i);
		bool isFrontFace = (projectionplane_normal_.dot(verts_projected_normal) < 0);
		bool& _cache = cache_front_face.get()[i];

		if (isFrontFace) {
			_cache = true;
		}
		else {
			_cache = false;
		}
	}
}