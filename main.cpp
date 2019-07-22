#include <Photographer/Photographer.h>
#include <GeneralMesh/GeneralMesh.h>
#include <BoundingBox.h>

#include <iostream>

#include <igl/unproject_onto_mesh.h>

#include <fstream>
#include <string>
int main(int argc, char *argv[])
{
    GeneralMesh object(argv[1], argv[2]);

    std::cout << "Mesh Loaded!" << std::endl;

    Photographer photographer(&object);

    // 2_2 front, 2 back
    //photographer.addCameraToPosition(0.0f, 1.0f, 3.0f, 4.0f);
    //photographer.addCameraToPosition(1.0f, -0.5f, 2.0f, 4.0f);
    //photographer.addCameraToPosition(0.0f, -1.0f, -2.0f, 5.0f);
    //photographer.addCameraToPosition(-1.0f, 1.0f, -2.0f, 5.0f);

    // 3_ 3 front
	photographer.addCameraToPosition(0.0f, 0.0f, 1.0f, 4.0f);
    photographer.addCameraToPosition(0.0f, 1.0f, 3.0f, 12.0f);
    photographer.addCameraToPosition(1.0f, -0.5f, 2.0f, 4.0f);
    photographer.addCameraToPosition(-1.0f, 0.0f, 1.0f, 4.0f);

	
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
	Eigen::Vector4f viewport;
	viewport(0) = 0;
	viewport(1) = 0;
	viewport(2) = 1024;
	viewport(3) = 1024;
	
	std::vector<Camera> image_cameras = photographer.getImageCameras();
	Eigen::Matrix4f view, proj;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			view(i, j) = (image_cameras[0].getGlViewMatrix())[i][j];
			std::cout << "(" << i << ", " << j << "): " << view(i, j) << std::endl;
		}
	}
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			proj(i, j) = (image_cameras[0].getGlProjectionMatrix())[i][j];
			std::cout << "(" << i << ", " << j << "): " << proj(i, j) << std::endl;
		}
	}

	std::cout << view.transpose() << std::endl;
	std::cout << proj.transpose() << std::endl;
	std::cout << viewport << std::endl;


	for (int y = 0; y < 1024; ++y) {
		int current = -1;
		for (int x = 0; x < 1024; ++x) {
			if (igl::unproject_onto_mesh(Eigen::Vector2f(x, viewport(3) - y), view.transpose(),
				proj.transpose(), viewport, object.getNormalizedVertices(), object.getFaces(), fid, bc))
			{
				if (current != fid) {
					current = fid;
				}
				else {
					continue;
				}
				// paint hit red
				//C.row(fid) << 1, 0, 0;
				//viewer.data().set_colors(C);
				out << "(" << x << ", " << y << "): " << fid << std::endl;
			}
		}
		out << std::endl;
	}
	out.close();
	photographer.renderToImages(argv[3]);
	photographer.saveImageCamerasParamsCV(argv[3]);

	photographer.viewScene();
}
