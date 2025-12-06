#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>
#include <thread>
#include <vector>
#include <TlHelp32.h>
#include <Psapi.h>
#include <winternl.h>
#include <intrin.h>
#include "../auth/skStr.h" // skCrypt
#include "discord.hpp"

// Forward-declare NT function (already linked via ntdll.lib)
extern "C" NTSTATUS NTAPI NtQuerySystemInformation(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
);

// Simple kernel debugger info struct
typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION {
    BOOLEAN KernelDebuggerEnabled;
    BOOLEAN KernelDebuggerNotPresent;
} SYSTEM_KERNEL_DEBUGGER_INFORMATION, * PSYSTEM_KERNEL_DEBUGGER_INFORMATION;

// Extended PEB (partial) for BeingDebugged
typedef struct _PEB_EXTENDED {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[1];
    PVOID Reserved3[2];
    PVOID Ldr;
    PVOID ProcessParameters;
    PVOID Reserved4[3];
    PVOID AtlThunkSListPtr;
    PVOID Reserved5;
    ULONG Reserved6;
    PVOID Reserved7;
    ULONG Reserved8;
    ULONG AtlThunkSListPtr32;
    PVOID Reserved9[45];
    BYTE Reserved10[96];
    PVOID PostProcessInitRoutine;
    BYTE Reserved11[128];
    PVOID Reserved12[1];
    ULONG SessionId;
} PEB_EXTENDED, * PPEB_EXTENDED;

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "ntdll.lib")

namespace security {

    enum debug_results {
        none = 0x0000,
        being_debugged_peb = 0x1000,
        remote_debugger_present = 0x1001,
        debugger_is_present = 0x1002,
        find_window = 0x1005,
        hardware_debug_registers = 0x4000,
        check_cpuid = 0x5000,
        vm = 0x5002,
        kernel_debugger = 0x6000,
        blacklisted_process = 0x7000,
        timing_attack = 0x8000
    };

    class AdvancedProtection {
    private:
        std::thread protection_thread;
        bool running = false;

        // webhook URL (narrow string)
        const std::string webhook_url = std::string(skCrypt("https://discord.com/api/webhooks/1429185359933214824/6B3dc3TYYTP9MGlfjSXPfwSi7OQCZlQQnJHHOI4-d6XEjkaL0d5hrlnz6m-HPvZWXXlY").decrypt());

        // helper to safely send alert and terminate
        void log_protection(const std::string& reason, const std::string& details = "") {
            try {
                // send alert (pass std::string)
                discord::send_discord_alert(reason, details);

                const char* imagePathC = skCrypt("C:\\Windows\\image.png").decrypt();
                CaptureScreenshot(imagePathC);
                image();

                // send screenshot via image() helper in discord.hpp (if available)
                // If you implemented image() expecting const char*, call with imagePathC
                // If image() signature differs, adjust accordingly.
                // image(); // optional: your discord.hpp image() uses internal webhook constant

                // give a moment for network ops
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            catch (...) {
                // swallow
            }

            // make sure process is terminated
            TerminateProcess(GetCurrentProcess(), 0);
        }

        // PEB BeingDebugged check
        bool check_peb_debug() {
#ifdef _M_X64
            PPEB_EXTENDED peb = (PPEB_EXTENDED)__readgsqword(0x60);
#else
            // x86 alternative (if you support x86)
            PPEB_EXTENDED peb = (PPEB_EXTENDED)__readfsdword(0x30);
#endif
            if (!peb) return false;
            return peb->BeingDebugged != 0;
        }

        // NtGlobalFlag check - simplified placeholder
        bool check_nt_global_flag() {
            // Accessing NtGlobalFlag is compiler/platform-specific; leave as false (or implement if needed)
            return false;
        }

        // Hardware debug registers
        bool check_hardware_breakpoints() {
            CONTEXT ctx = {};
            ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
            if (GetThreadContext(GetCurrentThread(), &ctx)) {
                return (ctx.Dr0 != 0 || ctx.Dr1 != 0 || ctx.Dr2 != 0 || ctx.Dr3 != 0);
            }
            return false;
        }

        // Kernel debugger via SystemKernelDebuggerInformation (0x23)
        bool check_kernel_debugger() {
            SYSTEM_KERNEL_DEBUGGER_INFORMATION dbgInfo = {};
            NTSTATUS status = NtQuerySystemInformation(
                (SYSTEM_INFORMATION_CLASS)0x23, // SystemKernelDebuggerInformation
                &dbgInfo,
                sizeof(dbgInfo),
                nullptr
            );
            return NT_SUCCESS(status) && dbgInfo.KernelDebuggerEnabled;
        }

        // Entire blacklisted process list (wide name, readable description encrypted)
        std::string check_blacklisted_processes() {
            // Prepare list: pair<wstring exeName, std::string description>
            const std::vector<std::pair<std::wstring, std::string>> blacklisted = {
{ L"extremeinjector.exe",        std::string(skCrypt("Extreme Injector").decrypt()) },
{ L"extreme_injector.exe",       std::string(skCrypt("Extreme Injector").decrypt()) },
{ L"extremeinjector64.exe",      std::string(skCrypt("Extreme Injector x64").decrypt()) },
{ L"xenos.exe",                  std::string(skCrypt("Xenos Injector").decrypt()) },
{ L"xenos64.exe",                std::string(skCrypt("Xenos Injector x64").decrypt()) },
{ L"cheatengine-x86_64.exe",     std::string(skCrypt("Cheat Engine x64").decrypt()) },
{ L"cheatengine-x86.exe",        std::string(skCrypt("Cheat Engine x86").decrypt()) },
{ L"injector.exe",               std::string(skCrypt("Generic DLL Injector").decrypt()) },
{ L"dllinjector.exe",            std::string(skCrypt("DLL Injector").decrypt()) },
{ L"simpleinjector.exe",         std::string(skCrypt("Simple Injector").decrypt()) },
{ L"malwarehunter.exe",          std::string(skCrypt("Malware Hunter (tool)").decrypt()) },
{ L"scylla_hide.exe",            std::string(skCrypt("ScyllaHide").decrypt()) },
{ L"scylla_exe.exe",             std::string(skCrypt("ScyllaEx").decrypt()) },
{ L"scylla_exe64.exe",           std::string(skCrypt("ScyllaEx x64").decrypt()) },
{ L"megadumperx64.exe",          std::string(skCrypt("MegaDumper x64").decrypt()) },
{ L"megadumperx86.exe",          std::string(skCrypt("MegaDumper x86").decrypt()) },
{ L"module_dump.exe",            std::string(skCrypt("Module Dumper").decrypt()) },
{ L"moduledumper.exe",           std::string(skCrypt("ModuleDumper").decrypt()) },
{ L"processhacker2.exe",         std::string(skCrypt("Process Hacker 2").decrypt()) },
{ L"processhacker3.exe",         std::string(skCrypt("Process Hacker 3").decrypt()) },
{ L"processhacker64.exe",        std::string(skCrypt("Process Hacker x64").decrypt()) },
{ L"reclassnet.exe",             std::string(skCrypt("ReClass.NET").decrypt()) },
{ L"reclass.exe",                std::string(skCrypt("ReClass").decrypt()) },
{ L"reclass64.exe",              std::string(skCrypt("ReClass x64").decrypt()) },
{ L"loadlibrary.exe",            std::string(skCrypt("LoadLibrary Helper").decrypt()) },
{ L"manualmap.exe",              std::string(skCrypt("Manual Mapper").decrypt()) },
{ L"manual_map.exe",             std::string(skCrypt("Manual Map Injector").decrypt()) },
{ L"manualmapper.exe",           std::string(skCrypt("ManualMapper").decrypt()) },
{ L"manual_m.exe",               std::string(skCrypt("Manual Map (alias)").decrypt()) },
{ L"reflective.dll",             std::string(skCrypt("Reflective Loader (DLL)").decrypt()) },
{ L"reflective_loader.exe",      std::string(skCrypt("Reflective Loader").decrypt()) },
{ L"reflective_injector.exe",    std::string(skCrypt("Reflective Injector").decrypt()) },
{ L"refined_injector.exe",       std::string(skCrypt("Refined Injector").decrypt()) },
{ L"xinjector.exe",              std::string(skCrypt("X-Injector").decrypt()) },
{ L"x32_injector.exe",           std::string(skCrypt("X32 Injector").decrypt()) },
{ L"x64_injector.exe",           std::string(skCrypt("X64 Injector").decrypt()) },
{ L"vmp_loader.exe",             std::string(skCrypt("VMP Loader Helper").decrypt()) },
{ L"vmpstub.exe",                std::string(skCrypt("VMP Stub").decrypt()) },
{ L"vmp_unpacker.exe",           std::string(skCrypt("VMP Unpacker (tool)").decrypt()) },
{ L"dnv_injector.exe",           std::string(skCrypt("DNV Injector").decrypt()) },
{ L"dll_loader.exe",             std::string(skCrypt("DLL Loader").decrypt()) },
{ L"payload_injector.exe",       std::string(skCrypt("Payload Injector").decrypt()) },
{ L"payload_loader.exe",         std::string(skCrypt("Payload Loader").decrypt()) },
{ L"process_injector.exe",       std::string(skCrypt("Process Injector").decrypt()) },
{ L"proc_inject.exe",            std::string(skCrypt("ProcInject").decrypt()) },
{ L"mh_injector.exe",            std::string(skCrypt("MH Injector").decrypt()) },
{ L"mh_inject64.exe",            std::string(skCrypt("MH Injector x64").decrypt()) },
{ L"madshi_detours.exe",         std::string(skCrypt("Madshi Detours Helper").decrypt()) },
{ L"madshi.exe",                 std::string(skCrypt("Madshi (tools)").decrypt()) },
{ L"detours_tool.exe",           std::string(skCrypt("Detours Tool").decrypt()) },
{ L"reflector.exe",              std::string(skCrypt("Assembly Reflector").decrypt()) },
{ L"reflexil_plugin.exe",        std::string(skCrypt("Reflexil Plugin").decrypt()) },
{ L"dotnet_peeker.exe",          std::string(skCrypt(".NET Peeker").decrypt()) },
{ L"dotpeek64.exe",              std::string(skCrypt("dotPeek x64").decrypt()) },
{ L"il2cpp_injector.exe",        std::string(skCrypt("il2cpp Injector").decrypt()) },
{ L"il2cpp_inject64.exe",        std::string(skCrypt("il2cpp Injector x64").decrypt()) },
{ L"il2cppdumpergui.exe",        std::string(skCrypt("Il2CppDumper GUI").decrypt()) },
{ L"il2cppdumpergui.exe",        std::string(skCrypt("Il2CppDumper GUI (alt)").decrypt()) },
{ L"dotnet_peeker64.exe",        std::string(skCrypt(".NET Peeker x64").decrypt()) },
{ L"dotnet_injector.exe",        std::string(skCrypt(".NET Injector").decrypt()) },
{ L"clr_injector.exe",           std::string(skCrypt("CLR Injector").decrypt()) },
{ L"clr_loader.exe",             std::string(skCrypt("CLR Loader").decrypt()) },
{ L"winject.exe",                std::string(skCrypt("Winject (injector)").decrypt()) },
{ L"winjector.exe",              std::string(skCrypt("Winjector").decrypt()) },
{ L"weiner_injector.exe",        std::string(skCrypt("Weiner Injector (alias)").decrypt()) },
{ L"uh_dll_injector.exe",        std::string(skCrypt("UH DLL Injector").decrypt()) },
{ L"g_injector.exe",             std::string(skCrypt("G Injector").decrypt()) },
{ L"g_inject64.exe",             std::string(skCrypt("G Injector x64").decrypt()) },
{ L"gm_hooker.exe",              std::string(skCrypt("GM Hooker").decrypt()) },
{ L"gm_hooker64.exe",            std::string(skCrypt("GM Hooker x64").decrypt()) },
{ L"frida_injector.exe",         std::string(skCrypt("Frida Injector Helper").decrypt()) },
{ L"fridacli.exe",               std::string(skCrypt("Frida CLI Helper").decrypt()) },
{ L"metasploit_inject.exe",      std::string(skCrypt("Metasploit Inject (tool)").decrypt()) },
{ L"meterpreter_x86.exe",        std::string(skCrypt("Meterpreter x86 Helper").decrypt()) },
{ L"meterpreter_x64.exe",        std::string(skCrypt("Meterpreter x64 Helper").decrypt()) },
{ L"mhook.exe",                  std::string(skCrypt("MHook Helper").decrypt()) },
{ L"mhook64.exe",                std::string(skCrypt("MHook x64").decrypt()) },
{ L"mhook32.exe",                std::string(skCrypt("MHook x86").decrypt()) },
{ L"gm_dumper.exe",              std::string(skCrypt("Game Maker Dumper").decrypt()) },
{ L"gm_dumper64.exe",            std::string(skCrypt("Game Maker Dumper x64").decrypt()) },
{ L"dump_tool.exe",              std::string(skCrypt("Generic Dump Tool").decrypt()) },
{ L"dump_tool64.exe",            std::string(skCrypt("Generic Dump Tool x64").decrypt()) },
{ L"vmprotect_unwrap.exe",       std::string(skCrypt("VMProtect Unwrap Helper").decrypt()) },
{ L"packer_detector.exe",        std::string(skCrypt("Packer Detector").decrypt()) },
{ L"anti_debug_helper.exe",      std::string(skCrypt("Anti-Debug Helper (tool)").decrypt()) },
{ L"anti_debug64.exe",           std::string(skCrypt("Anti-Debug x64").decrypt()) },
{ L"rootkitreveal.exe",          std::string(skCrypt("RootkitRevealer").decrypt()) },
{ L"gmer.exe",                   std::string(skCrypt("GMER Rootkit Scanner").decrypt()) },
{ L"gmer64.exe",                 std::string(skCrypt("GMER x64").decrypt()) },
{ L"autoruns64.exe",             std::string(skCrypt("Autoruns x64").decrypt()) },
{ L"procdump_tool.exe",          std::string(skCrypt("ProcDump Tool (alt)").decrypt()) },
{ L"procdump2.exe",              std::string(skCrypt("ProcDump v2").decrypt()) },
{ L"proc_extractor.exe",         std::string(skCrypt("Process Extractor").decrypt()) },
{ L"pe_bear.exe",                std::string(skCrypt("PE-bear").decrypt()) },
{ L"pestudio64.exe",             std::string(skCrypt("PEStudio x64").decrypt()) },
{ L"pestudio32.exe",             std::string(skCrypt("PEStudio x86").decrypt()) },
{ L"upx_unpacker.exe",           std::string(skCrypt("UPX Unpacker").decrypt()) },
{ L"upx_gui.exe",                std::string(skCrypt("UPX GUI").decrypt()) },
{ L"packer_detector64.exe",      std::string(skCrypt("Packer Detector x64").decrypt()) },
{ L"ida_free64.exe",             std::string(skCrypt("IDA Free 64 (alt)").decrypt()) },
{ L"ghidra64.exe",               std::string(skCrypt("Ghidra x64 Launcher").decrypt()) },
{ L"binaryninja64.exe",          std::string(skCrypt("Binary Ninja x64").decrypt()) },
{ L"binaryninja32.exe",          std::string(skCrypt("Binary Ninja x86").decrypt()) },
{ L"ghidra_headless.exe",        std::string(skCrypt("Ghidra Headless").decrypt()) },
{ L"ghidra_analyze.exe",         std::string(skCrypt("Ghidra Analyzer").decrypt()) },
{ L"ida_plugin.exe",             std::string(skCrypt("IDA Helper Plugin").decrypt()) },
{ L"ida_helper64.exe",           std::string(skCrypt("IDA Helper x64").decrypt()) },
{ L"olly_plugin.exe",            std::string(skCrypt("Olly Plugin").decrypt()) },
{ L"olly_helper.exe",            std::string(skCrypt("Olly Helper").decrypt()) },
{ L"hookshark.exe",              std::string(skCrypt("HookShark").decrypt()) },
{ L"hookfinder.exe",             std::string(skCrypt("Hook Finder").decrypt()) },
{ L"hookscanner.exe",            std::string(skCrypt("Hook Scanner").decrypt()) },
{ L"hookcleaner.exe",            std::string(skCrypt("Hook Cleaner").decrypt()) },
{ L"bypass_tool.exe",            std::string(skCrypt("Bypass Tool (generic)").decrypt()) },
{ L"bypass64.exe",               std::string(skCrypt("Bypass Tool x64").decrypt()) },

            };

            HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (hSnapshot == INVALID_HANDLE_VALUE) return std::string();

            PROCESSENTRY32W pe32;
            pe32.dwSize = sizeof(pe32);

            if (Process32FirstW(hSnapshot, &pe32)) {
                do {
                    for (const auto& entry : blacklisted) {
                        if (_wcsicmp(pe32.szExeFile, entry.first.c_str()) == 0) {
                            CloseHandle(hSnapshot);
                            return entry.second;
                        }
                    }
                } while (Process32NextW(hSnapshot, &pe32));
            }

            CloseHandle(hSnapshot);
            return std::string();
        }

        // Blacklisted windows (search by window class/title heuristics)
        std::string check_blacklisted_windows() {
            const std::vector<std::pair<std::wstring, std::string>> blacklisted_windows = {
{ L"extremeinjector.exe",        std::string(skCrypt("Extreme Injector").decrypt()) },
{ L"extreme_injector.exe",       std::string(skCrypt("Extreme Injector").decrypt()) },
{ L"extremeinjector64.exe",      std::string(skCrypt("Extreme Injector x64").decrypt()) },
{ L"xenos.exe",                  std::string(skCrypt("Xenos Injector").decrypt()) },
{ L"xenos64.exe",                std::string(skCrypt("Xenos Injector x64").decrypt()) },
{ L"cheatengine-x86_64.exe",     std::string(skCrypt("Cheat Engine x64").decrypt()) },
{ L"cheatengine-x86.exe",        std::string(skCrypt("Cheat Engine x86").decrypt()) },
{ L"injector.exe",               std::string(skCrypt("Generic DLL Injector").decrypt()) },
{ L"dllinjector.exe",            std::string(skCrypt("DLL Injector").decrypt()) },
{ L"simpleinjector.exe",         std::string(skCrypt("Simple Injector").decrypt()) },
{ L"malwarehunter.exe",          std::string(skCrypt("Malware Hunter (tool)").decrypt()) },
{ L"scylla_hide.exe",            std::string(skCrypt("ScyllaHide").decrypt()) },
{ L"scylla_exe.exe",             std::string(skCrypt("ScyllaEx").decrypt()) },
{ L"scylla_exe64.exe",           std::string(skCrypt("ScyllaEx x64").decrypt()) },
{ L"megadumperx64.exe",          std::string(skCrypt("MegaDumper x64").decrypt()) },
{ L"megadumperx86.exe",          std::string(skCrypt("MegaDumper x86").decrypt()) },
{ L"module_dump.exe",            std::string(skCrypt("Module Dumper").decrypt()) },
{ L"moduledumper.exe",           std::string(skCrypt("ModuleDumper").decrypt()) },
{ L"processhacker2.exe",         std::string(skCrypt("Process Hacker 2").decrypt()) },
{ L"processhacker3.exe",         std::string(skCrypt("Process Hacker 3").decrypt()) },
{ L"processhacker64.exe",        std::string(skCrypt("Process Hacker x64").decrypt()) },
{ L"reclassnet.exe",             std::string(skCrypt("ReClass.NET").decrypt()) },
{ L"reclass.exe",                std::string(skCrypt("ReClass").decrypt()) },
{ L"reclass64.exe",              std::string(skCrypt("ReClass x64").decrypt()) },
{ L"loadlibrary.exe",            std::string(skCrypt("LoadLibrary Helper").decrypt()) },
{ L"manualmap.exe",              std::string(skCrypt("Manual Mapper").decrypt()) },
{ L"manual_map.exe",             std::string(skCrypt("Manual Map Injector").decrypt()) },
{ L"manualmapper.exe",           std::string(skCrypt("ManualMapper").decrypt()) },
{ L"manual_m.exe",               std::string(skCrypt("Manual Map (alias)").decrypt()) },
{ L"reflective.dll",             std::string(skCrypt("Reflective Loader (DLL)").decrypt()) },
{ L"reflective_loader.exe",      std::string(skCrypt("Reflective Loader").decrypt()) },
{ L"reflective_injector.exe",    std::string(skCrypt("Reflective Injector").decrypt()) },
{ L"refined_injector.exe",       std::string(skCrypt("Refined Injector").decrypt()) },
{ L"xinjector.exe",              std::string(skCrypt("X-Injector").decrypt()) },
{ L"x32_injector.exe",           std::string(skCrypt("X32 Injector").decrypt()) },
{ L"x64_injector.exe",           std::string(skCrypt("X64 Injector").decrypt()) },
{ L"vmp_loader.exe",             std::string(skCrypt("VMP Loader Helper").decrypt()) },
{ L"vmpstub.exe",                std::string(skCrypt("VMP Stub").decrypt()) },
{ L"vmp_unpacker.exe",           std::string(skCrypt("VMP Unpacker (tool)").decrypt()) },
{ L"dnv_injector.exe",           std::string(skCrypt("DNV Injector").decrypt()) },
{ L"dll_loader.exe",             std::string(skCrypt("DLL Loader").decrypt()) },
{ L"payload_injector.exe",       std::string(skCrypt("Payload Injector").decrypt()) },
{ L"payload_loader.exe",         std::string(skCrypt("Payload Loader").decrypt()) },
{ L"process_injector.exe",       std::string(skCrypt("Process Injector").decrypt()) },
{ L"proc_inject.exe",            std::string(skCrypt("ProcInject").decrypt()) },
{ L"mh_injector.exe",            std::string(skCrypt("MH Injector").decrypt()) },
{ L"mh_inject64.exe",            std::string(skCrypt("MH Injector x64").decrypt()) },
{ L"madshi_detours.exe",         std::string(skCrypt("Madshi Detours Helper").decrypt()) },
{ L"madshi.exe",                 std::string(skCrypt("Madshi (tools)").decrypt()) },
{ L"detours_tool.exe",           std::string(skCrypt("Detours Tool").decrypt()) },
{ L"reflector.exe",              std::string(skCrypt("Assembly Reflector").decrypt()) },
{ L"reflexil_plugin.exe",        std::string(skCrypt("Reflexil Plugin").decrypt()) },
{ L"dotnet_peeker.exe",          std::string(skCrypt(".NET Peeker").decrypt()) },
{ L"dotpeek64.exe",              std::string(skCrypt("dotPeek x64").decrypt()) },
{ L"il2cpp_injector.exe",        std::string(skCrypt("il2cpp Injector").decrypt()) },
{ L"il2cpp_inject64.exe",        std::string(skCrypt("il2cpp Injector x64").decrypt()) },
{ L"il2cppdumpergui.exe",        std::string(skCrypt("Il2CppDumper GUI").decrypt()) },
{ L"il2cppdumpergui.exe",        std::string(skCrypt("Il2CppDumper GUI (alt)").decrypt()) },
{ L"dotnet_peeker64.exe",        std::string(skCrypt(".NET Peeker x64").decrypt()) },
{ L"dotnet_injector.exe",        std::string(skCrypt(".NET Injector").decrypt()) },
{ L"clr_injector.exe",           std::string(skCrypt("CLR Injector").decrypt()) },
{ L"clr_loader.exe",             std::string(skCrypt("CLR Loader").decrypt()) },
{ L"winject.exe",                std::string(skCrypt("Winject (injector)").decrypt()) },
{ L"winjector.exe",              std::string(skCrypt("Winjector").decrypt()) },
{ L"weiner_injector.exe",        std::string(skCrypt("Weiner Injector (alias)").decrypt()) },
{ L"uh_dll_injector.exe",        std::string(skCrypt("UH DLL Injector").decrypt()) },
{ L"g_injector.exe",             std::string(skCrypt("G Injector").decrypt()) },
{ L"g_inject64.exe",             std::string(skCrypt("G Injector x64").decrypt()) },
{ L"gm_hooker.exe",              std::string(skCrypt("GM Hooker").decrypt()) },
{ L"gm_hooker64.exe",            std::string(skCrypt("GM Hooker x64").decrypt()) },
{ L"frida_injector.exe",         std::string(skCrypt("Frida Injector Helper").decrypt()) },
{ L"fridacli.exe",               std::string(skCrypt("Frida CLI Helper").decrypt()) },
{ L"metasploit_inject.exe",      std::string(skCrypt("Metasploit Inject (tool)").decrypt()) },
{ L"meterpreter_x86.exe",        std::string(skCrypt("Meterpreter x86 Helper").decrypt()) },
{ L"meterpreter_x64.exe",        std::string(skCrypt("Meterpreter x64 Helper").decrypt()) },
{ L"mhook.exe",                  std::string(skCrypt("MHook Helper").decrypt()) },
{ L"mhook64.exe",                std::string(skCrypt("MHook x64").decrypt()) },
{ L"mhook32.exe",                std::string(skCrypt("MHook x86").decrypt()) },
{ L"gm_dumper.exe",              std::string(skCrypt("Game Maker Dumper").decrypt()) },
{ L"gm_dumper64.exe",            std::string(skCrypt("Game Maker Dumper x64").decrypt()) },
{ L"dump_tool.exe",              std::string(skCrypt("Generic Dump Tool").decrypt()) },
{ L"dump_tool64.exe",            std::string(skCrypt("Generic Dump Tool x64").decrypt()) },
{ L"vmprotect_unwrap.exe",       std::string(skCrypt("VMProtect Unwrap Helper").decrypt()) },
{ L"packer_detector.exe",        std::string(skCrypt("Packer Detector").decrypt()) },
{ L"anti_debug_helper.exe",      std::string(skCrypt("Anti-Debug Helper (tool)").decrypt()) },
{ L"anti_debug64.exe",           std::string(skCrypt("Anti-Debug x64").decrypt()) },
{ L"rootkitreveal.exe",          std::string(skCrypt("RootkitRevealer").decrypt()) },
{ L"gmer.exe",                   std::string(skCrypt("GMER Rootkit Scanner").decrypt()) },
{ L"gmer64.exe",                 std::string(skCrypt("GMER x64").decrypt()) },
{ L"autoruns64.exe",             std::string(skCrypt("Autoruns x64").decrypt()) },
{ L"procdump_tool.exe",          std::string(skCrypt("ProcDump Tool (alt)").decrypt()) },
{ L"procdump2.exe",              std::string(skCrypt("ProcDump v2").decrypt()) },
{ L"proc_extractor.exe",         std::string(skCrypt("Process Extractor").decrypt()) },
{ L"pe_bear.exe",                std::string(skCrypt("PE-bear").decrypt()) },
{ L"pestudio64.exe",             std::string(skCrypt("PEStudio x64").decrypt()) },
{ L"pestudio32.exe",             std::string(skCrypt("PEStudio x86").decrypt()) },
{ L"upx_unpacker.exe",           std::string(skCrypt("UPX Unpacker").decrypt()) },
{ L"upx_gui.exe",                std::string(skCrypt("UPX GUI").decrypt()) },
{ L"packer_detector64.exe",      std::string(skCrypt("Packer Detector x64").decrypt()) },
{ L"ida_free64.exe",             std::string(skCrypt("IDA Free 64 (alt)").decrypt()) },
{ L"ghidra64.exe",               std::string(skCrypt("Ghidra x64 Launcher").decrypt()) },
{ L"binaryninja64.exe",          std::string(skCrypt("Binary Ninja x64").decrypt()) },
{ L"binaryninja32.exe",          std::string(skCrypt("Binary Ninja x86").decrypt()) },
{ L"ghidra_headless.exe",        std::string(skCrypt("Ghidra Headless").decrypt()) },
{ L"ghidra_analyze.exe",         std::string(skCrypt("Ghidra Analyzer").decrypt()) },
{ L"ida_plugin.exe",             std::string(skCrypt("IDA Helper Plugin").decrypt()) },
{ L"ida_helper64.exe",           std::string(skCrypt("IDA Helper x64").decrypt()) },
{ L"olly_plugin.exe",            std::string(skCrypt("Olly Plugin").decrypt()) },
{ L"olly_helper.exe",            std::string(skCrypt("Olly Helper").decrypt()) },
{ L"hookshark.exe",              std::string(skCrypt("HookShark").decrypt()) },
{ L"hookfinder.exe",             std::string(skCrypt("Hook Finder").decrypt()) },
{ L"hookscanner.exe",            std::string(skCrypt("Hook Scanner").decrypt()) },
{ L"hookcleaner.exe",            std::string(skCrypt("Hook Cleaner").decrypt()) },
{ L"bypass_tool.exe",            std::string(skCrypt("Bypass Tool (generic)").decrypt()) },
{ L"bypass64.exe",               std::string(skCrypt("Bypass Tool x64").decrypt()) },
            };

            for (const auto& win : blacklisted_windows) {
                // FindWindowW expects LPCWSTR
                if (FindWindowW(win.first.c_str(), nullptr)) {
                    return win.second;
                }
            }
            return std::string();
        }

        // VM environment checks
        bool check_vm_environment() {
            HKEY hKey = nullptr;

            // VBoxService
            if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, skCrypt("SYSTEM\\CurrentControlSet\\Services\\VBoxService").decrypt(), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
                RegCloseKey(hKey);
                return true;
            }

            // VMTools (VMware)
            if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, skCrypt("SYSTEM\\CurrentControlSet\\Services\\VMTools").decrypt(), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
                RegCloseKey(hKey);
                return true;
            }

            // vm3dservice (other VM artifacts)
            if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, skCrypt("SYSTEM\\CurrentControlSet\\Services\\vm3dservice").decrypt(), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
                RegCloseKey(hKey);
                return true;
            }

            // CPUID check for hypervisor signature (simple heuristic)
            int cpuInfo[4] = { 0 };
            __cpuid(cpuInfo, 0x40000000);
            if (cpuInfo[0] >= 0x40000000) {
                return true;
            }

            return false;
        }

        // Timing attack heuristic
        bool check_timing_attack() {
            LARGE_INTEGER start{}, end{}, freq{};
            if (!QueryPerformanceFrequency(&freq)) return false;
            QueryPerformanceCounter(&start);

            // tiny op that might be slowed under debugger
            IsDebuggerPresent();

            QueryPerformanceCounter(&end);
            double elapsed = static_cast<double>(end.QuadPart - start.QuadPart) / static_cast<double>(freq.QuadPart);
            return elapsed > 0.001; // >1ms
        }

        // Remote desktop detection
        bool check_remote_desktop() {
            return GetSystemMetrics(SM_REMOTESESSION) != 0;
        }

        // Main protection loop
        void protection_loop() {
            while (running) {
                try {
                    if (check_peb_debug()) {
                        log_protection(std::string(skCrypt("PEB BeingDebugged flag detected").decrypt()), std::string(skCrypt("Kernel-level debugger detected").decrypt()));
                        return;
                    }

                    if (check_nt_global_flag()) {
                        log_protection(std::string(skCrypt("NtGlobalFlag debug flags detected").decrypt()), std::string(skCrypt("Debug heap flags set").decrypt()));
                        return;
                    }

                    if (check_hardware_breakpoints()) {
                        log_protection(std::string(skCrypt("Hardware breakpoints detected").decrypt()), std::string(skCrypt("DR0-DR3 registers set").decrypt()));
                        return;
                    }

                    if (check_kernel_debugger()) {
                        log_protection(std::string(skCrypt("Kernel debugger detected").decrypt()), std::string(skCrypt("System debugger enabled").decrypt()));
                        return;
                    }

                    std::string detected_process = check_blacklisted_processes();
                    if (!detected_process.empty()) {
                        log_protection(std::string(skCrypt("Blacklisted process detected").decrypt()), detected_process);
                        return;
                    }

                    std::string detected_window = check_blacklisted_windows();
                    if (!detected_window.empty()) {
                        log_protection(std::string(skCrypt("Blacklisted window detected").decrypt()), detected_window);
                        return;
                    }

                    if (check_vm_environment()) {
                        log_protection(std::string(skCrypt("VM environment detected").decrypt()), std::string(skCrypt("Virtual machine detected").decrypt()));
                        return;
                    }

                    if (check_timing_attack()) {
                        log_protection(std::string(skCrypt("Timing attack detected").decrypt()), std::string(skCrypt("Debugger slowing execution").decrypt()));
                        return;
                    }

                    if (check_remote_desktop()) {
                        log_protection(std::string(skCrypt("Remote desktop detected").decrypt()), std::string(skCrypt("Remote session active").decrypt()));
                        return;
                    }

                    // Standard API checks
                    if (IsDebuggerPresent()) {
                        log_protection(std::string(skCrypt("IsDebuggerPresent() returned true").decrypt()), std::string(skCrypt("Debugger attached").decrypt()));
                        return;
                    }

                    BOOL remote_debugger = FALSE;
                    if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &remote_debugger) && remote_debugger) {
                        log_protection(std::string(skCrypt("Remote debugger present").decrypt()), std::string(skCrypt("Remote debugging detected").decrypt()));
                        return;
                    }
                }
                catch (...) {
                    log_protection(std::string(skCrypt("Exception in protection loop").decrypt()), std::string(skCrypt("Unknown error occurred").decrypt()));
                    return;
                }

                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        }

    public:
        void start() {
            if (running) return;
            running = true;
            protection_thread = std::thread(&AdvancedProtection::protection_loop, this);
        }

        void stop() {
            running = false;
            if (protection_thread.joinable()) protection_thread.join();
        }

        ~AdvancedProtection() {
            stop();
        }
    };

    // Singleton instance pointer
    static AdvancedProtection* g_protection = nullptr;

    // Initialize protection
    void init_protection() {
        if (!g_protection) {
            g_protection = new AdvancedProtection();
            g_protection->start();
        }
    }

    // Backwards compat check
    debug_results check_security() {
        init_protection();
        return none;
    }

} // namespace security
