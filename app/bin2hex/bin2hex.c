#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hex[2];

void bintohex(int ch)
{
	hex[0] = ch % 0x10;
	hex[1] = ch / 0x10;
	if (0 <= hex[0] && hex[0] <= 9)
		hex[0] += 0x30;
	else
		hex[0] = hex[0] - 9 + 0x40;

	if (0 <= hex[1] && hex[1] <= 9)
		hex[1] += 0x30;
	else
		hex[1] = hex[1] - 9 + 0x40;
}

int main(int argc, char **argv)
{
	FILE *pInFile,*pOutFile;
	int i, ch, bit, buff[64], buff_w;

	if(argc < 5)
	{
		printf("userge:<input> <bit: 32/64/128> <endian: 0little,1big> <output>\r\n");
		return 0;
	}

	pInFile = fopen(argv[1],"rb");
	if(pInFile == NULL)
	{
		printf("Open input file failed!\r\n");
		return 0;
	}
	bit = atoi(argv[2]);
	if(bit != 32)
		if(bit != 64)
			if(bit !=128){
				printf("bit should be: 32/64/128\r\n");
				return -1;
			}
	pOutFile = fopen(argv[4],"w");
	if(pOutFile == NULL)
	{
		printf("Open output file failed!\r\n");
		return 0;
	}

	ch = fgetc(pInFile);
	i=0;
	buff_w=0;
	while(ch != EOF)
	{
		if(buff_w == bit/4) {
			for(i=0; i< bit/4; i++)
			{
				fputc(buff[bit/4-i-1], pOutFile);
			}
			fputc(0x0a, pOutFile);
			buff_w = 0;
		}
		bintohex(ch);
		buff[buff_w + 0] = hex[0];
		buff[buff_w + 1] = hex[1];

		//fputc(hex[0], pOutFile);
		//fputc(hex[1], pOutFile);
		ch = fgetc(pInFile);
		buff_w += 2;
	}

	if(buff_w != 0)
	{
		for(i=0; i< buff_w; i++)
			{
				fputc(buff[buff_w-i-1], pOutFile);
			}
		fputc(0x0a, pOutFile);
	}

	fclose(pInFile);
	fclose(pOutFile);
	return 0;
}
