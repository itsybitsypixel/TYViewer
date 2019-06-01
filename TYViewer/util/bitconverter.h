#pragma once

#include <stdint.h>
#include <climits>
#include <memory>

template <typename T>
T swap_endian(T u)
{
	static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

	union
	{
		T u;
		unsigned char u8[sizeof(T)];
	} source, dest;

	source.u = u;

	for (size_t k = 0; k < sizeof(T); k++)
		dest.u8[k] = source.u8[sizeof(T) - k - 1];

	return dest.u;
}

template<typename T>
T from_bytes(const char* buffer, size_t offset, bool big_endian = false)
{
	T t_buf = 0;
	memcpy(&t_buf, buffer + offset, sizeof(T));

	if (big_endian)
		t_buf = swap_endian<T>(t_buf);

	return t_buf;
}

inline float byte_to_single(const char* buffer, size_t offset)
{
	uint8_t b = from_bytes<uint8_t>(buffer, offset);

	//return (b < 128) ? b / 127.0f : (b > 128) ? -((256 - b) / 127.0f) : 0.0f;
	return (b / 128.0f);
}