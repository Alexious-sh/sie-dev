

#include <stdio.h>
#include <stdlib.h>
#include "loader.h"


int main()
{
	int size;
	char *buffer;
	FILE *f = fopen("C:\\test.elf", "rb");
	if(!f)
	{
		printf("Failed to open file\n");
		system("pause");
		return 1;
	}

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	buffer = new char[size+1];

	fread(buffer, size, 1, f);
	fclose(f);
	buffer[size] = 0;

	elf_to_binary(buffer, size);


	delete buffer;

	system("pause");
	return 0;
}
