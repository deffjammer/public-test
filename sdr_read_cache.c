#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
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
#define SDR_RECORD_TYPE_FULL_SENSOR             0x01
#define SDR_RECORD_TYPE_COMPACT_SENSOR          0x02
#define SDR_RECORD_TYPE_EVENTONLY_SENSOR        0x03
#define SDR_RECORD_TYPE_ENTITY_ASSOC            0x08
#define SDR_RECORD_TYPE_DEVICE_ENTITY_ASSOC     0x09
#define SDR_RECORD_TYPE_GENERIC_DEVICE_LOCATOR  0x10
#define SDR_RECORD_TYPE_FRU_DEVICE_LOCATOR      0x11
#define SDR_RECORD_TYPE_MC_DEVICE_LOCATOR       0x12
#define SDR_RECORD_TYPE_MC_CONFIRMATION         0x13
#define SDR_RECORD_TYPE_BMC_MSG_CHANNEL_INFO    0x14
#define SDR_RECORD_TYPE_OEM                     0xc0

struct sdr_get_rs {
        uint16_t id;            /* record id */
        uint8_t version;        /* SDR version (51h) */
       	uint8_t type;           /* record type */
        uint8_t length;         /* remaining record bytes */
        uint8_t raw;            /* raw */
};


int main(int argc, char *argv[])
{
        FILE *ptr_myfile;
        struct sdr_get_rs sdr_rs; 
        long cur;
        char name[16];

        ptr_myfile = fopen("r1i1c.sdrcache"/*service0.sdrcache"*/,"rb");
        if (!ptr_myfile) {
                printf("Unable to open file!");
                return 1;
        }

        while (fread(&sdr_rs, sizeof(struct sdr_get_rs), 1, ptr_myfile)){
                cur = ftell(ptr_myfile);
                fseek(ptr_myfile, (cur - 1) + (sdr_rs.length - 16), SEEK_SET);
                fread(&name, 16, 1,ptr_myfile);

                printf("ID 0x%x Ver 0x%x, Type 0x%x, Raw 0x%x, Length 0x%x Name %s\n",
                    	sdr_rs.id,
			sdr_rs.version,
                    	sdr_rs.type,
			sdr_rs.raw,
			sdr_rs.length,
			name);

                fseek(ptr_myfile, (cur - 1) + sdr_rs.length, SEEK_SET);

                if (feof(ptr_myfile)){
                    goto done;
                }
        }


done:
	fclose(ptr_myfile);
	return 0;
}

