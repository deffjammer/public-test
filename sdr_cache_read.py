#! /usr/bin/python -u

#struct sdr_get_rs {
#        uint16_t id;             /* Record id */
#        uint8_t  version;        /* SDR version (51h) */
#        uint8_t  type;           /* Record type */
#        uint8_t  length;         /* Remaining record bytes */
#        uint8_t  r1;             /* Sensor Ownser ID */
#        uint8_t  r2;             /* Sensor Owner LUN */
#	     uint8_t  sensor_id;      /* Sensor ID */
#};
SDR_RECORD_TYPE_FULL_SENSOR           = 0x01
SDR_RECORD_TYPE_COMPACT_SENSOR        = 0x02
SDR_RECORD_TYPE_EVENTONLY_SENSOR      = 0x03
SDR_RECORD_TYPE_ENTITY_ASSOC          = 0x08
SDR_RECORD_TYPE_DEVICE_ENTITY_ASSOC   = 0x09
SDR_RECORD_TYPE_GENERIC_DEVICE_LOCATOR  = 0x10
SDR_RECORD_TYPE_FRU_DEVICE_LOCATOR    = 0x11
SDR_RECORD_TYPE_MC_DEVICE_LOCATOR     = 0x12
SDR_RECORD_TYPE_MC_CONFIRMATION       = 0x13
SDR_RECORD_TYPE_BMC_MSG_CHANNEL_INFO  = 0x14
SDR_RECORD_TYPE_OEM                   = 0xc0
SDR_FULL_SENSOR_LENGTH_OFFSET         = 0x2f # 47
SDR_COMPACT_SENSOR_OFFSET             = 0x1f # 31
SDR_EVENTONLY_SENSOR_OFFSET           = 0x10 # 16

REC_LEN_OFFSET = 5
LENGTH_MASK    = 0x0f

import sys

f = open("r1i1c.sdrcache", "rb")
while True:
    try:
        pos         = f.tell()
        id1         = ord(f.read(1))
        id2         = ord(f.read(1))
        version     = ord(f.read(1))
        sensor_type = ord(f.read(1))
        length      = ord(f.read(1))

        if sensor_type == SDR_RECORD_TYPE_FULL_SENSOR:
            f.seek(pos + SDR_FULL_SENSOR_LENGTH_OFFSET)

        elif sensor_type == SDR_RECORD_TYPE_COMPACT_SENSOR:
            f.seek(pos + SDR_COMPACT_SENSOR_OFFSET)

        elif sensor_type == SDR_RECORD_TYPE_EVENTONLY_SENSOR:
            f.seek(pos + SDR_EVENTONLY_SENSOR_OFFSET)

        else:
            print 'Type not wanted 0x%02x' % sensor_type
            f.seek(f.tell() + length)
            continue

        name_code = ord(f.read(1))
        name_len = LENGTH_MASK & name_code
        name = f.read(name_len)
        f.seek(pos + (length + REC_LEN_OFFSET))

        print 'ID 0x%02x, Ver 0x%02x, Type 0x%02x, Length 0x%02x, Name Code 0x%02x, Name %s' \
            % (id1, version, sensor_type, length, name_code, name)

    except:
        break
f.close()

sys.exit(0)
