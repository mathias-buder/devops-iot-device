/**************************************************************************************************

    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: KalFilChk.h $
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

/**
 * @file
 * @brief general details of file
 */

#ifndef _KALFILTEST_H_
#define _KALFILTEST_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../UTIL/ModTypes.h"
#include "../UTIL/KalFilChk_Cfg.h"

/*************************************************************/
/*      DEFINES                                              */
/*************************************************************/


/*************************************************************/
/*      DATATYPES                                            */
/*************************************************************/


/*************************************************************/
/*   FUNCTION DECLARATIONS                                   */
/*************************************************************/
/**
 * @details This function initializes innovation check struct.
 * @param[in,out] 'p_innovation_check_s' is pointer to innovation check struct.
 */
void innovation_consistency_check_init( INNOVATION_CHECK_STRUCT* const p_innovation_check_s );

/**
 * @details This function performs innovation consistency check.
 * @param[in,out] 'p_innovation_check_s' is pointer to innovation check struct.
 * @param[in] 'innovation_value_f32' is the innovation
 * @param[in] 'innovation_variance_f32' is the innovation variance
 */
void innovation_consistency_check( INNOVATION_CHECK_STRUCT* const p_innovation_check_s,
                                   const F32 innovation_value_f32,
                                   const F32 innovation_variance_f32 );

#endif /* _KALFILTEST_H_ */
