includeexternal ("premake5-include.lua")

workspace "ipc"
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

    group "同步对象"    

        project "test-event"          
            kind "ConsoleApp"          
            --characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/ipc/%{prj.name}/**.h",
                "src/ipc/%{prj.name}/**.cpp", 
                "src/ipc/%{prj.name}/**.rc" 
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

        project "AsyncInvokeDemo"          
            kind "WindowedApp"          
            --characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/ipc/%{prj.name}/**.h",
                "src/ipc/%{prj.name}/**.cpp", 
                "src/ipc/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                "3rdparty/wtl"
            }  
            links
            {
                
            }


    group "asio"    

        project "asio05"          
            kind "ConsoleApp"          
            --characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/test-asio/%{prj.name}/**.h",
                "src/test-asio/%{prj.name}/**.cpp", 
                "src/test-asio/%{prj.name}/**.rc" 
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

    group "协程"    

        project "coroutine01"          
            kind "ConsoleApp"          
            --characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/test-coroutine/%{prj.name}/**.h",
                "src/test-coroutine/%{prj.name}/**.cpp", 
                "src/test-coroutine/%{prj.name}/**.rc" 
            }

        project "coroutine02"          
            kind "ConsoleApp"          
            --characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/test-coroutine/%{prj.name}/**.h",
                "src/test-coroutine/%{prj.name}/**.cpp", 
                "src/test-coroutine/%{prj.name}/**.rc" 
            }
            

        
            


    group "namedpipe"          
            
            
        project "NPServer"          
            kind "ConsoleApp"          
            characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/ipc/%{prj.name}/**.h",
                "src/ipc/%{prj.name}/**.cpp", 
                "src/ipc/%{prj.name}/**.rc" 
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

        project "NPClient"          
            kind "ConsoleApp"          
            characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/ipc/%{prj.name}/**.h",
                "src/ipc/%{prj.name}/**.cpp", 
                "src/ipc/%{prj.name}/**.rc" 
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

    group "socket-one-to-one"

        project "one-server"          
            kind "ConsoleApp"          
            characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/ipc/%{prj.name}/**.h",
                "src/ipc/%{prj.name}/**.cpp", 
                "src/ipc/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                
            }  
            links
            {
                "ws2_32.lib"    
            }

        project "one-client"          
            kind "ConsoleApp"          
            characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/ipc/%{prj.name}/**.h",
                "src/ipc/%{prj.name}/**.cpp", 
                "src/ipc/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                
            }  
            links
            {
                "ws2_32.lib"    
            }
    
    group "thread server"
         
        project "thread-server"          
            kind "ConsoleApp"          
            characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/ipc/%{prj.name}/**.h",
                "src/ipc/%{prj.name}/**.cpp", 
                "src/ipc/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                
            }  
            links
            {
                "ws2_32.lib"    
            }

        project "client-thread-server"          
            kind "ConsoleApp"          
            characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/ipc/%{prj.name}/**.h",
                "src/ipc/%{prj.name}/**.cpp", 
                "src/ipc/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                
            }  
            links
            {
                "ws2_32.lib"    
            }

        project "ServerSocket"          
            kind "WindowedApp"          
            --characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime", "MFC" }       
            defines {  }            
            files            
            {                                  
                "src/ipc/%{prj.name}/**.h",
                "src/ipc/%{prj.name}/**.cpp", 
                "src/ipc/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                
            }  
            links
            {
                "ws2_32.lib"    
            }
            pchsource "src/ipc/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h" 
        


    
    group "IOCP"
        
        project "ServerIOCP"          
            kind "ConsoleApp"          
            characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/ipc/%{prj.name}/**.h",
                "src/ipc/%{prj.name}/**.cpp", 
                "src/ipc/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                
            }  
            links
            {
                "ws2_32.lib"    
            }   

        project "ClientIOCP"          
            kind "ConsoleApp"          
            characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/ipc/%{prj.name}/**.h",
                "src/ipc/%{prj.name}/**.cpp", 
                "src/ipc/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                
            }  
            links
            {
                "ws2_32.lib"    
            }  
            
    group "命名管道封装"
        
        project "PipeClient"          
            kind "ConsoleApp"          
            --characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/ipc/%{prj.name}/**.h",
                "src/ipc/%{prj.name}/**.cpp", 
                "src/ipc/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                
            }  
            links
            {
                --"ws2_32.lib"    
            }  
        
        project "PipeServer"          
            kind "ConsoleApp"          
            --characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/ipc/%{prj.name}/**.h",
                "src/ipc/%{prj.name}/**.cpp", 
                "src/ipc/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                
            }  
            links
            {
                --"ws2_32.lib"    
            }   

    group "内存映射"

        project "memory-map"          
            kind "WindowedApp"          
            characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime", "MFC" }       
            defines {  }            
            files            
            {                                  
                "src/ipc/%{prj.name}/**.h",
                "src/ipc/%{prj.name}/**.cpp", 
                "src/ipc/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                
            }  
            links
            {
                --"ws2_32.lib"    
            }   


    group "线程池"
        
        project "threadpool"          
            kind "ConsoleApp"          
            --characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime", "MFC" }       
            defines {  }            
            files            
            {                                  
                "src/thread_pool/%{prj.name}/**.h",
                "src/thread_pool/%{prj.name}/**.cpp", 
                "src/thread_pool/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                
            }  
            links
            {
                --"ws2_32.lib"    
            }   
            pchsource "src/thread_pool/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h"            
            
        project "WQDemo"          
            kind "WindowedApp"          
            characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime", "MFC" }       
            defines {  }            
            files            
            {                                  
                "src/thread_pool/%{prj.name}/**.h",
                "src/thread_pool/%{prj.name}/**.cpp", 
                "src/thread_pool/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                
            }  
            links
            {
                --"ws2_32.lib"    
            }   
            pchsource "src/thread_pool/%{prj.name}/StdAfx.cpp"
            pchheader "StdAfx.h"   


        project "thread-message"          
            kind "WindowedApp"          
            --characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime", "MFC" }       
            defines {  }            
            files            
            {                                  
                "src/thread_pool/%{prj.name}/**.h",
                "src/thread_pool/%{prj.name}/**.cpp", 
                "src/thread_pool/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                
            }  
            links
            {
                --"ws2_32.lib"    
            }   
            pchsource "src/thread_pool/%{prj.name}/stdafx.cpp"
            pchheader "stdafx.h"   

        project "test-threadpool"          
            kind "ConsoleApp"          
            --characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files            
            {                                  
                "src/thread_pool/%{prj.name}/**.h",
                "src/thread_pool/%{prj.name}/**.cpp", 
                "src/thread_pool/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                
            }  
            libdirs
            {
                "lib/x86/%{_ACTION}/boost-1_56"
            }
            links
            {
                --"ws2_32.lib"    
            }   
                        



        