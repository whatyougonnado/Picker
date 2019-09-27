#include <Photographer.h>
#include <GeneralMesh.h>
#include <ParsingMesh.h>
#include <BoundingBox.h>
#include <PictureComparison.h>

#include <iostream>

#include <OBJController.h>
#include <Picking.h>

#include <fstream>
#include <string>

#include <igl/unproject_ray.h>
#include <igl/unproject_onto_mesh.h>

int main(int argc, char *argv[])
{
	std::string load_imagename = "/view_1.png";
	std::string save_imagename = "/fix_view_1.png";
	std::string color_csv_full_path_ = "D:/Data/GigaKorea/scans_obj/mman_coloring.txt";
	std::string map_full_path = "D:/Data/GigaKorea/scans_obj/color_map.png";

	std::string load_full_path, save_full_path, load_full_path2, save_full_path2;
	GeneralMesh object(argv[1], argv[2]);

	std::cout << "Mesh Loaded!" << std::endl;

	Photographer photographer(&object);
	
	bool first = false;
    // 3_ 3 front
	//--------------------------------------------------------------------------------------
	//photographer.addCameraToPosition(0.0f, 0.0f, 1.0f, 4.0f);
	//photographer.addCameraToPosition(1.0f, -0.5f, 2.0f, 4.0f);
	//photographer.addCameraToPosition(-1.0f, 0.0f, 1.0f, 4.0f);
	//--------------------------------------------------------------------------------------
	if (first) {
		photographer.addCameraToPositionAround(
			0.0f, 0.1f, 0.05f,
			0.0f, 0.1f, 0.05f,
			1.0f, 0.1f, 0.05f,
			4.0f
		);
		photographer.addCameraToPositionAround(
			1.0f, 0.1f, 0.05f,
			-0.5f, 0.1f, 0.05f,
			2.0f, 0.1f, 0.05f,
			4.0f
		);
		photographer.addCameraToPositionAround(
			-1.0f, 0.1f, 0.05f,
			0.0f, 0.1f, 0.05f,
			1.0f, 0.1f, 0.05f,
			4.0f
		);
		//Point symmetry
		photographer.addCameraToPositionAround(
			-0.0f, 0.1f, 0.05f,
			-0.0f, 0.1f, 0.05f,
			-1.0f, 0.1f, 0.05f,
			4.0f
		);
		photographer.addCameraToPositionAround(
			-1.0f, 0.1f, 0.05f,
			0.5f, 0.1f, 0.05f,
			-2.0f, 0.1f, 0.05f,
			4.0f
		);
		photographer.addCameraToPositionAround(
			1.0f, 0.1f, 0.05f,
			-0.0f, 0.1f, 0.05f,
			-1.0f, 0.1f, 0.05f,
			4.0f
		);
		photographer.setShader(Shader::ShaderTypes::FACEID_SHADER, Shader::ShaderTypes::FACEID_SHADER);
		photographer.renderToImages(argv[4]);
		photographer.saveImageCamerasParamsCV(argv[4]);
		photographer.setShader(Shader::ShaderTypes::FULL_SHADER, Shader::ShaderTypes::FULL_SHADER);
		photographer.renderToImages(argv[3]);
		photographer.saveImageCamerasParamsCV(argv[3]);

		//--------------------------------------------------------------------------------------
		// if the face id: 0 is on boundary, then the problem is occuring
		for (int i = 1; i <= 750; ++i) {

			load_imagename = "/view_" + std::to_string(i) + ".png";
			save_imagename = "/fix_view_" + std::to_string(i) + ".jpg";
			load_full_path = argv[3] + load_imagename;
			save_full_path = argv[3] + save_imagename;
			BoundingBox boundingbox(load_full_path);
			boundingbox.saveTexBounded(save_full_path,false);

			save_imagename = "/fix_view_" + std::to_string(i) + ".png";
			load_full_path2 = argv[4] + load_imagename;
			save_full_path2 = argv[4] + save_imagename;
			boundingbox.setTexData(load_full_path2);
			boundingbox.saveTexBounded(save_full_path2);
		}
	}
	//

	//--------------------------------------------------------------------------------------
	//CE2p
	
	////
	//--------------------------------------------------------------------------------------
	if (!first) {
		vector<string> face_color;
		map<string, array<float, 3>> color_table;

		vector<map<string, int>> face_color_counter_;
		PictureComparison myPictureComparison(object.getFaces().rows(), save_full_path, save_full_path2);
		myPictureComparison.setColorTable("D:/Data/GigaKorea/scans_obj/mman_coloring_fit_fit2.txt");
		myPictureComparison.initFaceColorTable();
		for (int i = 1; i <= 750; ++i) {
			save_imagename = "/fix_view_" + std::to_string(i) + ".png";
			save_full_path = "D:/Documents/Thesis/CaptureController/output/parse" + save_imagename;
			save_imagename = "/fix_view_" + std::to_string(i) + ".png";//jpg id -> error
			save_full_path2 = argv[4] + save_imagename;
			myPictureComparison.setTex(save_full_path, save_full_path2);
			myPictureComparison.computeIdColor();

		}
		////--------------------------------------------------------------------------------------
		myPictureComparison.setColorTable("D:/Data/GigaKorea/scans_obj/mman_coloring_fit_fit.txt");
		face_color = myPictureComparison.getFaceColor();
		color_table = myPictureComparison.getColorTableFloat();

		ParsingMesh myParsingObj(argv[1], face_color, color_table);
		Photographer photographerParsing(&myParsingObj);
		photographerParsing.setShader(Shader::ShaderTypes::MOSAIC_SHADER, Shader::ShaderTypes::MOSAIC_SHADER);

		//--------------------------------------------------------------------------------------
		// 3_ 3 front
		photographerParsing.addCameraToPosition(0.0f, 0.0f, 1.0f, 4.0f);
		photographerParsing.addCameraToPosition(1.0f, -0.5f, 2.0f, 4.0f);
		photographerParsing.addCameraToPosition(-1.0f, 0.0f, 1.0f, 4.0f);
		photographerParsing.addCameraToPosition(-0.0f, -0.0f, -1.0f, 4.0f);
		photographerParsing.addCameraToPosition(-1.0f, 0.5f, -2.0f, 4.0f);
		photographerParsing.addCameraToPosition(1.0f, -0.0f, -1.0f, 4.0f);

		photographerParsing.renderToImages(argv[3]);
		photographerParsing.saveImageCamerasParamsCV(argv[3]);

		Photographer photographerVisual(&object);
		photographer.addCameraToPosition(0.0f, 0.0f, 1.0f, 4.0f);
		photographer.addCameraToPosition(1.0f, -0.5f, 2.0f, 4.0f);
		photographer.addCameraToPosition(-1.0f, 0.0f, 1.0f, 4.0f);
		photographer.addCameraToPosition(-0.0f, -0.0f, -1.0f, 4.0f);
		photographer.addCameraToPosition(-1.0f, 0.5f, -2.0f, 4.0f);
		photographer.addCameraToPosition(1.0f, -0.0f, -1.0f, 4.0f);

		photographerVisual.setShader(Shader::ShaderTypes::FULL_SHADER, Shader::ShaderTypes::FULL_SHADER);
		photographerVisual.renderToImages(argv[5]);
		photographerVisual.saveImageCamerasParamsCV(argv[5]);
		//photographerParsing.viewScene(true);
	}
	//--------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------
	//int x, y, n_channels, x_out, y_out;
	//unsigned char *input_data, *output_data;
	//input_data = stbi_load(load_full_path.c_str(), &x, &y, &n_channels, 0);

	//x_out = x / 2;
	//y_out = y / 2;

	//stbir_resize_uint8(input_data, x, y, 0, output_data, x_out, y_out, 0, n_channels);
	//stbi_write_bmp("C:/Users/AnotherMotion/Desktop/resized_output_image.bmp", x_out, y_out, 0, output_data);
	//--------------------------------------------------------------------------------------

	//
	//std::vector<Camera> image_cameras = photographer.getImageCameras();
	////model matrix == I
	//Eigen::Matrix4d modelview, proj;
	//for (int i = 0; i < 4; ++i) {
	//	for (int j = 0; j < 4; ++j) {
	//		modelview(i, j) = (image_cameras[1].getGlViewMatrix())[i][j];
	//		proj(i, j) = (image_cameras[1].getGlProjectionMatrix())[i][j];
	//	}
	//}

	//Eigen::MatrixXd _V;
	//Eigen::MatrixXi _F;
	////
	//const Eigen::Matrix4d MVP = (proj.transpose())*(modelview.transpose());
	//const Eigen::Vector4d viewport(0, 0, 1024, 1024);
	//const Eigen::RowVector3d camera_look_vector(0.0f, 0.0f, -1.0f);
	//const std::string full_path = "D:/Documents/Thesis/CaptureController/CaptureController/the.obj";

	//std::cout << "start OBJController" << std::endl;
	////OBJController MyOBJController(MVP, object2.getNormalizedVertices(), object2.getFaces(), camera_look_vector, viewport);
	////MyOBJController.saveWindowFittedOutput(full_path);
	////MyOBJController.receiveOBJ(_V, _F);

	//std::cout << "finish OBJController" << std::endl;

	//Picking;
	//Picking MyPicking(_V, _F);
	//Picking::makeTexture(color_csv_full_path_, map_full_path);
	//MyPicking.run(map_full_path);

	//std::cout << "finish" << std::endl;
}
