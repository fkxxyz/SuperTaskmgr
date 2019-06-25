
#pragma once


BOOL AsJobObjKillProcess(DWORD pID);
BOOL KillProcessAllThreads(DWORD pID);
BOOL WriteProcessMemoryKill(DWORD pID,DWORD dwMilliseconds=1000);
BOOL KillProcessAllWindows(DWORD pID);


