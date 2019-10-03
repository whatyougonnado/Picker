#include <Photographer.h>

#include <GeneralMesh.h>
#include <GeneralMeshID.h>
#include <GeneralMeshTexture.h>
#include <ParsingMesh.h>

#include <BoundingBox.h>
#include <PictureComparison.h>
#include <GeneralUtility.h>

#include <iostream>

#include <OBJController.h>
#include <Picking.h>

#include <fstream>
#include <string>

#include <igl/unproject_ray.h>
#include <igl/unproject_onto_mesh.h>

int main(int argc, char *argv[])
{
    bool isFront = false;
    std::string obj_path, obj_list_name, tex_path, tex_list_name, root_path;
    std::string in_line;
    std::vector <std::string> obj_list, tex_list;
    ifstream objfile, texfile;

    ////
    obj_path = argv[1]; obj_path += "/";
    obj_list_name = argv[2];
    tex_path = argv[3]; tex_path += "/";
    tex_list_name = argv[4];
    root_path = argv[5]; root_path += "/";
    ////

    obj_list = mg::readTxt(obj_path + obj_list_name);
    tex_list = mg::readTxt(tex_path + tex_list_name);
    
    //for (auto i = 0; i < obj_list.size() && isFront; ++i)
    for (auto i = 0; i < obj_list.size() && isFront; ++i) {
        std::string image_path = root_path + mg::ssplit(obj_list[i],'.')[0] + "/";
        //std::string obj_full_path = obj_path + obj_list[i];
        //std::string tex_full_path = tex_path + tex_list[i];

        //std::string obj_name = mg::ssplit(obj_list[i], '.')[0];
        //std::string prefix = obj_name + "_";

        //std::cout << "The Mesh " + obj_name + " is Loaded!" << std::endl;

        ////--------------------------------------------------------------------------------------------------------------------------
        //GeneralMeshTexture myGeneralMeshTexture(obj_full_path.c_str(), tex_full_path.c_str());
        //Photographer photographer(&myGeneralMeshTexture);

        //photographer.addCameraRingRoutine(36, 0.0, 2.0f);
        //photographer.setShader(Shader::ShaderTypes::FULL_SHADER);
        //photographer.saveImageCamerasParamsCV(image_path, prefix);
        //std::vector<std::string> texmesh_name_list = photographer.renderToImages(image_path, "texmesh_view_" + prefix);
        //mg::writeTxt(texmesh_name_list, image_path + "texmesh_name_list.txt");
        ////photographer.viewScene(true);
        ////--------------------------------------------------------------------------------------------------------------------------
        //GeneralMeshID myGeneralMeshID(obj_full_path.c_str());
        //photographer.setObject(&myGeneralMeshID);

        //photographer.setShader(Shader::ShaderTypes::FACEID_SHADER);
        //photographer.saveImageCamerasParamsCV(image_path, prefix);
        //std::vector<std::string> id_name_list = photographer.renderToImages(image_path, "id_view_" + prefix);
        //mg::writeTxt(id_name_list, image_path + "id_name_list.txt");
        ////photographer.viewScene(true);

        std::vector<std::string> texmesh_name_list = mg::readTxt(image_path + "texmesh_name_list.txt");
        std::vector<std::string> id_name_list = mg::readTxt(image_path + "id_name_list.txt");
        ////--------------------------------------------------------------------------------------------------------------------------

        std::vector<std::string> bound_texmesh_name_list, bound_id_name_list;
        for (auto k = 0; k < texmesh_name_list.size(); ++k) {
            std::string load_path = image_path + texmesh_name_list[k];
            std::string bound_texmesh_name = mg::convertExtension("bd_" + texmesh_name_list[k], "jpg"); // <- need to convert jpg
            std::string save_path =  image_path + save_path; // <- need to convert jpg
            BoundingBox myBoundingbox(load_path);
            myBoundingbox.saveTexBounded(save_path);
            bound_texmesh_name_list.push_back(bound_texmesh_name);

            load_path = image_path + id_name_list[k];
            std::string bound_id_name = "bd_" + id_name_list[k]; // <- need to convert jpg
            save_path = image_path + save_path; // <- need to convert jpg
            BoundingBox myBoundingbox2(load_path);
            myBoundingbox2.saveTexBounded(save_path);
            bound_id_name_list.push_back(bound_id_name);
        }

        mg::writeTxt(bound_texmesh_name_list, image_path + "bd_texmesh_name_list.txt");
        mg::writeTxt(bound_id_name_list, image_path + "bd_id_name_list.txt");
    }

    ////--------------------------------------------------------------------------------------
    ////
    ////CE2p
    ////
    ////--------------------------------------------------------------------------------------

    std::vector<std::string> ce2p_texmesh_name_list, bound_id_name_list;

    for (auto i = 0; i < obj_list.size() && !isFront; ++i) {
        std::string image_path = root_path + mg::ssplit(obj_list[i], '.')[0] + "/";
        std::string parsing_path = image_path + "ce2p/";
        ce2p_texmesh_name_list = mg::readTxt(parsing_path + "bd_texmesh_name_list.txt"); // <- bound + ce2p
        bound_id_name_list = mg::readTxt(image_path + "bd_id_name_list.txt"); // <- bound

        std::string obj_full_path = obj_path + obj_list[i];
        GeneralMesh object(obj_full_path.c_str());

        vector<string> face_color;
        map<string, array<float, 3>> color_table;
        vector<map<string, int>> face_color_counter_;

        PictureComparison myPictureComparison(object.getFaces().rows());
        //std::cout << "object.getFaces().rows(): " << object.getFaces().rows() << std::endl;
        myPictureComparison.setColorTable("D:/Data/GigaKorea/scans_obj/mman_coloring_fit_fit2.txt");
        myPictureComparison.initFaceColorTable();

        for (int k = 0; k < 36; ++k) {
            std::string ce2p_texmesh_full_path = mg::convertExtension(parsing_path + ce2p_texmesh_name_list[k], "png");
            std::string bound_id_full_path = image_path + bound_id_name_list[k];

            myPictureComparison.setTex(ce2p_texmesh_full_path, bound_id_full_path);
            myPictureComparison.computeIdColor();

            
            face_color = myPictureComparison.getFaceColor();
            color_table = myPictureComparison.getColorTableFloat();

            std::cout << "k: " << k << std::endl;

        }
        myPictureComparison.setColorTable("D:/Data/GigaKorea/scans_obj/mman_coloring_fit_fit.txt");
        color_table = myPictureComparison.getColorTableFloat();

        ParsingMesh myParsingObj(obj_full_path.c_str(), face_color, color_table);
        Photographer photographerParsing(&myParsingObj);
        photographerParsing.setShader(Shader::ShaderTypes::MOSAIC_SHADER, Shader::ShaderTypes::MOSAIC_SHADER);

        //photographer.addCameraToPosition(0.0f, 0.0f, 1.0f, 4.0f);
        //    //photographer.addCameraToPosition(1.0f, -0.5f, 2.0f, 4.0f);
        //    //photographer.addCameraToPosition(-1.0f, 0.0f, 1.0f, 4.0f);
        //    //photographer.addCameraToPosition(-0.0f, -0.0f, -1.0f, 4.0f);
        //    //photographer.addCameraToPosition(-1.0f, 0.5f, -2.0f, 4.0f);
        //    //photographer.addCameraToPosition(1.0f, -0.0f, -1.0f, 4.0f);
        //photographerParsing.renderToImages(argv[5]);
        //photographerParsing.saveImageCamerasParamsCV(argv[5]);
        photographerParsing.viewScene(true);
    }
    //    myPictureComparison.setColorTable("D:/Data/GigaKorea/scans_obj/mman_coloring_fit_fit.txt");
    //    face_color = myPictureComparison.getFaceColor();
    //    color_table = myPictureComparison.getColorTableFloat();

    //    ParsingMesh myParsingObj(argv[1], face_color, color_table);
    //    Photographer photographerParsing(&myParsingObj);
    //    photographerParsing.setShader(Shader::ShaderTypes::MOSAIC_SHADER, Shader::ShaderTypes::MOSAIC_SHADER);

    //--------------------------------------------------------------------------------------
    //    //Photographer photographerVisual(&object);
    //    //photographer.addCameraToPosition(0.0f, 0.0f, 1.0f, 4.0f);
    //    //photographer.addCameraToPosition(1.0f, -0.5f, 2.0f, 4.0f);
    //    //photographer.addCameraToPosition(-1.0f, 0.0f, 1.0f, 4.0f);
    //    //photographer.addCameraToPosition(-0.0f, -0.0f, -1.0f, 4.0f);
    //    //photographer.addCameraToPosition(-1.0f, 0.5f, -2.0f, 4.0f);
    //    //photographer.addCameraToPosition(1.0f, -0.0f, -1.0f, 4.0f);

    //    //photographerVisual.setShader(Shader::ShaderTypes::FULL_SHADER, Shader::ShaderTypes::FULL_SHADER);
    //    //photographerVisual.renderToImages(argv[5]);
    //    //photographerVisual.saveImageCamerasParamsCV(argv[5]);
    //    //photographerParsing.viewScene(true);
    //
}