#include <Photographer/Photographer.h>
#include <GeneralMesh/GeneralMesh.h>
#include <BoundingBox.h>

#include <iostream>

//#include <igl/ray_mesh_intersect.cpp>

#include <OBJController.h>

#include <fstream>
#include <string>
int main(int argc, char *argv[])
{
	GeneralMesh object(argv[1], argv[2]);
	GeneralMesh object2(argv[1], argv[2]);
	
    std::cout << "Mesh Loaded!" << std::endl;

    Photographer photographer(&object);

    // 2_2 front, 2 back
    //photographer.addCameraToPosition(0.0f, 1.0f, 3.0f, 4.0f);
    //photographer.addCameraToPosition(1.0f, -0.5f, 2.0f, 4.0f);
    //photographer.addCameraToPosition(0.0f, -1.0f, -2.0f, 5.0f);
    //photographer.addCameraToPosition(-1.0f, 1.0f, -2.0f, 5.0f);

    // 3_ 3 front
	photographer.addCameraToPosition(0.0f, 0.0f, 1.0f, 4.0f);
    photographer.addCameraToPosition(1.0f, -0.5f, 2.0f, 4.0f);
    photographer.addCameraToPosition(-1.0f, 0.0f, 1.0f, 4.0f);

	// 4_ 3 front
	//photographer.addCameraToPosition(0.0f, 0.0f, 1.0f, 2.0f);
	//photographer.addCameraToPosition(1.0f, -0.5f, 2.0f, 2.0f);
	//photographer.addCameraToPosition(-1.0f, 0.0f, 1.0f, 2.0f);

	
	//Picker picker();
	//picker.setTex();
	//picker.saveModifiedTex_();
	//picker.getModifiedTex_();
	////picker.removePadding_();
	//shader.setUniform("view", camera.getGlViewMatrix());
	//shader.setUniform("projection", camera.getGlProjectionMatrix());
	//shader.setUniform("eye_pos", camera.getPosition());

	std::string in_line;
	std::ofstream out("test.txt");



	int fid;
	Eigen::Vector3f bc;
	
	std::vector<Camera> image_cameras = photographer.getImageCameras();
	//model matrix == I
	Eigen::Matrix4d modelview, proj;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			modelview(i, j) = (image_cameras[1].getGlViewMatrix())[i][j];
			proj(i, j) = (image_cameras[1].getGlProjectionMatrix())[i][j];
		}
	}

	//
	const Eigen::Matrix4d MVP = (proj.transpose())*(modelview.transpose());
	const Eigen::Vector4d viewport(0, 0, 1024, 1024);
	const std::string full_path = "D:/Documents/Thesis/CaptureController/CaptureController/the.obj";

	OBJController MyOBJController(MVP, object2.getNormalizedVertices(), object2.getFaces(), Eigen::RowVector3d(1.0f, 0.0f, -1.0f), viewport);
	MyOBJController.saveWindowFittedOutput(full_path);
	//

	
	//for (int y = 0; y < 300; ++y) {
	//	int current = -1;
	//	for (int x = 0; x < 300; ++x) {
	//		if (igl::unproject_onto_mesh(Eigen::Vector2f(x, viewport(3) - y), modelview.transpose(),
	//			proj.transpose(), viewport, object.getNormalizedVertices(), object.getFaces(), fid, bc))
	//		{
	//			if (current != fid) {
	//				current = fid;
	//			}
	//			else {
	//				continue;
	//			}
	//			// paint hit red
	//			//C.row(fid) << 1, 0, 0;
	//			//viewer.data().set_colors(C);
	//			out << "(" << x << ", " << y << "): " << fid << std::endl;
	//		}
	//	}
	//	out << std::endl;
	//}
	out.close();
	photographer.renderToImages(argv[3]);
	photographer.saveImageCamerasParamsCV(argv[3]);

	photographer.viewScene();
}
