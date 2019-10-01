#include <Photographer.h>
#include <GeneralMesh.h>
#include <ParsingMesh.h>
#include <BoundingBox.h>
#include <PictureComparison.h>
#include <Visualizer.h>

#include <iostream>

#include <OBJController.h>
#include <Picking.h>

#include <fstream>
#include <string>

#include <igl/unproject_ray.h>
#include <igl/unproject_onto_mesh.h>

int main(int argc, char *argv[])
{
	std::string obj_path, obj_list_name, tex_path, tex_list_name, root_path;
	std::string in_line;
	std::vector <std::string> obj_list, tex_list;
	ifstream objfile, texfile;

	obj_path		= argv[1];
	obj_list_name	= argv[2];
	tex_path		= argv[3];
	tex_list_name	= argv[4];
	root_path		= argv[5];

	obj_list = Visualizer::readFile(obj_path + "/" + obj_list_name);
	tex_list = Visualizer::readFile(tex_path + "/" + tex_list_name);

	GeneralMesh object((obj_path + "/" + obj_list[0]).c_str(), (tex_path + "/" + tex_list[0]).c_str());

	std::cout << "Mesh Loaded!" << std::endl;

	Photographer photographer(&object);

	photographer.addCameraRingRoutine(36, 0.0, 2.0f);
	photographer.setShader(Shader::ShaderTypes::FACEID_SHADER);
	photographer.renderToImages(argv[4]);
	photographer.saveImageCamerasParamsCV(argv[4]);

	photographer.setShader(Shader::ShaderTypes::FULL_SHADER);
	photographer.renderToImages(argv[3]);
	photographer.saveImageCamerasParamsCV(argv[3]);
	photographer.viewScene(true);
	//for (int i = 1; i <= 750; ++i) {

	//	load_imagename = "/view_" + std::to_string(i) + ".png";
	//	save_imagename = "/fix_view_" + std::to_string(i) + ".jpg";
	//	load_full_path = argv[3] + load_imagename;
	//	save_full_path = argv[3] + save_imagename;
	//	BoundingBox boundingbox(load_full_path);
	//	boundingbox.saveTexBounded(save_full_path, false);

	//	save_imagename = "/fix_view_" + std::to_string(i) + ".png";
	//	load_full_path2 = argv[4] + load_imagename;
	//	save_full_path2 = argv[4] + save_imagename;
	//	boundingbox.setTexData(load_full_path2);
	//	boundingbox.saveTexBounded(save_full_path2);
	//}

	////

	////--------------------------------------------------------------------------------------
	////CE2p
	//
	//////
	////--------------------------------------------------------------------------------------
	//if ((!first) && second) {
	//	vector<string> face_color;
	//	map<string, array<float, 3>> color_table;

	//	vector<map<string, int>> face_color_counter_;
	//	PictureComparison myPictureComparison(object.getFaces().rows(), save_full_path, save_full_path2);
	//	myPictureComparison.setColorTable("D:/Data/GigaKorea/scans_obj/mman_coloring_fit_fit2.txt");
	//	myPictureComparison.initFaceColorTable();
	//	for (int i = 1; i <= 750; ++i) {
	//		save_imagename = "/fix_view_" + std::to_string(i) + ".png";
	//		save_full_path = "D:/Documents/Thesis/CaptureController/output/parse" + save_imagename;
	//		save_imagename = "/fix_view_" + std::to_string(i) + ".png";//jpg id -> error
	//		save_full_path2 = argv[4] + save_imagename;
	//		myPictureComparison.setTex(save_full_path, save_full_path2);
	//		myPictureComparison.computeIdColor();

	//	}
	//	////--------------------------------------------------------------------------------------
	//	myPictureComparison.setColorTable("D:/Data/GigaKorea/scans_obj/mman_coloring_fit_fit.txt");
	//	face_color = myPictureComparison.getFaceColor();
	//	color_table = myPictureComparison.getColorTableFloat();

	//	ParsingMesh myParsingObj(argv[1], face_color, color_table);
	//	Photographer photographerParsing(&myParsingObj);
	//	photographerParsing.setShader(Shader::ShaderTypes::MOSAIC_SHADER, Shader::ShaderTypes::MOSAIC_SHADER);

	//	//--------------------------------------------------------------------------------------
	//	// 3_ 3 front
	//	photographerParsing.addCameraToPosition(0.0f, 0.0f, 1.0f, 4.0f);
	//	photographerParsing.addCameraToPosition(1.0f, -0.5f, 2.0f, 4.0f);
	//	photographerParsing.addCameraToPosition(-1.0f, 0.0f, 1.0f, 4.0f);
	//	photographerParsing.addCameraToPosition(-0.0f, -0.0f, -1.0f, 4.0f);
	//	photographerParsing.addCameraToPosition(-1.0f, 0.5f, -2.0f, 4.0f);
	//	photographerParsing.addCameraToPosition(1.0f, -0.0f, -1.0f, 4.0f);

	//	photographerParsing.renderToImages(argv[6]);
	//	photographerParsing.saveImageCamerasParamsCV(argv[6]);

	//	//Photographer photographerVisual(&object);
	//	//photographer.addCameraToPosition(0.0f, 0.0f, 1.0f, 4.0f);
	//	//photographer.addCameraToPosition(1.0f, -0.5f, 2.0f, 4.0f);
	//	//photographer.addCameraToPosition(-1.0f, 0.0f, 1.0f, 4.0f);
	//	//photographer.addCameraToPosition(-0.0f, -0.0f, -1.0f, 4.0f);
	//	//photographer.addCameraToPosition(-1.0f, 0.5f, -2.0f, 4.0f);
	//	//photographer.addCameraToPosition(1.0f, -0.0f, -1.0f, 4.0f);

	//	//photographerVisual.setShader(Shader::ShaderTypes::FULL_SHADER, Shader::ShaderTypes::FULL_SHADER);
	//	//photographerVisual.renderToImages(argv[5]);
	//	//photographerVisual.saveImageCamerasParamsCV(argv[5]);
	//	//photographerParsing.viewScene(true);
	//}
	//--------------------------------------------------------------------------------------
	std::string name = "D:/Documents/Thesis/CaptureController/output_chimerehl195/parse/fix_view_1.png";
	Visualizer myVisualizer(name);
	myVisualizer.saveTexVisualize("D:/Data/GigaKorea/scans_obj/mman_coloring_fit_table.txt", "D:/Documents/Thesis/CaptureController/changed_color/test.png");
	//--------------------------------------------------------------------------------------
	//// test
	////--------------------------------------------------------------------------------------
	//photographer.addCameraRingRoutine(36, 0.0, 2.0f);
	//photographer.setShader(Shader::ShaderTypes::FULL_SHADER);
	//photographer.renderToImages(argv[3]);
	//photographer.saveImageCamerasParamsCV(argv[3]);
	//photographer.viewScene(true);

	//photographer.addCameraRingRoutine(36);
	//--------------------------------------------------------------------------------------
}
//
//std::vector<std::string> readList(std::string path, std::string list_name) {
//	ifstream file;
//	string in_line;
//
//	file.open(path + "/" + list_name);
//
//	while (getline(file, in_line)) {
//		obj_list.push_back(in_line);
//	}
//
//	objfile.close();
//	texfile.close();
//}