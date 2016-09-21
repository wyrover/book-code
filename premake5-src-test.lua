workspace "test"
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
            "lib/x86/%{_ACTION}/boost-1_56",
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
   
    group "09 底层"

        project "cactus"
            
            kind "SharedLib"            
            defines { "_WINDOWS", "_USRDLL", "DLL_EXPORTS", "CACTUS_DLL", "CACTUS_EXPORTS" }               
            buildoptions { "/EHsc", "/Zm200" }
            flags { "NoManifest" }
            targetdir "bin/x86"   
            files
            {
                "src/cactus/**.h",
                "src/cactus/**.cpp",  
                "src/cactus/**.c",
                "include/buildcfg/version.rc",
                "include/buildcfg/versionno.rc2",                
                "include/buildcfg/buildcfg.h"
            }             
            removefiles
            {
                "src/cactus/7zHelper.cpp",
                "src/cactus/cactus_7z.cpp",
                "src/cactus/cactus_cmdline.cpp",
                "src/cactus/cactus_szlib.cpp",
                "src/cactus/cactus_xunlei.cpp",
                "src/cactus/log_impl.cpp",
                "src/cactus/PropVariant.cpp"
            }
            includedirs
            {            
                "3rdparty"    
            }                
            links
            {
                              
            }
            libdirs
            {
                "lib/win32"
            }

        project "libQREncode"            
            kind "StaticLib"            
            defines { "_WINDOWS", "HAVE_CONFIG_H" }                                    
            files
            {
                "3rdparty/libQREncode/**.h",
                "3rdparty/libQREncode/**.cpp",  
                "3rdparty/libQREncode/**.c"
            }             
            removefiles
            {
                "3rdparty/libQREncode/qrenc.c",
                "3rdparty/libQREncode/libQREncode.h",
                "3rdparty/libQREncode/libQREncode.cpp"
            }
            
             

        project "7zpp"            
            kind "StaticLib"            
            defines { "_LIB" }              
            targetname "7zpp_u"           
            files
            {
                "3rdparty/7zpp/**.h",
                "3rdparty/7zpp/**.cpp"
                
            }             
            removefiles
            {
                "3rdparty/7zpp/Simple/**"                
            }
            includedirs
            {            
                "3rdparty/7z",
                "3rdparty/7z/CPP",
                "3rdparty"
            }                
            links
            {
                              
            }
            libdirs
            {
                "lib/win32"
            }
            pchsource "3rdparty/7zpp/stdafx.cpp"
            pchheader "stdafx.h"            
            filter "files:3rdparty/7zpp/cactus_7zip.cpp"
                flags { "NoPCH" }
        


        project "duilib"
            
            kind "SharedLib"            
            defines { "_WINDOWS", "UILIB_EXPORTS", "DUILIB_DLL" }               
            flags { "NoManifest" }                          
            files
            {
                "3rdparty/DuiLib/**.h",
                "3rdparty/DuiLib/**.cpp",  
                "3rdparty/DuiLib/**.c",
                "include/buildcfg/version.rc",
                "include/buildcfg/versionno.rc2",                
                "include/buildcfg/buildcfg.h"
            }            
            removefiles
            {
                 
            }           
            includedirs
            {          
                "3rdparty/DuiLib",
                "3rdparty"    
            }            
            links
            {
                "cactus.lib"              
            }
            libdirs
            {
                "bin/x86",
                "lib/win32"
            }
            pchsource "3rdparty/DuiLib/StdAfx.cpp"
            pchheader "StdAfx.h"
            filter "files:**.c"
                flags { "NoPCH" }
            filter "files:3rdparty/DuiLib/Utils/XUnzip.cpp"
                flags { "NoPCH" }

        project "tinyxml"            
            kind "SharedLib"            
            defines { "_WINDOWS", "_DLL", "TINY_EXPORTS", "TINYXML_DLL" }      
            flags { "NoManifest" }                        
            files
            {
                "3rdparty/tinyxml/**.h",
                "3rdparty/tinyxml/**.cpp",
                "include/buildcfg/version.rc",
                "include/buildcfg/versionno.rc2",                
                "include/buildcfg/buildcfg.h"
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

        project "tracetool"
            removeconfigurations "TRACE*"            
            kind "StaticLib"                    
            files
            {
                "3rdparty/tracetool/**.h",
                "3rdparty/tracetool/**.cpp"                
            }             

        project "tracetool_mt"
            removeconfigurations "TRACE*"            
            kind "StaticLib"          
            flags { "StaticRuntime" }                 
            targetname "tracetool_mt"
            files
            {
                "3rdparty/tracetool/**.h",
                "3rdparty/tracetool/**.cpp"                
            }           


        project "getopt"            
            kind "StaticLib"         
            defines { "STATIC_GETOPT" }            
            files
            {
                "3rdparty/getopt/**.h",
                "3rdparty/getopt/**.c"      
                               
            } 
            
        project "getopt_mt"            
            kind "StaticLib"      
            defines { "STATIC_GETOPT" }
            flags { "StaticRuntime" }
            files
            {
                "3rdparty/getopt/**.h",
                "3rdparty/getopt/**.c"      
                               
            } 
            

             
            
    
    group "05 中间层"      

        

    group "00 UI 层"        
        


    group "02 test"          
        
        project "api-test"                    
            kind "ConsoleApp"                         
            flags { "WinMain", "NoManifest",  "StaticRuntime" }
            --prebuildcommands { "../../format_code.bat src/test/api-test" }
            files
            {
                "src/test/api-test/**.h",
                "src/test/api-test/**.c",
                "src/test/api-test/**.cpp"                            
            }
            removefiles
            {               
            }
            includedirs
            {   
                "3rdparty/catch",
                "3rdparty",
                
            }
            links
            { 
                "Shlwapi.lib",
                "wbemuuid.lib",
                "PowrProf.lib",
                "version.lib",
                "wintrust.lib",
                "crypt32.lib",
                "mstask.lib",
                "Winhttp.lib",
                "ws2_32.lib",
                "Dnsapi.lib"
            }                       

        project "boost-write-sharedmemery-test"            
            targetdir "bin/x86"
            kind "ConsoleApp"                                       
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }
            files
            {                                  
                "src/test/boost-write-sharedmemery-test/**.h",
                "src/test/boost-write-sharedmemery-test/**.cpp"                            
            }
            removefiles
            {               
            }
            includedirs
            {                   
                "3rdparty"   
            }               
                

        project "boost-read-sharedmemery-test"          
            kind "ConsoleApp"                                       
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }
            files
            {                                  
                "src/test/boost-read-sharedmemery-test/**.h",
                "src/test/boost-read-sharedmemery-test/**.cpp"                            
            }
            removefiles
            {               
            }
            includedirs
            {                   
                "3rdparty"   
            }           
            
        project "test-semaphore"          
            kind "WindowedApp"                            
            characterset "MBCS"
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files
            {                                  
                "src/test/%{prj.name}/**.h",
                "src/test/%{prj.name}/**.cpp", 
                "src/test/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                "3rdparty"   
            }      
            
        project "wtl_dialog_template"          
            kind "WindowedApp"          
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files
            {                                  
                "src/test/%{prj.name}/**.h",
                "src/test/%{prj.name}/**.cpp", 
                "src/test/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                "3rdparty/wtl"   
            }  

        project "dectet-process"          
            kind "WindowedApp"          
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }            
            files
            {                                  
                "src/test/%{prj.name}/**.h",
                "src/test/%{prj.name}/**.cpp", 
                "src/test/%{prj.name}/**.rc" 
            }
            removefiles
            {               
            }
            includedirs
            {                   
                "3rdparty/wtl"   
            }              
        
        project "clean-driverstore"          
            kind "ConsoleApp"                                       
            flags { "NoManifest", "WinMain", "StaticRuntime" }       
            defines {  }
            files
            {                                  
                "src/%{prj.name}/**.h",
                "src/%{prj.name}/**.cpp", 
                "src/%{prj.name}/**.rc"                           
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
                "Setupapi.lib"
            }
            
         
        project "vc-ltl-test"          
            kind "ConsoleApp"                                       
            flags { "NoManifest", "WinMain" }       
            defines {  }
            files
            {                                  
                "src/%{prj.name}/**.h",
                "src/%{prj.name}/**.cpp", 
                "src/%{prj.name}/**.rc"                           
            }
            removefiles
            {               
            }
            includedirs
            {                   
                "3rdparty"   
            }   
           
        

        






        