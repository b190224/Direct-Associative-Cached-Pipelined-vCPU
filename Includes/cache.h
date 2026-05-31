#pragma once
#ifndef CACHE_H
#define CACHE_H

#include <cstdint>
#include <array>

#define SIZE_OF_L1CACHE 16384

class L1Cache {
	private:
		struct CacheTableInstruction {
			std::array<bool, SIZE_OF_L1CACHE> valid {
				
			};
			std::array<uint8_t, SIZE_OF_L1CACHE> tag {
				
			};
			std::array<uint16_t, SIZE_OF_L1CACHE> ins {
				
			};
		};
		
		struct CacheTableData {
			std::array<bool, SIZE_OF_L1CACHE> valid {
				
			};
			std::array<uint8_t, SIZE_OF_L1CACHE> tag {
				
			};
			std::array<int16_t, SIZE_OF_L1CACHE> data {
				
			};
		};
	
	public:
		L1Cache() = default;
		
		bool isValidInstruction(uint16_t pc);
		uint16_t getValidInstruction(uint16_t pc);
		
		bool isValidData(uint16_t adr);
		int16_t getValidData(uint16_t adr);
		
		void handleCacheMiss(uint16_t i, uint16_t pc);
		void handleCacheMiss(int16_t d, uint16_t adr);
		void dataCacheStorage(int16_t d, uint16_t adr);
		
		CacheTableInstruction ins {};
		CacheTableData data {};
};

#endif
