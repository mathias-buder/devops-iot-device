"""
Use this to convert .can/.asc files to .log files.
Usage: simpleLogConvert.py sourceLog.asc targetLog.log
"""

import sys
import can.io.logger
import numpy as np
from random import seed
import random

DLG_ICM_20600_ACCEL_ID = 0x14
DLG_ICM_20600_GYRO_ID = 0x15
DLG_ICM_20600_TEMPERATURE_ID = 0x16
DLG_ICM_20600_SELF_TEST_ID = 0x17
DLG_ICM_20600_FACTORY_TRIM_ID = 0x18
DLG_ICM_20600_FACTORY_TRIM_DEV_ID = 0x19
DLG_ICM_20600_GENERAL_ID = 0x1A

time_stamp_interval = 0.1  # 2
n_samples = 10000

# writer = can.io.logger.Logger(sys.argv[2])
writer = can.io.logger.Logger( "test_1.blf" )
writer.channel = 1

DD_ICM_20600_accel_x = 0
DD_ICM_20600_accel_y = 0
DD_ICM_20600_accel_z = 0

for n in range( n_samples ):
    msg_icm_20600_accel = can.Message( arbitration_id=DLG_ICM_20600_ACCEL_ID,
                                       is_extended_id=False,
                                       timestamp=n * time_stamp_interval,
                                       data=[np.uint8( DD_ICM_20600_accel_x ),
                                             np.uint8( DD_ICM_20600_accel_x >> 8 ),
                                             np.uint8( DD_ICM_20600_accel_y ),
                                             np.uint8( DD_ICM_20600_accel_y >> 8 ),
                                             np.uint8( DD_ICM_20600_accel_z ),
                                             np.uint8( DD_ICM_20600_accel_z >> 8 )]
                                       )

    msg_icm_20600_gyro = can.Message( arbitration_id=DLG_ICM_20600_GYRO_ID,
                                      is_extended_id=False,
                                      timestamp=n * time_stamp_interval,
                                      data=[np.uint8( DD_ICM_20600_accel_x ),
                                            np.uint8( DD_ICM_20600_accel_x >> 8 ),
                                            np.uint8( DD_ICM_20600_accel_y ),
                                            np.uint8( DD_ICM_20600_accel_y >> 8 ),
                                            np.uint8( DD_ICM_20600_accel_z ),
                                            np.uint8( DD_ICM_20600_accel_z >> 8 )]
                                      )

    writer.on_message_received(msg_icm_20600_accel)
    writer.on_message_received(msg_icm_20600_gyro)

    DD_ICM_20600_accel_x = DD_ICM_20600_accel_x + 10
    DD_ICM_20600_accel_y = DD_ICM_20600_accel_y + 10
    DD_ICM_20600_accel_z = DD_ICM_20600_accel_z + 10

writer.stop()
