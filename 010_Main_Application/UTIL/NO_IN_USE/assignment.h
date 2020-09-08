/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: assignment.h $
        $Revision: 1.1.1.1 $
        $Date: 2018/07/03 11:28:58CEST $

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
 * @details General method for nxm assignment problem. Several algorithms can be chosen, the framework
 * is generally formulated.
 */

#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H          /*  defines this file */

    /*************************************************************/
    /*      INCLUDES                                             */
    /*************************************************************/
    #include "../UTIL/util_types.h"
    #include "../VIDEO/Interface/Comms/video_comms_types.h"             /* VIDEO types for MAX_NUM_MOBILEYE_OBJECTS */

    #ifdef UTIL_SUBSYSTEM
        /*************************************************************/
        /*      DEFINITIONS LOCAL TO THIS SUBSYSTEM                  */
        /*************************************************************/
    #endif
    /*************************************************************/
    /*      DEFINITIONS EXPORTED FROM THIS SUBSYSTEM             */
    /*************************************************************/

    /**
     * @details Maximum number of rows and columns in our rectangular vector.
     * Note: Feel free to make it bigger, if necessary (do not make it smaller though).
     */
    #define MAX_ROWS_ASSIGNMENT         MAX_NO_OF_TARGETS
    #define MAX_COLUMNS_ASSIGNMENT      MAX_NUM_MOBILEYE_OBJECTS

    /**
     * @details Distance for invalid assignment.
     */
    #define ASSIGNMENT_INVALID_DISTANCE INVALID_NUMBER

    /**
     * @details indicate no assignment.
     */
    #define NOT_ASSIGNED                -1

    /**
     * @details The algorithm used for the assignment.
     * Munkres algorithm can be found e.g. in "Multiple-Target Tracking with Radar Applications".
     * It is an optimal algorithm to solve the nxm assginment problem with a theoretical running time of O(n^2*m).
     *
     * The suboptimal algorithms are described in "Multi-Target Tracking with Radar Applications" from
     * Samuel S. Blackman p.95ff.
     * Suboptimal2 is in essence a greedy algorithm: It makes the connection of the closest track-to-observation pair,
     * removes it from the matrix and continues with the reduced matrix.
     */
    typedef enum {
#ifdef MUNKRES
                  MUNKRES,
#endif /* MUNKRES */
#ifdef SUBOPTIMAL1
                  SUBOPTIMAL1,
#endif /* SUBOPTIMAL1 */
                  SUBOPTIMAL2,
                  ASSIGNMENT_DIMENSION} ASSIGNMENT_TYPE;

    /**
     * @details This function computes the assignment vector for a given nxm distance matrix.
     * All this function does is check if input is valid and pass control flow to the selected method.
     * @param[out] assignment_result_vs16     n dimensional signed vector. Returns for every row the associated column.
     *                                        returns -1 as column if row not matched
     * @param[out] p_cost_f32                 the total costs of the association (one dimensional)
     * @param[in] dist_matrix_vf32            nxm distance matrix. Contains the distance for every of the (nxm) associations.
     *                                        For example, the tracks can be represented by the n rows, while the targets are
     *                                        represented by the columns.
     *                                        If no association possible, value has to be set to ASSIGNMENT_INVALID_DISTANCE.
     * @param[in] number_of_rows_u16          Total number of rows. Must not exeed MAX_ROWS_ASSIGNMENT
     * @param[in] number_of_columns_u16       Total number of columns. Must not exeed MAX_COLUMNS_ASSIGNMENT
     * @param[in] association_method_e        The algorithm used to associate.
     */
    void Compute_assignments( S16* const assignment_result_vs16,
                              F32* const p_cost_f32,
                              const F32* const dist_matrix_vf32,
                              const U16 number_of_rows_u16,
                              const U16 number_of_columns_u16,
                              ASSIGNMENT_TYPE association_method_e );

#endif /* ASSIGNMENT_H */
