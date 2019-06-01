#pragma once

#include <string>
#include <unordered_map>

#include <algorithm>

struct File
{
	std::string name = "";
	std::int32_t folder = 0;
	std::int64_t size = 0;
	std::int32_t offset = 0;
	std::int64_t date = 0;

	inline bool matches(const std::string& s) const
	{
		std::string a = name;
		std::string b = s;

		std::transform(a.begin(), a.end(), a.begin(), ::tolower);
		std::transform(b.begin(), b.end(), b.begin(), ::tolower);

		return a == b;
	}

	inline std::string extension() const
	{
		std::string res = name.substr(name.find_last_of('.') + 1);
		std::transform(res.begin(), res.end(), res.begin(), ::tolower);
		return res;
	}
};

class Archive
{
public:
	enum
	{
		UNKNOWN,
		RKV1,
		RKV2
	};

	bool load(const std::string& path);

	bool getFile(const std::string& name, File& file);
	bool getFileData(const std::string& name, std::vector<char>& data);

	std::string path;
private:
	void identify();

	void loadAsRKV1();
	void loadAsRKV2();


	
	int version;

	unsigned long size;

	std::unordered_map<std::string, File> files;
};