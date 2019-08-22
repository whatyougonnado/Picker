#include <Photographer/Photographer.h>
#include <GeneralMesh/GeneralMesh.h>
#include <BoundingBox.h>

#include <iostream>

#include <OBJController.h>
#include <Picking.h>

#include <fstream>
#include <string>

#include <igl/unproject_ray.h>
#include <igl/unproject_onto_mesh.h>

int main(int argc, char *argv[])
{
	std::string load_imagename = "view_1.png";
	std::string save_imagename = "fix_view_1.png";

	std::string load_full_path, save_full_path, load_full_path3, save_full_path3;
	GeneralMesh object(argv[1], argv[2]);
	//GeneralMesh object2(argv[1], argv[2]);
	
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

	photographer.renderToImages(argv[3]);
	photographer.saveImageCamerasParamsCV(argv[3]);

	photographer.viewScene(false);
	//--------------------------------------------------------------------------------------
	
	load_full_path = argv[3] + load_imagename;
	save_full_path = argv[3] + save_imagename;
	BoundingBox boundingbox(load_full_path);
	boundingbox.saveModifiedTex(save_full_path);

	//--------------------------------------------------------------------------------------
	/*
	{std::vector<Camera> image_cameras = photographer.getImageCameras();
	//model matrix == I
	Eigen::Matrix4d modelview, proj;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			modelview(i, j) = (image_cameras[1].getGlViewMatrix())[i][j];
			proj(i, j) = (image_cameras[1].getGlProjectionMatrix())[i][j];
		}
	}

	Eigen::MatrixXd _V;
	Eigen::MatrixXi _F;
	//
	const Eigen::Matrix4d MVP = (proj.transpose())*(modelview.transpose());
	const Eigen::Vector4d viewport(0, 0, 1024, 1024);
	const Eigen::RowVector3d camera_look_vector(0.0f, 0.0f, -1.0f);
	const std::string full_path = "D:/Documents/Thesis/CaptureController/CaptureController/the.obj";
	}*/
	//OBJController MyOBJController(MVP, object2.getNormalizedVertices(), object2.getFaces(), camera_look_vector, viewport);
	//MyOBJController.saveWindowFittedOutput(full_path);
	//MyOBJController.receiveOBJ(_V, _F);
	
	//Picking;
	//Picking MyPicking(_V, _F, viewport);
	//Picking::makeTexture("D:/Data/GigaKorea/scans_obj/mman_coloring.txt", "D:/Data/GigaKorea/scans_obj/color_map.png");
	//MyPicking.run();

	//int fid;
	//Eigen::Vector3f bc;
	//std::ofstream out("test.txt");
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
	//out.close();
	std::cout << "finish" << std::endl;
}
