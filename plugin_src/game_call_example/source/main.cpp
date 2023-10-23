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

#include <Common.h>

void Notify(const char *IconUri, const char *FMT, ...)
{
    OrbisNotificationRequest Buffer{};
    va_list args;
    va_start(args, FMT);
    vsprintf(Buffer.message, FMT, args);
    va_end(args);
    final_printf("Notify message:\n%s\n", Buffer.message);
    Buffer.type = NotificationRequest;
    Buffer.unk3 = 0;
    Buffer.useIconImageUri = 1;
    Buffer.targetId = -1;
    strcpy(Buffer.iconUri, IconUri);
    sceKernelSendNotificationRequest(0, &Buffer, sizeof(Buffer), 0);
}

#define TEX_ICON_SYSTEM "cxml://psnotification/tex_icon_system"

void *my_thread(void *args)
{
    bzero(&procInfo, sizeof(procInfo));
    if (sys_sdk_proc_info(&procInfo) == 0)
    {
        print_proc_info();
    }
    else
    {
        Notify(TEX_ICON_SYSTEM, "Failed to get process info");
    }
    uintptr_t startPtr = procInfo.base_address;
    uint32_t boot_wait = 10;
    Notify(TEX_ICON_SYSTEM, "Sleeping for %u seconds...", boot_wait);
    sleep(boot_wait);
    NativeArg = (NativeArg_s *)mmap(0, sizeof(NativeArg), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    nativeArgPtr = (uintptr_t *)mmap(0, sizeof(nativeArgPtr), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    Notify(TEX_ICON_SYSTEM, "NativeArg: 0x%p\nnativeArgPtr: 0x%p", NativeArg, nativeArgPtr);
    while (true)
    {
        /*** code added here ***/
        uintptr_t *nativeTablePtr = (uintptr_t *)(startPtr + (NATIVE_ADDR - NO_ASLR_ADDR));
        printf("nativeTablePtr: 0x%p\n", nativeTablePtr);
        if (nativeTablePtr && *nativeTablePtr) // if the table ptr actually has a value in game mem
        {
            printf("*nativeTablePtr: 0x%p\n", *nativeTablePtr);
            Player myPlayer{};
            Actor myActor{};
            puts("========================================");
            puts("before call");
            printf("myPlayer: %d\nmyActor: %d\n", myPlayer, myActor);
            myPlayer = PLAYER::GET_LOCAL_SLOT();
            printf("After PLAYER::GET_LOCAL_SLOT(): %d\n", myPlayer);
            if (myPlayer != -1)
            {
                puts("if (myPlayer != -1)");
                myActor = PLAYER::GET_PLAYER_ACTOR(myPlayer);
                puts("after call");
                printf("myPlayer: %d\nmyActor: %d\n", myPlayer, myActor);
                puts("========================================");
                if (myActor)
                {
                    ACTORINFO::SET_ACTOR_DRUNK(myActor, true);
                }
            }
        }
        printf("wait for %d\n", 16666);
        //puts("16666");
        sceKernelUsleep(16666);
    }
    scePthreadExit(NULL);
    return NULL;
}

extern "C"
{
    int32_t attr_public plugin_load(int32_t argc, const char *argv[])
    {
        OrbisPthread thread;
        final_printf("[GoldHEN] %s Plugin Started.\n", g_pluginName);
        final_printf("[GoldHEN] <%s\\Ver.0x%08x> %s\n", g_pluginName, g_pluginVersion, __func__);
        final_printf("[GoldHEN] Plugin Author(s): %s\n", g_pluginAuth);
        scePthreadCreate(&thread, NULL, my_thread, NULL, "my_thread");
        return 0;
    }

    int32_t attr_public plugin_unload(int32_t argc, const char *argv[])
    {
        final_printf("[GoldHEN] <%s\\Ver.0x%08x> %s\n", g_pluginName, g_pluginVersion, __func__);
        final_printf("[GoldHEN] %s Plugin Ended.\n", g_pluginName);
        return 0;
    }

    int32_t attr_module_hidden module_start(size_t argc, const void *args)
    {
        return 0;
    }

    int32_t attr_module_hidden module_stop(size_t argc, const void *args)
    {
        return 0;
    }
}
