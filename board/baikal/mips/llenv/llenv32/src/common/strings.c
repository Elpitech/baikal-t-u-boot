/*
 * Low-level and POST tests.
 *
 * Copyright (C) 2014 Baikal Electronics.
 *
 * Author: Alexey Malahov <Alexey.Malahov@baikalelectronics.com>
 *
 * Description: Helper functions.
 */

#include "llenv.h"
#include "cmdline.h"

/* Compare strings. */
int llenv_strcmp(const char *cs, const char *ct)
{
	do {
		if (*cs != *ct) {
			return (*cs - *ct);
		}

		if (!*cs)
			return 0;

		cs++; ct++;
		
	} while (1);
}

/* Copy string. */
char *llenv_strcpy(char *s1, const char *s2)
{
	char *str = s1;

	while ((*s1++ = *s2++));

	return str;
}

/* Calculate a string lenght. */
int llenv_strlen(const char *str)
{
	register int len = 0;

	while (*str++) {
		len++;
	}

	return len;
}

/* Convert character to upper one. */
char llenv32_toupper (char ch)
{
	if ((ch >= 'a') && (ch <= 'z')) {
		ch = 'A' + ch - 'a';
	}
	return ch;
}

/* Convert integer to string */
char *llenv32_itoa(char *buf, unsigned int dig, int base, int tlen, int caps)
{
	register char *str = (buf + tlen - 1);
	register unsigned int letter;

	*str = '\0';

	if (!dig) {
		*--str = '0';
	} else {
		while (dig) {
			letter = (dig % base);
			if (letter >= 10) {
				letter += ('a' - 10);
			} else {
				letter += '0';
			}

			*--str = ((caps) ? llenv32_toupper(letter) : letter);
			dig = dig / base;
		}
	}
	return (str);
}

/* Convert string to integer. */
int llenv32_atoi(char **buf, int base, int tlen)
{
	int dig = 0;
	unsigned char ch;
	char *str = *buf;

	if (!base) {
		base = LLENV32_ATOI_BASE10;
	}

	while(*str == ' ') {
		str++;
	}

	if (*str == '0') {
		tlen--;
		if (llenv32_toupper(*(++str)) == 'X') {
			base = LLENV32_ATOI_BASE16;
			str++;
			tlen--;
		}
	}

	while ((ch = *(str++)) != '\0') {
		if ((ch == SP) || (tlen == 0)) {
			*buf = str;
			return dig;
		}

		ch = llenv32_toupper(ch);

		if ((ch >= '0') && (ch <= '9')) {
			ch = ch - '0';
		} else if ((ch >= 'A') && (ch <= 'F')) {
			ch = ch + 10 - 'A';
		} else {
			*buf = str;
			return LLENV32_BAD_CMD;
		}

		dig = dig * base + ch;
		tlen--;
	}

	*buf = 0;
	return dig;
}

/* Set memory with a int */
void  *llenv32_memset(void *addr, int len, int c)
{
	int *ch_addr = addr;
	while (len--) {
		*ch_addr++ = c;
	}
	return addr;
}

void  *llenv32_memcpy(void *dst, void * src, unsigned int bytes)
{
        void * ret = dst;

        while (bytes--) {
                *(char *)dst = *(char *)src;
                dst = (char *)dst + 1;
                src = (char *)src + 1;
        }

        return(ret);
}
