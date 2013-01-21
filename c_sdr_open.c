#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>

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

struct sdr_data {
	/* sdr data */          /* offsets */
	unsigned short junk;    /* 0 */         
	unsigned long  sensor;  /* 1 - 8 */   
};

int main(int argc, char *argv[])
{
	int             p_sdr_f;
	struct sdr_data *sdr_dat;
        size_t len = 0;

	len = sizeof(struct sdr_data);

  	if ((sdr_dat = (struct sdr_data *)malloc(len)) == NULL) {
		printf("malloc error\n");
		exit(1);
	}	

	if ((p_sdr_f = open("service0.sdrcache", O_RDONLY)) < 0){
		printf("open failed\n");
		exit(1);
	}

	if ((len = read(p_sdr_f, sdr_dat, len)) < 0) {
                printf("Couldn't read\n");
                exit(1);
        }

	printf("Data: junk %u, sensor %lu 0x%x\n",sdr_dat->junk, sdr_dat->sensor, sdr_dat->sensor);

	close(p_sdr_f);
        if (sdr_dat){
        	free(sdr_dat);
        	exit(EXIT_SUCCESS);
       }

	return 0;
}

