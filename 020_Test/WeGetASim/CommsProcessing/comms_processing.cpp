#include "comms_processing.h"

#include "process_message.h"

#include <string.h>

double timestamp_ms = 0;

extern void reset_readtarget_list(void);
extern void empty_readtrack_list(void);

extern char  received_apps_ID[8];
extern U8 current_video_frame_ID_3bits_u8;
extern unsigned int CRB_status_rec;

float Fixed_Echo_speed_old;
float Fixed_Echo_level_dB_old;

void set_mest_version(void);
extern void PostProcessVideoGeneralData(const unsigned long CAN_message_id);
extern void PostProcessVideoObjectData(const unsigned long CAN_message_id);

unsigned int VideoProtocol_NumBursts = 0;

F32 birdview_radar_fov_x_bkpt_ve[(2*FU_RADAR_FOV_NR_X_BKPTS)-1];
F32 birdview_radar_fov_y_bkpt_ve[(2*FU_RADAR_FOV_NR_X_BKPTS)-1];

void ProcessIvecoCAN(void) {
    postProcessIvecoCAN(0x1CF0040F);
    postProcessIvecoCAN(0x1CF0030F);
    postProcessIvecoCAN(0x1C0B2A0F);
    postProcessIvecoCAN(0x18FCDC00);
    postProcessIvecoCAN(0xCFBF30B);
    postProcessIvecoCAN(0x18F0010F);
    postProcessIvecoCAN(0x18FDC40F);
    postProcessIvecoCAN(0x18FEEE00);
    postProcessIvecoCAN(0x18FE400F);
    postProcessIvecoCAN(0x1CFDCC0F);
    postProcessIvecoCAN(0x18FEC10F);
    postProcessIvecoCAN(0x18FFC521);
    postProcessIvecoCAN(0x18F0090F);
    postProcessIvecoCAN(0x18FE6E0F);
}


void GenerateRadarFov(F32 p_x_bkpt[], F32 p_y_bkpt[])
{
    birdview_radar_fov_y_bkpt_ve[0]  = -p_y_bkpt[0];
    birdview_radar_fov_y_bkpt_ve[1]  = -p_y_bkpt[1];
    birdview_radar_fov_y_bkpt_ve[2]  = -p_y_bkpt[2];
    birdview_radar_fov_y_bkpt_ve[3]  = -p_y_bkpt[3];
    birdview_radar_fov_y_bkpt_ve[4]  = -p_y_bkpt[4];
    birdview_radar_fov_y_bkpt_ve[5]  = -p_y_bkpt[5];
    birdview_radar_fov_y_bkpt_ve[6]  = -p_y_bkpt[6];

    birdview_radar_fov_y_bkpt_ve[7]  = p_y_bkpt[7];

    birdview_radar_fov_y_bkpt_ve[8]  = p_y_bkpt[6];
    birdview_radar_fov_y_bkpt_ve[9]  = p_y_bkpt[5];
    birdview_radar_fov_y_bkpt_ve[10] = p_y_bkpt[4];
    birdview_radar_fov_y_bkpt_ve[11] = p_y_bkpt[3];
    birdview_radar_fov_y_bkpt_ve[12] = p_y_bkpt[2];
    birdview_radar_fov_y_bkpt_ve[13] = p_y_bkpt[1];
    birdview_radar_fov_y_bkpt_ve[14] = p_y_bkpt[0];


    birdview_radar_fov_x_bkpt_ve[0]  = p_x_bkpt[0];
    birdview_radar_fov_x_bkpt_ve[1]  = p_x_bkpt[1];
    birdview_radar_fov_x_bkpt_ve[2]  = p_x_bkpt[2];
    birdview_radar_fov_x_bkpt_ve[3]  = p_x_bkpt[3];
    birdview_radar_fov_x_bkpt_ve[4]  = p_x_bkpt[4];
    birdview_radar_fov_x_bkpt_ve[5]  = p_x_bkpt[5];
    birdview_radar_fov_x_bkpt_ve[6]  = p_x_bkpt[6];

    birdview_radar_fov_x_bkpt_ve[7]  = p_x_bkpt[7];

    birdview_radar_fov_x_bkpt_ve[8]  = p_x_bkpt[6];
    birdview_radar_fov_x_bkpt_ve[9]  = p_x_bkpt[5];
    birdview_radar_fov_x_bkpt_ve[10] = p_x_bkpt[4];
    birdview_radar_fov_x_bkpt_ve[11] = p_x_bkpt[3];
    birdview_radar_fov_x_bkpt_ve[12] = p_x_bkpt[2];
    birdview_radar_fov_x_bkpt_ve[13] = p_x_bkpt[1];
    birdview_radar_fov_x_bkpt_ve[14] = p_x_bkpt[0];
}


void ProcessRadarProtocol()
{
    int data[7] = {0};
    postProcessRadarData(data);
    {
        int target_number_i = 0;
        for (; target_number_i< _read_gen_radar_status.number_of_targets_s16 && target_number_i < MAX_NO_OF_TARGETS; target_number_i++) {
            unsigned int target_number_i_shifted = target_number_i << 3;
            data[0] = target_number_i_shifted;
            // Part 0
            postProcessTargetData(data);
            // Part 1
            data[0] = target_number_i_shifted | 0x1;
            postProcessTargetData(data);
            // Part 2
            data[0] = target_number_i_shifted | 0x2;
            postProcessTargetData(data);
        }
    }

    {
        int track_number_i = 0;
        for (; track_number_i< _read_gen_radar_status.number_of_tracks_s16; track_number_i++) {
            data[0] = track_number_i << 3;
            // Part 0
            postProcessTrackData(data);
            // Part 1
            data[0] |= 0x1;
            postProcessTrackData(data);
        }
    }

    // special handling target_number=31
    // code from ACSIM:
#if 0
    // fixed echo relative velocity
    temp_s32 = ((data[2]&0x3F)<<8) + data[3];
    if (temp_s32 > 8191) temp_s32 -= 16384; // 14 bit
    Fixed_Echo_speed = (F32)temp_s32 * 0.01F;

    // fixed echo power
    Fixed_Echo_level_dB = (F32)data[6]*0.5F;

    Fixed_Echo_received_b = TRUE;
#endif
    Fixed_Echo_received_b = FALSE;
    if(Fixed_Echo_speed != 0 && Fixed_Echo_level_dB != 0) {
        if(Fixed_Echo_speed != Fixed_Echo_speed_old || Fixed_Echo_level_dB != Fixed_Echo_level_dB_old) {
            Fixed_Echo_received_b = TRUE;

            Fixed_Echo_speed_old = Fixed_Echo_speed;
            Fixed_Echo_level_dB_old = Fixed_Echo_level_dB;
        }
    }

    /* Generate radar FOV map based on current radar operation mode */
    switch(fu_input_s.radar_s.radar_op_mode_e)
    {
    case FU_RADAR_OPMODE_ACC:
        GenerateRadarFov(dx_ACC_bkpt, dy_ACC_dat);
        break;

    case FU_RADAR_OPMODE_AEB1:
        GenerateRadarFov(dx_AEB1_bkpt, dy_AEB1_dat);

        break;

    case FU_RADAR_OPMODE_AEB2:
        GenerateRadarFov(dx_AEB2_bkpt, dy_AEB2_dat);
        break;

    case FU_RADAR_OPMODE_AEB3:
        GenerateRadarFov(dx_AEB3_bkpt, dy_AEB3_dat);
        break;

    default:
        ASSERT(FALSE)
        break;
    }
}

void ProcessVideoProtocol(void)
{
    if(VideoProtocol_NumBursts) {
        set_mest_version();
        /******** ProcessVideoGeneralData **********/
        // VIDEO_DATA_GENERAL_A_ID
        PostProcessVideoGeneralData(VIDEO_DATA_GENERAL_A_ID);
        // VIDEO_DATA_GENERAL_B_ID
        PostProcessVideoGeneralData(VIDEO_DATA_GENERAL_B_ID);

        for (unsigned int current_video_index_u8 = 0; current_video_index_u8< MAX_NUM_MOBILEYE_OBJECTS;current_video_index_u8++){
            unsigned long CAN_message_id = current_video_index_u8*4 + VIDEO_OBJECT_0_A_ID;
            PostProcessVideoObjectData(CAN_message_id);
        }

        // Copy current lists to previous lists
        memcpy(&g_previous_video_data_s, &g_current_video_data_s, sizeof(SCALABLE_VIDEO_INPUT_STRUCT));

        // Copy preliminary lists to current lists
        memcpy(&g_current_video_data_s, &g_prelim_video_s, sizeof(SCALABLE_VIDEO_INPUT_STRUCT));

    }
}


void ProcessComms(void)
{
    timestamp_global = timestamp_ms/1000;

    // Copy of the content of preProcessCanMessageSet()
    {
        reset_readtarget_list();
        empty_readtrack_list();
        //Reset_Fusion_Data();
        // update intermediate variables

#if 0 // without this statement because it is only used by TPT and Gogeta has a different behavior than acsim. In ACSIM the function preProcessCanMessageSet() is called before the CAN Messages are interpreted and in Gogeta after
        _read_gen_radar_status.current_ramp_type_u16 = (U16)Current_ramp_type;
#endif
        //_read_gen_radar_status.state_video_mitigation_braking_u16 = (U16)state_video_mitigation_braking;
        //Reset_Fusion_Data();
    }


    ProcessIvecoCAN();
    ProcessRadarProtocol();
    ProcessVideoProtocol();

    compatibWithoutSigProcSubsys();
    Select_Mobileye_data();
}


//read_vehi_can_1_t _read_vehi_can_1
void InitComms(void)
{
    /* re simulation protocol must always be computed first */
    //InitReSimulationProtocol();
    memset(&_read_vehi_can_1,0,sizeof(read_vehi_can_1_t));
}
