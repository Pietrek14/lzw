#include "lzw.hpp"

int find(std::vector<std::string> dict, std::string value)
{
	for (int i = 0; i < dict.size(); i++)
	{
		if (dict[i] == value)
			return i;
	}

	return -1;
}

std::vector<int> LZW::compress(std::string input)
{
	std::vector<int> result;

	if (input == "")
		return result;

	std::string c;
	int index = 0;
	std::vector<std::string> dict;

	for (unsigned char i = 1; i < 127; i++)
	{
		dict.push_back(std::string(1, i));
	}

	c = input[index];

	while (index < input.length() - 1)
	{
		index++;
		char s = input[index];
		std::string sum = c + s;

		if (find(dict, sum) != -1)
		{
			c = c + s;
			continue;
		}

		result.push_back(find(dict, c));
		dict.push_back(sum);
		c = s;
	}

	if (c[0] != 0)
		result.push_back(find(dict, c));

	return result;
}

std::string LZW::decompress(std::vector<int> input)
{
	int pk;
	std::vector<std::string> dict;
	std::string result;
	int index = 0;

	for (unsigned char i = 1; i < 127; i++)
	{
		dict.push_back(std::string(1, i));
	}

	pk = input[index];

	result += dict[pk];

	while (index < input.size() - 1)
	{
		index++;
		int k = input[index];
		std::string pc = dict[pk];

		if (k < dict.size())
		{
			dict.push_back(pc + dict[k][0]);
			result += dict[k];
		}
		else
		{
			dict.push_back(pc + pc[0]);
			result += pc + pc[0];
		}

		pk = k;
	}

	return result;
}