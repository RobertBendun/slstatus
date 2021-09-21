#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define Command \
	"lsblk -nrpo \"name,type,size,mountpoint,fsused\" -x mountpoint" \
	"| awk '$4!~/\\/boot|\\/efi|SWAP/&&length($4)>=1{print $4 \" \" $5 \" of \" $3}'"

char buffer[512];

char* disks()
{
	FILE *f = popen(Command, "r");
	size_t offset = 0;

	char *line = NULL;
	size_t size = 0;
	ssize_t written = 0;
	while ((written = getline(&line, &size, f)) > 0) {
		printf("%s", line);
		strncpy(buffer + offset, line, written - 1);
		strcat(buffer, " | ");
		offset += written - 1 + 3;
		free(line);
		line = NULL;
	}
	free(line);

	buffer[offset-3] = '\0';
	pclose(f);
	return buffer;
}
