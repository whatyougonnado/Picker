#include <BoundingBox.h>

BoundingBox::BoundingBox() {
	init_();
}

BoundingBox::BoundingBox(const std::string& imagename) {
	init_();
	setTexData(imagename);
}
BoundingBox::~BoundingBox() {
	delete[] image_info_->before_data;
	delete[] image_info_->after_data;
}

bool BoundingBox::checkFileExist_(const char * filename){
	std::ifstream infile(filename);
	return infile.good();
}

void BoundingBox::init_() {
	//init_ state-machine
	stbi_flip_vertically_on_write(false);
	stbi_set_flip_vertically_on_load(false);

	std::shared_ptr<Visualizer::ImageInfo> _temp(new Visualizer::ImageInfo);
	std::swap(image_info_, _temp);
}
int BoundingBox::saveTexBounded(const std::string& save_imagename) {
	enum _Color {
		R = 0, G, B
	};
	int fail;
	std::array<int, 3> padding_pixel;

	padding_pixel[R] = padding_pixel[G] = padding_pixel[B] = 0;
	getMargin_(padding_pixel);
	removePadding_();

	fail = Visualizer::saveTexData(image_info_, save_imagename);

	return fail;
}

void BoundingBox::getMargin_(std::array<int, 3> lead_pixel) {
	enum _Color {
		R = 0, G, B
	};
	//left
	{
		bool isSameColor = true;
		int col;
		for (col = 0; col < (image_info_->before_width * image_info_->before_n_channels) && isSameColor; col += 3) {
			for (int j = 0; (j < image_info_->before_height) && isSameColor; ++j) {
				int local_R_idx = j * image_info_->before_width * image_info_->before_n_channels + (col + 0);
				int local_G_idx = local_R_idx + 1;
				int local_B_idx = local_R_idx + 2;

				int pixel_R = (int)image_info_->before_data[local_R_idx];
				int pixel_G = (int)image_info_->before_data[local_G_idx];
				int pixel_B = (int)image_info_->before_data[local_B_idx];

				if (!((pixel_R == lead_pixel[R]) && (pixel_G == lead_pixel[G]) && (pixel_B == lead_pixel[B]))) {
					isSameColor = false;
				}
			}
		}
		col -= 3;
		margin_.left = col / image_info_->before_n_channels;
	}

	//right
	{
		bool isSameColor = true;
		int col;
		for (col = (image_info_->before_width - 1) * image_info_->before_n_channels; (col >= 0) && isSameColor; col -= image_info_->before_n_channels) {
			for (int j = 0; (j < image_info_->before_height) && isSameColor; ++j) {
				int local_R_idx = j * image_info_->before_width * image_info_->before_n_channels + (col + 0);
				int local_G_idx = local_R_idx + 1;
				int local_B_idx = local_R_idx + 2;

				int pixel_R = (int)image_info_->before_data[local_R_idx];
				int pixel_G = (int)image_info_->before_data[local_G_idx];
				int pixel_B = (int)image_info_->before_data[local_B_idx];

				if (!((pixel_R == lead_pixel[R]) && (pixel_G == lead_pixel[G]) && (pixel_B == lead_pixel[B]))) {
					isSameColor = false;
				}
			}
		}
		col += image_info_->before_n_channels;
		margin_.right = ((image_info_->before_width - 1) * image_info_->before_n_channels - col) / image_info_->before_n_channels;
	}

	//top
	{
		bool isSameColor = true;

		int row;
		for (row = 0; (row < image_info_->before_height) && isSameColor; ++row) {
			for (int i = 0; (i < image_info_->before_width * image_info_->before_n_channels) && isSameColor; i += image_info_->before_n_channels) {
				int local_R_idx = row * image_info_->before_width * image_info_->before_n_channels + (i + 0);
				int local_G_idx = local_R_idx + 1;
				int local_B_idx = local_R_idx + 2;

				int pixel_R = (int)image_info_->before_data[local_R_idx];
				int pixel_G = (int)image_info_->before_data[local_G_idx];
				int pixel_B = (int)image_info_->before_data[local_B_idx];

				if (!((pixel_R == lead_pixel[R]) && (pixel_G == lead_pixel[G]) && (pixel_B == lead_pixel[B]))) {
					isSameColor = false;
				}
			}
		}
		--row;
		margin_.top = row;
	}

	//bottom
	{
		bool isSameColor = true;
		int row;
		for (row = image_info_->before_height - 1; (row >= 0) && isSameColor; --row) {
			for (int i = 0; (i < image_info_->before_width * image_info_->before_n_channels) && isSameColor; i += image_info_->before_n_channels) {
				int local_R_idx = row * image_info_->before_width * image_info_->before_n_channels + (i + 0);
				int local_G_idx = local_R_idx + 1;
				int local_B_idx = local_R_idx + 2;

				int pixel_R = (int)image_info_->before_data[local_R_idx];
				int pixel_G = (int)image_info_->before_data[local_G_idx];
				int pixel_B = (int)image_info_->before_data[local_B_idx];

				if (!((pixel_R == lead_pixel[R]) && (pixel_G == lead_pixel[G]) && (pixel_B == lead_pixel[B]))) {
					isSameColor = false;
				}
			}
		}
		++row;
		margin_.bottom = (image_info_->before_height - 1) - row;
	}

	//std::cout << margin_.top << " " << margin_.bottom << " " << margin_.left << " " << margin_.right << std::endl;
}
void BoundingBox::removePadding_() {
	setTexInfoBounded_();

	int row;
	int cnt = 0;
	for (row = margin_.top; (row < margin_.top + image_info_->after_height); ++row) {
		for (int i = margin_.left * image_info_->after_n_channels; (i < (margin_.left + image_info_->after_width) * image_info_->after_n_channels); i += image_info_->after_n_channels) {
			int local_R_idx = row * image_info_->before_width * image_info_->after_n_channels + (i + 0);
			int local_G_idx = local_R_idx + 1;
			int local_B_idx = local_R_idx + 2;

			unsigned char pixel_R = image_info_->before_data[local_R_idx];
			unsigned char pixel_G = image_info_->before_data[local_G_idx];
			unsigned char pixel_B = image_info_->before_data[local_B_idx];

			image_info_->after_data[cnt++] = pixel_R;
			image_info_->after_data[cnt++] = pixel_G;
			image_info_->after_data[cnt++] = pixel_B;
		}
	}

}

int BoundingBox::setTexData(const std::string& imagename) {

	if (!checkFileExist_(imagename.c_str())) {
		throw std::exception("BoundingBox: input file doesn't exist");

		return 1;
	}

	
	
	image_info_->before_data = stbi_load(imagename.c_str(), &(image_info_->before_width), &(image_info_->before_height), &(image_info_->before_n_channels), 0);

	return 0;
}
void BoundingBox::setTexInfoBounded_() {
	image_info_->after_width = image_info_->before_width - (margin_.left + margin_.right);
	image_info_->after_height = image_info_->before_height - (margin_.top + margin_.bottom);
	image_info_->after_n_channels = image_info_->before_n_channels;
	image_info_->after_data = new unsigned char[image_info_->after_width * image_info_->after_height * image_info_->after_n_channels];
}
int BoundingBox::saveTexData_(const std::string& save_imagename) {
	if (checkFileExist_(save_imagename.c_str())) {
		std::cout << "BoundingBox: " + save_imagename + "  file exist" << std::endl;

		return 1;
	}

	Visualizer::ImageExtension extension = Visualizer::getType(save_imagename);
	int fail = 1;

	switch (extension)
	{
	case Visualizer::ImageExtension::BMP:
		break;
	case Visualizer::ImageExtension::JPG:
		break;
	case Visualizer::ImageExtension::PNG:
		fail = stbi_write_png(save_imagename.c_str(), image_info_->after_width, image_info_->after_height, image_info_->after_n_channels, image_info_->after_data, 0);

		break;
	default:
		break;
	}
	return fail;
}