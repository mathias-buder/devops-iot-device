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

        @file util_gen.h
        @details Some detailed description

*********************************************************************/
#ifndef UTIL_GENERAL_H_
#define UTIL_GENERAL_H_

#include "../types.h"

#ifndef simulator
#include "esp_log.h"
#include "assert.h"
#endif

#include <math.h>

/*************************************************************/
/*      DEFINITIONS EXPORTED FROM THIS SUBSYSTEM             */
/*************************************************************/

/************************************************************************************/
/* Function:    SLEW and rate_limit                                                 */
/* Description: These macros have exactly the same functionality, they slew the     */
/* existing value towards the new value at the specified up and down rate.          */
/************************************************************************************/

#define SLEW( new, exist, up, down ) fminf( fmaxf( ( new ), ( exist ) - ( down ) ), ( exist ) + ( up ) )
#define rate_limit( new, old, delta ) fminf( fmaxf( ( new ), ( old ) - ( delta ) ), ( old ) + ( delta ) )

/************************************************************************************/
/* Function:    CLAMP                                                               */
/* Description: This macro clamps a value between the upper and lower bounds.       */
/************************************************************************************/

#define CLAMP( value, lower, upper ) fminf( fmaxf( ( value ), ( lower ) ), ( upper ) )

/************************************************************************************/
/* Function:    TEST_BIT                                                            */
/* Description: This macro tests the state of a bit (or bits) within a bit field.   */
/*              In order to allow a logical operation on the result a '0' or '1' is */
/*              returned (avoiding confusion with the bitwise result).              */
/************************************************************************************/

#define TEST_BIT( flags, bit_mask ) ( ( ( ( flags ) & ( bit_mask ) ) == 0U ) ? 0U : 1U )

/************************************************************************************/
/* Function:    SET_BIT                                                             */
/* Description: This macro set a bit to 1 within a bit field                        */
/************************************************************************************/

#define SET_BIT( type, data, mask ) ( ( data ) = ( ( type )( ( data ) | ( mask ) ) ) )

/************************************************************************************/
/* Function:    CLEAR_BIT                                                           */
/* Description: This macro clears a bit to 0 within a bit field.                    */
/************************************************************************************/

#define CLEAR_BIT( type, data, mask ) ( ( data ) = ( type )( ( data ) & ~( mask ) ) )

/************************************************************************************/
/* Function:    IS_NAN                                                              */
/* Description: This macro checks to see if a the floating point variable is in     */
/*              a valid flating point format. i.e. is it "Not a Number (NaN)"       */
/* Return:      FALSE if valid floating point number, TRUE if NaN                   */
/************************************************************************************/

#define IS_NAN( f32 ) ( ( *(U32*) &(f32) &0x7F800000 ) == 0x7F800000 )

/************************************************************************************/
/* Function:    IN_RANGE                                                            */
/* Description: This macro checks whether a variable lies within the range of the   */
/* minimum and maximum bounds.                                                      */
/************************************************************************************/

#define IN_RANGE( var, min, max ) ( ( ( ( var ) >= ( min ) ) && ( ( var ) <= ( max ) ) ) ? 1U : 0U )

/************************************************************************************/
/* Function:    INVERT_BITS_IN_BYTE                                                 */
/* Description: This macro inverts bits in a byte                                   */
/************************************************************************************/

#define INVERT_BITS_IN_BYTE( byte_value ) ( ( U8 )( 0xFFU ^ ( byte_value ) ) )

/************************************************************************************/
/* Function:    SWAP_BYTES_IN_WORD                                                  */
/* Description: This macro swaps LSByte and MSByte within a given word (16bit)      */
/************************************************************************************/

#define SWAP_BYTES_IN_WORD( word_value ) ( ( ( word_value & 0x00FF ) << 8U ) | ( word_value & 0xFF00 ) >> 8U )

/************************************************************************************/
/* Function:    TO_BOOLEAN                                                          */
/* Description: This macro maps a compiler defined boolean to TRUE or FALSE         */
/************************************************************************************/

#define TO_BOOLEAN( condition ) ( ( condition ) ? TRUE : FALSE )

#define SQUARE( x ) ( ( x ) * ( x ) )

#define max( val_1, val_2 ) ( ( ( val_1 ) >= ( val_2 ) ) ? ( val_1 ) : ( val_2 ) )
#define min( val_1, val_2 ) ( ( ( val_1 ) <= ( val_2 ) ) ? ( val_1 ) : ( val_2 ) )


/************************************************************************************/
/* Function:    ASSERT / LOG                                                        */
/* Description: This macro sends a debug message to the standard error device if    */
/*              the argument 'expression' is usually false.                         */
/************************************************************************************/
#if ( defined( simulator ) )
    /* ASSERT feature can be used to inform the user about critical failures */
    #define ASSERT( expression )                                  \
        if ( !( expression ) )                                    \
        {                                                         \
            message_logger( #expression, __FILE__, __LINE__, 1 ); \
        }

    #define LOG_E( tag, format, ... )
    #define LOG_I( tag, format, ... )
    #define LOG_D( tag, format, ... )
    #define LOG_V( tag, format, ... )
#else
    #define ASSERT( expression )          assert( expression );
    #define LOG_E( tag, format, ... )     ESP_LOGE( tag, format, ##__VA_ARGS__ );
    #define LOG_W( tag, format, ... )     ESP_LOGW( tag, format, ##__VA_ARGS__ );
    #define LOG_I( tag, format, ... )     ESP_LOGI( tag, format, ##__VA_ARGS__ );
    #define LOG_D( tag, format, ... )     ESP_LOGD( tag, format, ##__VA_ARGS__ );
    #define LOG_V( tag, format, ... )     ESP_LOGV( tag, format, ##__VA_ARGS__ );
#endif

/************************************************************************************/
/* Global function declarations */

#if ( defined( simulator ) )
extern "C" void message_logger( const char* p_expression_c,
                                const char* p_file_path_c,
                                int         line_s32,
                                int         log_level_s32 );
#endif

extern F32 squaref( const F32 x_f32 );
/**
 * @description     Returns an interpolated floating point value from a 1D
 *                  map with floating point variable breakpoints.
 * @brief           map_1d
 * @param[in]       mapin - A pointer to the specified map
 *                  x     - input value to the map
 * @param[out]      F32  - the interpolated value o/p from the map
 */
extern F32 map_1d( const MAPTYPE* mapin, F32 x );

/**
 * @description     Returns an interpolated floating point value from a 2D
 *                  map with floating point variable breakpoints
 * @brief           map_2d
 * @param[in]       mapin - A pointer to the specified map
 *                  x     - input value to the map
 *                  y     - input value to the map
 * @param[out]      F32  - the interpolated value o/p from the map
 */
extern F32  map_2d( const MAPTYPE* mapin, F32 x, F32 y );
extern S32  convert_float_to_S32( F32 value_f, F32 inverse_resn_f, F32 offset_f );
extern U32  convert_float_to_U32( F32 value_f, F32 inverse_resn_f, F32 offset_f );
extern void util_rotate_point( POINT_2D_TYPE& r_point_s, F32 angle_deg_f32 );

typedef struct
{
    F32* array;
    int  length;
} delay_str_type;

#endif /* UTIL_GENERAL_H_ */
