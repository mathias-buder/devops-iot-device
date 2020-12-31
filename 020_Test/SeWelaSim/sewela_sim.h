/*********************************************************************
    MODULE SPECIFICATION:

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

        @file sewela_sim.h
        @details Some detailed description

*********************************************************************/
#ifndef SEWELA_SIM_H
#define SEWELA_SIM_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../../010_Construction/MainApplication/SENSE/SENSE.h"
#include "../../010_Construction/MainApplication/UTIL/UTIL.h"
#include "../../010_Construction/MainApplication/VE/VE.h"

#include <stdbool.h>

/*************************************************************
*      DEFINES                                               *
*************************************************************/
#define MAXIMUM_FILE_LENGTH       50U
#define MAXIMUM_EXPRESSION_LENGTH 50U

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
extern SENSE_DATA_IN_TYPE_TAG  _read_sense_data_in_s;
extern SENSE_DATA_OUT_TYPE_TAG _sense_data_out_s;
extern VE_DATA_IN_TYPE_TAG     _ve_data_in_s;
extern VE_DATA_OUT_TYPE_TAG    _ve_data_out_s;

/*************************************************************/
/*      FUNCTION PROTOTYPED                                  */
/*************************************************************/
typedef void ( *LogMsgPtr )( const char* module, const char* message, int level );

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    __declspec( dllexport ) bool init();
    __declspec( dllexport ) bool runCycle();
    __declspec( dllexport ) void registerLogMsg( LogMsgPtr );

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* SEWELA_SIM_H */
