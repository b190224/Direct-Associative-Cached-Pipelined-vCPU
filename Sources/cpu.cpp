#include <iostream>
#include "../Includes/cpu.h"

void CPU::updatePipelineRegisters() {
	readpip[IF_ID] = writepip[IF_ID];
	readpip[ID_EX] = writepip[ID_EX];
	readpip[EX_MEM] = writepip[EX_MEM];
	readpip[MEM_WB] = writepip[MEM_WB];
}

void CPU::updateSignals(ControlSignals& cs, bool pcwrite, bool pcread, bool regwrite, bool regread, bool memwrite, bool memread, bool immsel) {
	cs.pcwrite = pcwrite;
	cs.pcread = pcread;
	cs.regwrite = regwrite;
	cs.regread = regread;
	cs.memwrite = memwrite;
	cs.memread = memread;
	cs.immsel = immsel;
}

void CPU::defaultSignals(ControlSignals& cs) {
	cs.pcwrite = true;
	cs.pcread = false;
	cs.regwrite = false;
	cs.regread = false;
	cs.memwrite = false;
	cs.memread = false;
}

void CPU::cycle() {	
	while (run) {
		if (pc > 15) break;

		
		writeBack();
		dataMemoryAccess();
		execute();
		decode();
		fetch();
		
		updatePipelineRegisters();
	}
}

void CPU::fetch() {	
	if (cache.isValidInstruction(pc)) {
		writepip[IF_ID].ins = cache.getValidInstruction(pc);
	}
	else {
		cache.handleCacheMiss(memory.ins[pc], pc);
		writepip[IF_ID].ins = ins;
	}
	
	if (readpip[IF_ID].cs.pcwrite) ++pc;
}

void CPU::decode() {
	writepip[ID_EX].op = (readpip[IF_ID].ins & 0xFE00) >> 9;

	switch (writepip[ID_EX].op) {
		case NOP: {
			defaultSignals(writepip[ID_EX].cs);
			break;	
		}
		
		case MOV: {
			updateSignals(writepip[ID_EX].cs, true, false, true, true, false, false, false);
			break;
		}
		
		case LDA: {
			updateSignals(writepip[ID_EX].cs, true, false, true, true, false, true, true);
			break;
		}
		
		case STA: {
			updateSignals(writepip[ID_EX].cs, true, false, false, true, true, false, true);
			break;
		}
		
		default: {
			defaultSignals(writepip[ID_EX].cs);
			break;
		}
	}
	
	if (writepip[ID_EX].cs.regread) {
		writepip[ID_EX].opa = regs[(readpip[IF_ID].ins & 0x1C0) >> 6];
		writepip[ID_EX].opb = regs[(readpip[IF_ID].ins & 0x38) >> 3];
		writepip[ID_EX].opc = regs[readpip[IF_ID].ins & 0x7];
	}
	
	if (writepip[ID_EX].cs.regwrite) {
		writepip[ID_EX].regdest = (readpip[IF_ID].ins & 0x1C0) >> 6;
	}
	
	if (writepip[ID_EX].cs.immsel) {
		writepip[ID_EX].imm = readpip[IF_ID].ins & 0x7;
	}
}

void CPU::execute() {
	writepip[EX_MEM].cs = readpip[ID_EX].cs;
	
	switch (readpip[ID_EX].op) {
		case NOP: {
			nop();
			break;
		}
		
		case MOV: {
			mov();
			break;
		}
		
		case LDA: {
			lda();
			break;
		}
		
		case STA: {
			sta();
			break;
		}
		
		default: {}
	}
}

void CPU::dataMemoryAccess() {
	writepip[MEM_WB].cs = readpip[EX_MEM].cs;
	writepip[MEM_WB].result = readpip[EX_MEM].result;
	writepip[MEM_WB].regdest = readpip[EX_MEM].regdest;
	
	if (readpip[EX_MEM].cs.memread) {
		if (cache.isValidData(readpip[EX_MEM].memdest)) {
			writepip[MEM_WB].result = cache.getValidData(readpip[EX_MEM].memdest);
		}
		else {
			cache.handleCacheMiss(readpip[EX_MEM].result, readpip[EX_MEM].memdest);
			writepip[MEM_WB].result = memory.data[readpip[EX_MEM].memdest];
			
			std::cout << int(writepip[MEM_WB].result);
		}	
	}
}

void CPU::writeBack() {

}

void CPU::nop() {}

void CPU::mov() {
	writepip[EX_MEM].regdest = readpip[ID_EX].regdest;
	writepip[EX_MEM].result = readpip[ID_EX].opb;
}

void CPU::lda() {
	writepip[EX_MEM].regdest = readpip[ID_EX].regdest;
	writepip[EX_MEM].memdest = readpip[ID_EX].opb + readpip[ID_EX].imm;
}

void CPU::sta() {
	writepip[EX_MEM].memdest = readpip[ID_EX].opb + readpip[ID_EX].imm;
	writepip[EX_MEM].result = readpip[ID_EX].opa;
}
