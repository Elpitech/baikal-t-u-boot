#include <common.h>
#include <asm/dbgout.h>

void debug_out(unsigned int type, unsigned int val) {
	int arr[4] = {0x12345678, 0xA55A5AA5, 0xBEAFDEAD, 0xBABADEDA};
//	printf("DBG: arr address 0x%08X\n", arr);
//	printf("DBG: %s()\n", __func__);
//	printf("DBG: type: 0x%08X, TYPEMASK: 0x%08X, TYPEMAGIC: 0x%08X\n", type, TYPEMASK, TYPEMAGIC);
//	printf("DBG: type & TYPEMASK: 0x%08X\n", type & TYPEMASK);
//	printf("DBG: type & TYPEMASK != TYPEMAGIC: %d\n", type & TYPEMASK != TYPEMAGIC);
	if(type & TYPEMASK != TYPEMAGIC) {
		printf("DBG_OUT: Type:0x%X, Value:0x%X\n", type, val);
		return;
	}
	if(type == VAL) {
		printf("DBG_OUT: Value:0x%X\n", val);
		return;
	}
	if(type == TREG) {
		printf("DBG_OUT: Register:0x%X\n", val);
		return;
	}
	if(type == CFG0) {
		printf("DBG_OUT: Config register 0 value:0x%08X\n", val);
		return;
	}
	if(type == CFG1) {
		printf("DBG_OUT: Config register 1 value:0x%08X\n", val);
		return;
	}
	if(type == CFG2) {
		printf("DBG_OUT: Config register 2 value:0x%08X\n", val);
		return;
	}
	if(type == CFG3) {
		printf("DBG_OUT: Config register 3 value:0x%08X\n", val);
		return;
	}
	if(type == CFG4) {
		printf("DBG_OUT: Config register 4 value:0x%08X\n", val);
		return;
	}
	if(type == CFG5) {
		printf("DBG_OUT: Config register 5 value:0x%08X\n", val);
		return;
	}
	if(type == CFG6) {
		printf("DBG_OUT: Config register 6 value:0x%08X\n", val);
		return;
	}

}
