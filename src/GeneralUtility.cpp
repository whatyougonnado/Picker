#include <GeneralUtility.h>
#include "..\header\GeneralUtility.h"

mg::ImageExtension mg::getType(const std::string & full_path)
{
	std::vector<std::string> token_list = ssplit(full_path, '.');
	std::string type = token_list.back();
	ImageExtension extension;

	for_each(type.begin(), type.end(), [](char &chr) {chr = tolower(chr); });

	if (type.compare("bmp")) {
		extension = ImageExtension::BMP;
	}
	else if (type.compare("jpg") || type.compare("jpeg")) {
		extension = ImageExtension::JPG;
	}
	else if (type.compare("png")) {
		extension = ImageExtension::PNG;
	}
	else {
		extension = ImageExtension::UNKNOWN;
	}

	return extension;
}

std::vector<std::string> mg::ssplit(std::string input, char criteria)
{
	std::vector<std::string> ret;
	std::istringstream ss(input);
	std::string token;

	while (std::getline(ss, token, criteria)) {
		ret.push_back(token);
	}

	return ret;
}

std::vector<std::vector<std::string>> mg::readCSV(const std::string & full_path)
{
	std::ifstream file;
	std::string in_line;
	std::vector<std::vector<std::string> > csv_contents;

	file.open(full_path);

	if (!file.is_open()) {
		std::cout << "std::vector<std::string> mg::readCSV(...): " << std::endl;
		std::cout << "Error opening file: " << full_path << std::endl;
		exit(1);
	}

	while (getline(file, in_line)) {
		csv_contents.push_back(ssplit(in_line, ','));
	}

	return csv_contents;
}

std::vector<std::string> mg::readTxt(const std::string & full_path)
{
	std::ifstream file;
	std::string in_line;
	std::vector<std::string> cache;

	file.open(full_path);

	if (!file.is_open()) {
		std::cout << "mg::readTxt(...): " << std::endl;
		std::cout << "Error opening file: " << full_path << std::endl;
		exit(1);
	}

	while (getline(file, in_line)) {
		cache.push_back(in_line);
	}

	return cache;
}

bool mg::saveTexData(std::shared_ptr<ImageInfo> image_info, const std::string & full_path)
{
	if (checkFileExist(full_path.c_str())) {
		std::cout << "mg::saveTexData(,): " + full_path + "  file exist" << std::endl;

		return 1;
	}

	bool isFail;
	ImageExtension extension = getType(full_path);
	switch (extension)
	{
	case ImageExtension::BMP:
		isFail = (bool)stbi_write_bmp(full_path.c_str(), image_info->after_width, image_info->after_height, image_info->after_n_channels, image_info->after_data);
		break;
	case ImageExtension::JPG:
		isFail = (bool)stbi_write_jpg(full_path.c_str(), image_info->after_width, image_info->after_height, image_info->after_n_channels, image_info->after_data, 0);
		break;
	case ImageExtension::PNG:
		isFail = (bool)stbi_write_png(full_path.c_str(), image_info->after_width, image_info->after_height, image_info->after_n_channels, image_info->after_data, 0);
		break;
	default:
		break;
	}
	return isFail;
}

bool mg::writeTxt(std::vector<std::string> cache_list, const std::string & full_path)
{
	std::ofstream file(full_path);

	for (auto &iter : cache_list) {
		file << iter << std::endl;
	}

	return true;
}

std::string mg::convertExtension(std::string filename, std::string extension)
{
	std::vector<std::string> token_list = ssplit(filename, '.');
	int extension_size = token_list.back().size();

	std::string converted = filename.substr(0, filename.size() - extension_size);
	converted += extension;

	return converted;
}

bool mg::checkFileExist(const char *filename)
{
	std::ifstream infile(filename);
	return infile.good();
}

