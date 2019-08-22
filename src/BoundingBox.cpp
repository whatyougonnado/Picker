#include <BoundingBox.h>

BoundingBox::BoundingBox() {
	init();
}

BoundingBox::BoundingBox(const std::string& imagename) {
	init();
	setTexData1_(imagename);
}
BoundingBox::~BoundingBox() {
	delete[] image_info_->before_data;
	delete[] image_info_->after_data;
}

bool BoundingBox::checkFileExist_(const char * filename){
	std::ifstream infile(filename);
	return infile.good();
}

void BoundingBox::init() {
	//init state-machine
	stbi_flip_vertically_on_write(false);
	stbi_set_flip_vertically_on_load(false);

	std::shared_ptr<ImageInfo> _temp(new ImageInfo);
	std::swap(image_info_, _temp);
}
int BoundingBox::saveModifiedTex(const std::string& save_imagename) {
	enum _Color {
		R = 0, G, B
	};
	int fail;
	std::array<int, 3> padding_pixel;

	padding_pixel[R] = padding_pixel[G] = padding_pixel[B] = 0;
	getMargin_(padding_pixel);
	removePadding_();
	fail = setTexData2_(save_imagename);
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
		for (col = 0; col < (image_info_->before_width * image_info_->n_channels) && isSameColor; col += 3) {
			for (int j = 0; (j < image_info_->before_height) && isSameColor; ++j) {
				int local_R_idx = j * image_info_->before_width * image_info_->n_channels + (col + 0);
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
		image_info_->margin.left = col / image_info_->n_channels;
	}

	//right
	{
		bool isSameColor = true;
		int col;
		for (col = (image_info_->before_width - 1) * image_info_->n_channels; (col >= 0) && isSameColor; col -= image_info_->n_channels) {
			for (int j = 0; (j < image_info_->before_height) && isSameColor; ++j) {
				int local_R_idx = j * image_info_->before_width * image_info_->n_channels + (col + 0);
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
		col += image_info_->n_channels;
		image_info_->margin.right = ((image_info_->before_width - 1) * image_info_->n_channels - col) / image_info_->n_channels;
	}

	//top
	{
		bool isSameColor = true;

		int row;
		for (row = 0; (row < image_info_->before_height) && isSameColor; ++row) {
			for (int i = 0; (i < image_info_->before_width * image_info_->n_channels) && isSameColor; i += image_info_->n_channels) {
				int local_R_idx = row * image_info_->before_width * image_info_->n_channels + (i + 0);
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
		image_info_->margin.top = row;
	}

	//bottom
	{
		bool isSameColor = true;
		int row;
		for (row = image_info_->before_height - 1; (row >= 0) && isSameColor; --row) {
			for (int i = 0; (i < image_info_->before_width * image_info_->n_channels) && isSameColor; i += image_info_->n_channels) {
				int local_R_idx = row * image_info_->before_width * image_info_->n_channels + (i + 0);
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
		image_info_->margin.bottom = (image_info_->before_height - 1) - row;
	}

	std::cout << image_info_->margin.top << " " << image_info_->margin.bottom << " " << image_info_->margin.left << " " << image_info_->margin.right << std::endl;
}
void BoundingBox::removePadding_() {
	setTexInfo2_();

	int row;
	int cnt = 0;
	for (row = image_info_->margin.top; (row < image_info_->margin.top + image_info_->after_height); ++row) {
		for (int i = image_info_->margin.left * image_info_->n_channels; (i < (image_info_->margin.left + image_info_->after_width) * image_info_->n_channels); i += image_info_->n_channels) {
			int local_R_idx = row * image_info_->before_width * image_info_->n_channels + (i + 0);
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

int BoundingBox::setTexData1_(const std::string& imagename) {

	if (!checkFileExist_(imagename.c_str())) {
		throw std::exception("BoundingBox: input file doesn't exist");

		return 1;
	}

	
	
	image_info_->before_data = stbi_load(imagename.c_str(), &(image_info_->before_width), &(image_info_->before_height), &(image_info_->n_channels), 0);

	return 0;
}
void BoundingBox::setTexInfo2_() {
	image_info_->after_width = image_info_->before_width - (image_info_->margin.left + image_info_->margin.right);
	image_info_->after_height = image_info_->before_height - (image_info_->margin.top + image_info_->margin.bottom);
	image_info_->after_data = new unsigned char[image_info_->after_width * image_info_->after_height * image_info_->n_channels];
}
int BoundingBox::setTexData2_(const std::string& save_imagename) {
	int fail;
	fail = stbi_write_png(save_imagename.c_str(), image_info_->after_width, image_info_->after_height, image_info_->n_channels, image_info_->after_data, 0);
	return fail;
}