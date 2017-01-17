includeexternal ("premake5-include.lua")

workspace "rsa"
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"    
    toolset "v140_xp"
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
        includedirs
        {
            "3rdparty/boost_1_60_0",
        }
        libdirs 
        {
            "lib/x86/%{_ACTION}",            
            "lib/x86/%{_ACTION}/boost-1_60",
            "bin/x86/%{_ACTION}"            
        }
    
    filter { "platforms:x64" }
        system "Windows"
        architecture "x64"   
        includedirs
        {
            "3rdparty/boost_1_60_0",
        }
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



    group "rsa"            

        project "rsa-client-server"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain" }   
            defines { "RSA_CLIENT_SERVER" }
            files
            {
               
                "src/%{wks.name}/%{prj.name}/**.h",
                "src/%{wks.name}/%{prj.name}/**.cpp",
                "3rdparty/jsoncpp/**.cpp",
                "include/buildcfg/vs2015/buildcfg.h",
                "include/buildcfg/vs2015/version.rc",                
                "include/buildcfg/vs2015/versionno.rc2"
            }
            removefiles
            {               
            }
            includedirs
            {          
                "lib/x86/vs2015/curl-7.48/include",
                "3rdparty/cryptopp565",
                "3rdparty/jsoncpp/include",
                "3rdparty",
                "include/buildcfg",
                "include"
            }
            links
            { 
                "cryptlib.lib",
                "libcurl.lib"
            }
            libdirs
            {
                "lib/x86/vs2015/curl-7.48/lib"
            }  

            
        project "rsa_sign_tool"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain" }   
            defines { "RSA_CLIENT_SERVER" }
            files
            {
               
                "src/%{wks.name}/%{prj.name}/**.h",
                "src/%{wks.name}/%{prj.name}/**.cpp",
                "include/buildcfg/vs2015/buildcfg.h",
                "include/buildcfg/vs2015/version.rc",                
                "include/buildcfg/vs2015/versionno.rc2"
            }
            removefiles
            {               
            }
            includedirs
            {          
               
                "3rdparty/cryptopp565",
                "3rdparty",
                "include/buildcfg",
                "include"
            }
            links
            { 
                "cryptlib.lib"
                
            }
            libdirs
            {
                
            }  

        project "rsa_encrypt"            
            kind "SharedLib"                         
            flags { "NoManifest", "StaticRuntime" }                  
            files
            {
                "src/%{wks.name}/rsa_encrypt_dll/**.h",
                "src/%{wks.name}/rsa_encrypt_dll/**.cpp",
                "src/%{wks.name}/rsa_encrypt_dll/**.def",
                
            }             
            removefiles
            {
                 
            }
            includedirs
            {          
                "3rdparty/cryptopp565",
                "3rdparty",
                "include/buildcfg",
                "include"
            }                
            links
            {
                "cryptlib_mt.lib", 
            }
            libdirs
            {
               
            }
        

        project "rsa_encrypt_dll_test"            
            kind "WindowedApp"                         
            language "C#"               
            files
            {
               
                "src/%{wks.name}/%{prj.name}/**.cs",
                "src/%{wks.name}/%{prj.name}/Properties/**",
                "src/%{wks.name}/%{prj.name}/**.config",
              
            }
            removefiles
            {          
                
            
            }
            links {                
                "System",
                "System.Core",
                "System.Data",               
                "System.Deployment",
                "System.Drawing",               
                "System.Windows.Forms"
                
            }



        project "rsa_encrypt_dll_test_console"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain" }   
            defines { "RSA_CLIENT_SERVER" }
            files
            {
               
                "src/%{wks.name}/%{prj.name}/**.h",
                "src/%{wks.name}/%{prj.name}/**.cpp",
                "include/buildcfg/vs2015/buildcfg.h",
                "include/buildcfg/vs2015/version.rc",                
                "include/buildcfg/vs2015/versionno.rc2"
            }
            removefiles
            {               
            }
            includedirs
            {          
               
                "3rdparty/cryptopp565",
                "3rdparty",
                "include/buildcfg",
                "include"
            }
            links
            { 
                "rsa_encrypt.lib"
                
            }
            libdirs
            {
                
            }  

        project "aes_cbc"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain" }   
            defines { "RSA_CLIENT_SERVER" }
            files
            {
               
                "src/%{wks.name}/%{prj.name}/**.h",
                "src/%{wks.name}/%{prj.name}/**.cpp",
                "include/buildcfg/vs2015/buildcfg.h",
                "include/buildcfg/vs2015/version.rc",                
                "include/buildcfg/vs2015/versionno.rc2"
            }
            removefiles
            {               
            }
            includedirs
            {          
               
                "3rdparty/cryptopp565",
                "3rdparty",
                "include/buildcfg",
                "include"
            }
            links
            { 
                "cryptlib.lib"
                
            }
            libdirs
            {
                
            }  

        
        project "install_report"            
            kind "SharedLib"                         
            flags { "NoManifest", "StaticRuntime" }                  
            files
            {
                "src/%{wks.name}/install_report_dll/**.h",
                "src/%{wks.name}/install_report_dll/**.cpp",
                "src/%{wks.name}/install_report_dll/**.def",
                
            }             
            removefiles
            {
                 
            }
            includedirs
            {          
                "3rdparty/cryptopp565",
                "3rdparty",
                "include/buildcfg",
                "include"
            }                
            links
            {
                "cryptlib_mt.lib", 
            }
            libdirs
            {
               
            }