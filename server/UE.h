#ifndef SERVER_UE_H
#define SERVER_UE_H

enum Action_to_perform
{
    random_access_response,
    rrc_connection_response
};

class UE
{
    public:
        UE(Action_to_perform flag);
        UE(int RA_RNTI);
        int RA_RNTI;
        int C_RNTI;
        Action_to_perform get_flag();

        void setUE_state(int UE_state);
        void setTiming_advance(int timing_advance);
        void setUplink_resource_grant(int uplink_resource_grant);
        void setUplink_power_control(int uplink_power_control);
        void setUl_sch_config(int ul_sch_config);
        void setSrb_identity(int srb_identity);
        void set_flag(Action_to_perform flag);
        void set_socket_fd(int socket_fd);
        int get_scheduling_request() const;
        void set_scheduling_request(int drx_type_choice);

        int get_socket_fd() const;

    private:
        Action_to_perform flag;

        int socket_fd;
        int UE_state;
        int timing_advance;
        int uplink_resource_grant;
        int uplink_power_control;
        int ul_sch_config;
        int srb_identity;
        int drx_type_choice;
};


#endif //SERVER_UE_H
