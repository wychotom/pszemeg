#include "header.h"

#define GREEN_TEXT "\e[1;36m"
#define YELLOW_TEXT "\033[1;33m"
#define DEFAULT "\e[0m"

void printlog_mib(struct MIB_MESSAGE mib_msg)
{
    printf("["GREEN_TEXT"MIB"DEFAULT"]\n"
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
    printf("["GREEN_TEXT"DCI"DEFAULT"]\n"
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
    printf("["GREEN_TEXT"RANDOM_ACCESS_RESPONSE"DEFAULT"]\n"
            YELLOW_TEXT"\tTIMING_ADVANCE"DEFAULT" = %d\n"
            YELLOW_TEXT"\tUPLINK_RESOURCE_GRANT"DEFAULT" = %d\n"
            YELLOW_TEXT"\tTEMPORARY_C_RNTI"DEFAULT" = %d\n"
            YELLOW_TEXT"\tCHECKSUM"DEFAULT" = %ld\n",
			rar_msg.timing_advance, rar_msg.uplink_resource_grant,
			rar_msg.temporary_c_rnti, rar_msg.checksum);
}

void printlog_rrc(struct RRC_CONN_SETUP rrc_msg)
{
    printf("["GREEN_TEXT"RRC_CONNECTION_SETUP"DEFAULT"]\n"
            YELLOW_TEXT"\tC_RNTI"DEFAULT" = %d\n"
            YELLOW_TEXT"\tSRB_IDENTITY"DEFAULT" = %d\n"
            YELLOW_TEXT"\tDL_AM_RLC"DEFAULT" = %d\n"
            YELLOW_TEXT"\tUL_AM_RLC"DEFAULT" = %d\n"
            YELLOW_TEXT"\tUL_SCH_RLC"DEFAULT" = %d\n"
            YELLOW_TEXT"\tPHR_CONFIG"DEFAULT" = %d\n"
            YELLOW_TEXT"\tON_DURATION_TIMER"DEFAULT" = %d\n"
            YELLOW_TEXT"\tDRX_CYCLE_TYPE"DEFAULT" = %d\n",
            rrc_msg.C_RNTI, rrc_msg.srb_identity, rrc_msg.dl_am_rlc, rrc_msg.ul_am_rlc,
            rrc_msg.ul_sch_config, rrc_msg.phr_config,
            rrc_msg.uplink_power_control.on_duration_timer, rrc_msg.uplink_power_control.drx_cycle_type);
}

void printlog_drx(struct DRX_CONFIG drx_conf)
{
    printf("["GREEN_TEXT"DRX_CONFIG"DEFAULT"]\n"
            YELLOW_TEXT"\tON_DURATION_TIMER"DEFAULT" = %d\n"
            YELLOW_TEXT"\tSHORT_DRX_TIMER"DEFAULT" = %d\n"
            YELLOW_TEXT"\tLONG_DRX_TIMER"DEFAULT" = %d\n"
            YELLOW_TEXT"\tDRX_CYCLE_TYPE"DEFAULT" = %d\n",
            drx_conf.on_duration_timer, drx_conf.short_drx_timer,
            drx_conf.long_drx_timer, drx_conf.drx_cycle_type);
}

void printlog_drop(struct eNB_conn_info connections)
{
    printf("["GREEN_TEXT"DROPPING"DEFAULT"]\n"
            YELLOW_TEXT"\tBROADCAST MESSAGES\n"DEFAULT
            YELLOW_TEXT"\tDL_SCH MESSAGES\n"DEFAULT
            YELLOW_TEXT"\tPDCCH MESSAGES\n"DEFAULT);
}

void printlog_default(void)
{
    printf("DEBUG\n");
}