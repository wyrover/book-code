includeexternal ("premake5-include.lua")

workspace "libuv"
    language "C++"
    location "build/%{_ACTION}"    

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
        targetdir "bin/x86/%{_ACTION}"         
    filter { "kind:ConsoleApp or WindowedApp or SharedLib", "platforms:x64" }
        targetdir "bin/x64/%{_ACTION}" 
        

    filter { "platforms:Win32" }
        system "Windows"
        architecture "x32"
        libdirs 
        {
            "lib/x86/%{_ACTION}",
            "lib/x86/%{_ACTION}/boost-1_60",
            "bin/x86/%{_ACTION}"            
        }
    
    filter { "platforms:x64" }
        system "Windows"
        architecture "x64"   
        libdirs
        {
            "lib/x64/%{_ACTION}",
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

    group "libuv"    

        project "libuv01"          
            kind "ConsoleApp"          
            --characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/libuv/%{prj.name}/**.h",
                "src/libuv/%{prj.name}/**.cpp", 
                "src/libuv/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                "3rdparty/libuv"
            }  
            links
            {                  
                "libuv.lib",
                "ws2_32.lib",
                "Psapi.lib",
                "Iphlpapi.lib",
                "advapi32.lib",
                "iphlpapi.lib",
                "shell32.lib",
                "user32.lib", 
                "userenv.lib"

            }

        project "idle-basic"          
            kind "ConsoleApp"          
            --characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/libuv/%{prj.name}/**.h",
                "src/libuv/%{prj.name}/**.cpp", 
                "src/libuv/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                "3rdparty/libuv"
            }  
            links
            {                  
                "libuv.lib",
                "ws2_32.lib",
                "Psapi.lib",
                "Iphlpapi.lib",
                "advapi32.lib",
                "iphlpapi.lib",
                "shell32.lib",
                "user32.lib", 
                "userenv.lib"

            }

        project "queue-work"          
            kind "ConsoleApp"          
            --characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/libuv/%{prj.name}/**.h",
                "src/libuv/%{prj.name}/**.cpp", 
                "src/libuv/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                "3rdparty/libuv"
            }  
            links
            {                  
                "libuv.lib",
                "ws2_32.lib",
                "Psapi.lib",
                "Iphlpapi.lib",
                "advapi32.lib",
                "iphlpapi.lib",
                "shell32.lib",
                "user32.lib", 
                "userenv.lib"

            }
        
        project "windows-pipe-server"          
            kind "ConsoleApp"          
            --characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/libuv/%{prj.name}/**.h",
                "src/libuv/%{prj.name}/**.cpp", 
                "src/libuv/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                "3rdparty/libuv"
            }  
            links
            {                  
                "libuv.lib",
                "ws2_32.lib",
                "Psapi.lib",
                "Iphlpapi.lib",
                "advapi32.lib",
                "iphlpapi.lib",
                "shell32.lib",
                "user32.lib", 
                "userenv.lib"

            }
        
        project "windows-pipe-client"          
            kind "ConsoleApp"          
            --characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/libuv/%{prj.name}/**.h",
                "src/libuv/%{prj.name}/**.cpp", 
                "src/libuv/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                "3rdparty/libuv"
            }  
            links
            {                  
                "libuv.lib",
                "ws2_32.lib",
                "Psapi.lib",
                "Iphlpapi.lib",
                "advapi32.lib",
                "iphlpapi.lib",
                "shell32.lib",
                "user32.lib", 
                "userenv.lib"

            }





        