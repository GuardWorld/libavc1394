
#include <config.h>
#include "raw1394util.h"
#include <errno.h>

int cooked1394_read(raw1394handle_t handle, nodeid_t node, nodeaddr_t addr,
                    size_t length, quadlet_t *buffer)
{
    int retval, ackcode, rcode, i;
    for(i=0; i<MAXTRIES; i++) {
        retval = raw1394_read(handle, node, addr, length, buffer);
#ifdef RAW1394_V_0_9
        switch (raw1394_errcode_to_errno( raw1394_get_errcode(handle))) {
            case 0:
                return retval;
            case EAGAIN:
                usleep(RETRY_DELAY);
                break;
            default:
                return -1;
        }
#else
        if( retval < 0 ) return retval;
        ackcode = RAW1394_MASK_ACK(retval);
        rcode = RAW1394_MASK_RCODE(retval);
        if( ackcode == ACK_BUSY_X || ackcode == ACK_BUSY_A
                || ackcode == ACK_BUSY_B ) {
            usleep(RETRY_DELAY);
        } else {
            break;
        }
#endif
    }

#ifdef RAW1394_V_0_9
    return -1;
#else
    if( ackcode != ACK_PENDING && ackcode != ACK_COMPLETE &&
            ackcode != ACK_LOCAL)
        return -1;
    return retval;
#endif
}

int cooked1394_write(raw1394handle_t handle, nodeid_t node, nodeaddr_t addr,
                     size_t length, quadlet_t *data)
{
    int retval, ackcode, rcode, i;
    for(i=0; i<MAXTRIES; i++) {
        retval = raw1394_write(handle, node, addr, length, data);
#ifdef RAW1394_V_0_9
        switch (raw1394_errcode_to_errno( raw1394_get_errcode(handle))) {
            case 0:
                return retval;
            case EAGAIN:
                usleep(RETRY_DELAY);
                break;
            default:
                return -1;
        }
#else
        if(retval < 0 )
            return retval;
        ackcode = RAW1394_MASK_ACK(retval);
        rcode = RAW1394_MASK_RCODE(retval);
        if( ackcode == ACK_BUSY_X || ackcode == ACK_BUSY_A
                || ackcode == ACK_BUSY_B ) {
            usleep(RETRY_DELAY);
        } else {
            break;
        }
#endif
    }

#ifdef RAW1394_V_0_9
    return -1;
#else
    if( ackcode != ACK_PENDING && ackcode != ACK_COMPLETE &&
            ackcode != ACK_LOCAL)
        return -1;
    return retval;
#endif
}
