#pragma once
//
//usage: 
//1. myPictureComparison([n_face])  //
//2. setColorTable(...)             //some colors that your classified image file has
//3. initFaceColorTable();
//4. In a for-loop, setTex([classfied image file], [face idx]) // for-loop must be ended when you merge all multiview images.
//5.                computeIdxColor();
//6.                save getFaceColor() to face color cache (you will use majority voting at ParsingMesh)
//7. get color table for ParsingMesh

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <array>
#include <map>
#include <algorithm> // for for_each

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <stb/stb_image_resize.h>

#include <GeneralMesh.h>
#include <GeneralMeshTexture.h>

#include <GeneralUtility.h>


using namespace std;

class PictureComparison {
public:
    PictureComparison(int face_size);
    PictureComparison(int face_size, const std::string & tex_pic_name, const std::string & tex_faceid_name);
    ~PictureComparison();

    void setColorTable(const std::string& full_path);
    int decodeNormalizedNumeralSystem(glm::ivec3 val, int from = 255); // base is decimal
    void computeIdxColor();

    void setTex(const std::string & tex_pic_name, const std::string & tex_faceid_name);

    vector<string> getFaceColor() { return face_color_; }
    map<string, array<unsigned char, 3>> getColorTable() const { return color_table_; };
    map<string, array<float, 3>> getColorTableFloat() const;
    void initFaceColorTable();

private:
    int face_size_;
    int sort_size_;
    GeneralMeshTexture::TextureInfo tex_picture_, tex_faceid_;

    map<string, array<unsigned char, 3>> color_table_;
    
    int* id_table_;
    vector<map<string, int>> face_color_counter_;
    vector<string> face_color_;

    std::string getSynchronizedColorName_(unsigned char r, unsigned char g, unsigned char b);

    void extractFaceColor_();
};