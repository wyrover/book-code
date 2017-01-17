includeexternal ("premake5-include.lua")

workspace "crack"
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
        buildoptions { "/X", "/Gz", "/GR-" } 
        --linkoptions { "/SUBSYSTEM:NATIVE", "/DRIVER", "/ENTRY:\"DriverEntry\"", "/NODEFAULTLIB" }
        linkoptions { "/SUBSYSTEM:NATIVE", "/DRIVER", "/ENTRY:\"DriverEntry\"" }
        flags { "StaticRuntime", "NoManifest", "No64BitChecks", "NoBufferSecurityCheck", "NoRuntimeChecks" }
        files
        {
            dir .. "/%{prj.name}/**.h",
            dir .. "/%{prj.name}/**.cpp", 
            dir .. "/%{prj.name}/**.c", 
            dir .. "/%{prj.name}/**.rc" 
--            "3rdparty/WinRing0/**.h",
--            "3rdparty/WinRing0/**.c",
--            "3rdparty/WinRing0/**.cpp",
--            "3rdparty/WinRing0/**.rc"                
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
        defines { "WIN32", "_CONSOLE", "_DDK_" }
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
        filter "configurations:Release"
            defines { "_NDEBUG", "DBG=0" }
            buildoptions { "/Ob2", "/Oi", "/GL" } 
    end



    

        --create_console_project("file-2-hex", "src/tools")         
        --create_console_project("bin2cpp", "hack-tools", "mbcs")        
        
    group "01"
        
        project "console_entry"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain", "StaticRuntime" }  
            --defines { "STATIC_GETOPT" }
            files
            {
                                
                "crack-tutorial/example/%{prj.name}/**.h",
                "crack-tutorial/example/%{prj.name}/**.cpp"                            
            }
            removefiles
            {               
            }
            includedirs
            {                   
                
            }
            links
            { 
                
            }
            libdirs
            {
                     
            }
            pchsource "crack-tutorial/example/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h"   
            
        
        project "win_entry"            
            kind "WindowedApp"  
            characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }  
            --defines { "STATIC_GETOPT" }
            files
            {
                                
                "crack-tutorial/example/%{prj.name}/**.h",
                "crack-tutorial/example/%{prj.name}/**.cpp"                            
            }
            removefiles
            {               
            }
            includedirs
            {                   
                
            }
            links
            { 
                
            }
            libdirs
            {
                     
            }
            pchsource "crack-tutorial/example/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h" 

        project "LittleEndian"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain", "StaticRuntime" }              
            files
            {
                                
                "crack-tutorial/example/%{prj.name}/**.h",
                "crack-tutorial/example/%{prj.name}/**.cpp"                            
            }            
            pchsource "crack-tutorial/example/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h"  

        project "StackFrame"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain", "StaticRuntime" }              
            files
            {
                                
                "crack-tutorial/example/%{prj.name}/**.h",
                "crack-tutorial/example/%{prj.name}/**.cpp"                            
            }            
            pchsource "crack-tutorial/example/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h"  

            
        project "localN"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain", "StaticRuntime" }              
            files
            {
                                
                "crack-tutorial/example/%{prj.name}/**.h",
                "crack-tutorial/example/%{prj.name}/**.cpp"                            
            }            
            pchsource "crack-tutorial/example/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h"  
            
        
    group "02"
        
        project "if_else"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain", "StaticRuntime" }              
            files
            {
                                
                "crack-tutorial/example/%{prj.name}/**.h",
                "crack-tutorial/example/%{prj.name}/**.cpp"                            
            }            
            pchsource "crack-tutorial/example/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h"  
    
        
        project "switch"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain", "StaticRuntime" }             
            files
            {
                                
                "crack-tutorial/example/%{prj.name}/**.h",
                "crack-tutorial/example/%{prj.name}/**.cpp"                            
            }            
            pchsource "crack-tutorial/example/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h"  


        project "for"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain", "StaticRuntime" }             
            files
            {
                                
                "crack-tutorial/example/%{prj.name}/**.h",
                "crack-tutorial/example/%{prj.name}/**.cpp"                            
            }            
            pchsource "crack-tutorial/example/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h"  

        project "loop"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain", "StaticRuntime" }              
            files
            {
                                
                "crack-tutorial/example/%{prj.name}/**.h",
                "crack-tutorial/example/%{prj.name}/**.cpp"                            
            }            
            pchsource "crack-tutorial/example/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h"  

    group "03"

        project "stdcall"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain", "StaticRuntime" }              
            files
            {
                                
                "crack-tutorial/example/%{prj.name}/**.h",
                "crack-tutorial/example/%{prj.name}/**.cpp"                            
            }            
            pchsource "crack-tutorial/example/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h"  

       
        project "return"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain", "StaticRuntime" }              
            files
            {
                                
                "crack-tutorial/example/%{prj.name}/**.h",
                "crack-tutorial/example/%{prj.name}/**.cpp"                            
            }            
            pchsource "crack-tutorial/example/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h"  

    group "04"
    
        project "array"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain", "StaticRuntime" }              
            files
            {
                                
                "crack-tutorial/example/%{prj.name}/**.h",
                "crack-tutorial/example/%{prj.name}/**.cpp"                            
            }            
            pchsource "crack-tutorial/example/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h" 
            

        project "struct"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain", "StaticRuntime" }              
            files
            {
                                
                "crack-tutorial/example/%{prj.name}/**.h",
                "crack-tutorial/example/%{prj.name}/**.cpp"                            
            }            
            pchsource "crack-tutorial/example/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h"  

        project "object"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain", "StaticRuntime" }              
            files
            {
                                
                "crack-tutorial/example/%{prj.name}/**.h",
                "crack-tutorial/example/%{prj.name}/**.cpp"                            
            }            
            pchsource "crack-tutorial/example/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h"  

        project "ShowNumber"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain", "StaticRuntime" }              
            files
            {
                                
                "crack-tutorial/example/%{prj.name}/**.h",
                "crack-tutorial/example/%{prj.name}/**.cpp"                            
            }            
            pchsource "crack-tutorial/example/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h"
        

        project "GlobalObj"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain", "StaticRuntime" }              
            files
            {
                                
                "crack-tutorial/example/%{prj.name}/**.h",
                "crack-tutorial/example/%{prj.name}/**.cpp"                            
            }            
            pchsource "crack-tutorial/example/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h"
        
        project "VirtualFunction"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain", "StaticRuntime" }              
            files
            {
                                
                "crack-tutorial/example/%{prj.name}/**.h",
                "crack-tutorial/example/%{prj.name}/**.cpp"                            
            }            
            pchsource "crack-tutorial/example/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h"

        
        