#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base32.h"

int main(int argc, char **argv)
{
	bool decode = 0;
	static char inbuf[1000 /* multiple of 5 and 8 to avoid padding */
	                  + 1  /* because we still need to add a \0 at the end */];
	static char outbuf[BASE32_LEN(sizeof(inbuf)-1)];

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-d"))
			decode = 1;
		else
			stdin = fopen(argv[i], "r");
	}
	if (!stdin) {
		fprintf(stderr, "%s [-d] [FILE]\n", argv[0]);
		return EXIT_FAILURE;
	}

	size_t len;
	while (0 != (len = fread(inbuf, 1,  sizeof(inbuf) - 1, stdin))) {
		if (!decode) {
			base32_encode((unsigned char*)inbuf, len, (unsigned char*)outbuf);
			len = BASE32_LEN(len);
		} else {
			assert(len < sizeof(inbuf));
			inbuf[len] = '\0';
			len = base32_decode((unsigned char*)inbuf, (unsigned char*)outbuf);
		}
		assert(len <= sizeof(outbuf));
		fwrite(outbuf, 1, len, stdout);
	}
	if (!decode)
		printf("\n");
	return EXIT_SUCCESS;
}
