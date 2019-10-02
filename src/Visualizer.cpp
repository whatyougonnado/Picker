#include <Visualizer.h>

Visualizer::Visualizer() {
	init_();
}

Visualizer::Visualizer(const std::string& imagename) {
	init_();
	setTexData(imagename);
}

Visualizer::~Visualizer() {
	delete[] image_info_->before_data;
	delete[] image_info_->after_data;
}

void Visualizer::init_() {
	//init_ state-machine
	stbi_flip_vertically_on_write(false);
	stbi_set_flip_vertically_on_load(false);

	std::shared_ptr<mg::ImageInfo> _temp(new mg::ImageInfo);
	std::swap(image_info_, _temp);
}

bool Visualizer::saveTexVisualize(const std::string & reference_file_path, const std::string& save_imagename) {
	bool isFail;

	setTexInfo_();

	setReferenceTable_(reference_file_path);
	
	changeColor_();
	
	isFail = saveTexData(image_info_, save_imagename);
	return isFail;
}

void Visualizer::setTexInfo_() {
	image_info_->after_width = image_info_->before_width;
	image_info_->after_height = image_info_->before_height;
	image_info_->after_n_channels = 3; // R,G,B
	image_info_->after_data = new unsigned char[image_info_->after_width * image_info_->after_height * image_info_->after_n_channels];
}

void Visualizer::setReferenceTable_(const std::string & full_path)
{
	std::vector<std::vector<std::string> > table_element;
	table_element = mg::readCSV(full_path);
	table_.resize(table_element.size());


	//index 0: class name
	//index 1,2,3: same color values	(from)
	//index 4,5,6: new color values		(to)
	for (int i = 0; i < table_.size(); ++i) {
		std::vector<std::string> &data_chip = table_element[i];

		int color_from = std::stoi(data_chip[1]);
		int R = std::stoi(data_chip[4]);
		int G = std::stoi(data_chip[5]);
		int B = std::stoi(data_chip[6]);

		table_[color_from][0] = R;
		table_[color_from][1] = G;
		table_[color_from][2] = B;
	}
}

void Visualizer::changeColor_() {
	enum _Color {
		R = 0, G, B
	};

	for (int i = 0; i < (image_info_->before_width * image_info_->before_height * image_info_->before_n_channels); ++i) {
		int table_idx = image_info_->before_data[i];

		int colorR = table_[table_idx][R];
		int colorG = table_[table_idx][G];
		int colorB = table_[table_idx][B];

		image_info_->after_data[3 * i + 0] = colorR;
		image_info_->after_data[3 * i + 1] = colorG;
		image_info_->after_data[3 * i + 2] = colorB;
	}
}

int Visualizer::setTexData(const std::string& full_path) {

	if (!mg::checkFileExist(full_path.c_str())) {
		throw std::exception("Visualizer: input file doesn't exist");

		return 1;
	}

	image_info_->before_data = stbi_load(full_path.c_str(), &(image_info_->before_width), &(image_info_->before_height), &(image_info_->before_n_channels), 0);

	return 0;
}
