#ifndef VIRTUALDEBUG_H_
#define VIRTUALDEBUG_H_

#include "../../../../nCom/PipeBase.h"
#include "../../xVMRuntime/VirtualDebugCRT/mmCom.h"

extern CPipeBase* gPipeSend;
extern CPipeBase* gPipeRecv;

void EnableVirtualDebug();
void DisableVirtualDebug();


#endif //VIRTUALDEBUG_H_
