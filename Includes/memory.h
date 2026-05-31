#pragma once
#ifndef MEMORY_H
#define MEMORY_H

#include <array>
#include <cstdint>

#define MEMORY_SIZE 65535

class Memory {
	public:
		Memory() = default;
	
	public:
		std::array<uint16_t, MEMORY_SIZE> ins { 0xC07 };
		std::array<int16_t, MEMORY_SIZE> data {};
};

#endif
