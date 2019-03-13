#pragma once

#include <vector>

class BitConverter
{
public:
	static int8_t toInt8(const char* buffer, size_t position);
	static int16_t toInt16(const char* buffer, size_t position);
	static int32_t toInt32(const char* buffer, size_t position);
	static int64_t toInt64(const char* buffer, size_t position);

	static uint8_t toUInt8(const char* buffer, size_t position);
	static uint16_t toUInt16(const char* buffer, size_t position);
	static uint32_t toUInt32(const char* buffer, size_t position);
	static uint64_t toUInt64(const char* buffer, size_t position);

	static float toSingle(const char* buffer, size_t position);
	static double toDouble(const char* buffer, size_t position);


	static float toSingleFromByte(const unsigned char byte);
};