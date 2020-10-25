import sys
import os
import re
import can.io.logger
import numpy as np
import struct as st
import ctypes
from CanMessage import CanMessage

#%% CAN Message IDs
# ICM-20600
DLG_ICM_20600_GENERAL_ID                = 0x14 # 20
DLG_ICM_20600_ACCEL_ID                  = 0x15 # 21
DLG_ICM_20600_GYRO_ID                   = 0x16 # 22
DLG_ICM_20600_TEMPERATURE_ID            = 0x17 # 23
DLG_ICM_20600_SELF_TEST_ID              = 0x18 # 24
DLG_ICM_20600_FACTORY_ACCEL_TRIM_ID     = 0x19 # 25
DLG_ICM_20600_FACTORY_GYRO_TRIM_ID      = 0x1A # 26
DLG_ICM_20600_FACTORY_ACCL_TRIM_DEV_ID  = 0x1B # 27
DLG_ICM_20600_FACTORY_GYRO_TRIM_DEV_ID  = 0x1C # 28

# INA-219
DD_INA_219_DATA_A                        = 0x1E # 30
DD_INA_219_DATA_B                        = 0x1F # 31

# MAX-30102
DLG_MAX_30102_GENERAL_ID                = 0x3C # 60
DLG_MAX_30102_DATA_ID                   = 0x3D # 61

# I2C ERROR
DLG_I2C_ERROR_ID                        = 0x28 # 40

# DLG specific
DLG_LOG_General                         = 0x32 # 50

# ADC specific
DLG_ADC                                 = 0x46 # 70

# SENSE TS specific
DLG_SENSE_TS                            = 0x50 # 80

# %% Helpers
def atoi(text):
    return int(text) if text.isdigit() else text

def natural_keys(text):
    '''
    alist.sort(key=natural_keys) sorts in human order
    http://nedbatchelder.com/blog/200712/human_sorting.html
    (See Toothy's implementation in the comments)
    '''
    return [ atoi(c) for c in re.split(r'(\d+)', text) ]

# %% Extract all .sbf files
# search_path = sys.argv[1]
search_path = "E:/"

print("Searching in " + search_path + " for .sbf files")

files = []
# r=root, d=directories, f = files
for r, d, f in os.walk(search_path):
    for file in f:
        if '.sbf' in file:
            files.append(os.path.join(r, file))

print("Found " + str(len(files)) + " .sfb files")

# Sort files
files.sort(key=natural_keys, reverse=False)

path, base_name = os.path.split(files[0])
blf_file_name = path + os.sep + base_name.split('_')[0] + ".blf"

print("Opening file " + blf_file_name)

# .blf file name
writer = can.io.logger.Logger(blf_file_name)

# %% CAN Setup
# Channel number
writer.channel = 1

# Message layout
DLG_ICM_20600_GENERAL = CanMessage('DLG_ICM_20600_GENERAL', 20)
# DLG_ICM_20600_GENERAL signals
DLG_ICM_20600_GENERAL.AddSignal('DD_ICM_20600_chip_id',          1, 0.0, 8, 0)
DLG_ICM_20600_GENERAL.AddSignal('DD_ICM_20600_device_state',     1, 0.0, 8, 8)
DLG_ICM_20600_GENERAL.AddSignal('DD_ICM_20600_is_calibrated',    1, 0.0, 1, 16)
DLG_ICM_20600_GENERAL.AddSignal('DD_ICM_20600_self_test_passed', 1, 0.0, 1, 17)

# def AddSignal(self, name, ScaleFactor, offset, LengthInBit, Startbit):

DLG_ICM_20600_ACCEL = CanMessage('DLG_ICM_20600_ACCEL', 21)
# DLG_ICM_20600_ACCEL signals
DLG_ICM_20600_ACCEL.AddSignal('DD_ICM_20600_raw_accel_x', 1, -32768, 16, 0)
DLG_ICM_20600_ACCEL.AddSignal('DD_ICM_20600_raw_accel_y', 1, -32768, 16, 16)
DLG_ICM_20600_ACCEL.AddSignal('DD_ICM_20600_raw_accel_z', 1, -32768, 16, 32)


DLG_ICM_20600_GYRO = CanMessage('DLG_ICM_20600_GYRO', 22)
# DLG_ICM_20600_GYRO signals
DLG_ICM_20600_GYRO.AddSignal('DD_ICM_20600_raw_gyro_x', 1, -32768, 16, 0)
DLG_ICM_20600_GYRO.AddSignal('DD_ICM_20600_raw_gyro_y', 1, -32768, 16, 16)
DLG_ICM_20600_GYRO.AddSignal('DD_ICM_20600_raw_gyro_z', 1, -32768, 16, 32)


DLG_ICM_20600_TEMPERATURE = CanMessage('DLG_ICM_20600_TEMPERATURE', 23)
# DLG_ICM_20600_TEMPERATURE signals
DLG_ICM_20600_TEMPERATURE.AddSignal('DD_ICM_20600_temperature_raw', 1,        0.0, 16, 0)
DLG_ICM_20600_TEMPERATURE.AddSignal('DD_ICM_20600_temperature_deg', 0.01, -327.68, 16, 16)


DLG_ICM_20600_SELF_TEST             = CanMessage('DLG_ICM_20600_SELF_TEST', 24)
# DLG_ICM_20600_SELF_TEST signals
DLG_ICM_20600_SELF_TEST.AddSignal('DD_ICM_20600_self_test_xa', 1, 0.0, 8, 0)
DLG_ICM_20600_SELF_TEST.AddSignal('DD_ICM_20600_self_test_xg', 1, 0.0, 8, 8)
DLG_ICM_20600_SELF_TEST.AddSignal('DD_ICM_20600_self_test_ya', 1, 0.0, 8, 16)
DLG_ICM_20600_SELF_TEST.AddSignal('DD_ICM_20600_self_test_yg', 1, 0.0, 8, 24)
DLG_ICM_20600_SELF_TEST.AddSignal('DD_ICM_20600_self_test_za', 1, 0.0, 8, 32)
DLG_ICM_20600_SELF_TEST.AddSignal('DD_ICM_20600_self_test_zg', 1, 0.0, 8, 40)



DLG_ICM_20600_FACTORY_ACCEL_TRIM = CanMessage('DLG_ICM_20600_FACTORY_ACCEL_TRIM', 25)
DLG_ICM_20600_FACTORY_ACCEL_TRIM.AddSignal('DD_ICM_20600_factory_trim_xa', 1, 0.0, 16, 0)
DLG_ICM_20600_FACTORY_ACCEL_TRIM.AddSignal('DD_ICM_20600_factory_trim_ya', 1, 0.0, 16, 16)
DLG_ICM_20600_FACTORY_ACCEL_TRIM.AddSignal('DD_ICM_20600_factory_trim_za', 1, 0.0, 16, 32)

DLG_ICM_20600_FACTORY_GYRO_TRIM     = CanMessage('DLG_ICM_20600_GENERAL', 26)
DLG_ICM_20600_FACTORY_ACCL_TRIM_DEV = CanMessage('DLG_ICM_20600_GENERAL', 27)
DLG_ICM_20600_FACTORY_GYRO_TRIM_DEV = CanMessage('DLG_ICM_20600_GENERAL', 28)
DLG_INA_219_DATA_A                  = CanMessage('DLG_ICM_20600_GENERAL', 30)
DLG_INA_219_DATA_B                  = CanMessage('DLG_ICM_20600_GENERAL', 31)
DLG_I2C_ERROR                       = CanMessage('DLG_ICM_20600_GENERAL', 40)
DLG_LOG_GENERAL                     = CanMessage('DLG_ICM_20600_GENERAL', 50)
DLG_MAX_30102_GENERAL               = CanMessage('DLG_ICM_20600_GENERAL', 60)
DLG_MAX_30102_DATA                  = CanMessage('DLG_ICM_20600_GENERAL', 61)
DLG_ADC_DATA                        = CanMessage('DLG_ICM_20600_GENERAL', 70)
DLG_SENSE_TS_DATA                   = CanMessage('DLG_ICM_20600_GENERAL', 80)



# %% Define .sbf file layout

# Check https://docs.python.org/3/library/struct.html for format characters
dlg_log_data_fmt = '< 25f 2I 8H 7h 25B x'

struct_len = st.calcsize( dlg_log_data_fmt )
struct_unpack = st.Struct( dlg_log_data_fmt ).unpack_from


for sfb_file in files:

    print("Writing content of:", sfb_file)

    with open( sfb_file, "rb" ) as file:
        while True:
            data = file.read( struct_len )
            if not data: break
            [
            dlg_time_stamp_f32,
            icm_20600_temperature_deg_f32,
            icm_20600_factory_trim_xa_f32,
            icm_20600_factory_trim_ya_f32,
            icm_20600_factory_trim_za_f32,
            icm_20600_factory_trim_xg_f32,
            icm_20600_factory_trim_yg_f32,
            icm_20600_factory_trim_zg_f32,
            icm_20600_factory_trim_dev_xa_f32,
            icm_20600_factory_trim_dev_ya_f32,
            icm_20600_factory_trim_dev_za_f32,
            icm_20600_factory_trim_dev_xg_f32,
            icm_20600_factory_trim_dev_yg_f32,
            icm_20600_factory_trim_dev_zg_f32,
            max_30102_temperature_f32,
            adc_raw_level_f32,
            adc_filtered_level_f32,
            sense_ts_alpha_filtered_adc_level_f32,
            sense_ts_alpha_beta_filtered_adc_level_f32,
            sense_ts_touch_confidence_f32,
            sense_ts_touch_confidence_max_f32,
            dd_ina_219_shunt_voltage_mV_f32,
            dd_ina_219_bus_voltage_V_f32,
            dd_ina_219_power_mW_f32,
            dd_ina_219_current_mA_f32,

            max_30102_red_data_raw_u32,
            max_30102_ir_data_raw_u32,

            icm_20600_temperature_raw_u16,
            adc_raw_data_u16,
            adc_previous_raw_data_u16,
            adc_voltage_u16,
            dd_ina_219_shunt_voltage_raw_u16,
            dd_ina_219_power_raw_u16,
            dd_ina_219_current_raw_u16,
            dd_ina_219_bus_voltage_raw_u16,

            icm_20600_accel_raw_data_x_s16,
            icm_20600_accel_raw_data_y_s16,
            icm_20600_accel_raw_data_z_s16,
            icm_20600_gyro_raw_data_x_s16,
            icm_20600_gyro_raw_data_y_s16,
            icm_20600_gyro_raw_data_z_s16,
            i2c_error_code_s16,

            icm_20600_chip_id_u8,
            icm_20600_dev_state_u8,
            icm_20600_is_calibrated_u8,
            icm_20600_self_test_passed_u8,
            icm_20600_self_test_xa_u8,
            icm_20600_self_test_ya_u8,
            icm_20600_self_test_za_u8,
            icm_20600_self_test_xg_u8,
            icm_20600_self_test_yg_u8,
            icm_20600_self_test_zg_u8,
            i2c_is_error_present_b,
            i2c_access_type_u8,
            i2c_device_addr_u8,
            i2c_register_addr_u8,
            max_30102_int_status_u8,
            max_30102_part_id_u8,
            max_30102_rev_id_u8,
            max_30102_read_ptr_u8,
            max_30102_write_ptr_u8,
            max_30102_mode_u8,
            max_30102_temperature_raw_int_u8,
            max_30102_temperature_raw_frac_u8,
            dd_ina_219_shunt_voltage_range_u8,
            dd_ina_219_bus_voltage_range_u8,

            dlg_global_msg_cnt_u8,

            ] = struct_unpack( data )

            msg_icm_20600_accel_data = st.pack( '3h', icm_20600_accel_raw_data_x_s16,
                                                      icm_20600_accel_raw_data_y_s16,
                                                      icm_20600_accel_raw_data_z_s16 )

            msg_icm_20600_accel = can.Message( arbitration_id=DLG_ICM_20600_ACCEL_ID,
                                               is_extended_id=False,
                                               timestamp=dlg_time_stamp_f32,
                                               data=msg_icm_20600_accel_data )

            msg_icm_20600_gyro_data = st.pack( '3h', icm_20600_gyro_raw_data_x_s16,
                                                     icm_20600_gyro_raw_data_y_s16,
                                                     icm_20600_gyro_raw_data_z_s16 )

            msg_icm_20600_gyro = can.Message( arbitration_id=DLG_ICM_20600_GYRO_ID,
                                              is_extended_id=False,
                                              timestamp=dlg_time_stamp_f32,
                                              data=msg_icm_20600_gyro_data )

            msg_icm_20600_general_data = st.pack( '3B', np.uint8( icm_20600_chip_id_u8 ),
                                                        np.uint8( icm_20600_dev_state_u8 ),
                                                        np.uint8(   ( (icm_20600_self_test_passed_u8 & 0x01 ) << 1)
                                                                  | ( icm_20600_is_calibrated_u8 & 0x01           ) ) )

            msg_icm_20600_general = can.Message( arbitration_id=DLG_ICM_20600_GENERAL_ID,
                                                 is_extended_id=False,
                                                 timestamp=dlg_time_stamp_f32,
                                                 data=msg_icm_20600_general_data )

            # Scale (according to DLG.dbc) and pack date
            icm_20600_temperature_deg_f32 = icm_20600_temperature_deg_f32 * 100

            msg_icm_20600_temperature_date = st.pack( 'Hh', np.uint16(icm_20600_temperature_raw_u16),
                                                            np.int16(icm_20600_temperature_deg_f32 ) )

            msg_icm_20600_temperature = can.Message( arbitration_id=DLG_ICM_20600_TEMPERATURE_ID,
                                                     is_extended_id=False,
                                                     timestamp=dlg_time_stamp_f32,
                                                     data=msg_icm_20600_temperature_date )

            msg_icm_20600_self_test_data = st.pack( '6B', np.uint8( icm_20600_self_test_xa_u8 ),
                                                          np.uint8( icm_20600_self_test_ya_u8 ),
                                                          np.uint8( icm_20600_self_test_za_u8 ),
                                                          np.uint8( icm_20600_self_test_xg_u8 ),
                                                          np.uint8( icm_20600_self_test_yg_u8 ),
                                                          np.uint8( icm_20600_self_test_zg_u8 ) )

            msg_icm_20600_self_test = can.Message( arbitration_id=DLG_ICM_20600_SELF_TEST_ID,
                                                   is_extended_id=False,
                                                   timestamp=dlg_time_stamp_f32,
                                                   data=msg_icm_20600_self_test_data )

            msg_icm_20600_factory_accl_trim_data = st.pack( '3h', np.int16( icm_20600_factory_trim_xa_f32 ),
                                                                  np.int16( icm_20600_factory_trim_ya_f32 ),
                                                                  np.int16( icm_20600_factory_trim_za_f32 ) )

            msg_icm_20600_factory_accl_trim = can.Message( arbitration_id=DLG_ICM_20600_FACTORY_ACCEL_TRIM_ID,
                                                           is_extended_id=False,
                                                           timestamp=dlg_time_stamp_f32,
                                                           data=msg_icm_20600_factory_accl_trim_data )

            msg_icm_20600_factory_gyro_trim_data = st.pack( '3h', np.int16( icm_20600_factory_trim_xg_f32 ),
                                                                  np.int16( icm_20600_factory_trim_yg_f32 ),
                                                                  np.int16( icm_20600_factory_trim_zg_f32 ) )

            msg_icm_20600_factory_gyro_trim = can.Message( arbitration_id=DLG_ICM_20600_FACTORY_GYRO_TRIM_ID,
                                                           is_extended_id=False,
                                                           timestamp=dlg_time_stamp_f32,
                                                           data=msg_icm_20600_factory_gyro_trim_data )

            # Scale (according to DLG.dbc) and pack date
            icm_20600_factory_trim_dev_xa_f32 = icm_20600_factory_trim_dev_xa_f32 * 1000.0
            icm_20600_factory_trim_dev_ya_f32 = icm_20600_factory_trim_dev_ya_f32 * 1000.0
            icm_20600_factory_trim_dev_za_f32 = icm_20600_factory_trim_dev_za_f32 * 1000.0

            msg_icm_20600_factory_accl_trim_dev_data = st.pack( '3h', np.int16( icm_20600_factory_trim_dev_xa_f32 ),
                                                                      np.int16( icm_20600_factory_trim_dev_ya_f32 ),
                                                                      np.int16( icm_20600_factory_trim_dev_za_f32 ) )

            msg_icm_20600_accl_factory_trim_dev = can.Message( arbitration_id=DLG_ICM_20600_FACTORY_ACCL_TRIM_DEV_ID,
                                                               is_extended_id=False,
                                                               timestamp=dlg_time_stamp_f32,
                                                               data=msg_icm_20600_factory_accl_trim_dev_data )

            # Scale (according to DLG.dbc) and pack date
            icm_20600_factory_trim_dev_xg_f32 = icm_20600_factory_trim_dev_xg_f32 * 1000.0
            icm_20600_factory_trim_dev_yg_f32 = icm_20600_factory_trim_dev_yg_f32 * 1000.0
            icm_20600_factory_trim_dev_zg_f32 = icm_20600_factory_trim_dev_zg_f32 * 1000.0

            msg_icm_20600_factory_gyro_trim_dev_data = st.pack( '3h', np.int16( icm_20600_factory_trim_dev_xg_f32 ),
                                                                      np.int16( icm_20600_factory_trim_dev_yg_f32 ),
                                                                      np.int16( icm_20600_factory_trim_dev_zg_f32 ) )

            msg_icm_20600_gyro_factory_trim_dev = can.Message( arbitration_id=DLG_ICM_20600_FACTORY_GYRO_TRIM_DEV_ID,
                                                               is_extended_id=False,
                                                               timestamp=dlg_time_stamp_f32,
                                                               data=msg_icm_20600_factory_gyro_trim_dev_data )


            msg_max_30102_general_data = st.pack( '8B', np.uint8( max_30102_int_status_u8 ),
                                                        np.uint8( max_30102_part_id_u8 ),
                                                        np.uint8( max_30102_rev_id_u8 ),
                                                        np.uint8( max_30102_read_ptr_u8 ),
                                                        np.uint8( max_30102_write_ptr_u8 ),
                                                        np.uint8( max_30102_mode_u8 ),
                                                        np.uint8( max_30102_temperature_raw_int_u8 ),
                                                        np.uint8( max_30102_temperature_raw_frac_u8 ) )

            msg_max_30102_general = can.Message( arbitration_id=DLG_MAX_30102_GENERAL_ID,
                                                 is_extended_id=False,
                                                 timestamp=dlg_time_stamp_f32,
                                                 data=msg_max_30102_general_data )


            # Scale (according to DLG.dbc) and pack data
            max_30102_temperature_f32  = max_30102_temperature_f32  * 100.0

            msg_max_30102_data_data = st.pack( 'h H 3B', np.int16( max_30102_temperature_f32 ),
                                                         np.uint16( max_30102_red_data_raw_u32 ),
                                                         np.uint8( np.uint8( max_30102_ir_data_raw_u32 << 2 ) | np.uint8( ( max_30102_red_data_raw_u32 >> 24 ) & 0x03 ) ),
                                                         np.uint8( max_30102_ir_data_raw_u32 >> 6 ),
                                                         np.uint8( max_30102_ir_data_raw_u32 >> 14 ) )

            msg_max_30102_data = can.Message( arbitration_id=DLG_MAX_30102_DATA_ID,
                                              is_extended_id=False,
                                              timestamp=dlg_time_stamp_f32,
                                              data=msg_max_30102_data_data )


            msg_i2c_error_data = st.pack( '4B h', np.uint8( i2c_is_error_present_b ),
                                                  np.uint8( i2c_access_type_u8 ),
                                                  np.uint8( i2c_device_addr_u8 ),
                                                  np.uint8( i2c_register_addr_u8 ),
                                                  np.int16( i2c_error_code_s16 ) )

            msg_i2c_error = can.Message( arbitration_id=DLG_I2C_ERROR_ID,
                                         is_extended_id=False,
                                         timestamp=dlg_time_stamp_f32,
                                         data=msg_i2c_error_data )

            msg_dlg_log_general_data = st.pack( 'B', np.int16( dlg_global_msg_cnt_u8 ) )

            msg_dlg_log_general = can.Message( arbitration_id=DLG_LOG_General,
                                               is_extended_id=False,
                                               timestamp=dlg_time_stamp_f32,
                                               data=msg_dlg_log_general_data )

            # DLG_ADC
            
            # Scale (according to DLG.dbc) and pack data
            adc_raw_level_f32       = adc_raw_level_f32       * 100.0
            adc_filtered_level_f32  = adc_filtered_level_f32  * 100.0

            msg_adc_data_data = st.pack( '3H 2B', np.uint16( adc_previous_raw_data_u16 ),
                                                  np.uint16( adc_raw_data_u16 ),
                                                  np.uint16( adc_voltage_u16 ),
                                                  np.uint8( adc_raw_level_f32 ),
                                                  np.uint8( adc_filtered_level_f32 ) )

            msg_adc_data = can.Message( arbitration_id=DLG_ADC,
                                          is_extended_id=False,
                                          timestamp=dlg_time_stamp_f32,
                                          data=msg_adc_data_data )

            # DLG_SENSE_TS

            # Scale (according to DLG.dbc) and pack data
            sense_ts_alpha_filtered_adc_level_f32       = sense_ts_alpha_filtered_adc_level_f32 * 100.0
            sense_ts_alpha_beta_filtered_adc_level_f32  = sense_ts_alpha_beta_filtered_adc_level_f32 * 100.0
            sense_ts_touch_confidence_f32               = sense_ts_touch_confidence_f32 * 100.0
            sense_ts_touch_confidence_max_f32           = sense_ts_touch_confidence_max_f32 * 100.0


            msg_sense_ts_data_b_data = st.pack( '4B', np.uint8( sense_ts_alpha_filtered_adc_level_f32 ),
                                                      np.uint8( sense_ts_alpha_beta_filtered_adc_level_f32 ),
                                                      np.uint8( sense_ts_touch_confidence_f32 ),
                                                      np.uint8( sense_ts_touch_confidence_max_f32 ) )

            msg_sense_ts_data = can.Message( arbitration_id=DLG_SENSE_TS,
                                             is_extended_id=False,
                                             timestamp=dlg_time_stamp_f32,
                                             data=msg_sense_ts_data_b_data )

            # DD_INA_219_DATA_A
            msg_ina_219_data_a_data = st.pack( '4H', np.uint16( dd_ina_219_bus_voltage_raw_u16   ),
                                                     np.uint16( dd_ina_219_shunt_voltage_raw_u16 ),
                                                     np.uint16( dd_ina_219_current_raw_u16       ),
                                                     np.uint16( dd_ina_219_power_raw_u16         ) )

            msg_ina_219_data_a = can.Message( arbitration_id=DD_INA_219_DATA_A,
                                              is_extended_id=False,
                                              timestamp=dlg_time_stamp_f32,
                                              data=msg_ina_219_data_a_data )


            # DD_INA_219_DATA_B
            
            # Scale (according to DLG.dbc) and pack data
            dd_ina_219_shunt_voltage_mV_f32 = dd_ina_219_shunt_voltage_mV_f32 * 10.0
            dd_ina_219_bus_voltage_V_f32    = dd_ina_219_bus_voltage_V_f32 * 10.0

            msg_ina_219_data_b_data = st.pack( '4B 2H', np.uint8( dd_ina_219_current_mA_f32 ),
                                                        np.uint8( dd_ina_219_bus_voltage_V_f32 ),
                                                        np.uint8( dd_ina_219_shunt_voltage_range_u8),
                                                        np.uint8( dd_ina_219_bus_voltage_range_u8 ),
                                                        np.uint16( dd_ina_219_shunt_voltage_mV_f32 ),
                                                        np.uint16( dd_ina_219_power_mW_f32 ) )

            msg_ina_219_data_b = can.Message( arbitration_id=DD_INA_219_DATA_B,
                                              is_extended_id=False,
                                              timestamp=dlg_time_stamp_f32,
                                              data=msg_ina_219_data_b_data )




            # DD ICM-20600 Messages
            writer.on_message_received( msg_icm_20600_accel )
            writer.on_message_received( msg_icm_20600_gyro )
            writer.on_message_received( msg_icm_20600_general )
            writer.on_message_received( msg_icm_20600_temperature )
            writer.on_message_received( msg_icm_20600_self_test )
            writer.on_message_received( msg_icm_20600_factory_accl_trim )
            writer.on_message_received( msg_icm_20600_factory_gyro_trim )
            writer.on_message_received( msg_icm_20600_accl_factory_trim_dev )
            writer.on_message_received( msg_icm_20600_gyro_factory_trim_dev )
            writer.on_message_received( msg_dlg_log_general )

            # DD MAX-30102 Messages
            writer.on_message_received( msg_max_30102_general )
            writer.on_message_received( msg_max_30102_data )

            # DD I2C Messaegs
            writer.on_message_received( msg_i2c_error )

            # DD ADC Messaegs
            writer.on_message_received( msg_adc_data )

            # DD INA-219 Messaegs
            writer.on_message_received( msg_ina_219_data_a )
            writer.on_message_received( msg_ina_219_data_b )

            # SENSE TS Messaegs
            writer.on_message_received( msg_sense_ts_data )



            # print( "Time: " + str( global_time_f32 ) )
writer.stop()

print("Done")
