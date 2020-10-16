/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

*********************************************************************/

/**
 * @file dd_types_Cfg.h
 * @details This files lists type specific defines and array sizes
 * for types used in the Touch Sensor domain.
 */

#ifndef DD_TYPES_CFG_H
#define DD_TYPES_CFG_H

/*************************************************************/
/*      DEFINES                                              */
/*************************************************************/

/* Global definition to setup all required peripherals to
 * support JTAG debugging */
// #define JTAG_DEBUG                             /*!< If defined, JTAG debugging enabled */

#define DD_I2C_ERROR_BUFFER_LENGTH      10U    /*!< Maximum number of I2C errors in corresponding buffer */

#define DD_SD_MAX_FILE_PATH_LENGTH      128U   /*!< Maximum number of file name characters */

/*************************************************************/
/*      ENUMS                                                */
/*************************************************************/

#endif /* DD_TYPES_CFG_H */
