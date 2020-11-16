/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        @file dd_sd.h
        @details Some detailed description

        (c) SEWELA 2020

*********************************************************************/

#ifndef DD_SD_H_
#define DD_SD_H_

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/
#include "../../types.h"

#include <sys/unistd.h>
#include <sys/stat.h>

/*************************************************************/
/*      COMPILE TIME CONFIGURATION                           */
/*************************************************************/
/* Log message TAG string */
#define DD_SD_LOG_MSG_TAG               "DD_SD"         /**< @details Domain log message tag string */
#define DD_SD_MISO_PIN                  GPIO_NUM_19     /**< @details MISO GPIO pin number to configre the SPI interface */
#define DD_SD_MOSI_PIN                  GPIO_NUM_23     /**< @details MOSI GPIO pin number to configre the SPI interface */
#define DD_SD_CLK_PIN                   GPIO_NUM_18     /**< @details Clock GPIO pin number to configre the SPI interface */
#define DD_SD_CS_PIN                    GPIO_NUM_5      /**< @details Chip select GPIO pin number to configre the SPI interface */
#define DD_SD_MOUNT_POINT               "/sdcard"       /**< @details SD card file system mount point */
#define DD_SD_FORMAT_ON_FAILED_MOUNT    FALSE           /**< @details Flag to control if the medium shall be formattet of not in case it fails to be mounted */
#define DD_SD_MAX_OPEN_FILES            5U              /**< @details Max number of open files */
#define DD_SD_ALLOC_UNIT_SIZE           ( 16U * 1024U ) /**< @details SD card allocation unit size */
#define DD_SD_MAX_FILE_PATH_LENGTH      128U            /*!< Maximum number of characters within a file name */


/*************************************************************
*                      ENUMERATORS                           *
*************************************************************/
/**
 * @details File access types
 */
typedef enum DD_SD_FILE_MODE_TAG
{
    DD_SD_FILE_MODE_WRITE,        /**< @details Write mode */
    DD_SD_FILE_MODE_WRITE_BINARY, /**< @details Bibary write mode */
    DD_SD_FILE_MODE_SIZE          /**< @details Maximum number of file access types */
} DD_SD_FILE_MODE;

/*************************************************************
*      STRUCTURES                                            *
*************************************************************/

/**
 * @brief  File Structure
 * @details
 * @ingroup DriverStructures
 */
typedef struct DD_SD_FILE_TYPE_TAG
{
    FILE*       p_handle;                                 /**< @details FILE pointer */
    struct stat attrib_s;                                 /**< @details POSIX structure containing file attributes */
    char        full_path_vc[DD_SD_MAX_FILE_PATH_LENGTH]; /**< @details Complete file path (incl. mount point) */
    BOOLEAN     is_open_b;                                /**< @details Flag indicating that file is currently open */
} DD_SD_FILE_TYPE;

/**
 * @brief  SD-Card Output Interface Data Structure
 * @details
 * @ingroup DriverStructures
 */
typedef struct DD_SD_DATA_TYPE_TAG
{
    DD_SD_FILE_TYPE file_s;
    BOOLEAN         is_fs_mounted_b; /**< @details Flag indicating that file system is mounted on SD card */
} DD_SD_DATA_OUT_TYPE;



/*************************************************************/
/*      CLASS DEFINITION                                     */
/*************************************************************/
class DD_SD_C {

  private:
    static DD_SD_DATA_OUT_TYPE data_out_s;

    /**
     * @details Deinitialized the CD-Card
     * @return TRUE in case of success
     */
    static BOOLEAN deinit( void );

    /**
     * @details This function created a new file with name p_file_name_c. In case overwrite_b
     *          is set to TRUE an already existring file will be overwritten. Otherwise function
     *          will bypass and return FALSE.
     * @param[in] p_file_name_c Name of file to be created
     * @param[in] file_mode_e Access mode of file to be created
     * @param[in] overwrite_b Flag to indicate whether an existing file shall be overwritten or not
     * @return TRUE in case the file was created successfully
     */
    static BOOLEAN open_file( char*                 p_file_name_c,
                              const DD_SD_FILE_MODE file_mode_e,
                              BOOLEAN               overwrite_b );

  public:

    /**
     * @details Default constructor
     */
    DD_SD_C();

    /**
     * @details Default destructor
     */
    ~DD_SD_C();

    /**
     * @details Initializes the device driver
     * @return TRUE in case of success
     */
    static BOOLEAN init( void );

    /**
     * @details This function created a new file with name p_file_name_c and access type DD_SD_FILE_MODE_WRITE
     * @param[in] p_file_name_c Name of file to be created
     * @return Pointer of type FILE* in case of success, NULL otherwise
     */
    static FILE*   create_file( char* p_file_name_c );

    /**
     * @details This function created a new file with name p_file_name_c and access type DD_SD_FILE_MODE_WRITE_BINARY
     * @param[in] p_file_name_c Name of file to be created
     * @return Pointer of type FILE* in case of success, NULL otherwise
     */
    static FILE*   create_binary_file( char* p_file_name_c );

    /**
     * @details This function closes the file. since the drive currently supports only one file at a time
     *          no arguments are required to be passed in.
     * @return TRUE in case of success, FALSE otherwise
     */
    static BOOLEAN close_file( void );

    /**
     * @details This function indicates if an file is currently open
     * @return TRUE in case a file is currently open, FALSE otherwise
     */
    static BOOLEAN is_file_open( void );
};

#endif /* DD_SD_H_ */
