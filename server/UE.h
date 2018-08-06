#ifndef PSZEMEG_UE_H
#define PSZEMEG_UE_H

//enum channel_flags;

class UE
{
    public:
        UE(int socket_fd);
        void set_RA_RNTI(int RA_RNTI);
        void set_C_RNTI(int C_RNTI);
        int get_socket_fd();

    private:
        int RA_RNTI;
        int C_RNTI;
        int socket_fd;
};


#endif //PSZEMEG_UE_H
