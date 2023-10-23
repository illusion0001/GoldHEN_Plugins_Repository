#pragma once

#define NATIVE_ADDR 0x25E3528

#include <stdint.h>
#include <sys/mman.h>

struct NativeArg_s
{
    int32_t *ReturnValue;
    uint32_t ArgCount;
    int32_t *ArgValues;
    uint32_t ReadCount;
};

struct Native_s
{
    uint32_t nativeHash;
    uintptr_t nativeFunction;
};

// static NativeArg_s* NativeArg = (NativeArg_s*)0x10060000;
static NativeArg_s *NativeArg = (NativeArg_s *)mmap(0, sizeof(NativeArg), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
static int32_t *nativeArgPtr = (int32_t *)mmap(0, sizeof(nativeArgPtr), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

void ResetArgs()
{
    NativeArg->ArgCount = 0;
    NativeArg->ReadCount = 0;
    // NativeArg->ArgValues = (int*)0x10050000;
    NativeArg->ArgValues = nativeArgPtr;
    NativeArg->ReturnValue = NativeArg->ArgValues;
}

void CallHash(uint32_t hash, NativeArg_s *pArg)
{
    /*uint32_t l_uiHashTablePointer = 0x1782CE8;
    uint32_t l_uiModulator = *(uint32_t *)0x1782CEC; //== 0x1782CE8 + 0x04*/
    // 1.03 patch
    uintptr_t l_uiHashTablePointer = NATIVE_ADDR; // ptr is 64bit on this platform
    uint32_t l_uiModulator = *(uint32_t *)NATIVE_ADDR + sizeof(uintptr_t);

    int32_t index = hash % l_uiModulator;
    uintptr_t l_uiHashTableAddress = *(uintptr_t *)l_uiHashTablePointer;
    uint32_t temp_native_hash = hash;

    while (l_uiModulator != 0)
    {
        Native_s table = *(Native_s *)(l_uiHashTableAddress + 8 * index); // cast to Native_s struc captures the raw 8 bytes of jenkins hash and function offset from native table
        if (table.nativeHash == hash)
        {
            // if (table.nativeFunction != 0 && table.nativeFunction != 0xCDCDCDCD)
            if (table.nativeFunction != 0 && table.nativeFunction != 0x00000000)
            {
                ((void (*)(NativeArg_s *))table.nativeFunction)(pArg);
                return;
            }
        }

        temp_native_hash = (temp_native_hash >> 1) + 1;
        index = (temp_native_hash + index) % l_uiModulator;
    }
    return;
}

template <typename T>
void PushArg(T Value)
{
    *(T *)&NativeArg->ArgValues[NativeArg->ArgCount] = Value;
    NativeArg->ArgCount++;
}
template <typename R>
R GetReturn()
{
    R *Result;
    Result = (R *)&NativeArg->ReturnValue[0];
    return *(R *)Result;
}

void ResetArgs();
void CallHash(uint32_t address, NativeArg_s *pArg);

template <typename T>
T invoke(uint32_t NativeHash)
{
    ResetArgs();
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1>
T invoke(uint32_t NativeHash, P1 p0)
{
    ResetArgs();
    PushArg(p0);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2>
T invoke(uint32_t NativeHash, P1 p0, P2 p1)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    PushArg(p9);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    PushArg(p9);
    PushArg(p10);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    PushArg(p9);
    PushArg(p10);
    PushArg(p11);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    PushArg(p9);
    PushArg(p10);
    PushArg(p11);
    PushArg(p12);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    PushArg(p9);
    PushArg(p10);
    PushArg(p11);
    PushArg(p12);
    PushArg(p13);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    PushArg(p9);
    PushArg(p10);
    PushArg(p11);
    PushArg(p12);
    PushArg(p13);
    PushArg(p14);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    PushArg(p9);
    PushArg(p10);
    PushArg(p11);
    PushArg(p12);
    PushArg(p13);
    PushArg(p14);
    PushArg(p15);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    PushArg(p9);
    PushArg(p10);
    PushArg(p11);
    PushArg(p12);
    PushArg(p13);
    PushArg(p14);
    PushArg(p15);
    PushArg(p16);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    PushArg(p9);
    PushArg(p10);
    PushArg(p11);
    PushArg(p12);
    PushArg(p13);
    PushArg(p14);
    PushArg(p15);
    PushArg(p16);
    PushArg(p17);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18, typename P19>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17, P19 p18)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    PushArg(p9);
    PushArg(p10);
    PushArg(p11);
    PushArg(p12);
    PushArg(p13);
    PushArg(p14);
    PushArg(p15);
    PushArg(p16);
    PushArg(p17);
    PushArg(p18);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18, typename P19, typename P20>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17, P19 p18, P20 p19)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    PushArg(p9);
    PushArg(p10);
    PushArg(p11);
    PushArg(p12);
    PushArg(p13);
    PushArg(p14);
    PushArg(p15);
    PushArg(p16);
    PushArg(p17);
    PushArg(p18);
    PushArg(p19);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18, typename P19, typename P20, typename P21>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17, P19 p18, P20 p19, P21 p20)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    PushArg(p9);
    PushArg(p10);
    PushArg(p11);
    PushArg(p12);
    PushArg(p13);
    PushArg(p14);
    PushArg(p15);
    PushArg(p16);
    PushArg(p17);
    PushArg(p18);
    PushArg(p19);
    PushArg(p20);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18, typename P19, typename P20, typename P21, typename P22>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17, P19 p18, P20 p19, P21 p20, P22 p21)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    PushArg(p9);
    PushArg(p10);
    PushArg(p11);
    PushArg(p12);
    PushArg(p13);
    PushArg(p14);
    PushArg(p15);
    PushArg(p16);
    PushArg(p17);
    PushArg(p18);
    PushArg(p19);
    PushArg(p20);
    PushArg(p21);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18, typename P19, typename P20, typename P21, typename P22, typename P23>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17, P19 p18, P20 p19, P21 p20, P22 p21, P23 p22)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    PushArg(p9);
    PushArg(p10);
    PushArg(p11);
    PushArg(p12);
    PushArg(p13);
    PushArg(p14);
    PushArg(p15);
    PushArg(p16);
    PushArg(p17);
    PushArg(p18);
    PushArg(p19);
    PushArg(p20);
    PushArg(p21);
    PushArg(p22);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18, typename P19, typename P20, typename P21, typename P22, typename P23, typename P24>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17, P19 p18, P20 p19, P21 p20, P22 p21, P23 p22, P24 p23)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    PushArg(p9);
    PushArg(p10);
    PushArg(p11);
    PushArg(p12);
    PushArg(p13);
    PushArg(p14);
    PushArg(p15);
    PushArg(p16);
    PushArg(p17);
    PushArg(p18);
    PushArg(p19);
    PushArg(p20);
    PushArg(p21);
    PushArg(p22);
    PushArg(p23);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18, typename P19, typename P20, typename P21, typename P22, typename P23, typename P24, typename P25>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17, P19 p18, P20 p19, P21 p20, P22 p21, P23 p22, P24 p23, P25 p24)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    PushArg(p9);
    PushArg(p10);
    PushArg(p11);
    PushArg(p12);
    PushArg(p13);
    PushArg(p14);
    PushArg(p15);
    PushArg(p16);
    PushArg(p17);
    PushArg(p18);
    PushArg(p19);
    PushArg(p20);
    PushArg(p21);
    PushArg(p22);
    PushArg(p23);
    PushArg(p24);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}

template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16, typename P17, typename P18, typename P19, typename P20, typename P21, typename P22, typename P23, typename P24, typename P25, typename P26>
T invoke(uint32_t NativeHash, P1 p0, P2 p1, P3 p2, P4 p3, P5 p4, P6 p5, P7 p6, P8 p7, P9 p8, P10 p9, P11 p10, P12 p11, P13 p12, P14 p13, P15 p14, P16 p15, P17 p16, P18 p17, P19 p18, P20 p19, P21 p20, P22 p21, P23 p22, P24 p23, P25 p24, P26 p25)
{
    ResetArgs();
    PushArg(p0);
    PushArg(p1);
    PushArg(p2);
    PushArg(p3);
    PushArg(p4);
    PushArg(p5);
    PushArg(p6);
    PushArg(p7);
    PushArg(p8);
    PushArg(p9);
    PushArg(p10);
    PushArg(p11);
    PushArg(p12);
    PushArg(p13);
    PushArg(p14);
    PushArg(p15);
    PushArg(p16);
    PushArg(p17);
    PushArg(p18);
    PushArg(p19);
    PushArg(p20);
    PushArg(p21);
    PushArg(p22);
    PushArg(p23);
    PushArg(p24);
    PushArg(p25);
    CallHash(NativeHash, NativeArg);
    return GetReturn<T>();
}
