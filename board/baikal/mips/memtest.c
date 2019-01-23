#include <common.h>
#include <command.h>
//#include <getopt.h>
#include <post.h>

int do_memtest (cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{	
	mem_test_t mem_test = {0,1,0,0};
	int ret = 0;
	int argn = 1;

	if (argc == 1) {
		ret = memory_test(&mem_test);
		return ret;
	}

	for (;argn < argc;) {
		printf("DBG: %s(): argv[argn]: %s\n", __func__, argv[argn]);
		if (strcmp(argv[argn], "-s") == 0) {
			mem_test.immediate_stop = 1;
			++argn;
			continue;
		}
		
		if (strcmp(argv[argn], "-r") == 0) {
			char *str_next = 0;
			mem_test.start_addr = simple_strtoul(argv[argn + 1], &str_next, 16);
			
			printf("DBG: %s(): str_next + 1: %s\n", __func__, str_next + sizeof(char));
			if (*str_next != ':') {
				mem_test.start_addr = 0;
				printf("DBG: %s(): continue\n", __func__);
				mem_test.stop_addr = 0;
				argn++;
				continue;
			}
			
			mem_test.stop_addr = simple_strtoul(str_next + sizeof(char), NULL, 16);
			argn +=2;
			continue;
		}

		if (strcmp(argv[argn], "-i") == 0) {
			mem_test.iteration_count = simple_strtoul(argv[argn + 1], NULL, 0);
			argn += 2;
			continue;
		}
		++argn;
	}
	ret = memory_test(&mem_test);
	return ret;
}

U_BOOT_CMD(
	memtest, 6, 0, do_memtest,
	"Make a test of memory",
	"memtest -s -r 0x0:0x100000 -i 10"
);

