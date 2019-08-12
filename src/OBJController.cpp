#include <OBJController.h>

OBJController::OBJController(Eigen::Matrix4d MVP, Eigen::MatrixXd verts_normalized, Eigen::MatrixXi faces, 
	Eigen::Vector3d projectionplane_normal, Eigen::Vector4d viewport):
	MVP_(MVP), verts_normalized_(verts_normalized), verts_projected_(verts_normalized), faces_(faces), projectionplane_normal_(projectionplane_normal), viewport_(viewport){
	std::shared_ptr<bool> ptr(new bool[faces_.rows()], std::default_delete<bool[]>());
	std::swap(cache_front_face, ptr);

	projectionplane_normal_.normalize();

	projectedVertices_();
	projetedVerticesNormal_();
	backfaceCulling_();
	screenizedVertices_();
}

OBJController::~OBJController() {
}

Eigen::Matrix4d OBJController::getViewPortMatrix() {
	Eigen::Matrix4d ret;
	double sx, sy, ws, hs;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			ret(i, j) = 0;
		}
	}

	sx = sy = 0;
	ws = viewport_(2) - viewport_(0);
	hs = viewport_(3) - viewport_(1);
	ret(0, 0) = ws * 0.5;
	ret(0, 3) = (sx + ws) * 0.5;
	ret(1, 1) = hs * 0.5;
	ret(1, 3) = (sy + hs) * 0.5;
	ret(2, 2) = 0.5;
	ret(2, 3) = 0.5;
	ret(3, 3) = 1;

	return ret;
}

void OBJController::receiveOBJ(Eigen::MatrixXd& verts_at_window, Eigen::MatrixXi& faces_front) const {
	verts_at_window = verts_at_window_;
	faces_front = faces_front_;
}

void OBJController::saveWindowFittedOutput(std::string full_path) {
	igl::writeOBJ(full_path, verts_at_window_, faces_front_);
}

//param T: transformation matrix
void OBJController::HomogeneousMultiplication(const Eigen::Matrix4d& left_matrix, const Eigen::MatrixXd& cache, Eigen::MatrixXd& output) {
	Eigen::MatrixXd _homogenous, temp;

	_homogenous = cache.array().rowwise().homogeneous();
	output = (left_matrix * _homogenous.transpose()).transpose();

	//	output = output.array().rowwise().hnormalized();
	temp.resize(output.rows(), 3);
	for (int i = 0; i < output.rows(); ++i) {
		temp(i, 0) = output(i, 0) / output(i, 3);
		temp(i, 1) = output(i, 1) / output(i, 3);
		temp(i, 2) = output(i, 2) / output(i, 3);
	}
	output = temp;
}


void OBJController::projectedVertices_() {
	HomogeneousMultiplication(MVP_, verts_normalized_, verts_projected_);
}

void OBJController::projetedVerticesNormal_() {
	igl::per_face_normals(verts_projected_, faces_, faces_projected_normals_);
}

void OBJController::backfaceCulling_() {
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
			cnt++;
		}
	}

}

void OBJController::screenizedVertices_() {
	Eigen::Matrix4d _viewport_matrix;
	Eigen::RowVector3d pivot;
	
	_viewport_matrix= getViewPortMatrix();
	HomogeneousMultiplication(_viewport_matrix, verts_projected_, verts_at_window_);

	pivot(0) = viewport_(0);
	pivot(1) = viewport_(1);
	pivot(2) = 0;

	verts_at_window_ = verts_at_window_.rowwise() + pivot;
}