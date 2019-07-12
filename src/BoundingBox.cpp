#include <BoundingBox.h>

BoundingBox::BoundingBox() {
	std::shared_ptr<ImageInfo> _temp(new ImageInfo);
	std::swap(image_info_, _temp);

	image_info_->margin.left = image_info_->margin.right = image_info_->margin.top = image_info_->margin.bottom = -1;
}

BoundingBox::~BoundingBox() {
	delete[] image_info_->before_data;
	delete[] image_info_->after_data;
}

bool BoundingBox::setTex(const std::string& imagename) {
	if (!checkFileExist_(imagename.c_str())) {
		throw std::exception("BoundingBox: input file doesn't exist");
		
		return false;
	}
	
	image_info_->before_data = stbi_load(imagename.c_str(), &(image_info_->before_width), &(image_info_->before_height), &(image_info_->n_channels), 0);

	return true;
}

bool BoundingBox::checkFileExist_(const char * filename){
	std::ifstream infile(filename);
	return infile.good();
}

int BoundingBox::saveModifiedTex(const std::string& save_imagename) {
	enum _Color {
		R = 0, G, B
	};

	removePadding_(0, 0, 0, 0);

	image_info_->after_width = image_info_->before_width - (image_info_->margin.left + image_info_->margin.right);
	image_info_->after_height = image_info_->before_height - (image_info_->margin.top + image_info_->margin.bottom);

	image_info_->after_data = new unsigned char[image_info_->after_width * image_info_->after_height * image_info_->n_channels];

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

	int success = stbi_write_png(save_imagename.c_str(), image_info_->after_width, image_info_->after_height, image_info_->n_channels, image_info_->after_data, 0);

	return success;
}

//if line is all black, consider line as padding line
void BoundingBox::removePadding_(int min_margin_left, int min_margin_right, int min_margin_top, int min_margin_bottom) {
	enum _Color {
		R = 0, G, B
	};
	std::array<int, 3> lead_pixel;

	lead_pixel[R] = 0;
	lead_pixel[G] = 0;
	lead_pixel[B] = 0;

	image_info_->margin.left = image_info_->margin.right = image_info_->margin.top = image_info_->margin.bottom = 0;


	//left
	{
		bool isSameColor = true;
		std::array<int, 3> lead_pixel;
		lead_pixel[R] = 0;
		lead_pixel[G] = 0;
		lead_pixel[B] = 0;

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
		image_info_->margin.left = col / image_info_->n_channels;
	}

	//right
	{
		bool isSameColor = true;
		std::array<int, 3> lead_pixel;
		lead_pixel[R] = 0;
		lead_pixel[G] = 0;
		lead_pixel[B] = 0;

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
		image_info_->margin.top = row;
	}

	//bottom
	{
		bool isSameColor = true;
		std::array<int, 3> lead_pixel;
		lead_pixel[R] = 0;
		lead_pixel[G] = 0;
		lead_pixel[B] = 0;

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
		image_info_->margin.bottom = (image_info_->before_height - 1) - row;
	}
}
