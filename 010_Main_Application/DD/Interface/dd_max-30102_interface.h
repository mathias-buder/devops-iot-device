/*********************************************************************

        SEWELA owns the copyright in this document and associated
        documents and all rights are reserved. These documents must
        not be used for any purpose other than that for which they
        are supplied and must not be copied in whole or in part or
        disclosed to others without prior written consent of SEWELA.
        Any copy of this drawing or document made by any method
        must also include a copy of this legend.

        (c) SEWELA 2020

*********************************************************************/

/**
 * @file dd_icm-20600_oif.h
 * @details Some detailed description
 */

#ifndef DD_MAX_30102_OIF_H
#define DD_MAX_30102_OIF_H

/*************************************************************/
/*      INCLUDES                                             */
/*************************************************************/



/*************************************************************/
/*      GLOBAL VARIABLES                                     */
/*************************************************************/

/**
 * @details This function returns pointer to the osl object for the given id
 * of the linked environment object.
 * @param[in] linked_env_id_u8 is id of the linked environment object of the requested osl object
 * @return pointer to osl object or NULL if no appropriate object exists.
 */
void dd_max_30102_acquire_sensor_data(void);

#endif /* DD_MAX_30102_OIF_H */