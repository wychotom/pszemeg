struct MIB_MESSAGE
{
    uint16_t prach_port;
    uint16_t broadcast_port;
    uint16_t dl_sch_port;
    uint16_t ul_sch_port;
    uint16_t pdcch_port;
    uint16_t pucch_port;
};

struct DCI_MESSAGE
{
    unsigned char format0_a_flag;
    unsigned char freqency_hooping;
    int riv;
    int mcs;
    unsigned char ndi;
    int tpc;
    int cyclic_shift;
    unsigned char cqi_request;
};