/* PRQA S 0292 17 */
/*********************************************************************
    MODULE SPECIFICATION:

        $ProjectName: /DAS/060_Projects/<WBC> Wabco/<WAB1Fus> CV MY2021i AC1000T Fusion/060_Software_Construction/MainApplication/UTIL/project.pj $
        $RCSfile: KalFilChk_Cfg.c $
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

*********************************************************************/

/**
 * @file
 * @brief
 */

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../FOUNDATION/FOUNDATION.h"
#include "../UTIL/KalFilChk_Cfg.h"


/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/
const U8 kalfilchk_min_consistency_check_sequence_length_u8 = 50U;

/*************************************************************/
/*      LOCAL VARIABLES                                      */
/*************************************************************/
/**
 * details: lower bound for chi by 95%.
 * the values are generated by "chib_low = chi2inv( 0.025, x ) ./ x" in matlab.
 * */
const F32 chib_low_vf32[MAX_NUMBER_CHI_SQUARED_INVERSE_BKPTS] =
{
    0.0010F, 0.0253F, 0.0719F, 0.1211F, 0.1662F, 0.2062F, 0.2414F, 0.2725F, 0.3000F, 0.3247F,
    0.3469F, 0.3670F, 0.3853F, 0.4021F, 0.4175F, 0.4317F, 0.4450F, 0.4573F, 0.4688F, 0.4795F,
    0.4897F, 0.4992F, 0.5082F, 0.5167F, 0.5248F, 0.5325F, 0.5398F, 0.5467F, 0.5533F, 0.5597F,
    0.5658F, 0.5716F, 0.5772F, 0.5825F, 0.5877F, 0.5927F, 0.5974F, 0.6021F, 0.6065F, 0.6108F,
    0.6150F, 0.6190F, 0.6229F, 0.6267F, 0.6304F, 0.6339F, 0.6374F, 0.6407F, 0.6440F, 0.6471F,
    0.6502F, 0.6532F, 0.6562F, 0.6590F, 0.6618F, 0.6645F, 0.6671F, 0.6697F, 0.6722F, 0.6747F,
    0.6771F, 0.6795F, 0.6818F, 0.6840F, 0.6862F, 0.6884F, 0.6905F, 0.6925F, 0.6946F, 0.6965F,
    0.6985F, 0.7004F, 0.7023F, 0.7041F, 0.7059F, 0.7077F, 0.7094F, 0.7111F, 0.7128F, 0.7144F,
    0.7160F, 0.7176F, 0.7192F, 0.7207F, 0.7222F, 0.7237F, 0.7252F, 0.7266F, 0.7280F, 0.7294F,
    0.7308F, 0.7321F, 0.7335F, 0.7348F, 0.7361F, 0.7373F, 0.7386F, 0.7398F, 0.7410F, 0.7422F,
    0.7434F, 0.7446F, 0.7457F, 0.7468F, 0.7480F, 0.7491F, 0.7502F, 0.7512F, 0.7523F, 0.7533F,
    0.7544F, 0.7554F, 0.7564F, 0.7574F, 0.7584F, 0.7593F, 0.7603F, 0.7612F, 0.7622F, 0.7631F,
    0.7640F, 0.7649F, 0.7658F, 0.7667F, 0.7676F, 0.7684F, 0.7693F, 0.7701F, 0.7710F, 0.7718F,
    0.7726F, 0.7734F, 0.7742F, 0.7750F, 0.7758F, 0.7765F, 0.7773F, 0.7781F, 0.7788F, 0.7795F,
    0.7803F, 0.7810F, 0.7817F, 0.7824F, 0.7831F, 0.7838F, 0.7845F, 0.7852F, 0.7859F, 0.7866F,
    0.7872F, 0.7879F, 0.7885F, 0.7892F, 0.7898F, 0.7905F, 0.7911F, 0.7917F, 0.7923F, 0.7929F,
    0.7935F, 0.7941F, 0.7947F, 0.7953F, 0.7959F, 0.7965F, 0.7971F, 0.7976F, 0.7982F, 0.7988F,
    0.7993F, 0.7999F, 0.8004F, 0.8010F, 0.8015F, 0.8020F, 0.8026F, 0.8031F, 0.8036F, 0.8041F,
    0.8046F, 0.8051F, 0.8056F, 0.8061F, 0.8066F, 0.8071F, 0.8076F, 0.8081F, 0.8086F, 0.8091F,
    0.8095F, 0.8100F, 0.8105F, 0.8109F, 0.8114F, 0.8118F, 0.8123F, 0.8128F, 0.8132F, 0.8136F
};

/**
 * details: upper bound for chi by 95%.
 * the values are generated by "chib_up = chi2inv( 0.975, x ) ./ x" in matlab.
 * */
const F32 chib_up_vf32[MAX_NUMBER_CHI_SQUARED_INVERSE_BKPTS] =
{
    5.0239F, 3.6889F, 3.1161F, 2.7858F, 2.5665F, 2.4082F, 2.2875F, 2.1918F, 2.1136F, 2.0483F,
    1.9927F, 1.9447F, 1.9027F, 1.8656F, 1.8326F, 1.8028F, 1.7759F, 1.7515F, 1.7291F, 1.7085F,
    1.6895F, 1.6719F, 1.6555F, 1.6402F, 1.6259F, 1.6124F, 1.5998F, 1.5879F, 1.5766F, 1.5660F,
    1.5559F, 1.5463F, 1.5371F, 1.5284F, 1.5201F, 1.5121F, 1.5045F, 1.4973F, 1.4903F, 1.4835F,
    1.4771F, 1.4709F, 1.4649F, 1.4591F, 1.4536F, 1.4482F, 1.4430F, 1.4380F, 1.4331F, 1.4284F,
    1.4238F, 1.4194F, 1.4151F, 1.4110F, 1.4069F, 1.4030F, 1.3992F, 1.3954F, 1.3918F, 1.3883F,
    1.3849F, 1.3815F, 1.3782F, 1.3751F, 1.3720F, 1.3689F, 1.3660F, 1.3631F, 1.3602F, 1.3575F,
    1.3548F, 1.3521F, 1.3495F, 1.3470F, 1.3445F, 1.3421F, 1.3397F, 1.3374F, 1.3351F, 1.3329F,
    1.3307F, 1.3285F, 1.3264F, 1.3243F, 1.3223F, 1.3203F, 1.3183F, 1.3164F, 1.3145F, 1.3126F,
    1.3108F, 1.3090F, 1.3072F, 1.3055F, 1.3038F, 1.3021F, 1.3004F, 1.2988F, 1.2972F, 1.2956F,
    1.2941F, 1.2925F, 1.2910F, 1.2895F, 1.2881F, 1.2866F, 1.2852F, 1.2838F, 1.2824F, 1.2811F,
    1.2797F, 1.2784F, 1.2771F, 1.2758F, 1.2745F, 1.2733F, 1.2720F, 1.2708F, 1.2696F, 1.2684F,
    1.2673F, 1.2661F, 1.2650F, 1.2638F, 1.2627F, 1.2616F, 1.2605F, 1.2594F, 1.2584F, 1.2573F,
    1.2563F, 1.2553F, 1.2543F, 1.2533F, 1.2523F, 1.2513F, 1.2503F, 1.2494F, 1.2484F, 1.2475F,
    1.2466F, 1.2456F, 1.2447F, 1.2438F, 1.2430F, 1.2421F, 1.2412F, 1.2404F, 1.2395F, 1.2387F,
    1.2378F, 1.2370F, 1.2362F, 1.2354F, 1.2346F, 1.2338F, 1.2330F, 1.2322F, 1.2315F, 1.2307F,
    1.2300F, 1.2292F, 1.2285F, 1.2278F, 1.2270F, 1.2263F, 1.2256F, 1.2249F, 1.2242F, 1.2235F,
    1.2228F, 1.2221F, 1.2215F, 1.2208F, 1.2201F, 1.2195F, 1.2188F, 1.2182F, 1.2175F, 1.2169F,
    1.2163F, 1.2157F, 1.2150F, 1.2144F, 1.2138F, 1.2132F, 1.2126F, 1.2120F, 1.2114F, 1.2109F,
    1.2103F, 1.2097F, 1.2091F, 1.2086F, 1.2080F, 1.2075F, 1.2069F, 1.2064F, 1.2058F, 1.2053F
};


/*************************************************************/
/*   FUNCTION DEFINITIONS                                    */
/*************************************************************/

