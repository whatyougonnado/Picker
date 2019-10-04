#include <iostream>

#include <GeneralUtility.h>
#include <Photographer.h>

#include <GeneralMesh.h>
#include <GeneralMeshIdx.h>
#include <GeneralMeshTexture.h>
#include <ParsingMesh.h>

#include <BoundingBox.h>
#include <PictureComparison.h>
#include <OBJController.h>
#include <Picking.h>

std::string obj_path, obj_list_name, tex_path, tex_list_name, root_path;
std::vector <std::string> obj_list, tex_list;
int n_segment = 36;
bool is_front;

void preprocessing_front() {
    std::cout << "\n\n//---------------------------------\n Preprocessing Front, start!\n//---------------------------------\n\n";
    Photographer myPhotographer;
    myPhotographer.addCameraRingRoutine(n_segment, 0.0, 2.0f);

    for (auto i = 0; i < obj_list.size(); ++i) {
        std::cout << ">>> The Mesh '" + obj_list[i] + "' is loaded!" << std::endl;

        std::string image_path = root_path + mg::ssplit(obj_list[i], '.')[0] + "/";
        std::string obj_full_path = obj_path + obj_list[i];
        std::string tex_full_path = tex_path + tex_list[i];

        std::string obj_name = mg::ssplit(obj_list[i], '.')[0];
        std::string prefix = obj_name + "_";


        //--------------------------------------------------------------------------------------------------------------------------
        GeneralMeshTexture myGeneralMeshTexture(obj_full_path.c_str(), tex_full_path.c_str());

        myPhotographer.setInit(&myGeneralMeshTexture);
        myPhotographer.setShader(Shader::ShaderTypes::TEXTURE_SHADER);
        myPhotographer.saveImageCamerasParamsCV(image_path, prefix);
        std::vector<std::string> texmesh_name_list = myPhotographer.renderToImages(image_path, "texmesh_view_" + prefix);
        mg::writeTxt(texmesh_name_list, image_path + "texmesh_name_list.txt");
        //myPhotographer.viewScene(true);
        //--------------------------------------------------------------------------------------------------------------------------
        GeneralMeshIdx myGeneralMeshID(obj_full_path.c_str());
        myPhotographer.setObject(&myGeneralMeshID);

        myPhotographer.setShader(Shader::ShaderTypes::FACEIDX_SHADER);
        myPhotographer.saveImageCamerasParamsCV(image_path, prefix);
        std::vector<std::string> id_name_list = myPhotographer.renderToImages(image_path, "id_view_" + prefix);
        mg::writeTxt(id_name_list, image_path + "id_name_list.txt");
        //myPhotographer.viewScene(true);

        ////--------------------------------------------------------------------------------------------------------------------------
        texmesh_name_list = mg::readTxt(image_path + "texmesh_name_list.txt");
        id_name_list = mg::readTxt(image_path + "id_name_list.txt");

        std::vector<std::string> bound_texmesh_name_list, bound_id_name_list, test_id_list; // test_id is the bound_texmesh_name without extension
        for (auto k = 0; k < texmesh_name_list.size(); ++k) {
            std::string load_path = image_path + texmesh_name_list[k];
            std::string bound_texmesh_name = mg::convertExtension("bd_" + texmesh_name_list[k], "jpg"); // <- need to convert jpg
            std::string test_id_name = mg::convertExtension("bd_" + texmesh_name_list[k], ""); // <- need to convert (blank)
            std::string save_path = image_path + bound_texmesh_name; // <- need to convert jpg
            BoundingBox myBoundingbox(load_path);
            myBoundingbox.saveTexBounded(save_path);
            bound_texmesh_name_list.push_back(bound_texmesh_name);
            test_id_list.push_back(test_id_name);

            load_path = image_path + id_name_list[k];
            std::string bound_id_name = "bd_" + id_name_list[k]; // <- need to convert jpg
            save_path = image_path + bound_id_name; // <- need to convert jpg
            BoundingBox myBoundingbox2(load_path);
            myBoundingbox2.saveTexBounded(save_path);
            bound_id_name_list.push_back(bound_id_name);
        }

        mg::writeTxt(bound_texmesh_name_list, image_path + "bd_texmesh_name_list.txt");
        mg::writeTxt(bound_texmesh_name_list, image_path + "/ce2p/" + "bd_texmesh_name_list.txt");
        mg::writeTxt(test_id_list, image_path + "/ce2p/" + "test_id.txt");
        mg::writeTxt(bound_id_name_list, image_path + "bd_id_name_list.txt");
    }


    std::cout << "\n\n//---------------------------------\n Preprocessing Front, done!\n//---------------------------------\n\n";
}

void preprocessing_back() {
    std::cout << "\n\n//---------------------------------\n Preprocessing Back, start!\n//---------------------------------\n\n";
    std::vector<std::string> ce2p_texmesh_name_list, bound_id_name_list;

    for (auto i = 0; i < obj_list.size(); ++i) {
        std::cout << ">>> The Mesh '" + obj_list[i] + "' is loaded!" << std::endl;

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
        myPictureComparison.setColorTable(tex_path + "coloring_table_ce2p.txt");
        myPictureComparison.initFaceColorTable();

        for (int k = 0; k < 36; ++k) {
            std::string ce2p_texmesh_full_path = mg::convertExtension(parsing_path + ce2p_texmesh_name_list[k], "png");
            std::string bound_id_full_path = image_path + bound_id_name_list[k];

            myPictureComparison.setTex(ce2p_texmesh_full_path, bound_id_full_path);
            myPictureComparison.computeIdxColor();

            face_color = myPictureComparison.getFaceColor();

            std::cout << "  Merge (" << k+1 << "/" << n_segment << ") Picture" << std::endl;
        }
        myPictureComparison.setColorTable(tex_path + "coloring_table_visualization.txt");
        color_table = myPictureComparison.getColorTableFloat();

        ParsingMesh myParsingObj(obj_full_path.c_str(), face_color, color_table);
        myParsingObj.saveOBJ(tex_path + "uv_coloring.txt", obj_path + "Parsing" + obj_list[i]);
    }
    std::cout << "\n\n//---------------------------------\n Preprocessing Back, done!\n//---------------------------------\n\n";
}

void test() {
    std::cout << "\n\n//---------------------------------\n Test, start!\n//---------------------------------\n\n";
    for (auto i = 0; i < obj_list.size(); ++i) {
        std::cout << ">>> The Mesh '" + obj_list[i] + "' is loaded!" << std::endl;

        std::string obj_full_path_test = obj_path + "Parsing" + obj_list[i];
        std::string tex_full_path_test = tex_path + "coloring_visualization.jpg";

        GeneralMeshTexture myGeneralMeshTextureTest(obj_full_path_test.c_str(), tex_full_path_test.c_str());

        Photographer myPhotographer;
        myPhotographer.setInit(&myGeneralMeshTextureTest);
        myPhotographer.setShader(Shader::ShaderTypes::TEXTURE_SHADER);
        myPhotographer.viewScene(true);
    }

    std::cout << "\n\n//---------------------------------\n Test, done!\n//---------------------------------\n\n";
}

int main(int argc, char *argv[])
{

    ////initialize path and flag
    obj_path = argv[1]; obj_path += "/";
    obj_list_name = argv[2];
    tex_path = argv[3]; tex_path += "/";
    tex_list_name = argv[4];
    root_path = argv[5]; root_path += "/";
    bool is_front = std::stoi(std::string(argv[6]));
    
    obj_list = mg::readTxt(obj_path + obj_list_name);
    tex_list = mg::readTxt(tex_path + tex_list_name);
    ////

    if (is_front) {
        preprocessing_front();
    }
    
    ////--------------------------------------------------------------------------------------
    ////preprocessing_middle()
    ////CE2P
    ////
    ////--------------------------------------------------------------------------------------

    if (!is_front) {
        preprocessing_back();
        test();
    }

    return 0;
}