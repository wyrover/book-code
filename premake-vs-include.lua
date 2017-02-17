configurations { "Debug", "Release", "Debug_MT", "Release_MT", "TRACE", "TRACE_MT" }
    platforms { "Win32", "x64" }      

    filter { "action:vs2005" }
        includedirs
        {                  
            "3rdparty/msvc_compat",
            
        }      
        
    filter { "platforms:Win32" }
        architecture "x86"
    filter { "platforms:x64" }
        architecture "x64"
        
    filter { "kind:StaticLib", "platforms:Win32" }
        linkoptions { "/MACHINE:X86" }        
    filter { "kind:StaticLib", "platforms:x64" }
        linkoptions { "/MACHINE:X64" }

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
    filter { "kind:SharedLib", "platforms:Win32", "configurations:*_MT" }
        implibdir "lib/x86/%{_ACTION}/mt"
    filter { "kind:SharedLib", "platforms:x64" }
        implibdir "lib/x64/%{_ACTION}/md"        
    filter { "kind:SharedLib", "platforms:x64", "configurations:*_MT" }
        implibdir "lib/x64/%{_ACTION}/mt" 
        

    -- EXE 和 动态库输出

    filter { "kind:ConsoleApp or WindowedApp or SharedLib", "platforms:Win32", "configurations:not *_MT" }
        targetdir "bin/x86/%{_ACTION}/%{wks.name}/md"  
        debugdir "bin/x86/%{_ACTION}/%{wks.name}/md"      
        if _ACTION == "vs2015" then
            flags { "NoManifest" } 
            files { "include/buildcfg/vs2015/version.rc" }
            includedirs
            {              
                "include/buildcfg/vs2015"                
            }
        end
    filter { "kind:ConsoleApp or WindowedApp or SharedLib", "platforms:Win32", "configurations:*_MT" }
        targetdir "bin/x86/%{_ACTION}/%{wks.name}/mt"
        debugdir "bin/x86/%{_ACTION}/%{wks.name}/mt"        
    filter { "kind:ConsoleApp or WindowedApp or SharedLib", "platforms:x64", "configurations:not *_MT" }
        targetdir "bin/x64/%{_ACTION}/%{wks.name}/md" 
        debugdir "bin/x64/%{_ACTION}/%{wks.name}/md"  
        if _ACTION == "vs2015" then
            flags { "NoManifest" } 
            files { "include/buildcfg/vs2015/version.rc" }
            includedirs
            {              
                "include/buildcfg/vs2015"                
            }
        end
    filter { "kind:ConsoleApp or WindowedApp or SharedLib", "platforms:x64", "configurations:*_MT" }
        targetdir "bin/x64/%{_ACTION}/%{wks.name}/mt"
        debugdir "bin/x64/%{_ACTION}/%{wks.name}/md"        


    filter { kind "ConsoleApp" }
        flags { "WinMain" } 

     
    filter { "platforms:Win32 or x64" }
        includedirs
        {                  
            "3rdparty/boost_1_60_0",
            "3rdparty"
        }    
        

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
        --buildoptions { "/Od" } 

    filter "configurations:Release_MT"
        defines { "NDEBUG" }
        flags { "Symbols", "StaticRuntime", "NoManifest" }
        optimize "Speed"  
        --buildoptions { "/Od" } 
    
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
        
    filter { "kind:StaticLib", "configurations:Debug*", "configurations:*_MT"}
        targetsuffix "-mt-s-gd"    
    filter { "kind:StaticLib", "configurations:Debug*", "configurations:not *_MT"}
        targetsuffix "-s-gd"
    filter { "kind:StaticLib", "configurations:not Debug*", "configurations:*_MT"}
        targetsuffix "-mt-s"    
    filter { "kind:StaticLib", "configurations:not Debug*", "configurations:not *_MT"}
        targetsuffix "-s"

    filter { "kind:SharedLib", "configurations:Debug*", "configurations:*_MT"}
        targetsuffix "-mt-gd"        
    filter { "kind:SharedLib", "configurations:Debug*", "configurations:not *_MT"}
        targetsuffix "-gd"        
    filter { "kind:SharedLib", "configurations:not Debug*", "configurations:*_MT"}
        targetsuffix "-mt"        
    filter { "kind:SharedLib", "configurations:not Debug*", "configurations:not *_MT"}
        targetsuffix ""
        


        


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


    function has_stdafx(name, dir)
        if os.isfile(dir .. "/" .. name .. "/stdafx.cpp") then              
            local upper_flag = false
            local result = os.matchfiles(dir .. "/" .. name .. "/*.cpp")
            for i = #result, 1, -1 do                
                local filename = path.getname(result[i])
                if filename == "StdAfx.cpp" then
                    upper_flag = true
                elseif filename == "stdafx.cpp" then
                    upper_flag = false   
                end
                
            end
                
            if upper_flag then
                pchsource(dir .. "/%{prj.name}/StdAfx.cpp")
                pchheader("StdAfx.h")
            else
                pchsource(dir .. "/%{prj.name}/stdafx.cpp")
                pchheader("stdafx.h")    
            end
        end
    end


    function create_mfc_console_project(name, dir)        
        project(name)          
        kind "WindowedApp"                             
        defines {  }
        flags { "MFC", "WinMain" }
        removeconfigurations "*_MT"      
        linkoptions
        {
            "/ENTRY:\"wmainCRTStartup\""
        }
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
            "3rdparty",          
        }        
        
    end


    function create_mfc_project(name, dir)        
        project(name)          
        kind "WindowedApp"                             
        defines {  }
        flags { "MFC", "WinMain" }
        removeconfigurations "*_MT"              
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
            "3rdparty",          
        }  
        has_stdafx(name, dir)  
        
    end

    function create_mfc_charset_project(name, dir, mbcs)        
        project(name)          
        kind "WindowedApp"                             
        defines {  }
        flags { "MFC", "WinMain" }
        removeconfigurations "*_MT"   
        if mbcs == "mbcs" then
            characterset "MBCS"
        end 
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
            "3rdparty",          
        }  
        has_stdafx(name, dir)  
        
    end


    function create_console_project(name, dir)        
        project(name)          
        kind "ConsoleApp"                                             
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
            "3rdparty",          
        }         
        has_stdafx(name, dir)               
    end    
    

    function create_console_charset_project(name, dir, mbcs)        
        project(name)          
        kind "ConsoleApp"                          
        if mbcs == "mbcs" then
            characterset "MBCS"
        end        
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
        has_stdafx(name, dir)
    end


    function create_wtl_project(name, dir)        
        project(name)          
        kind "WindowedApp"                                                     
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
            "3rdparty",   
            "3rdparty/wtl"
        }        
        has_stdafx(name, dir)
    end


    function create_dll_project(name, dir)        
        project(name)          
        kind "SharedLib"                                           
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
            "3rdparty",          
        }         
        has_stdafx(name, dir)               
    end


    function create_sfml_project(name, dir)        
        project(name)          
        kind "ConsoleApp"                                
        defines {  }
        removeconfigurations "*_MT"      
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
            "3rdparty",          
        }        
        libdirs
        {
            "lib/x86/%{_ACTION}/md/sfml"
        }
        filter "configurations:Debug"
            links
            {
                "sfml-main-d.lib",
                "sfml-system-d.lib",
                "sfml-window-d.lib",
                "sfml-network-d.lib",
                "sfml-graphics-d.lib",
                "sfml-audio-d.lib"
            }  

        filter "configurations:Release"
            links
            {
                "sfml-main.lib",
                "sfml-system.lib",
                "sfml-window.lib",
                "sfml-network.lib",
                "sfml-graphics.lib",
                "sfml-audio.lib"
            }  
    end


    function create_glfw_console_project(name, dir)        
        project(name)          
        kind "ConsoleApp"                                             
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
            "3rdparty/GLFW",          
        }  
        links
        {
            "glfw3.lib",
            "opengl32.lib"
        }        
        libdirs
        {
            "lib/x86/vs2005/md/glfw"
        }       
    end


    function create_glfw2_console_project(name, dir)        
        project(name)          
        kind "ConsoleApp"                                             
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
            "3rdparty/GLFW",   
            "3rdparty/glew/include",
            "3rdparty"
        }  
        links
        {
            "glfw3.lib",
            "glew-s.lib",
            "opengl32.lib"
        }        
        libdirs
        {
            "lib/x86/vs2005/md/glfw"
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
            --"C:/Program Files (x86)/Microsoft Visual Studio 8/VC/include"
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