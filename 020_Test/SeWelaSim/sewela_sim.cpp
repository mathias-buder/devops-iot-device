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

        @file sewela_sim.cpp
        @details Contains all heard-rate related functions

*********************************************************************/

/*********************************************************************/
/*      INCLUDES                                                     */
/*********************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/* #include "generated_src/wrapper.h" */

#ifdef __cplusplus
}
#endif

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "sewela_sim.h"


/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
LogMsgPtr               p_gogeta_logger;

SENSE_DATA_IN_TYPE_TAG  _read_sense_data_in_s;
SENSE_DATA_OUT_TYPE_TAG _sense_data_out_s;
VE_DATA_IN_TYPE_TAG     _ve_data_in_s;
VE_DATA_OUT_TYPE_TAG    _ve_data_out_s;

/*************************************************************/
/*      FUNCTION DEFINITION                                  */
/*************************************************************/
void registerLogMsg( LogMsgPtr logger )
{
    p_gogeta_logger = logger;
}

extern "C" void message_logger( const char* p_expression_u8,
                                const char* p_file_path_u8,
                                int   line_s32,
                                int   critical_level_flag_s32 )
{
    /* make a buffer for combining the text including expression message, file path and code line */
    char text_vc[MAXIMUM_FILE_LENGTH + MAXIMUM_EXPRESSION_LENGTH + 50U];

    unsigned int file_name_index = strlen( p_file_path_u8 ) - MAXIMUM_FILE_LENGTH - 1U;

    /* if the path is larger than maximum, then the string is cut from right hand side to keep at least the file name
     * the expression name will be cut from left hand side in case larger than maximum*/
    if ( MAXIMUM_FILE_LENGTH > strlen( p_file_path_u8 ) )
    {
        /* the message is smaller than the limit, so no modification is required*/
        snprintf( text_vc, sizeof( text_vc ), "issue detected: %.*s in file %s, line %d.", (int) min( MAXIMUM_EXPRESSION_LENGTH, strlen( p_expression_u8 ) ), p_expression_u8, p_file_path_u8, line_s32 );
    }
    else
    {
        snprintf( text_vc, sizeof( text_vc ), "issue detected: %.*s in file ...%s, line %d.", (int) min( MAXIMUM_EXPRESSION_LENGTH, strlen( p_expression_u8 ) ), p_expression_u8, &p_file_path_u8[file_name_index], line_s32 );
    }
    /* make sure that the pointer is not empty */
    if ( NULL != p_gogeta_logger )
    {
        /* call the function pointer */
        p_gogeta_logger( "libSeWelaSim", text_vc, critical_level_flag_s32 );
    }
}


bool init()
{
    /* init__wrapper_s(); */

    SENSE_C::init(); /* Initialize Sensor Processing Domain ( SENSE ) */
    VE_C::init();    /* Initialize Vibration Engine Domain ( VE ) */

    return true;
}


bool runCycle( void )
{

    _sense_data_out_s = SENSE_C::main( _read_sense_data_in_s );
    _ve_data_out_s    = VE_C::main( _ve_data_in_s );

    /* copy__wrapper_s(); */

    return true;
}
