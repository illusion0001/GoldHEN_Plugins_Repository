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

uintptr_t script_addr = 0;
float old_delta = 0.f;

void (*pthread_script)() = nullptr;

HOOK_INIT(pthread_script);

extern "C" void pthread_script_hook()
{
    uintptr_t pthread_addr = *(uintptr_t *)(procInfo.base_address + (0x025e6598 - NO_ASLR_ADDR));
    /// printf("pthread_addr: 0x%lx\n", pthread_addr);
    if (pthread_addr)
    {
        OrbisPthreadMutex *old_pthread = (OrbisPthreadMutex *)pthread_addr;
        scePthreadMutexUnlock(old_pthread);
    }
    /*** code added here ***/
    uintptr_t *nativeTablePtr = (uintptr_t *)(procInfo.base_address + (NATIVE_ADDR - NO_ASLR_ADDR));
    // printf("nativeTablePtr: 0x%p\n", nativeTablePtr);
    if (nativeTablePtr && *nativeTablePtr) // if the table ptr actually has a value in game mem
    {
        // printf("*nativeTablePtr: 0x%lx\n", *nativeTablePtr);
        Player myPlayer{};
        Actor myActor{};
        // puts("========================================");
        // puts("before call");
        // printf("myPlayer: %d\nmyActor: %d\n", myPlayer, myActor);
        // slow!
        myPlayer = PLAYER::GET_LOCAL_SLOT();
        // printf("After PLAYER::GET_LOCAL_SLOT(): %d\n", myPlayer);
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
}

void sys_proc_rw(void* Address, void *Data, u64 Length)
{
    if (!Address || !Length)
    {
        final_printf("No target (0x%p) or length (%li) provided!\n", Address, Length);
        return;
    }
    struct proc_rw process_rw_data{};
    process_rw_data.address = (uint64_t)Address;
    process_rw_data.data = Data;
    process_rw_data.length = Length;
    process_rw_data.write_flags = 1;
    sys_sdk_proc_rw(&process_rw_data);
}

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
    NativeArg = (NativeArg_s *)mmap(0, sizeof(NativeArg), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    nativeArgPtr = (uintptr_t *)mmap(0, sizeof(nativeArgPtr), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (NativeArg && nativeArgPtr)
    {
        uint8_t hook_array[16] = {
            0xFF, 0x15, 0x02, 0x00, 0x00, 0x00,            // call qword ptr [$+6]
            0xEB, 0x08, // jmp +8
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // ptr
        };
        uint8_t nop = 0x90;
        for (uint32_t i = 0; i < sizeof(hook_array) + 1; i++)
        {
            sys_proc_rw((void*)(procInfo.base_address + (0x005f07d1 - NO_ASLR_ADDR)+i), &nop, 1);
        }
        sys_proc_rw((void*)(procInfo.base_address + (0x005f07d1 - NO_ASLR_ADDR)), hook_array, sizeof(hook_array));
        uint64_t hook_addr = (uint64_t)(void *)pthread_script_hook;
        sys_proc_rw((void*)(procInfo.base_address + (0x005f07d1 - NO_ASLR_ADDR)+8), &hook_addr, sizeof(hook_addr));
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
