/*
 * T-Platforms bootconf (new version) support
 */

#include <common.h>

#define TP_BC_MAX_LINES	32

/*
 * bootconf format:
 * # <any comments>
 * default <label>
 * <label> <uImage-name> <dtb-name> <initrd-name> <args...>
 * [<another-label> ...]
 */

int split_lines(char *buf, int size, char *lines[], int max_lines)
{
	char *next_line = buf;
	char *s, *t;
	int nl = 0;

	while (nl < max_lines) {
		t = memchr(next_line, '\n', size - (next_line - buf));
		if (!t)
			break; // ignore last line without NL
		*t = 0;
		s = next_line;
		next_line = t + 1;
		while ((t >= s) && (*t == ' ' || *t == '\t'))
			*t-- = 0; // remove trailing space
		while ((s < t) && (*s == ' ' || *s == '\t'))
			s++;
		if (*s == '#' || s >= t)
			continue; // line empty or commented out
		lines[nl++] = s;
	}

	return nl;
}

/*
 * bootconf_parse: parse bootconf in memory, set environment variables
 */

int bootconf_parse(char *buf, int size)
{
	char *lines[TP_BC_MAX_LINES];
	int n_lines;
	int i, len;
	char *t;
	char *dfl = NULL;
	char *image, *fdt, *initrd, *bootargs;

	n_lines = split_lines(buf, size, lines, TP_BC_MAX_LINES);

	/* find "default" */
	len = strlen("default");
	for (i = 0; i < n_lines; i++) {
		t = lines[i];
		if (memcmp(t, "default", len) == 0)
			break;
	}
	if (i >= n_lines) {
		printf("bootconf: default boot not found\n");
		return -EINVAL;
	}
	t += len;
	while (*t && (*t == ' ' || *t == '\t'))
		t++;
	if (*t) {
		dfl = t;
	} else {
		printf("bootconf: default label is nul?\n");
		return -EINVAL;
	}

	len = strlen(dfl);
	for (i = 0; i < n_lines; i++) {
		t = lines[i];
		if (memcmp(t, dfl, len) == 0)
			break;
	}
	if (i >= n_lines) {
		printf("bootconf: default line <%s> not found\n", dfl);
		return -ENOENT;
	}
	t += len;
	while (*t && (*t == ' ' || *t == '\t'))
		t++;

	image = t;
	if (!*t) {
		printf("bootconf: no image name\n");
		return -EINVAL;
	}
	while (*t != ' ' && *t != '\t')
		t++;
	if (*t) {
		*t++ = 0;
		while (*t && (*t == ' ' || *t == '\t'))
			t++;
	}

	fdt = t;
	if (!*t) {
		printf("bootconf: no fdt name\n");
		return -EINVAL;
	}
	while (*t != ' ' && *t != '\t')
		t++;
	if (*t) {
		*t++ = 0;
		while (*t && (*t == ' ' || *t == '\t'))
			t++;
	}

	initrd = t;
	if (!*t) {
		printf("bootconf: no initrd name\n");
		return -EINVAL;
	}
	while (*t != ' ' && *t != '\t')
		t++;
	if (*t) {
		*t++ = 0;
		while (*t && (*t == ' ' || *t == '\t'))
			t++;
	}

	bootargs = t;
	if (!*t) {
		printf("bootconf: no bootargs\n");
		return -EINVAL;
	}

	env_set("image_name", image);
	env_set("fdt_name", fdt);
	env_set("initrd_name", initrd);
	env_set("bootargs", bootargs);

	return 0;
}

static int do_bootconf(struct cmd_tbl *cmdtp, int flag, int argc,
		       char * const argv[])
{
	unsigned long addr, size;

	if (argc != 3)
		return CMD_RET_USAGE;
	addr = simple_strtoul(argv[1], NULL ,16);
	size = simple_strtoul(argv[2], NULL, 16);

	return bootconf_parse((char *)addr, size);
}

U_BOOT_CMD(
	bootconf,	3,	3,	do_bootconf,
	"Bootconf support",
	"bootconf addr size	- parse bootconf at 'addr'\n"
);

