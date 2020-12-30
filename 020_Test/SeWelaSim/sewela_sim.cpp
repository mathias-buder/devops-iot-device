#include "sewela_sim.h"

#include <string.h>
#include <stdio.h>
#include <math.h>

#include "../../010_Construction/MainApplication/SENSE/SENSE.h"
#include "../../010_Construction/MainApplication/UTIL/UTIL.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "generated_src/wrapper.h"

#ifdef __cplusplus
}
#endif

#define MAXIMUM_FILE_LENGTH         50U
#define MAXIMUM_EXPRESSION_LENGTH   50U

LogMsgPtr gogeta_Logger = 0;

void registerLogMsg( LogMsgPtr logger )
{

    gogeta_Logger = logger;
}

extern "C" void message_logger(char* expression, char* filePath, int line, int critical_level_flag) {

        /* make a buffer for combining the text including expression message, file path and code line */
        char text[MAXIMUM_FILE_LENGTH + MAXIMUM_EXPRESSION_LENGTH + 50U];

        unsigned int file_name_index = strlen(filePath)-MAXIMUM_FILE_LENGTH-1U;

        /* if the path is larger than maximum, then the string is cut from right hand side to keep at least the file name
         * the expression name will be cut from left hand side in case larger than maximum*/

        if(MAXIMUM_FILE_LENGTH > strlen(filePath))
        {        /* the message is smaller than the limit, so no modification is required*/
             snprintf(text,sizeof(text), "issue detected: %.*s in file %s, line %d.", (int)min(MAXIMUM_EXPRESSION_LENGTH,strlen(expression)), expression, filePath, line);
        }
        else
        {
             snprintf(text,sizeof(text), "issue detected: %.*s in file ...%s, line %d.", (int)min(MAXIMUM_EXPRESSION_LENGTH, strlen(expression)), expression, &filePath[file_name_index], line);
        }
        /* make sure that the pointer is not empty */
        if(NULL != gogeta_Logger)
        {
            gogeta_Logger("libWeGetASim", text, critical_level_flag);    /* call the function pointer */
        }
}

bool init()
{
    init__wrapper_s();

    SENSE_C::init();

    return true;
}

bool runCycle(void)
{



    copy__wrapper_s();

    return true;
}
