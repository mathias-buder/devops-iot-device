import sys
import os
import re
import can.io.logger
import numpy as np
import struct as st
import ctypes
from CanMessage import CanMessage

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


DLG_ICM_20600_ACCEL_RAW = CanMessage('DLG_ICM_20600_ACCEL', 21)
# DLG_ICM_20600_ACCEL_RAW signals
DLG_ICM_20600_ACCEL_RAW.AddSignal('DD_ICM_20600_raw_accel_x', 1, -32768, 16, 0)
DLG_ICM_20600_ACCEL_RAW.AddSignal('DD_ICM_20600_raw_accel_y', 1, -32768, 16, 16)
DLG_ICM_20600_ACCEL_RAW.AddSignal('DD_ICM_20600_raw_accel_z', 1, -32768, 16, 32)


DLG_ICM_20600_GYRO_RAW = CanMessage('DLG_ICM_20600_GYRO', 22)
# DLG_ICM_20600_GYRO_RAW signals
DLG_ICM_20600_GYRO_RAW.AddSignal('DD_ICM_20600_raw_gyro_x', 1, -32768, 16, 0)
DLG_ICM_20600_GYRO_RAW.AddSignal('DD_ICM_20600_raw_gyro_y', 1, -32768, 16, 16)
DLG_ICM_20600_GYRO_RAW.AddSignal('DD_ICM_20600_raw_gyro_z', 1, -32768, 16, 32)


DLG_ICM_20600_TEMPERATURE = CanMessage('DLG_ICM_20600_TEMPERATURE', 23)
# DLG_ICM_20600_TEMPERATURE signals
DLG_ICM_20600_TEMPERATURE.AddSignal('DD_ICM_20600_temperature_raw', 1,        0.0, 16, 0)
DLG_ICM_20600_TEMPERATURE.AddSignal('DD_ICM_20600_temperature_deg', 0.01, -327.68, 16, 16)


DLG_ICM_20600_SELF_TEST = CanMessage('DLG_ICM_20600_SELF_TEST', 24)
# DLG_ICM_20600_SELF_TEST signals
DLG_ICM_20600_SELF_TEST.AddSignal('DD_ICM_20600_self_test_xa', 1, 0.0, 8, 0)
DLG_ICM_20600_SELF_TEST.AddSignal('DD_ICM_20600_self_test_xg', 1, 0.0, 8, 8)
DLG_ICM_20600_SELF_TEST.AddSignal('DD_ICM_20600_self_test_ya', 1, 0.0, 8, 16)
DLG_ICM_20600_SELF_TEST.AddSignal('DD_ICM_20600_self_test_yg', 1, 0.0, 8, 24)
DLG_ICM_20600_SELF_TEST.AddSignal('DD_ICM_20600_self_test_za', 1, 0.0, 8, 32)
DLG_ICM_20600_SELF_TEST.AddSignal('DD_ICM_20600_self_test_zg', 1, 0.0, 8, 40)


DLG_ICM_20600_FACTORY_ACCEL_TRIM = CanMessage('DLG_ICM_20600_FACTORY_ACCEL_TRIM', 25)
# DLG_ICM_20600_FACTORY_ACCEL_TRIM signals
DLG_ICM_20600_FACTORY_ACCEL_TRIM.AddSignal('DD_ICM_20600_factory_trim_xa', 1, -32768, 16, 0)
DLG_ICM_20600_FACTORY_ACCEL_TRIM.AddSignal('DD_ICM_20600_factory_trim_ya', 1, -32768, 16, 16)
DLG_ICM_20600_FACTORY_ACCEL_TRIM.AddSignal('DD_ICM_20600_factory_trim_za', 1, -32768, 16, 32)


DLG_ICM_20600_FACTORY_GYRO_TRIM = CanMessage('DLG_ICM_20600_FACTORY_GYRO_TRIM', 26)
# DLG_ICM_20600_FACTORY_GYRO_TRIM signals
DLG_ICM_20600_FACTORY_GYRO_TRIM.AddSignal('DD_ICM_20600_factory_trim_xg', 1, -32768, 16, 0)
DLG_ICM_20600_FACTORY_GYRO_TRIM.AddSignal('DD_ICM_20600_factory_trim_yg', 1, -32768, 16, 16)
DLG_ICM_20600_FACTORY_GYRO_TRIM.AddSignal('DD_ICM_20600_factory_trim_zg', 1, -32768, 16, 32)


DLG_ICM_20600_FACTORY_ACCEL_TRIM_DEV = CanMessage('DLG_ICM_20600_FACTORY_ACCL_TRIM_DEV', 27)
# DLG_ICM_20600_FACTORY_ACCL_TRIM_DEV signals
DLG_ICM_20600_FACTORY_ACCEL_TRIM_DEV.AddSignal('DD_ICM_20600_factory_trim_dev_xa', 0.001, -32.768, 16, 0)
DLG_ICM_20600_FACTORY_ACCEL_TRIM_DEV.AddSignal('DD_ICM_20600_factory_trim_dev_ya', 0.001, -32.768, 16, 16)
DLG_ICM_20600_FACTORY_ACCEL_TRIM_DEV.AddSignal('DD_ICM_20600_factory_trim_dev_za', 0.001, -32.768, 16, 32)


DLG_ICM_20600_FACTORY_GYRO_TRIM_DEV = CanMessage('DLG_ICM_20600_FACTORY_GYRO_TRIM_DEV', 28)
# DLG_ICM_20600_FACTORY_GYRO_TRIM_DEV signals
DLG_ICM_20600_FACTORY_GYRO_TRIM_DEV.AddSignal('DD_ICM_20600_factory_trim_dev_xg', 0.001, -32.768, 16, 0)
DLG_ICM_20600_FACTORY_GYRO_TRIM_DEV.AddSignal('DD_ICM_20600_factory_trim_dev_yg', 0.001, -32.768, 16, 16)
DLG_ICM_20600_FACTORY_GYRO_TRIM_DEV.AddSignal('DD_ICM_20600_factory_trim_dev_zg', 0.001, -32.768, 16, 32)


DLG_INA_219_DATA_A = CanMessage('DLG_INA_219_DATA_A', 30)
# DLG_INA_219_DATA_A signals
DLG_INA_219_DATA_A.AddSignal('DD_INA_219_bus_voltage_raw',      1, 0.0, 16, 0)
DLG_INA_219_DATA_A.AddSignal('DD_INA_219_shunt_voltage_raw',    1, 0.0, 16, 16)
DLG_INA_219_DATA_A.AddSignal('DD_INA_219_current_raw',          1, 0.0, 16, 32)
DLG_INA_219_DATA_A.AddSignal('DD_INA_219_power_raw',            1, 0.0, 16, 48)


DLG_INA_219_DATA_B = CanMessage('DLG_INA_219_DATA_B', 31)
# DLG_INA_219_DATA_B signals
DLG_INA_219_DATA_B.AddSignal('DD_INA_219_current',              0.1, 0.0, 14, 0)
DLG_INA_219_DATA_B.AddSignal('DD_INA_219_shunt_voltage_range',  1,   0.0, 2,  14)
DLG_INA_219_DATA_B.AddSignal('DD_INA_219_power',                0.1, 0.0, 14, 16)
DLG_INA_219_DATA_B.AddSignal('DD_INA_219_bus_voltage_range',    1,   0.0, 1,  30)
DLG_INA_219_DATA_B.AddSignal('DD_INA_219_shunt_voltage',        0.1, 0.0, 9,  32)
DLG_INA_219_DATA_B.AddSignal('DD_INA_219_bus_voltage',          0.1, 0.0, 8,  48)


DLG_I2C_ERROR = CanMessage('DLG_I2C_ERROR', 40)
# DLG_I2C_ERROR signals
DLG_I2C_ERROR.AddSignal('DD_I2C_error_state',         1,    0.0, 8, 0)
DLG_I2C_ERROR.AddSignal('DD_I2C_error_access_type',   1,    0.0, 8, 8)
DLG_I2C_ERROR.AddSignal('DD_I2C_error_device_addr',   1,    0.0, 8, 16)
DLG_I2C_ERROR.AddSignal('DD_I2C_error_register_addr', 1,    0.0, 8, 24)
DLG_I2C_ERROR.AddSignal('DD_I2C_error_code',          1, -128.0, 8, 32)


DLG_LOG_GENERAL = CanMessage('DLG_LOG_GENERAL', 50)
# DLG_LOG_GENERAL signals
DLG_LOG_GENERAL.AddSignal('DLG_LOG_global_msg_counter', 1, 0.0, 8, 0)


DLG_MAX_30102_GENERAL = CanMessage('DLG_MAX_30102_GENERAL', 60)
# DLG_MAX_30102_GENERAL signals
DLG_MAX_30102_GENERAL.AddSignal('DD_MAX_30102_int_status',   1, 0.0, 8, 0)
DLG_MAX_30102_GENERAL.AddSignal('DD_MAX_30102_part_id',      1, 0.0, 8, 8)
DLG_MAX_30102_GENERAL.AddSignal('DD_MAX_30102_rev_id',       1, 0.0, 8, 16)
DLG_MAX_30102_GENERAL.AddSignal('DD_MAX_30102_read_ptr',     1, 0.0, 8, 24)
DLG_MAX_30102_GENERAL.AddSignal('DD_MAX_30102_write_ptr',    1, 0.0, 8, 32)
DLG_MAX_30102_GENERAL.AddSignal('DD_MAX_30102_mode',         1, 0.0, 8, 40)
DLG_MAX_30102_GENERAL.AddSignal('DD_MAX_30102_temp_raw_int', 1, 0.0, 8, 48)
DLG_MAX_30102_GENERAL.AddSignal('DD_MAX_30102_temp_raw_frac',1, 0.0, 8, 56)


DLG_MAX_30102_DATA = CanMessage('DLG_MAX_30102_DATA', 61)
# DLG_MAX_30102_DATA signals
DLG_MAX_30102_DATA.AddSignal('DD_MAX_30102_temperature',   0.01, -327.68, 16, 0)
DLG_MAX_30102_DATA.AddSignal('DD_MAX_30102_red_data_raw',     1,     0.0, 18, 16)
DLG_MAX_30102_DATA.AddSignal('DD_MAX_30102_ir_data_raw',      1,     0.0, 18, 34)


DLG_ADC_DATA  = CanMessage('DLG_ADC_DATA', 70)
# DLG_ADC_DATA signals
DLG_ADC_DATA.AddSignal('DD_ADC_previous_raw_data', 1,    0.0, 16, 0)
DLG_ADC_DATA.AddSignal('DD_ADC_raw_data',          1,    0.0, 16, 16)
DLG_ADC_DATA.AddSignal('DD_ADC_voltage',           1,    0.0, 16, 32)
DLG_ADC_DATA.AddSignal('DD_ADC_raw_level',         0.01, 0.0,  8, 48)
DLG_ADC_DATA.AddSignal('DD_ADC_filtered_level',    0.01, 0.0,  8, 56)


DLG_SENSE_TS_DATA = CanMessage('DLG_SENSE_TS_DATA', 80)
# DLG_SENSE_TS_DATA signals
DLG_SENSE_TS_DATA.AddSignal('SENSE_TS_alpha_adc_level',      0.01, 0.0, 8, 0)
DLG_SENSE_TS_DATA.AddSignal('SENSE_TS_alpha_beta_adc_level', 0.01, 0.0, 8, 8)
DLG_SENSE_TS_DATA.AddSignal('SENSE_TS_touch_confidence',     0.01, 0.0, 8, 16)
DLG_SENSE_TS_DATA.AddSignal('SENSE_TS_touch_confidence_max', 0.01, 0.0, 8, 24)

DLG_DD_MCPWM_DATA_A = CanMessage('DLG_DD_MCPWM_DATA_A', 90)
# DLG_DD_MCPWM_DATA_A signals
DLG_DD_MCPWM_DATA_A.AddSignal('DD_MCPWM_Ch_1_Duty_Cycle', 0.1, 0.0, 10, 0)
DLG_DD_MCPWM_DATA_A.AddSignal('DD_MCPWM_Ch_2_Duty_Cycle', 0.1, 0.0, 10, 10)
DLG_DD_MCPWM_DATA_A.AddSignal('DD_MCPWM_Ch_3_Duty_Cycle', 0.1, 0.0, 10, 20)
DLG_DD_MCPWM_DATA_A.AddSignal('DD_MCPWM_Ch_4_Duty_Cycle', 0.1, 0.0, 10, 30)
DLG_DD_MCPWM_DATA_A.AddSignal('DD_MCPWM_Ch_5_Duty_Cycle', 0.1, 0.0, 10, 40)
DLG_DD_MCPWM_DATA_A.AddSignal('DD_MCPWM_Ch_6_Duty_Cycle', 0.1, 0.0, 10, 50)

DLG_DD_MCPWM_DATA_B = CanMessage('DLG_DD_MCPWM_DATA_B', 91)
# DLG_DD_MCPWM_DATA_B signals
DLG_DD_MCPWM_DATA_B.AddSignal('DD_MCPWM_Ch_7_Duty_Cycle',  0.1, 0.0, 10, 0)
DLG_DD_MCPWM_DATA_B.AddSignal('DD_MCPWM_Ch_8_Duty_Cycle',  0.1, 0.0, 10, 10)
DLG_DD_MCPWM_DATA_B.AddSignal('DD_MCPWM_Ch_9_Duty_Cycle',  0.1, 0.0, 10, 20)
DLG_DD_MCPWM_DATA_B.AddSignal('DD_MCPWM_Ch_10_Duty_Cycle', 0.1, 0.0, 10, 30)
DLG_DD_MCPWM_DATA_B.AddSignal('DD_MCPWM_Ch_11_Duty_Cycle', 0.1, 0.0, 10, 40)
DLG_DD_MCPWM_DATA_B.AddSignal('DD_MCPWM_Ch_12_Duty_Cycle', 0.1, 0.0, 10, 50)

DLG_DD_MCPWM_DATA_C = CanMessage('DLG_DD_MCPWM_DATA_C', 92)
# DLG_DD_MCPWM_DATA_C signals
DLG_DD_MCPWM_DATA_C.AddSignal('DD_MCPWM_Ch_1_mode',  1, 0.0, 2, 0)
DLG_DD_MCPWM_DATA_C.AddSignal('DD_MCPWM_Ch_2_mode',  1, 0.0, 2, 2)
DLG_DD_MCPWM_DATA_C.AddSignal('DD_MCPWM_Ch_3_mode',  1, 0.0, 2, 4)
DLG_DD_MCPWM_DATA_C.AddSignal('DD_MCPWM_Ch_4_mode',  1, 0.0, 2, 6)
DLG_DD_MCPWM_DATA_C.AddSignal('DD_MCPWM_Ch_5_mode',  1, 0.0, 2, 8)
DLG_DD_MCPWM_DATA_C.AddSignal('DD_MCPWM_Ch_6_mode',  1, 0.0, 2, 10)
DLG_DD_MCPWM_DATA_C.AddSignal('DD_MCPWM_Ch_7_mode',  1, 0.0, 2, 12)
DLG_DD_MCPWM_DATA_C.AddSignal('DD_MCPWM_Ch_8_mode',  1, 0.0, 2, 14)
DLG_DD_MCPWM_DATA_C.AddSignal('DD_MCPWM_Ch_9_mode',  1, 0.0, 2, 16)
DLG_DD_MCPWM_DATA_C.AddSignal('DD_MCPWM_Ch_10_mode', 1, 0.0, 2, 18)
DLG_DD_MCPWM_DATA_C.AddSignal('DD_MCPWM_Ch_11_mode', 1, 0.0, 2, 20)
DLG_DD_MCPWM_DATA_C.AddSignal('DD_MCPWM_Ch_12_mode', 1, 0.0, 2, 22)

DLG_DD_TMP_102_DATA = CanMessage('DD_TMP_102_DATA', 100)
# DLG_DD_TMP_102_DATA signals
DLG_DD_TMP_102_DATA.AddSignal('DD_TMP_102_temperature_raw',  1,   -32768.0, 16, 0)
DLG_DD_TMP_102_DATA.AddSignal('DD_TMP_102_temperature_deg',  0.01, -81.92,  14, 16)

# %% Define .sbf file layout

# Check https://docs.python.org/3/library/struct.html for format characters
dlg_log_data_fmt = '< 38f 2I 8H 8h 37B 3x'

struct_len = st.calcsize( dlg_log_data_fmt )
struct_unpack = st.Struct( dlg_log_data_fmt ).unpack_from

# %% Loop over all available .sbf fiels
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
            dd_tmp_102_temperature_deg_f32,
            dd_mcpwm_ch_1_duty_cycle_f32,
            dd_mcpwm_ch_2_duty_cycle_f32,
            dd_mcpwm_ch_3_duty_cycle_f32,
            dd_mcpwm_ch_4_duty_cycle_f32,
            dd_mcpwm_ch_5_duty_cycle_f32,
            dd_mcpwm_ch_6_duty_cycle_f32,
            dd_mcpwm_ch_7_duty_cycle_f32,
            dd_mcpwm_ch_8_duty_cycle_f32,
            dd_mcpwm_ch_9_duty_cycle_f32,
            dd_mcpwm_ch_10_duty_cycle_f32,
            dd_mcpwm_ch_11_duty_cycle_f32,
            dd_mcpwm_ch_12_duty_cycle_f32,

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
            dd_tmp_102_temperature_raw_s16,

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
            dd_mcpwm_ch_1_mode_u8,
            dd_mcpwm_ch_2_mode_u8,
            dd_mcpwm_ch_3_mode_u8,
            dd_mcpwm_ch_4_mode_u8,
            dd_mcpwm_ch_5_mode_u8,
            dd_mcpwm_ch_6_mode_u8,
            dd_mcpwm_ch_7_mode_u8,
            dd_mcpwm_ch_8_mode_u8,
            dd_mcpwm_ch_9_mode_u8,
            dd_mcpwm_ch_10_mode_u8,
            dd_mcpwm_ch_11_mode_u8,
            dd_mcpwm_ch_12_mode_u8,

            dlg_global_msg_cnt_u8,

            ] = struct_unpack( data )

            # %% Update all CAN messages
            DLG_ICM_20600_ACCEL_RAW.SetSignal( 'DD_ICM_20600_raw_accel_x', icm_20600_accel_raw_data_x_s16 )
            DLG_ICM_20600_ACCEL_RAW.SetSignal( 'DD_ICM_20600_raw_accel_y', icm_20600_accel_raw_data_y_s16 )
            DLG_ICM_20600_ACCEL_RAW.SetSignal( 'DD_ICM_20600_raw_accel_z', icm_20600_accel_raw_data_z_s16 )
            DLG_ICM_20600_ACCEL_RAW.SetTimeStamp( dlg_time_stamp_f32 )

            DLG_ICM_20600_GYRO_RAW.SetSignal( 'DD_ICM_20600_raw_gyro_x', icm_20600_gyro_raw_data_x_s16 )
            DLG_ICM_20600_GYRO_RAW.SetSignal( 'DD_ICM_20600_raw_gyro_y', icm_20600_gyro_raw_data_y_s16 )
            DLG_ICM_20600_GYRO_RAW.SetSignal( 'DD_ICM_20600_raw_gyro_z', icm_20600_gyro_raw_data_z_s16 )
            DLG_ICM_20600_GYRO_RAW.SetTimeStamp( dlg_time_stamp_f32 )
       
            DLG_ICM_20600_GENERAL.SetSignal( 'DD_ICM_20600_chip_id',          icm_20600_chip_id_u8 )
            DLG_ICM_20600_GENERAL.SetSignal( 'DD_ICM_20600_device_state',     icm_20600_dev_state_u8 )
            DLG_ICM_20600_GENERAL.SetSignal( 'DD_ICM_20600_is_calibrated',    icm_20600_is_calibrated_u8 )
            DLG_ICM_20600_GENERAL.SetSignal( 'DD_ICM_20600_self_test_passed', icm_20600_self_test_passed_u8 )
            DLG_ICM_20600_GENERAL.SetTimeStamp( dlg_time_stamp_f32 )

            DLG_ICM_20600_TEMPERATURE.SetSignal( 'DD_ICM_20600_temperature_raw', icm_20600_temperature_raw_u16 )
            DLG_ICM_20600_TEMPERATURE.SetSignal( 'DD_ICM_20600_temperature_deg', icm_20600_temperature_deg_f32 )
            DLG_ICM_20600_TEMPERATURE.SetTimeStamp( dlg_time_stamp_f32 )

            DLG_ICM_20600_SELF_TEST.SetSignal( 'DD_ICM_20600_self_test_xa', icm_20600_self_test_xa_u8 )
            DLG_ICM_20600_SELF_TEST.SetSignal( 'DD_ICM_20600_self_test_xg', icm_20600_self_test_ya_u8 )
            DLG_ICM_20600_SELF_TEST.SetSignal( 'DD_ICM_20600_self_test_ya', icm_20600_self_test_za_u8 )
            DLG_ICM_20600_SELF_TEST.SetSignal( 'DD_ICM_20600_self_test_yg', icm_20600_self_test_xg_u8 )
            DLG_ICM_20600_SELF_TEST.SetSignal( 'DD_ICM_20600_self_test_za', icm_20600_self_test_yg_u8 )
            DLG_ICM_20600_SELF_TEST.SetSignal( 'DD_ICM_20600_self_test_zg', icm_20600_self_test_zg_u8 )
            DLG_ICM_20600_SELF_TEST.SetTimeStamp( dlg_time_stamp_f32 )

            DLG_ICM_20600_FACTORY_ACCEL_TRIM.SetSignal( 'DD_ICM_20600_factory_trim_xa', icm_20600_factory_trim_xa_f32 )
            DLG_ICM_20600_FACTORY_ACCEL_TRIM.SetSignal( 'DD_ICM_20600_factory_trim_ya', icm_20600_factory_trim_ya_f32 )
            DLG_ICM_20600_FACTORY_ACCEL_TRIM.SetSignal( 'DD_ICM_20600_factory_trim_za', icm_20600_factory_trim_za_f32 )
            DLG_ICM_20600_FACTORY_ACCEL_TRIM.SetTimeStamp( dlg_time_stamp_f32 )

            DLG_ICM_20600_FACTORY_GYRO_TRIM.SetSignal( 'DD_ICM_20600_factory_trim_xg', icm_20600_factory_trim_xg_f32 )
            DLG_ICM_20600_FACTORY_GYRO_TRIM.SetSignal( 'DD_ICM_20600_factory_trim_yg', icm_20600_factory_trim_yg_f32 )
            DLG_ICM_20600_FACTORY_GYRO_TRIM.SetSignal( 'DD_ICM_20600_factory_trim_zg', icm_20600_factory_trim_zg_f32 )
            DLG_ICM_20600_FACTORY_GYRO_TRIM.SetTimeStamp( dlg_time_stamp_f32 )

            DLG_ICM_20600_FACTORY_ACCEL_TRIM_DEV.SetSignal( 'DD_ICM_20600_factory_trim_dev_xa', icm_20600_factory_trim_dev_xa_f32 )
            DLG_ICM_20600_FACTORY_ACCEL_TRIM_DEV.SetSignal( 'DD_ICM_20600_factory_trim_dev_ya', icm_20600_factory_trim_dev_ya_f32 )
            DLG_ICM_20600_FACTORY_ACCEL_TRIM_DEV.SetSignal( 'DD_ICM_20600_factory_trim_dev_za', icm_20600_factory_trim_dev_za_f32 )
            DLG_ICM_20600_FACTORY_ACCEL_TRIM_DEV.SetTimeStamp( dlg_time_stamp_f32 )

            DLG_ICM_20600_FACTORY_GYRO_TRIM_DEV.SetSignal( 'DD_ICM_20600_factory_trim_dev_xg', icm_20600_factory_trim_dev_xg_f32 )
            DLG_ICM_20600_FACTORY_GYRO_TRIM_DEV.SetSignal( 'DD_ICM_20600_factory_trim_dev_yg', icm_20600_factory_trim_dev_yg_f32 )
            DLG_ICM_20600_FACTORY_GYRO_TRIM_DEV.SetSignal( 'DD_ICM_20600_factory_trim_dev_zg', icm_20600_factory_trim_dev_zg_f32 )
            DLG_ICM_20600_FACTORY_GYRO_TRIM_DEV.SetTimeStamp( dlg_time_stamp_f32 )

            DLG_MAX_30102_GENERAL.SetSignal( 'DD_MAX_30102_int_status',    max_30102_int_status_u8 )
            DLG_MAX_30102_GENERAL.SetSignal( 'DD_MAX_30102_part_id',       max_30102_part_id_u8 )
            DLG_MAX_30102_GENERAL.SetSignal( 'DD_MAX_30102_rev_id',        max_30102_rev_id_u8 )
            DLG_MAX_30102_GENERAL.SetSignal( 'DD_MAX_30102_read_ptr',      max_30102_read_ptr_u8 )
            DLG_MAX_30102_GENERAL.SetSignal( 'DD_MAX_30102_write_ptr',     max_30102_write_ptr_u8 )
            DLG_MAX_30102_GENERAL.SetSignal( 'DD_MAX_30102_mode',          max_30102_mode_u8 )
            DLG_MAX_30102_GENERAL.SetSignal( 'DD_MAX_30102_temp_raw_int',  max_30102_temperature_raw_int_u8 )
            DLG_MAX_30102_GENERAL.SetSignal( 'DD_MAX_30102_temp_raw_frac', max_30102_temperature_raw_frac_u8 )
            DLG_MAX_30102_GENERAL.SetTimeStamp( dlg_time_stamp_f32 )

            DLG_MAX_30102_DATA.SetSignal( 'DD_MAX_30102_temperature',  max_30102_temperature_f32 )
            DLG_MAX_30102_DATA.SetSignal( 'DD_MAX_30102_red_data_raw', max_30102_red_data_raw_u32 )
            DLG_MAX_30102_DATA.SetSignal( 'DD_MAX_30102_ir_data_raw',  max_30102_ir_data_raw_u32 )
            DLG_MAX_30102_DATA.SetTimeStamp( dlg_time_stamp_f32 )

            DLG_I2C_ERROR.SetSignal( 'DD_I2C_error_state',         i2c_is_error_present_b )
            DLG_I2C_ERROR.SetSignal( 'DD_I2C_error_access_type',   i2c_access_type_u8 )
            DLG_I2C_ERROR.SetSignal( 'DD_I2C_error_device_addr',   i2c_device_addr_u8 )
            DLG_I2C_ERROR.SetSignal( 'DD_I2C_error_register_addr', i2c_register_addr_u8 )
            DLG_I2C_ERROR.SetSignal( 'DD_I2C_error_code',          i2c_error_code_s16 )
            DLG_I2C_ERROR.SetTimeStamp( dlg_time_stamp_f32 )

            DLG_ADC_DATA.SetSignal( 'DD_ADC_previous_raw_data', adc_previous_raw_data_u16 )
            DLG_ADC_DATA.SetSignal( 'DD_ADC_raw_data',          adc_raw_data_u16 )
            DLG_ADC_DATA.SetSignal( 'DD_ADC_voltage',           adc_voltage_u16 )
            DLG_ADC_DATA.SetSignal( 'DD_ADC_raw_level',         adc_raw_level_f32 )
            DLG_ADC_DATA.SetSignal( 'DD_ADC_filtered_level',    adc_filtered_level_f32 )
            DLG_ADC_DATA.SetTimeStamp( dlg_time_stamp_f32 )

            DLG_SENSE_TS_DATA.SetSignal( 'SENSE_TS_alpha_adc_level',      sense_ts_alpha_filtered_adc_level_f32 )
            DLG_SENSE_TS_DATA.SetSignal( 'SENSE_TS_alpha_beta_adc_level', sense_ts_alpha_beta_filtered_adc_level_f32 )
            DLG_SENSE_TS_DATA.SetSignal( 'SENSE_TS_touch_confidence',     sense_ts_touch_confidence_f32 )
            DLG_SENSE_TS_DATA.SetSignal( 'SENSE_TS_touch_confidence_max', sense_ts_touch_confidence_max_f32 )
            DLG_SENSE_TS_DATA.SetTimeStamp( dlg_time_stamp_f32 )

            DLG_INA_219_DATA_A.SetSignal( 'DD_INA_219_bus_voltage_raw',   dd_ina_219_bus_voltage_raw_u16 )
            DLG_INA_219_DATA_A.SetSignal( 'DD_INA_219_shunt_voltage_raw', dd_ina_219_shunt_voltage_raw_u16 )
            DLG_INA_219_DATA_A.SetSignal( 'DD_INA_219_current_raw',       dd_ina_219_current_raw_u16 )
            DLG_INA_219_DATA_A.SetSignal( 'DD_INA_219_power_raw',         dd_ina_219_power_raw_u16 )
            DLG_INA_219_DATA_A.SetTimeStamp( dlg_time_stamp_f32 )

            DLG_INA_219_DATA_B.SetSignal( 'DD_INA_219_current',             dd_ina_219_current_mA_f32 )
            DLG_INA_219_DATA_B.SetSignal( 'DD_INA_219_shunt_voltage_range', dd_ina_219_shunt_voltage_range_u8 )
            DLG_INA_219_DATA_B.SetSignal( 'DD_INA_219_power',               dd_ina_219_power_mW_f32 )
            DLG_INA_219_DATA_B.SetSignal( 'DD_INA_219_bus_voltage_range',   dd_ina_219_bus_voltage_range_u8 )
            DLG_INA_219_DATA_B.SetSignal( 'DD_INA_219_shunt_voltage',       dd_ina_219_shunt_voltage_mV_f32 )
            DLG_INA_219_DATA_B.SetSignal( 'DD_INA_219_bus_voltage',         dd_ina_219_bus_voltage_V_f32 )
            DLG_INA_219_DATA_B.SetTimeStamp( dlg_time_stamp_f32 )
            

            DLG_DD_TMP_102_DATA.SetSignal( 'DD_TMP_102_temperature_raw',    dd_tmp_102_temperature_raw_s16 )
            DLG_DD_TMP_102_DATA.SetSignal( 'DD_TMP_102_temperature_deg',    dd_tmp_102_temperature_deg_f32 )
            DLG_DD_TMP_102_DATA.SetTimeStamp( dlg_time_stamp_f32 )


            DLG_DD_MCPWM_DATA_A.SetSignal( 'DD_MCPWM_Ch_1_Duty_Cycle', dd_mcpwm_ch_1_duty_cycle_f32 )
            DLG_DD_MCPWM_DATA_A.SetSignal( 'DD_MCPWM_Ch_2_Duty_Cycle', dd_mcpwm_ch_2_duty_cycle_f32 )
            DLG_DD_MCPWM_DATA_A.SetSignal( 'DD_MCPWM_Ch_3_Duty_Cycle', dd_mcpwm_ch_3_duty_cycle_f32 )
            DLG_DD_MCPWM_DATA_A.SetSignal( 'DD_MCPWM_Ch_4_Duty_Cycle', dd_mcpwm_ch_4_duty_cycle_f32 )
            DLG_DD_MCPWM_DATA_A.SetSignal( 'DD_MCPWM_Ch_5_Duty_Cycle', dd_mcpwm_ch_5_duty_cycle_f32 )
            DLG_DD_MCPWM_DATA_A.SetSignal( 'DD_MCPWM_Ch_6_Duty_Cycle', dd_mcpwm_ch_6_duty_cycle_f32 )
            DLG_DD_MCPWM_DATA_A.SetTimeStamp( dlg_time_stamp_f32 )

            DLG_DD_MCPWM_DATA_B.SetSignal( 'DD_MCPWM_Ch_7_Duty_Cycle',  dd_mcpwm_ch_7_duty_cycle_f32 )
            DLG_DD_MCPWM_DATA_B.SetSignal( 'DD_MCPWM_Ch_8_Duty_Cycle',  dd_mcpwm_ch_8_duty_cycle_f32 )
            DLG_DD_MCPWM_DATA_B.SetSignal( 'DD_MCPWM_Ch_9_Duty_Cycle',  dd_mcpwm_ch_9_duty_cycle_f32 )
            DLG_DD_MCPWM_DATA_B.SetSignal( 'DD_MCPWM_Ch_10_Duty_Cycle', dd_mcpwm_ch_10_duty_cycle_f32 )
            DLG_DD_MCPWM_DATA_B.SetSignal( 'DD_MCPWM_Ch_11_Duty_Cycle', dd_mcpwm_ch_11_duty_cycle_f32 )
            DLG_DD_MCPWM_DATA_B.SetSignal( 'DD_MCPWM_Ch_12_Duty_Cycle', dd_mcpwm_ch_12_duty_cycle_f32 )
            DLG_DD_MCPWM_DATA_B.SetTimeStamp( dlg_time_stamp_f32 )

            DLG_DD_MCPWM_DATA_C.SetSignal( 'DD_MCPWM_Ch_1_mode',  dd_mcpwm_ch_1_mode_u8 )
            DLG_DD_MCPWM_DATA_C.SetSignal( 'DD_MCPWM_Ch_2_mode',  dd_mcpwm_ch_2_mode_u8 )
            DLG_DD_MCPWM_DATA_C.SetSignal( 'DD_MCPWM_Ch_3_mode',  dd_mcpwm_ch_3_mode_u8 )
            DLG_DD_MCPWM_DATA_C.SetSignal( 'DD_MCPWM_Ch_4_mode',  dd_mcpwm_ch_4_mode_u8 )
            DLG_DD_MCPWM_DATA_C.SetSignal( 'DD_MCPWM_Ch_5_mode',  dd_mcpwm_ch_5_mode_u8 )
            DLG_DD_MCPWM_DATA_C.SetSignal( 'DD_MCPWM_Ch_6_mode',  dd_mcpwm_ch_6_mode_u8 )
            DLG_DD_MCPWM_DATA_C.SetSignal( 'DD_MCPWM_Ch_7_mode',  dd_mcpwm_ch_7_mode_u8 )
            DLG_DD_MCPWM_DATA_C.SetSignal( 'DD_MCPWM_Ch_8_mode',  dd_mcpwm_ch_8_mode_u8 )
            DLG_DD_MCPWM_DATA_C.SetSignal( 'DD_MCPWM_Ch_9_mode',  dd_mcpwm_ch_9_mode_u8 )
            DLG_DD_MCPWM_DATA_C.SetSignal( 'DD_MCPWM_Ch_10_mode', dd_mcpwm_ch_10_mode_u8 )
            DLG_DD_MCPWM_DATA_C.SetSignal( 'DD_MCPWM_Ch_11_mode', dd_mcpwm_ch_11_mode_u8 )
            DLG_DD_MCPWM_DATA_C.SetSignal( 'DD_MCPWM_Ch_12_mode', dd_mcpwm_ch_12_mode_u8 )
            DLG_DD_MCPWM_DATA_C.SetTimeStamp( dlg_time_stamp_f32 )

            DLG_LOG_GENERAL.SetSignal('DLG_LOG_global_msg_counter', dlg_global_msg_cnt_u8)
            DLG_LOG_GENERAL.SetTimeStamp( dlg_time_stamp_f32 )

            # %%  Write all CAN messages to .blf file
            writer.on_message_received( DLG_ICM_20600_ACCEL_RAW.GetMessage()              )
            writer.on_message_received( DLG_ICM_20600_GYRO_RAW.GetMessage()               )
            writer.on_message_received( DLG_ICM_20600_GENERAL.GetMessage()                )
            writer.on_message_received( DLG_ICM_20600_TEMPERATURE.GetMessage()            )
            writer.on_message_received( DLG_ICM_20600_SELF_TEST.GetMessage()              )
            writer.on_message_received( DLG_ICM_20600_FACTORY_GYRO_TRIM.GetMessage()      )
            writer.on_message_received( DLG_ICM_20600_FACTORY_ACCEL_TRIM.GetMessage()     )
            writer.on_message_received( DLG_ICM_20600_FACTORY_ACCEL_TRIM_DEV.GetMessage() )
            writer.on_message_received( DLG_ICM_20600_FACTORY_GYRO_TRIM_DEV.GetMessage()  )
            
            writer.on_message_received( DLG_MAX_30102_GENERAL.GetMessage() )
            writer.on_message_received( DLG_MAX_30102_DATA.GetMessage()    )

            writer.on_message_received( DLG_I2C_ERROR.GetMessage() )

            writer.on_message_received( DLG_ADC_DATA.GetMessage() )

            writer.on_message_received( DLG_INA_219_DATA_A.GetMessage() )
            writer.on_message_received( DLG_INA_219_DATA_B.GetMessage() )

            writer.on_message_received( DLG_DD_TMP_102_DATA.GetMessage() )

            writer.on_message_received( DLG_SENSE_TS_DATA.GetMessage() )

            writer.on_message_received( DLG_DD_MCPWM_DATA_A.GetMessage() )
            writer.on_message_received( DLG_DD_MCPWM_DATA_B.GetMessage() )
            writer.on_message_received( DLG_DD_MCPWM_DATA_C.GetMessage() )

            writer.on_message_received( DLG_LOG_GENERAL.GetMessage() )

            # print( "Time: " + str( global_time_f32 ) )
writer.stop()

print("Done")
