/* PRQA S 0292 17 */
/**************************************************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: KalFilChk_Cfg.h $
        $Revision: 1.1 $
        $Date: 2018/05/28 16:34:24CEST $

        TRW Ltd owns the copyright in this document and associated
        documents and all rights are reserved.  These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of TRW
        Ltd. Any copy of this drawing or document made by any method
        must also include a copy of this legend.

    $CopyrightDate: (c) ZF 2019 $

***************************************************************************************************/

#ifndef UTIL_KALFILCHK_CFG_H_
#define UTIL_KALFILCHK_CFG_H_

/*************************************************************/
/*      FILE DEFINITIONS                                     */
/*************************************************************/
/**
 * @file
 * @brief general details of file
 */


/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../FOUNDATION/types.h"

/*************************************************************/
/*      DEFINES                                              */
/*************************************************************/
#define MAX_NUMBER_CHI_SQUARED_INVERSE_BKPTS    200U /* must not be smaller than DIM_INNOVATION_SEQUENCE_BACKGROUND_BUFFER*INNOVATION_SEQUENCE_DELAY */
#define DIM_INNOVATION_SEQUENCE_BACKGROUND_BUFFER 24U
#define INNOVATION_SEQUENCE_DELAY 8U

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
/**
 * @details minimum of sequence length to allow innovation consistency check
 */
extern const U8 kalfilchk_min_consistency_check_sequence_length_u8;

/*************************************************************/
/*      LOCAL VARIABLES                                      */
/*************************************************************/
extern const F32 chib_low_vf32[MAX_NUMBER_CHI_SQUARED_INVERSE_BKPTS];
extern const F32 chib_up_vf32[MAX_NUMBER_CHI_SQUARED_INVERSE_BKPTS];

#endif /* UTIL_KALFILCHK_CFG_H_ */
