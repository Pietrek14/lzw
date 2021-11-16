#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include "lzw/lzw.hpp"

std::string read_source(std::string path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		std::cout << "File not found" << std::endl;
		return "";
	}

	std::ostringstream sstr;
	sstr << file.rdbuf();
	std::string source = sstr.str();

	file.close();

	return source;
}

void save_compressed_binary(std::string path, std::vector<int> compressed)
{
	compressed.push_back(-1);

	std::ofstream output(path, std::ofstream::binary);

	output.write(reinterpret_cast<const char *>(compressed.data()), sizeof(int) * compressed.size());

	output.close();
}

void save_compressed_text(std::string path, std::vector<int> compressed)
{
	std::ofstream output(path);

	for (int i = 0; i < compressed.size(); i++)
	{
		output << compressed[i] << std::endl;
	}
}

std::vector<int> read_compressed_binary(std::string path)
{
	std::ifstream input(path, std::ifstream::binary);

	std::vector<int> compressed;

	int value = 0;

	while (true)
	{
		input.read(reinterpret_cast<char *>(&value), sizeof(int));
		
		if(value == -1)
			break;
		
		compressed.push_back(value);
	}

	input.close();

	return compressed;
}

std::vector<int> read_compressed_text(std::string path)
{
	std::ifstream file(path);

	std::vector<int> data;
	std::string line;

	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::vector<int> values((std::istream_iterator<int>(iss)), std::istream_iterator<int>());
		data.insert(data.end(), values.begin(), values.end());
	}

	file.close();

	return data;
}

int main()
{
	std::cout << "What do you want to do? (compress source - 1, decompress - 2)" << std::endl;

	int mode;
	std::cin >> mode;

	if (mode == 1)
	{
		std::cout << "Enter source path: ";
		std::string source_path;
		std::cin >> source_path;

		std::cout << "Enter destination path: ";
		std::string destination_path;
		std::cin >> destination_path;

		std::string source = read_source(source_path);

		std::vector<int> compressed = LZW::compress(source);

		int save_mode;
		std::cout << "Save as binary? (1 - yes, 2 - no)" << std::endl;
		std::cin >> save_mode;

		if (save_mode == 1)
		{
			save_compressed_binary(destination_path, compressed);
		}
		else if (save_mode == 2)
		{
			save_compressed_text(destination_path, compressed);
		}
		else
		{
			std::cout << "Wrong mode" << std::endl;
		}
	}
	else if (mode == 2)
	{
		std::cout << "Enter source path: ";
		std::string source_path;
		std::cin >> source_path;

		std::vector<int> data;

		int read_mode;
		std::cout << "Read as binary? (1 - yes, 2 - no)" << std::endl;
		std::cin >> read_mode;

		if (read_mode == 1)
		{
			data = read_compressed_binary(source_path);
		}
		else if (read_mode == 2)
		{
			data = read_compressed_text(source_path);
		}
		else
		{
			std::cout << "Wrong mode" << std::endl;
		}

		std::cout << LZW::decompress(data) << std::endl;
	}
	else
	{
		std::cout << "Invalid mode" << std::endl;
	}

	return 0;
}