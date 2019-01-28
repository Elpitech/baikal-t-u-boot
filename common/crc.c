#include <stdint.h>

uint16_t crc16 (uint8_t *buffer, uint32_t count)
{
	uint16_t crc = 0, i, j;
	for (j = 0; j < count; ++j) {
		crc ^= buffer[j] << 8;
		for (i = 0; i < 8; ++i) {
			uint16_t next = crc << 1;
			if (crc & 0x8000)
				next ^= 0x1021;
			crc = next;
		}
	}
	return crc;
}

