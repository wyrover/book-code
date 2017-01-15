configurations { "Debug", "Release", "Debug_MT", "Release_MT", "TRACE", "TRACE_MT" }
    platforms { "Win32", "x64" }      

    -- 静态库
    filter { "kind:StaticLib", "platforms:Win32" }
        targetdir "lib/x86/%{_ACTION}/md" 
    filter { "kind:StaticLib", "platforms:Win32", "configurations:*_MT" }
        targetdir "lib/x86/%{_ACTION}/mt"         

    filter { "kind:StaticLib", "platforms:x64" }
        targetdir "lib/x64/%{_ACTION}/md" 
    filter { "kind:StaticLib", "platforms:x64", "configurations:*_MT" }
        targetdir "lib/x64/%{_ACTION}/mt" 


    -- 动态库    
    filter { "kind:SharedLib", "platforms:Win32" }
        implibdir "lib/x86/%{_ACTION}/md" 
        implibsuffix  "_impl"
    filter { "kind:SharedLib", "platforms:Win32", "configurations:*_MT" }
        implibdir "lib/x86/%{_ACTION}/mt" 
        implibsuffix  "_impl"

    filter { "kind:SharedLib", "platforms:x64" }
        implibdir "lib/x64/%{_ACTION}/md" 
        implibsuffix  "_impl"
    filter { "kind:SharedLib", "platforms:x64", "configurations:*_MT" }
        implibdir "lib/x64/%{_ACTION}/mt" 
        implibsuffix  "_impl"

    -- EXE 和 动态库输出

    filter { "kind:ConsoleApp or WindowedApp or SharedLib", "platforms:Win32" }
        targetdir "bin/x86/%{_ACTION}/%{wks.name}/md"     
    filter { "kind:ConsoleApp or WindowedApp or SharedLib", "platforms:Win32", "configurations:*_MT" }
        targetdir "bin/x86/%{_ACTION}/%{wks.name}/mt"

    filter { "kind:ConsoleApp or WindowedApp or SharedLib", "platforms:x64" }
        targetdir "bin/x64/%{_ACTION}/%{wks.name}/md" 
    filter { "kind:ConsoleApp or WindowedApp or SharedLib", "platforms:x64", "configurations:*_MT" }
        targetdir "bin/x64/%{_ACTION}/%{wks.name}/mt"


    filter { kind "ConsoleApp" }
        flags { "WinMain" } 
        

    filter { "platforms:Win32", "configurations:not *_MT" }
        system "Windows"
        architecture "x32"
        includedirs
        {            
            --"lib/v7.1/Include"    
        }    
        libdirs 
        {
            --"lib/v7.1/Lib",
            "lib/x86/%{_ACTION}/md",
            "lib/x86/%{_ACTION}/md/boost-1_60",
            "bin/x86/%{_ACTION}/md"            
        }

    filter { "platforms:Win32", "configurations:*_MT" }
        system "Windows"
        architecture "x32"
        includedirs
        {            
            --"lib/v7.1/Include"    
        }    
        libdirs 
        {
            --"lib/v7.1/Lib",
            "lib/x86/%{_ACTION}/mt",
            "lib/x86/%{_ACTION}/mt/boost-1_60",
            "bin/x86/%{_ACTION}/mt"            
        }    

    
    filter { "platforms:x64", "configurations:not *_MT" }
        system "Windows"
        architecture "x64"   
        libdirs
        {
            --"lib/v7.1/Lib/x64",
            "lib/x64/%{_ACTION}/md",
            "lib/x64/%{_ACTION}/md/boost-1_60",
            "bin/x64/%{_ACTION}/md"
        }
    
    filter { "platforms:x64", "configurations:*_MT" }
        system "Windows"
        architecture "x64"   
        libdirs
        {
            --"lib/v7.1/Lib/x64",
            "lib/x64/%{_ACTION}/mt",
            "lib/x64/%{_ACTION}/mt/boost-1_60",
            "bin/x64/%{_ACTION}/mt"
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    filter { "configurations:Debug_MT" }
        defines { "DEBUG" }       
        flags { "Symbols", "StaticRuntime", "NoManifest" } 

    filter "configurations:Release"
        defines { "NDEBUG" }
        flags { "Symbols" }
        optimize "Speed"  
        buildoptions { "/Od" } 

    filter "configurations:Release_MT"
        defines { "NDEBUG" }
        flags { "Symbols", "StaticRuntime", "NoManifest" }
        optimize "Speed"  
        buildoptions { "/Od" } 
    
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
        flags { "Symbols", "StaticRuntime", "NoManifest" }
        optimize "On"  
        buildoptions { "/Od" }  
        includedirs
        {            
            "3rdparty"    
        }    
        links { "tracetool.lib" }        

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