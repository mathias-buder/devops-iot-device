#ifndef ECU_CONTROL_H
#define ECU_CONTROL_H

#include <stdbool.h>

typedef void(*LogMsgPtr)(const char* module, const char* message, int level);

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

__declspec(dllexport) bool init();
__declspec(dllexport) bool runCycle();
__declspec(dllexport) void registerLogMsg(LogMsgPtr);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* ECU_CONTROL_H */
