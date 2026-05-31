#include <iostream>
#include "../Includes/cache.h"

bool L1Cache::isValidInstruction(uint16_t pc) {
	if (ins.valid[((pc & 0xFFFC) >> 2)] && (ins.tag[((pc & 0xFFFC) >> 2)] == (pc & 0x3))) return true; 
	else return false;
}

uint16_t L1Cache::getValidInstruction(uint16_t pc) {
	return ins.ins[(pc & 0xFFFC) >> 2];
}

void L1Cache::handleCacheMiss(uint16_t i, uint16_t pc) {
	ins.valid[((pc & 0xFFFC) >> 2)] = true;
	ins.tag[((pc & 0xFFFC) >> 2)] = pc & 0x3;
	ins.ins[((pc & 0xFFFC) >> 2)] = i; 
}

void L1Cache::handleCacheMiss(int16_t d, uint16_t adr) {
	data.valid[((adr & 0xFFFC) >> 2)] = true;
	data.tag[((adr & 0xFFFC) >> 2)] = adr & 0x3;
	data.data[((adr & 0xFFFC) >> 2)] = d; 		
}

void L1Cache::dataCacheStorage(int16_t d, uint16_t adr) {
	
}

bool L1Cache::isValidData(uint16_t adr) {
	if (data.valid[((adr & 0xFFFC) >> 2)] && (data.tag[((adr & 0xFFFC) >> 2)] == (adr & 0x3))) return true; 
	else return false;
}

int16_t L1Cache::getValidData(uint16_t adr) {
	return data.data[(adr & 0xFFFC) >> 2];
}
