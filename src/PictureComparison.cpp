#include "../header/PictureComparison.h"

PictureComparison::PictureComparison(int face_size) : face_size_(face_size)
{
    face_color_counter_.resize(face_size_);
    face_color_.resize(face_size_);
}

PictureComparison::PictureComparison(int face_size, const std::string & tex_pic_name, const std::string & tex_faceid_name): face_size_(face_size)
{
    setTex(tex_pic_name, tex_faceid_name);

    if ((tex_picture_.width != tex_faceid_.width) || (tex_picture_.height != tex_faceid_.height)) {
        std::cout << "PictureComparison( ... ) has different size pictures" << std::endl;
        exit(1);
    }

    face_color_counter_.resize(face_size_);
    face_color_.resize(face_size_);
}

PictureComparison::~PictureComparison() {
    //for (auto k = 0; k < face_size_; ++k) {
    //    delete[] face_color_counter_;
    //}

    //delete[] face_color_counter_;
}

void PictureComparison::setColorTable(const std::string& full_path)
{
    std::vector<std::string> token_list;
    string in_line;
    ifstream in;
    in.open(full_path);
    
    sort_size_ = 0;
    color_table_.clear();

    while (getline(in,in_line)) {
        token_list = mg::ssplit(in_line, ',');
        color_table_.insert(pair<string, array<unsigned char, 3>>{token_list[0], array<unsigned char, 3>{
            (unsigned char)atoi(token_list[1].c_str()), (unsigned char)atoi(token_list[2].c_str()), (unsigned char)atoi(token_list[3].c_str())}});
        ++sort_size_;
    }
    in.close();

}

int PictureComparison::decodeNormalizedNumeralSystem(glm::ivec3 val, int from)
{
    int id;
    id = val[0] * from * from + val[1] * from + val[2] - 1;

    return id;
}

void PictureComparison::computeIdxColor()
{
    std::string color_name;
    glm::ivec3 color, encoded_id;
    int id, full_size;
    
    full_size = tex_picture_.width * tex_picture_.height * 3; //r,g,b == 3

    for (int k = 0; k < full_size; k += 3) {
        for (int l = 0; l < 3; ++l) {
            color[l] = (int)tex_picture_.data[k/3];
            encoded_id[l] = (int)tex_faceid_.data[k + l];
        }

        color_name = getSynchronizedColorName_(color[0], color[1], color[2]);

        try {
            if (color_name == "ERROR") { // ERROR is occuring because of expanding picture
                //need nearest neighbor(preserve hard edges) like photoshop
                //cout << "void PictureComparison::computeIdxColor() ERROR: There is color not included in color table" << endl;
                throw k;
            }
        }
        catch(int expn){
            //cout << "Error at " << expn << endl;
            //cout << (int)color[0] << ", " << (int)color[1] << ", " << (int)color[2] << endl;
            color_name = "Unknown";
        }

        id = decodeNormalizedNumeralSystem(encoded_id);

        if (id < 0) { //background(no face)
            continue;
        }

        try {
            if (id > face_size_) {
                throw(id);
            }
            ++face_color_counter_[id][color_name];
        }
        catch(int expn){
            std::cout <<"PictureComparison::computeIdColor(), ERROR IDX: " << id << std::endl;
        }

    }

    extractFaceColor_();
}

void PictureComparison::setTex(const std::string & tex_pic_name, const std::string & tex_faceid_name)
{
    GeneralMeshTexture::readTex(tex_picture_, tex_pic_name);
    GeneralMeshTexture::readTex(tex_faceid_, tex_faceid_name);
}

map<string, array<float, 3>> PictureComparison::getColorTableFloat() const
{
    map<string, array<float, 3>> ret;

    for (auto iter : color_table_) {
        string name = iter.first;
        array<float, 3 > rgb = array<float, 3>{(float)((int)iter.second[0])/255.0f, (float)((int)iter.second[1]) / 255.0f, (float)((int)iter.second[2]) / 255.0f};
        
        ret.insert(pair<string, array<float, 3>>{name, rgb});
    }

    return ret;
}

std::string PictureComparison::getSynchronizedColorName_(unsigned char r, unsigned char g, unsigned char b)
{
    std::string ret;
    ret = "ERROR"; // default

    for (auto &color_chip: color_table_) {
        auto& colors = color_chip.second;
        if ((colors[0] == r) && (colors[1] == g) && (colors[2] == b)) {
            ret = color_chip.first;
            return ret;
        }
    }
    return ret;
}

void PictureComparison::initFaceColorTable()
{
    string color_name;

    for (auto k = 0; k < face_size_; ++k) {
        for_each(color_table_.begin(), color_table_.end(), [&](std::pair<string, array<unsigned char, 3>>&& color_chip) {
            auto& colors = color_chip.second;
            color_name = color_chip.first;
            face_color_counter_[k].insert(pair<string, int>{color_name, 0});
            });
    }
}

void PictureComparison::extractFaceColor_()
{
    for (int i = 0; i < face_size_; ++i) {
        string max_color_name = "Unknown";
        int max_cnt = 0;
        for_each(face_color_counter_[i].begin(), face_color_counter_[i].end(), [&](pair<string, int> color_chip) {
            if (color_chip.first.compare(string{ "Unknown" }) != 0 && (color_chip.first.compare(string{ "BackGround" }) != 0)) {
                if (max_cnt < color_chip.second) {
                    max_color_name = color_chip.first;
                    max_cnt = color_chip.second;
                }
            }
        });
        face_color_[i] = max_color_name;
    }
}
