#include "header.h"

#define BLUE_TEXT "\e[1;36m"
#define YELLOW_TEXT "\033[1;33m"
#define RED_TEXT "\e[1;31m"
#define DEFAULT "\e[0m"

void printlog_mib(struct MIB_MESSAGE mib_msg)
{
    printf("["BLUE_TEXT"MIB"DEFAULT"]\n"
            YELLOW_TEXT"\tBROADCAST_PORT"DEFAULT" = %d\n"
            YELLOW_TEXT"\tPRACH_PORT"DEFAULT" = %d\n"
            YELLOW_TEXT"\tDL_SCH_PORT"DEFAULT" = %d\n"
            YELLOW_TEXT"\tUL_SCH_PORT"DEFAULT" = %d\n"
            YELLOW_TEXT"\tPDCCH_PORT"DEFAULT" = %d\n"
            YELLOW_TEXT"\tPUCCH_PORT"DEFAULT" = %d\n",
				mib_msg.broadcast_port, mib_msg.prach_port, mib_msg.dl_sch_port,
				mib_msg.ul_sch_port, mib_msg.pdcch_port, mib_msg.pucch_port);
}

void printlog_dci(struct DCI_MESSAGE dci_msg)
{
    printf("["BLUE_TEXT"DCI"DEFAULT"]\n"
            YELLOW_TEXT"\tFORMAT0_flag"DEFAULT" = %d\n"
            YELLOW_TEXT"\tFREQUENCY_HOOPING"DEFAULT" = %d\n"
            YELLOW_TEXT"\tRIV"DEFAULT" = %d\n"
            YELLOW_TEXT"\tMCS"DEFAULT" = %d\n"
            YELLOW_TEXT"\tNDI"DEFAULT" = %d\n"
            YELLOW_TEXT"\tTPC"DEFAULT" = %d\n"
            YELLOW_TEXT"\tCYCLIC_SHIFT"DEFAULT" = %d\n"
            YELLOW_TEXT"\tCQI_REQUEST"DEFAULT" = %d\n"
            YELLOW_TEXT"\tDRX_CYCLETYPE"DEFAULT" = %d\n",
				dci_msg.format0_a_flag, dci_msg.freqency_hooping, dci_msg.riv, dci_msg.mcs,
				dci_msg.ndi, dci_msg.tpc, dci_msg.cyclic_shift, dci_msg.cqi_request,
				dci_msg.drx_config.drx_cycle_type);
}

void printlog_rar(struct RANDOM_ACCESS_RESPONSE rar_msg)
{
    printf("["BLUE_TEXT"RANDOM_ACCESS_RESPONSE"DEFAULT"]\n"
            YELLOW_TEXT"\tTIMING_ADVANCE"DEFAULT" = %d\n"
            YELLOW_TEXT"\tUPLINK_RESOURCE_GRANT"DEFAULT" = %d\n"
            YELLOW_TEXT"\tTEMPORARY_C_RNTI"DEFAULT" = %d\n",
			rar_msg.timing_advance, rar_msg.uplink_resource_grant,
			rar_msg.temporary_c_rnti);
}

void printlog_rrc_setup(struct RRC_CONN_SETUP rrc_msg)
{
    printf("["BLUE_TEXT"RRC_CONNECTION_SETUP"DEFAULT"]\n"
            YELLOW_TEXT"\tC_RNTI"DEFAULT" = %d\n"
            YELLOW_TEXT"\tSRB_IDENTITY"DEFAULT" = %d\n"
            YELLOW_TEXT"\tDRB_IDENTITY"DEFAULT" = %d\n"
            YELLOW_TEXT"\tDL_AM_RLC"DEFAULT" = %d\n"
            YELLOW_TEXT"\tUL_AM_RLC"DEFAULT" = %d\n"
            YELLOW_TEXT"\tUL_SCH_RLC"DEFAULT" = %d\n"
            YELLOW_TEXT"\tPHR_CONFIG"DEFAULT" = %d\n"
            YELLOW_TEXT"\tON_DURATION_TIMER"DEFAULT" = %d\n"
            YELLOW_TEXT"\tDRX_CYCLE_TYPE"DEFAULT" = %d\n",
            rrc_msg.C_RNTI, rrc_msg.srb_identity, rrc_msg.drb_identity, rrc_msg.dl_am_rlc,
            rrc_msg.ul_am_rlc, rrc_msg.ul_sch_config, rrc_msg.phr_config,
            rrc_msg.uplink_power_control.on_duration_timer, rrc_msg.uplink_power_control.drx_cycle_type);
}

void printlog_drx(struct DRX_CONFIG drx_conf)
{
    printf("["BLUE_TEXT"DRX_CONFIG"DEFAULT"]\n"
            YELLOW_TEXT"\tON_DURATION_TIMER"DEFAULT" = %d\n"
            YELLOW_TEXT"\tSHORT_DRX_TIMER"DEFAULT" = %d\n"
            YELLOW_TEXT"\tLONG_DRX_TIMER"DEFAULT" = %d\n"
            YELLOW_TEXT"\tDRX_CYCLE_TYPE"DEFAULT" = %d\n",
            drx_conf.on_duration_timer, drx_conf.short_drx_timer,
            drx_conf.long_drx_timer, drx_conf.drx_cycle_type);
}

void printlog_drop(struct eNB_conn_info connections)
{
    printf("["BLUE_TEXT"DROPPING"DEFAULT"]\n"
            YELLOW_TEXT"\tBROADCAST MESSAGES\n"DEFAULT
            YELLOW_TEXT"\tDL_SCH MESSAGES\n"DEFAULT
            YELLOW_TEXT"\tPDCCH MESSAGES\n"DEFAULT);
}

void printlog_rap(struct RANDOM_ACCESS_PREAMBLE rap_msg)
{
    printf("["RED_TEXT"RANDOM_ACCESS_PREAMBLE"DEFAULT"]\n"
            YELLOW_TEXT"\tRA_RNTI"DEFAULT" = %d\n"
            YELLOW_TEXT"\tPREAMBLE"DEFAULT" = %d\n",
            rap_msg.RA_RNTI, rap_msg.preamble);
}

void printlog_uci(struct UPLINK_CONTROL_INFORMATION uci_msg)
{
    printf("["RED_TEXT"UCI"DEFAULT"]\n"
                "\t["RED_TEXT"UE_INFO"DEFAULT"]\n"
                YELLOW_TEXT"\t\tUE_STATE"DEFAULT" = %d\n"
                YELLOW_TEXT"\t\tRNTI"DEFAULT" = %d\n"
                YELLOW_TEXT"\t\tTIMING_ADVANCE"DEFAULT" = %d\n"
                YELLOW_TEXT"\t\tUPLINK_RESOURCE_GRANT"DEFAULT" = %d\n"
                YELLOW_TEXT"\t\tUL_SCH_CONFIG"DEFAULT" = %d\n"
                YELLOW_TEXT"\t\tSRB_IDENTITY"DEFAULT" = %d\n"
                YELLOW_TEXT"\t\tDRX_CYCLE_START"DEFAULT" = %ld\n"
                YELLOW_TEXT"\t\tBATTERY_LIFE"DEFAULT" = %d\n"
            YELLOW_TEXT"\tC_RNTI"DEFAULT" = %d\n"
            YELLOW_TEXT"\tSCHEDULING_REQUEST"DEFAULT" = %d\n"
            YELLOW_TEXT"\tHARQ_ACK"DEFAULT" = %d\n"
            YELLOW_TEXT"\tCQI"DEFAULT" = %d\n",
            uci_msg.ue_info.UE_state, uci_msg.ue_info.RNTI,
            uci_msg.ue_info.timing_advance, uci_msg.ue_info.uplink_resource_grant,
            uci_msg.ue_info.ul_sch_config, uci_msg.ue_info.srb_identity,
            uci_msg.ue_info.drx_cycle_start, uci_msg.ue_info.battery_life,
            uci_msg.C_RNTI, uci_msg.scheduling_request, uci_msg.harq_ack, uci_msg.cqi);
}

void printlog_rrc_req(struct RRC_CONN_REQUEST rrc_msg)
{
    printf("["RED_TEXT"RRC_CONNECTION_REQUEST"DEFAULT"]\n"
            YELLOW_TEXT"\tC_RNTI"DEFAULT" = %d\n"
            YELLOW_TEXT"\tUE_IDENTITY"DEFAULT" = %d\n"
            YELLOW_TEXT"\tESTABLISHMENT_CAUSE"DEFAULT" = %d\n",
            rrc_msg.C_RNTI, rrc_msg.UE_identity, rrc_msg.establishment_cause);
}

void printlog_rrc_complete(struct RRC_CONN_SETUP_COMPLETE rrc_msg)
{
    printf("["RED_TEXT"RRC_CONNECTION_SETUP_COMPLETE"DEFAULT"]\n"
            YELLOW_TEXT"\tC_RNTI"DEFAULT" = %d\n"
            YELLOW_TEXT"\tPLMN_IDENTITY"DEFAULT" = %d\n"
            YELLOW_TEXT"\tOLD_TAI"DEFAULT" = %d\n"
            YELLOW_TEXT"\tOLD_GUMMEI"DEFAULT" = %d\n"
            YELLOW_TEXT"\tOLD_GUTI"DEFAULT" = %d\n"
            YELLOW_TEXT"\tUL_SCH_CONFIG"DEFAULT" = %d\n"
            YELLOW_TEXT"\tPHR_CONFIG"DEFAULT" = %d\n"
            YELLOW_TEXT"\tUPLINK_POWER_CONTROL"DEFAULT" = %d\n",
            rrc_msg.C_RNTI, rrc_msg.PLMN_identity, rrc_msg.old_tai,
            rrc_msg.old_gummei, rrc_msg.old_guti, rrc_msg.ul_sch_config,
            rrc_msg.phr_config, rrc_msg.uplink_power_control);
}

void printlog_default(void)
{
    printf("DEBUG\n");
}