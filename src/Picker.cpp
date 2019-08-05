#include <Picker.h>

Picker::Picker(Eigen::MatrixXd MVP, Eigen::MatrixXd verts_normalized, Eigen::MatrixXi faces, Eigen::Vector3d projectionplane_normal):
	MVP_(MVP), verts_normalized_(verts_normalized), verts_projected_(verts_normalized), faces_(faces), projectionplane_normal_(projectionplane_normal){
	std::shared_ptr<bool> ptr(new bool[faces_.rows()], std::default_delete<bool[]>());
	std::swap(cache_front_face, ptr);

	projectionplane_normal_.normalize();

	projectedVertices_();
	projetedVerticesNormal_();
	backfaceCulling_();
}

Picker::~Picker() {
}

void Picker::projectedVertices_() {
	Eigen::MatrixXd padding, verts_normalized_homogenous, homogenous, temp;

	verts_normalized_homogenous = verts_normalized_.array().rowwise().homogeneous();
	verts_projected_ = (MVP_ * verts_normalized_homogenous.transpose()).transpose();

	temp.resize(verts_projected_.rows(), 3);
	for (int i = 0; i < verts_projected_.rows(); ++i) {
		temp(i, 0) = verts_projected_(i, 0) / verts_projected_(i, 3);
		temp(i, 1) = verts_projected_(i, 1) / verts_projected_(i, 3);
		temp(i, 2) = verts_projected_(i, 2) / verts_projected_(i, 3);
	}
	verts_projected_ = temp;
//	verts_projected_ = verts_projected_.array().rowwise().hnormalized();

	igl::writeOBJ("D:/Documents/Thesis/CaptureController/CaptureController/the.obj", verts_projected_, faces_);
}

void Picker::projetedVerticesNormal_() {
	igl::per_face_normals(verts_projected_, faces_, faces_projected_normals_);
}

void Picker::backfaceCulling_() {
	int size_face = faces_.rows();
	int size_front_face = 0;
	for (auto i = 0; i < size_face; ++i) {
		Eigen::RowVector3d verts_projected_normal = faces_projected_normals_.row(i);
		bool isFrontFace = (projectionplane_normal_.dot(verts_projected_normal) < 0);
		bool& _cache = cache_front_face.get()[i];

		if (isFrontFace) {
			_cache = true;
			size_front_face++;
		}
		else {
			_cache = false;
		}
	}

	faces_front_.resize(size_front_face, 3);
	int cnt = 0;
	for (int i = 0; (i < size_face) && (cnt < size_front_face); ++i) {
		if (cache_front_face.get()[i]) {
			faces_front_.row(cnt) = faces_.row(i);
			//face_front(ccnt, 0) = faces_(i, 0);
			//face_front(ccnt, 1) = faces_(i,1);
			//face_front(ccnt, 2) = faces_(i, 2);

			cnt++;
		}
	}

	igl::writeOBJ("D:/Documents/Thesis/CaptureController/CaptureController/the2.obj", verts_projected_, faces_front_);
}