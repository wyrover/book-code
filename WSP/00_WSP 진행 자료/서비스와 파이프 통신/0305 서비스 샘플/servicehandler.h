//servicehandler.h
void MyServiceHandler(DWORD opCode);
void MySetStatus(DWORD dwState, DWORD dwAccept=SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SESSIONCHANGE);