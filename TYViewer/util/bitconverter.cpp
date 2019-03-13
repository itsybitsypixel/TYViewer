#include "bitconverter.h"

int8_t BitConverter::toInt8(const char* buffer, size_t position)
{
	return int8_t(
		(unsigned char)(buffer[position + 0]));
}
int16_t BitConverter::toInt16(const char* buffer, size_t position)
{
	return int16_t(
		(unsigned char)(buffer[position + 1]) << 8 |
		(unsigned char)(buffer[position + 0]));
}
int32_t BitConverter::toInt32(const char* buffer, size_t position)
{
	return int32_t(
		(unsigned char)(buffer[position + 3]) << 24 |
		(unsigned char)(buffer[position + 2]) << 16 |
		(unsigned char)(buffer[position + 1]) << 8 |
		(unsigned char)(buffer[position + 0]));
}

std::int64_t BitConverter::toInt64(const char* buffer, size_t position)
{
	return int64_t(
		(unsigned long long)(buffer[position + 7]) << 56 |
		(unsigned long long)(buffer[position + 6]) << 48 |
		(unsigned long long)(buffer[position + 5]) << 40 |
		(unsigned long long)(buffer[position + 4]) << 32 |
		(unsigned char)(buffer[position + 3]) << 24 |
		(unsigned char)(buffer[position + 2]) << 16 |
		(unsigned char)(buffer[position + 1]) << 8 |
		(unsigned char)(buffer[position + 0]));
}

uint8_t BitConverter::toUInt8(const char* buffer, size_t position)
{
	return uint8_t(
		(unsigned char)(buffer[position + 0]));
}
uint16_t BitConverter::toUInt16(const char* buffer, size_t position)
{
	return uint16_t(
		(unsigned char)(buffer[position + 1]) << 8 |
		(unsigned char)(buffer[position + 0]));
}
uint32_t BitConverter::toUInt32(const char* buffer, size_t position)
{
	return uint32_t(
		(unsigned char)(buffer[position + 3]) << 24 |
		(unsigned char)(buffer[position + 2]) << 16 |
		(unsigned char)(buffer[position + 1]) << 8 |
		(unsigned char)(buffer[position + 0]));
}
uint64_t BitConverter::toUInt64(const char * buffer, size_t position)
{
	return uint64_t(
		(unsigned long long)(buffer[position + 7]) << 56 |
		(unsigned long long)(buffer[position + 6]) << 48 |
		(unsigned long long)(buffer[position + 5]) << 40 |
		(unsigned long long)(buffer[position + 4]) << 32 |
		(unsigned char)(buffer[position + 3]) << 24 |
		(unsigned char)(buffer[position + 2]) << 16 |
		(unsigned char)(buffer[position + 1]) << 8 |
		(unsigned char)(buffer[position + 0]));
}

float BitConverter::toSingle(const char* buffer, size_t position)
{
	uint32_t f = toInt32(buffer, position);
	return *reinterpret_cast<float*>(&f);
}
double BitConverter::toDouble(const char* buffer, size_t position)
{
	uint64_t d = toInt64(buffer, position);
	return *reinterpret_cast<double*>(&d);
}


float BitConverter::toSingleFromByte(const unsigned char byte)
{
	if (byte < 128) // POSITIVE VALUE
	{
		return byte / 127.0f;
	}
	if (byte > 128) // NEGATIVE VALUE
	{
		return -((256 - byte) / 127.0f);
	}
	return 0.0f;
}
