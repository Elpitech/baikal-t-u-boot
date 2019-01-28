#include <common.h>
#include <command.h>
#include <post.h>

int do_memtest (cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{	
	mem_test_t mem_test = {0,1,0,0};
	int ret = 0;
	int argn = 1;
	int error = 0;
	if (argc != 1) {
		for (;argn < argc;) {
			
//			printf("DBG: %s(): argv[argn]: %s\n", __func__, argv[argn]);
			
			if (strcmp(argv[argn], "-s") == 0) {
				
				mem_test.immediate_stop = 1;
				++argn;
				continue;
			}
			
			if (strcmp(argv[argn], "-r") == 0) {
				
				char *str_next = 0;
				if (argn == argc - 1) {
					error = 1;
					break;
				}
				mem_test.start_addr = simple_strtoul(argv[argn + 1], &str_next, 16);
				
//				printf("DBG: %s(): str_next + 1: %s\n", __func__, str_next + sizeof(char));
				
				if (*str_next != ':') {
					mem_test.start_addr = 0;
//					printf("DBG: %s(): continue\n", __func__);
					mem_test.stop_addr = 0;
					argn++;
					continue;
				}
				
				mem_test.stop_addr = simple_strtoul(str_next + sizeof(char), NULL, 16);
				argn +=2;
				continue;
			}
	
			if (strcmp(argv[argn], "-i") == 0) {
				
				if (argn == argc -1) {
					error = 1;
					break;
				}
				
				mem_test.iteration_count = simple_strtoul(argv[argn + 1], NULL, 0);
				argn += 2;
				continue;
			}
			error = 1;
			break;	
		}
	}	

	if (error !=0) {
		printf("Invalid argument\n");
		return -1;
	}
	

	printf("Program will reboot board and start memory testing\n");
	int cont = 0;
	do {
		printf("Are you sure you want to do this? Y/N\n");
		switch (getc()) {
			case 'y':
			case 'Y':
				cont = 1;
				break;
			case 'n':
			case 'N':
				return 0;
		}
	}while ( cont == 0 );

	void* addr = CONFIG_SRAM_MEMTEST_BASE;
	
	uint32_t *crc = ((uint32_t*)addr);
	mem_test_t *sram_mem_test = ((mem_test_t*)(addr + sizeof(uint32_t)));
	
	*crc = 0xDEADBEEF;
	*crc = crc16(&mem_test, sizeof(mem_test_t));
	memcpy(sram_mem_test, &mem_test, sizeof(mem_test_t));
	
	printf("crc=0x%08X\n", *crc);
	
	_machine_restart();
	return ret;
}

U_BOOT_CMD(
	memtest, 6, 0, do_memtest,
	"Make a test of memory",
	"memtest -s -r 0x0:0x100000 -i 10\n"
	"s - Stop immediately after error\n"
	"i - Iteration count, 0 - infinite loop\n"
	"r - Range of memory to check\n"
);

