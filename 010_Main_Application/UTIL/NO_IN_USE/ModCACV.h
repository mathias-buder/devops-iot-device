/**************************************************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: ModCACV.h $
        $Revision: 1.1 $
        $Date: 2018/05/28 16:34:25CEST $

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

#ifndef _MODCACV_H_
#define _MODCACV_H_

/*************************************************************/
/*   FUNCTION DECLARATIONS                                   */
/*************************************************************/
/**
 * @details
 * @param[in] 'p_CACV_Kalman_model_s' is pointer to CACV kalman model.
 */
void create_CACV_Kalman_model(KALMAN_MODEL_STRUCT* const p_CACV_Kalman_model_s);

#endif /* _MODCACV_H_ */
