// Game Call Example: Calling game function example.

// Author: icemesh @ https://github.com/icemesh
// Author: illusion0001 @ https://github.com/illusion0001
// Repository: https://github.com/GoldHEN/GoldHEN_Plugins_Repository

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "plugin_common.h"
#include <orbis/libkernel.h>
#include "natives.hpp"

attr_public const char *g_pluginName = "game_call_example";
attr_public const char *g_pluginDesc = "Calling game function example";
attr_public const char *g_pluginAuth = "icemesh, illusion";
attr_public uint32_t g_pluginVersion = 0x00000100; // 1.00

struct proc_info procInfo;
#define NO_ASLR_ADDR 0x00400000

void* my_thread(void* args)
{
    uint64_t startPtr = procInfo.base_address;
    // Define game functions with parameters.
    //void (*MyFunc1) (int param_1) = ((void(*) (int param_1)) (startPtr + (0x0224f980 - NO_ASLR_ADDR)));
   // void (*MyFunc2) (const char *char_format, ...) = ((void(*)(const char *char_format, ...)) (startPtr + (0x0224f980 - NO_ASLR_ADDR)));
    uint32_t boot_wait = 10;
    final_printf("Sleeping for %u seconds...\n", boot_wait);
    sleep(boot_wait);
	
	/** added **/
	Player myPlayer;
	Actor myActor;
	//uint64_t *nativeTablePtr;
	
    while (true)
    {
        //MyFunc1(2);
        //MyFunc2("Hello from void* my_thread() (msgcon)\n");
		
		/*** code added here ***/
	    uint64_t *nativeTablePtr = NULL;
		//memset(&nativeTablePtr, 0, sizeof(nativeTablePtr));
		memcpy(nativeTablePtr, (startPtr + (0x25E3528 - NO_ASLR_ADDR)), sizeof(nativeTablePtr));
		if (nativeTablePtr && *nativeTablePtr) // if the table ptr actually has a value in game mem
		{
			// R1 + right d-pad taken from rdr ps3 menu code
			if (GAME::IS_BUTTON_PRESSED(0, INPUT_FRONTEND_RT, 1, 0) && GAME::IS_BUTTON_PRESSED(0, INPUT_FRONTEND_RIGHT, 1, 0))
			{
				myActor = PLAYER::GET_PLAYER_ACTOR(myPlayer);
				ACTORINFO::SET_ACTOR_DRUNK(myActor, true);
			}				
		}
		
        sceKernelUsleep(16666);
    }
    scePthreadExit(NULL);
    return NULL;
}

int32_t attr_public plugin_load(int32_t argc, const char* argv[])
{
    OrbisPthread thread;
    final_printf("[GoldHEN] %s Plugin Started.\n", g_pluginName);
    final_printf("[GoldHEN] <%s\\Ver.0x%08x> %s\n", g_pluginName, g_pluginVersion, __func__);
    final_printf("[GoldHEN] Plugin Author(s): %s\n", g_pluginAuth);
    scePthreadCreate(&thread, NULL, my_thread, NULL, "my_thread");
    return 0;
}

int32_t attr_public plugin_unload(int32_t argc, const char* argv[])
{
    final_printf("[GoldHEN] <%s\\Ver.0x%08x> %s\n", g_pluginName, g_pluginVersion, __func__);
    final_printf("[GoldHEN] %s Plugin Ended.\n", g_pluginName);
    return 0;
}

s32 attr_module_hidden module_start(s64 argc, const void *args)
{
    return 0;
}

s32 attr_module_hidden module_stop(s64 argc, const void *args)
{
    return 0;
}
