#pragma once
#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <array>

#include "../Includes/memory.h"
#include "../Includes/cache.h"

#define NUM_OF_GENERAL_REGISTERS 8
#define NUM_OF_PIPELINED_REGSITERS 4

#define STALL_BIT_POS 15
#define RESUME_BIT_POS 14
#define ZERO_BIT_POS 13
#define HIT_BIT_POS 12
#define MISS_BIT_POS 11

#define FLAG_STALL(flag, stall) ((flag) |= (stall << STALL_BIT_POS))
#define FLAG_RESUME(flag, resume) ((flag) |= (resume << RESUME_BIT_POS))
#define FLAG_ZERO(flag, zero) ((flag) |= (zero << ZERO_BIT_POS))
#define FLAG_HIT(flag, hit) ((flag) |= (hit << ZERO_BIT_POS))
#define FLAG_MISS(flag, miss) ((flag) |= (miss << ZERO_BIT_POS))

#define IF_ID 0
#define ID_EX 1
#define EX_MEM 2
#define MEM_WB 3

#define NOP 0x0
#define MOV 0x5
#define LDA 0x6
#define STA 0x26

class CPU {	
	private:
		struct ControlSignals {
			bool pcwrite = true;
			bool pcread = false;
			bool regwrite = false;
			bool regread = false;
			bool memwrite = false;
			bool memread = false;
			bool immsel = false;
		};
	
		struct PipelineRegisters {
			ControlSignals cs;
			uint16_t ins;
			uint8_t op;
			int16_t opa, opb, opc;
			uint8_t imm;
			int16_t result;
			uint8_t regdest;
			uint16_t memdest;
		};
	
		L1Cache cache = L1Cache();
		Memory memory = Memory();
		
		std::array<int16_t, NUM_OF_GENERAL_REGISTERS> regs {};
		std::array<PipelineRegisters, NUM_OF_PIPELINED_REGSITERS> readpip {};
		std::array<PipelineRegisters, NUM_OF_PIPELINED_REGSITERS> writepip {};
		
		bool run = true;
		uint16_t pc = 0x0;
		uint16_t flag = 0x0;
		
	public:
		CPU() = default;
		void cycle();
		
	private:	
		void fetch();
		void decode();
		void execute();
		void dataMemoryAccess();
		void writeBack();
		
		void defaultSignals(ControlSignals& cs);
		void updateSignals(ControlSignals& cs, bool pcwrite, bool pcread, bool regwrite, bool regread, bool memwrite, bool memread, bool immsel);
		void updateFlagRegister(bool stall, bool resume, bool zero, bool hit, bool miss);
		void updatePipelineRegisters();
		
		void nop();
		void mov();
		void lda();
		void sta();
};

#endif
