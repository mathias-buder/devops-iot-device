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

        @file util_conf.h
        @details Some detailed description

*********************************************************************/
#ifndef UTIL_CONF_H_
#define UTIL_CONF_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/

#include "../types.h"

/*************************************************************/
/*      DEFINES                                              */
/*************************************************************/

#define UTIL_CONF_MAX_FIR_CONFIDENCE_BUFFER_LENGTH      64U

/*************************************************************/
/*      DATATYPES                                            */
/*************************************************************/
typedef enum UTIL_CONF_DETECTION_STATE_TYPE
{
    NO_UPDATE,
    NO_DETECTION,
    IS_DETECTION
} UTIL_CONF_DETECTION_STATE;

typedef struct UTIL_FIR_CONFIDENCE_STRUCT_TAG
{
    F32 confidence_f32;
    F32 confidence_max_f32;
    U64 asso_history_u64;
} UTIL_FIR_CONFIDENCE_STRUCT;

/*************************************************************/
/*   FUNCTION DECLARATIONS                                   */
/*************************************************************/
/**
 * @details Updates confidences based on a weighted FIR filter
 * @param[out] 'p_confidence_f32' confidence value to be updated
 * @param[out] 'p_confidence_max_f32' max confidence value to be updated
 * @param[out] 'p_asso_history_u64' association history (64-bit bitfield) used as history for confidence calculation
 * @param[in]  'history_length_u8' length of history that shall be taken into account (max length: 64)
 * @param[in]  'detection_status_e' current detection status
 */
void util_update_fir_confidence( F32* const                      p_confidence_f32,
                                 F32* const                      p_confidence_max_f32,
                                 U64* const                      p_asso_history_u64,
                                 const U8                        history_length_u8,
                                 const UTIL_CONF_DETECTION_STATE detection_status_e );

#endif /* UTIL_CONF_H_ */
