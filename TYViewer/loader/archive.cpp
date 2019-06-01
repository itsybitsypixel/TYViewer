#include "archive.h"

#include <fstream>
#include <algorithm>

#include "util/bitconverter.h"
#include "util/stringext.h"

#include "debug.h"

bool Archive::load(const std::string& path)
{
	this->path = path;

	std::ifstream stream(path, std::ios::binary | std::ios::ate);
	if (stream.fail())
	{
		Debug::log("Failed to load archive!");
		stream.close();
		return false;
	}
	size = stream.tellg();
	stream.close();

	identify();

	switch (version)
	{
	case Archive::RKV1:
		loadAsRKV1();
		return true;
		break;
	case Archive::RKV2:
		loadAsRKV2();
		return true;
		break;
	case Archive::UNKNOWN:
		Debug::log("File isn't an TY archive!");
		break;
	}

	return false;
}

bool Archive::getFile(const std::string& name, File& file)
{
	std::string key = name;
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);

	if (files.find(key) != files.end())
	{
		file = files[key];
		return true;
	}
	return false;
}

bool Archive::getFileData(const std::string& name, std::vector<char>& data)
{
	std::string key = name;
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);

	File file;
	if (files.find(key) != files.end())
	{
		file = files[key];
	}

	// File found!
	if (file.size != 0)
	{
		std::ifstream stream(path, std::ios::binary);

		data = std::vector<char>(file.size);
		stream.seekg(file.offset, std::ios::beg);
		stream.read(&data[0], file.size);

		stream.close();

		return true;
	}

	return false;
}

void Archive::identify()
{
	std::ifstream stream(path, std::ios::binary | std::ios::beg);

	std::vector<char> ext(4);
	stream.read(&ext[0], 4);

	// There's no way to identify a RKV1 archive as far as I've found,
	// so we'll assume that if the archive is not RKV2 it's RKV1.

	if (std::string(ext.begin(), ext.end()) == "RKV2")
	{
		version = Archive::RKV2;
		return;
	}
	else
	{
		version = Archive::RKV1;
	}

	stream.close();
}

void Archive::loadAsRKV1()
{
	std::ifstream stream(path, std::ios::binary | std::ios::ate);
	stream.seekg(-((std::streampos)8), std::ios::end);

	// Last 4 bytes contain information 
	// regarding filecount and foldercount
	stream.seekg(-(8), std::ios::end);

	std::vector<char> info(8);
	stream.read(&info[0], 8);

	int filecount = from_bytes<uint32_t>(info.data(), 0);
	int foldercount = from_bytes<uint32_t>(info.data(), 4);

	// Goto beginning of table.
	stream.seekg(-(8 + foldercount * 256 + filecount * 64), std::ios::end);

	for (unsigned int i = 0; i < filecount; i++)
	{
		std::vector<char> buffer(64);
		stream.read(&buffer[0], 64);


		File file;
		file.name	= nts(buffer.data(), 0, 32);
		file.folder = from_bytes<uint32_t>(buffer.data(), 32);
		file.size	= from_bytes<uint32_t>(buffer.data(), 36);
		file.offset = from_bytes<uint32_t>(buffer.data(), 44);
		file.date	= from_bytes<uint32_t>(buffer.data(), 52);

		std::string key = file.name;
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);

		files[key] = file;
	}

	stream.close();
}

void Archive::loadAsRKV2()
{

}
