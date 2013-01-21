#include <stdio.h>
#include <stdlib.h>
/*
  In the file, bytes 4/5 are the record type & length (byte offset starts at 1, not 0)
   sensor ID is in byte 8
   type 01 bytes 48/49 for length & data start
   type 02 bytes 32/33  "
   type 03 bytes 17/18  "

An example to get a sensor name for sensor number 02:

    get file name of sdr cache derived from hostname:
/var/cache/sdr-logger/r1i0n15.sdrcache

loop start:
     read the file, examine first record bytes 4 & 5 for record type
and record length. Get byte 8 offset for sensor number.
        if this is desired sensor number, calculate name offset &
length from record type
        return name string.
 if not our sensor number, calculate start of next record from record length. Then loop back.
*/
/*
 byte 2, 51 -------> in the spec this is byte 3, SDR version
 byte 3, 12 -------> this is record type (mgmt controlle rrecord, skip it)
 byte 4, 11 -------> length 11 (which means start + 11h gets the start of next record)
 byte 5, 20 -------> record length
 byte 6, 0
 byte 7, 0
 byte 8, ffffff9f  -> sensor ID?  --> for record type 12 this means somethign else
*/
struct sdr_header {
 short b4;
 short b5;
};
#define BYTES_READ 5 
char a[BYTES_READ];
char b[100000];

int main(int argc, char *argv[])
{
	//int counter;
	FILE *ptr_myfile;
	//struct rec my_record;
	//struct sdr_header bs; 
	int i,x;
	long cur, data_start, length;

	ptr_myfile = fopen("r1i1c.sdrcache"/*service0.sdrcache"*/,"rb");
	if (!ptr_myfile)
	{
		printf("Unable to open file!");
		return 1;
	}

	while (fread(&b, 1 /*size*/, BYTES_READ /*numb elem*/, ptr_myfile)){
		cur = ftell(ptr_myfile);
		x = b[4];
		printf("%ld\n",x + cur);
		fread(&b[cur - 1], 1, x, ptr_myfile);
		for (i=0;  i < x ; i++)
		 	printf("byte %d, %x\n", i ,b[i]);
		cur = ftell(ptr_myfile);
		switch(b[3]){
			case 0x1:// type 01 bytes 48/49 for length & data start
				data_start = b[48];
				length     = b[49];				
				printf("RECORD TYPE = 0x%x, sensor id 0x%x data start 0x%x length 0x%x\n",
					b[3],b[7], data_start, length); 
				break;
			case 0x2:// type 02 bytes 32/33  "
                                data_start = b[32];
                                length     = b[33];     
				printf("RECORD TYPE = 0x%x, sensor id 0x%x data start 0x%x length 0x%x\n",
					b[3],b[7], data_start, length); 
				break;
			case 0x3:// type 03 bytes 17/18  "
                               	data_start = b[17];
                                length     = b[18];     
				printf("RECORD TYPE = 0x%x, sensor id 0x%x data start 0x%x length 0x%x\n",
					b[3],b[7], data_start, length); 
				break;
			default:
				printf("RECORD TYPE 0x%x\n",b[3]);
		}
		printf("++++++++pos %ld++++++\n",cur);
		if (feof(ptr_myfile)){
			printf("EOF\n");
			goto done;
		}
	}	
	
	//fseek(ptr_myfile, 3, SEEK_SET);
	//fread(&b, 1, 2, ptr_myfile);
	//fread(&bs,1 , 2, ptr_myfile);
	//printf("sdr_header type %x header %x\n",bs.b4, bs.b5);

done:
	fclose(ptr_myfile);
	return 0;
}

