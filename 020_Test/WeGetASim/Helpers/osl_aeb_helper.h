/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/OSL_AEB/project.pj $
        $RCSfile: OSL_AEB.h $
        $Revision: 1.1 $
        $Date: 2019/08/13 11:19:30CEST $

        TRW Ltd owns the copyright in this document and associated
        documents and all rights are reserved.  These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of TRW
        Ltd. Any copy of this drawing or document made by any method
        must also include a copy of this legend.

    $CopyrightDate: (c) ZF 2019 $

*********************************************************************/

/**
 * @file osl_aeb_helper.h
 * @details
 */

#ifndef OSL_AEB_HELPER_H
#define OSL_AEB_HELPER_H

/*************************************************************/
/*      INCLUDE FILES                                        */
/*************************************************************/
#include <string.h>
#include <math.h>

#include "../../MainApplication/FOUNDATION/types.h"

/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
#define DIST_Y_AT_TTX0_BKPT_MIN         (-5.25F) /* m */
#define DIST_Y_AT_TTX0_BKPT_MAX         ( 5.25F) /* m */
#define DIST_Y_AT_TTX0_STEP_SIZE        ( 0.1F ) /* m */
#define DIST_Y_AT_TTX0_BKPT_RANGE       ( abs( DIST_Y_AT_TTX0_BKPT_MIN - DIST_Y_AT_TTX0_BKPT_MAX ) ) /* m */
#define DIST_Y_AT_TTX0_BKPT             ( (U16) (DIST_Y_AT_TTX0_BKPT_RANGE / DIST_Y_AT_TTX0_STEP_SIZE ) )
#define DIST_Y_AT_TTX0_BKPT_OFFSET      ( -0.5F * ( ( DIST_Y_AT_TTX0_BKPT - 1) * DIST_Y_AT_TTX0_STEP_SIZE ) )

/*************************************************************/
/*      ENUMERATORS                                          */
/*************************************************************/

/*************************************************************/
/*      STRUCTURES                                           */
/*************************************************************/


/**************************************************************
 *   FUNCTION DECLARATIONS
 *************************************************************/
void osl_aeb_helper_main(void);

void osl_aeb_helper_init(void);

#endif /* OSL_AEB_HELPER_H */
