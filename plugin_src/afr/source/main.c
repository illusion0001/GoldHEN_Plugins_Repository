// Author: jocover @ https://github.com/jocover
// Author: SiSTR0 @ https://github.com/SiSTR0
// Repository: https://github.com/GoldHEN/GoldHEN_Plugins_Repository

#include "Common.h"
#include "plugin_common.h"
#include <sys/fcntl.h>

attr_public const char *g_pluginName = "afr";
attr_public const char *g_pluginDesc = "Application File Redirector";
attr_public const char *g_pluginAuth = "jocover, SiSTR0";
attr_public u32 g_pluginVersion = 0x00000100; // 1.00

int32_t sceFiosFHOpen(const void *arg1, int32_t *out_handle, const char *file_path, const void *arg4);
int32_t sceFiosFHOpenSync(const void *arg1, int32_t *out_handle, const char *file_path, const void *arg4);

HOOK_INIT(sceFiosFHOpen);
HOOK_INIT(sceFiosFHOpenSync);
HOOK_INIT(sceKernelOpen);
HOOK_INIT(sceKernelStat);
HOOK_INIT(fopen);

bool isApp0(const char* path)
{
    return (path[0] == '/' && path[1] == 'a' && path[2] == 'p' && path[3] == 'p' &&
            path[4] == '0' && strlen(path) > 6);
}

void sys_proc_rw(const uintptr_t Address, const void *Data, const uint64_t Length)
{
    if (!Address || !Length)
    {
        final_printf("No target (0x%lx) or length (%li) provided!\n", Address, Length);
        return;
    }
    struct proc_rw process_rw_data;
    bzero(&process_rw_data, sizeof(process_rw_data));
    process_rw_data.address = Address;
    process_rw_data.data = Data;
    process_rw_data.length = Length;
    process_rw_data.write_flags = 1;
#if (__FINAL__) == 0
    debug_printf("process_rw_data: %p\n", &process_rw_data);
    debug_printf("address: 0x%lx\n", process_rw_data.address);
    debug_printf("hex_dump: ");
    hex_dump(process_rw_data.data, process_rw_data.length);
    debug_printf("length: 0x%lx\n", process_rw_data.length);
    int32_t ret = sys_sdk_proc_rw(&process_rw_data);
    debug_printf("sys_sdk_proc_rw: returned 0x%08x\n", ret);
#else
    sys_sdk_proc_rw(&process_rw_data);
#endif
}

void WriteJump64(const uintptr_t jump_src, const uintptr_t jump_dst)
{
    const uint8_t jump_ptr[] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp qword ptr [$+6]
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // ptr
    debug_printf("jump_src: 0x%lx\n", jump_src);
    debug_printf("jump_dst: 0x%lx\n", jump_dst);
    sys_proc_rw(jump_src, jump_ptr, sizeof(jump_ptr));
    sys_proc_rw(jump_src+6, &jump_dst, sizeof(uintptr_t));
}

char titleid[16] = {0};

FILE* fopen_hook(const char *path, const char *mode)
{
    FILE* fp = NULL;
    if (isApp0(path))
    {
        char possible_path[MAX_PATH_] = {0};
        snprintf(possible_path, sizeof(possible_path), GOLDHEN_PATH "/AFR/%s/%s", titleid, path + 6);

        fp = HOOK_CONTINUE(fopen,
                           FILE *(*)(const char *, const char *),
                           possible_path, mode);
        if (fp)
        {
            final_printf("new_path: %s FILE*: 0x%p\n", possible_path, &fp);
            return fp;
        }
    }

    fp = HOOK_CONTINUE(fopen,
                       FILE *(*)(const char *, const char *),
                       path, mode);
    debug_printf("path: %s FILE*: 0x%p\n", path, &fp);
    return fp;
}

s32 sceKernelStat_hook(const char *path, struct stat* stat_buf)
{
    // FIXME: use errno for correct `stat()` return values
    s32 ret = 0;
    s32 ret_pos = 0;
    ret = stat(path, stat_buf);
    if (isApp0(path))
    {
        char possible_path[MAX_PATH_] = {0};
        snprintf(possible_path, sizeof(possible_path), GOLDHEN_PATH "/AFR/%s/%s", titleid, path + 6);

        ret_pos = stat(possible_path, stat_buf);
        if (ret_pos < 0)
        {
            debug_printf("old: %s\n", path);
            debug_printf("old stat: 0x%08x\n", ret);
            return ret;
        }
        else
        {
            final_printf("new: %s\n", possible_path);
            final_printf("new stat: 0x%08x\n", ret_pos);
            return ret_pos;
        }
    }
    return ret;
}

s32 sceKernelOpen_hook(const char *path, s32 flags, OrbisKernelMode mode)
{
    s32 fd = 0;
    if (isApp0(path))
    {
        char possible_path[MAX_PATH_] = {0};
        snprintf(possible_path, sizeof(possible_path), GOLDHEN_PATH "/AFR/%s/%s", titleid, path + 6);
        fd = HOOK_CONTINUE(sceKernelOpen,
                           s32 (*)(const char *, s32, OrbisKernelMode),
                           possible_path, flags, mode);

        if (fd >= 0)
        {
            final_printf("new_path: %s\n", possible_path);
            final_printf("new fd: 0x%08x\n", fd);
            return fd;
        }
    }

    fd = HOOK_CONTINUE(sceKernelOpen,
                       s32 (*)(const char *, s32, OrbisKernelMode),
                       path, flags, mode);
    debug_printf("path: %s\n", path);
    debug_printf("fd: 0x%08x\n", fd);
    return fd;
}

s32 sceFiosFHOpen_hook(const void *arg1, int32_t *out_handle, const char *file_path, const void *arg4)
{
    debug_printf("arg1: %p\n", arg1);
    debug_printf("*out_handle: %x\n", *out_handle);
    debug_printf("file_path: %s\n", file_path);
    debug_printf("arg4: %p\n", arg4);
    s32 fd = 0;
    if (isApp0(file_path))
    {
        char possible_path[MAX_PATH_] = {0};
        snprintf(possible_path, sizeof(possible_path), GOLDHEN_PATH "/AFR/%s/%s", titleid, (file_path[0] == '/' ? file_path + 1 : file_path));
        fd = sceKernelOpen(possible_path, O_RDONLY, 0777);
        if (fd > 0)
        {
            *out_handle = fd;
            debug_printf("possible_path: %s\n", possible_path);
            debug_printf("*out_handle: %d\n", *out_handle);
            return 0;
        }
    }
    else
    {
        int32_t ret = sceFiosFHOpen(arg1, out_handle, file_path, arg4);
        debug_printf("sceFiosFHOpen: 0x%08x\n", ret);
        debug_printf("*out_handle: %d\n", *out_handle);
        return ret;
    }
    return 0;
}

s32 sceFiosFHOpenSync_hook(const void *arg1, int32_t *out_handle, const char *file_path, const void *arg4)
{
    debug_printf("arg1: %p\n", arg1);
    debug_printf("*out_handle: %x\n", *out_handle);
    debug_printf("file_path: %s\n", file_path);
    debug_printf("arg4: %p\n", arg4);
    s32 fd = 0;
    char possible_path[MAX_PATH_] = {0};
    snprintf(possible_path, sizeof(possible_path), GOLDHEN_PATH "/AFR/%s/%s", titleid, (file_path[0] == '/' ? file_path + 1 : file_path));
    fd = sceKernelOpen(possible_path, O_RDONLY, 0777);
    if (fd > 0)
    {
        *out_handle = fd;
        debug_printf("possible_path: %s\n", possible_path);
        debug_printf("*out_handle: %d\n", *out_handle);
    }
    else
    {
        int32_t ret = sceFiosFHOpenSync(arg1, out_handle, file_path, arg4);
        debug_printf("sceFiosFHOpenSync: 0x%08x\n", ret);
        debug_printf("*out_handle: %d\n", *out_handle);
        return ret;
    }
    return 0;
}

s32 attr_public plugin_load(s32 argc, const char* argv[])
{
    final_printf("[GoldHEN] <%s\\Ver.0x%08x> %s\n", g_pluginName, g_pluginVersion, __func__);
    final_printf("[GoldHEN] Plugin Author(s): %s\n", g_pluginAuth);
    boot_ver();
    struct proc_info procInfo;
    bzero(&procInfo, sizeof(procInfo));
    if (!sys_sdk_proc_info(&procInfo))
    {
        bzero(titleid, sizeof(titleid));
        strcpy(titleid, procInfo.titleid);
        print_proc_info();
    }
    WriteJump64(0x017f5e60, (uintptr_t)&sceFiosFHOpen_hook);
    WriteJump64(0x017f5e50, (uintptr_t)&sceFiosFHOpenSync_hook);
    // HOOK32(sceFiosFHOpen);
    // HOOK32(sceFiosFHOpenSync);
    HOOK32(sceKernelOpen);
    HOOK32(sceKernelStat);
    HOOK32(fopen);
    return 0;
}

s32 attr_public plugin_unload(s32 argc, const char* argv[])
{
    final_printf("[GoldHEN] <%s\\Ver.0x%08x> %s\n", g_pluginName, g_pluginVersion, __func__);
    // UNHOOK(sceFiosFHOpen);
    // UNHOOK(sceFiosFHOpenSync);
    UNHOOK(sceKernelOpen);
    UNHOOK(sceKernelStat);
    UNHOOK(fopen);
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
