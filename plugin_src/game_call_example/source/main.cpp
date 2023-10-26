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

extern "C" void pthread_script_hook(float delta_time)
{
    old_delta = delta_time;
    uintptr_t *nativeTablePtr = (uintptr_t *)(procInfo.base_address + (NATIVE_ADDR - NO_ASLR_ADDR));
    if (nativeTablePtr && *nativeTablePtr) // if the table ptr actually has a value in game mem
    {
        Player myPlayer{};
        Actor myActor{};
        myPlayer = PLAYER::GET_LOCAL_SLOT();
        if (myPlayer != -1)
        {
            myActor = PLAYER::GET_PLAYER_ACTOR(myPlayer);
            if (myActor)
            {
                ACTORINFO::SET_ACTOR_DRUNK(myActor, true);
            }
        }
    }
}

extern "C" void no_addr()
{
    Notify(TEX_ICON_SYSTEM, "script_addr: 0x%lx!!", script_addr);
}

void __attribute__((naked)) script_hook_enter()
{
    __asm__(".intel_syntax noprefix\n"
            "vmovss dword ptr [rip + old_delta], xmm0\n"
            "call pthread_script_hook\n"
            "vmovss xmm0, dword ptr [rip + old_delta]\n"
            "jmp qword ptr [rip + script_addr]\n"
            "crash_me:\n"
            "call no_addr\n"
            "ud2\n"
            ".att_syntax");
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
    script_addr = *(uintptr_t*)(startPtr + ((0x1E09E08+0x30) - NO_ASLR_ADDR));
    if (script_addr)
    {
        uint64_t hook_addr = (uint64_t)(void *)script_hook_enter;
        sys_proc_rw((void*)(startPtr + ((0x1E09E08+0x30) - NO_ASLR_ADDR)), &hook_addr, sizeof(void*));
        NativeArg = (NativeArg_s *)mmap(0, sizeof(NativeArg), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        nativeArgPtr = (uintptr_t *)mmap(0, sizeof(nativeArgPtr), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
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
