includeexternal ("premake5-include.lua")

workspace "sys"
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"    

    configurations { "Debug", "Release", "TRACE", "TRACE_MT" }
    platforms { "Win32", "x64" }    

    filter { "kind:StaticLib", "platforms:Win32" }
        targetdir "lib/x86/%{_ACTION}" 
    filter { "kind:StaticLib", "platforms:x64" }
        targetdir "lib/x64/%{_ACTION}" 
    filter { "kind:SharedLib", "platforms:Win32" }
        implibdir "lib/x86/%{_ACTION}" 
    filter { "kind:SharedLib", "platforms:x64" }
        implibdir "lib/x64/%{_ACTION}" 
    filter { "kind:ConsoleApp or WindowedApp or SharedLib", "platforms:Win32" }
        targetdir "bin/x86/%{_ACTION}/%{wks.name}"         
    filter { "kind:ConsoleApp or WindowedApp or SharedLib", "platforms:x64" }
        targetdir "bin/x64/%{_ACTION}/%{wks.name}" 
        

    filter { "platforms:Win32" }
        system "Windows"
        architecture "x32"
        libdirs 
        {
            "lib/x86/%{_ACTION}",
            "lib/x86/%{_ACTION}/boost-1_56",
            "lib/x86/%{_ACTION}/boost-1_60",
            "bin/x86/%{_ACTION}"            
        }
    
    filter { "platforms:x64" }
        system "Windows"
        architecture "x64"   
        libdirs
        {
            "lib/x64/%{_ACTION}",
            "lib/x64/%{_ACTION}/boost-1_56",
            "lib/x64/%{_ACTION}/boost-1_60",
            "bin/x64/%{_ACTION}"
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        flags { "Symbols" }
        optimize "Speed"  
    
    filter "configurations:TRACE"
        defines { "NDEBUG", "TRACE_TOOL" }
        flags { "Symbols" }
        optimize "Speed"  
        buildoptions { "/Od" } 
        includedirs
        {            
            "3rdparty"    
        }  
        links { "tracetool.lib" }         

    filter "configurations:TRACE_MT"
        defines { "NDEBUG", "TRACE_TOOL" }
        flags { "Symbols" }
        optimize "On"  
        buildoptions { "/Od" }  
        includedirs
        {            
            "3rdparty"    
        }    
        links { "tracetool_mt.lib" }        

    configuration "vs*"
        warnings "Extra"                    -- 开启最高级别警告
        defines
        {
            "WIN32",
            "WIN32_LEAN_AND_MEAN",
            "_WIN32_WINNT=0x501",           -- 支持到 xp
            "_CRT_SECURE_NO_WARNINGS",        
            "_CRT_SECURE_NO_DEPRECATE",            
            "STRSAFE_NO_DEPRECATE",
            "_CRT_NON_CONFORMING_SWPRINTFS"
        }
        buildoptions
        {
            "/wd4267",                      -- 关闭 64 位检测
            "/wd4996"
        }    
        
    print("test")

    function create_console_project(name, dir, mbcs)        
        project(name)          
        kind "ConsoleApp"                          
        if mbcs == "mbcs" then
            characterset "MBCS"
        end
        flags { "NoManifest", "WinMain", "StaticRuntime" }       
        defines {  }
        files
        {                                  
            dir .. "/%{prj.name}/**.h",
            dir .. "/%{prj.name}/**.cpp", 
            dir .. "/%{prj.name}/**.c", 
            dir .. "/%{prj.name}/**.rc" 
        }
        removefiles
        {               
        }
        includedirs
        {                   
            "3rdparty"   
        }       
        links
        {
            "Wtsapi32.lib"
        }
    end

    function create_sys_project(name, dir)
        project(name)          
        targetextension ".sys"
        kind "WindowedApp"               
                 
        --linkoptions { "/SUBSYSTEM:NATIVE", "/DRIVER", "/ENTRY:\"DriverEntry\"", "/NODEFAULTLIB" }
        linkoptions { "/STACK:0x400000,0x1000", "/INCREMENTAL:NO", "/NOLOGO", "/MANIFEST:NO", "/NODEFAULTLIB", "/SUBSYSTEM:NATIVE", "/DRIVER", "/ENTRY:\"DriverEntry\"", "/BASE:\"0x10000\"", "/SECTION:INIT,D /IGNORE:4078" }
        flags { "StaticRuntime", "NoManifest", "No64BitChecks", "NoBufferSecurityCheck", "NoRuntimeChecks" }
        files
        {
            dir .. "/%{prj.name}/**.h",
            dir .. "/%{prj.name}/**.cpp", 
            dir .. "/%{prj.name}/**.c", 
            dir .. "/%{prj.name}/**.rc"               
        }
        removefiles
        {               
        }
        includedirs
        {   
            "C:/WinDDK/7600.16385.1/inc/ddk",
            "C:/WinDDK/7600.16385.1/inc/api",
            "C:/WinDDK/7600.16385.1/inc/crt",
            "C:/Program Files (x86)/Microsoft Visual Studio 8/VC/include"
        }
        --pchsource "3rdparty/WinRing0/stdafx.cpp"
        --pchheader "stdafx.h"                    



        defines { "WIN32=100", "_CONSOLE", "_DDK_" }
        filter { "platforms:Win32" }
            defines { "_X86_" }     
            libdirs
            {
                "C:/WinDDK/7600.16385.1/lib/WIN7/i386"
                
            }
            links
            { 
                "ntoskrnl.lib",
                "hal.lib",
                "int64.lib",
                "ntstrsafe.lib",
                "exsup.lib",
                "ksecdd.lib"                      
            }            
        filter "platforms:x64"
            defines { "_AMD64_" }
            libdirs
            {
                "C:/WinDDK/7600.16385.1/lib/WIN7/amd64"                    
            }
            links
            { 
                "ntoskrnl.lib",
                "hal.lib",                    
                "ntstrsafe.lib",                    
                "ksecdd.lib"                      
            }            
            targetsuffix "x64"
        filter "configurations:Debug"
            defines { "_DEBUG", "DBG=1" }
            buildoptions { "/Od", "/X", "/FD", "/GS-", "/Gy", "/GR-", "/c", "/Zi", "/Gz", "/TP", "/errorReport:prompt" }
        filter "configurations:Release"
            defines { "_NDEBUG", "DBG=0" }
            buildoptions { "/Ox", "/Ob2", "/Oi", "/GL", "/X", "/GF", "/FD", "/GS-", "/Gy", "/GR-", "/nologo", "/c", "/Zi", "/Gz", "/TP", "/errorReport:prompt" } 
    end





    group "hack-tools"            

        create_console_project("Win.JobLock", "hack-tools")  
        create_console_project("EnumerateTasks", "hack-tools")  
        create_console_project("driver-loader", "hack-tools")  
        create_console_project("bin2cpp", "hack-tools", "mbcs")
        
        --create_console_project("sharedmemory-client", "src/ipc")           

    group "WinRing0"
        
        create_sys_project("WinRing0", "hack-tools")

        project "WinRing0_DLL"            
            targetname "WinRing0"
            kind "SharedLib"
            flags { "StaticRuntime", "NoManifest" }
            files
            {
                "hack-tools/WinRing0_DLL/**.h",
                "hack-tools/WinRing0_DLL/**.c",
                "hack-tools/WinRing0_DLL/**.cpp",
                "hack-tools/WinRing0_DLL/**.rc",
                "hack-tools/WinRing0_DLL/**.def"
            }
            removefiles
            {               
            }
            includedirs
            {   

            }
            pchsource "hack-tools/WinRing0_DLL/stdafx.cpp"
            pchheader "stdafx.h"       

            filter { "platforms:Win32" }
                defines { "WIN32", "_WINDOWS" }     
                libdirs
                {                    
                }
                links
                {
                    
                }                
            filter "platforms:x64"
                defines { "WIN32", "_WINDOWS", "_USRDLL", "OLSDLL_EXPORTS", "OLS_WIN_RING0" }
                files
                {
                    "hack-tools/WinRing0_DLL/**.h",
                    "hack-tools/WinRing0_DLL/**.c",
                    "hack-tools/WinRing0_DLL/**.cpp",
                    "hack-tools/WinRing0_DLL/**.asm",  
                    "hack-tools/WinRing0_DLL/**.rc",
                    "hack-tools/WinRing0_DLL/**.def"  
                }
                libdirs
                {                                        
                }
                links
                {                     
                }                
                targetsuffix "x64"
            filter "files:**.asm"
                buildmessage "Compiling %{file.relpath}"
                buildcommands 
                {
                    "ml64 -c \"-Fl%{cfg.objdir}/%{file.basename}.lst\" \"-Fo%{cfg.objdir}/%{file.basename}.obj\" \"%{file.relpath}\""
                }
                buildoutputs
                {
                    "%{cfg.objdir}/%{file.basename}.obj"
                }


    group "sys-examples"
        create_console_project("LoadNTDriver", "hack-tools", "mbcs")
        create_console_project("sys-loader", "hack-tools", "mbcs")

        create_sys_project("TestHello", "hack-tools/sys-examples")
        create_sys_project("HelloDDK", "hack-tools/sys-examples")
        create_sys_project("HelloDDK2", "hack-tools/sys-examples")
        create_sys_project("MyDriver", "hack-tools/sys-examples")
        create_sys_project("basic01", "hack-tools/sys-examples")
        create_sys_project("basic02", "hack-tools/sys-examples")
        create_sys_project("basic03", "hack-tools/sys-examples")
        create_sys_project("basic04", "hack-tools/sys-examples")

        create_sys_project("TestString", "hack-tools/sys-examples")
        create_sys_project("KeyBoard", "hack-tools/sys-examples")

        create_sys_project("BlackBoneDrv", "hack-tools/sys-examples")

    group "demo01"
        create_sys_project("demo01", "hack-tools/sys-examples/demo01")
        create_console_project("demo01_user", "hack-tools/sys-examples/demo01", "mbcs")

    group "demo02"
        create_sys_project("demo02", "hack-tools/sys-examples/demo02")
        create_console_project("demo02_user", "hack-tools/sys-examples/demo02", "mbcs")

    
    group "dispatch_test"
        create_sys_project("dispatch_test_sys", "hack-tools/sys-examples/DispatchTest")
        create_console_project("dispatch_test_user", "hack-tools/sys-examples/DispatchTest", "mbcs")

    group "ioctl_test"
        create_sys_project("ioctl_test_sys", "hack-tools/sys-examples/IOCTL_Test")
        create_console_project("ioctl_test_user", "hack-tools/sys-examples/IOCTL_Test", "mbcs")

    group "mdl_test"
        create_sys_project("mdl_test_sys", "hack-tools/sys-examples/MDL_Test")
        create_console_project("mdl_test_user", "hack-tools/sys-examples/MDL_Test", "mbcs")

    group "read_test"
        create_sys_project("read_test_sys", "hack-tools/sys-examples/ReadTest")
        create_console_project("read_test_user", "hack-tools/sys-examples/ReadTest", "mbcs")    




    group "TitanHide"
        create_sys_project("TitanHide", "hack-tools/TitanHide")
        
 



        