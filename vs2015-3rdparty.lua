includeexternal ("premake-vs-include.lua")

workspace "tools"
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"    
    toolset "v140_xp"    
        
    print("test")

    group "tools"                          
        
        project "file-2-hex"            
            kind "ConsoleApp"                                     
            defines { "STATIC_GETOPT", "FILE_2_HEX_EXE" }
            files
            {
                "3rdparty/getopt/**.h",
                "3rdparty/getopt/**.c",                
                "src/%{wks.name}/%{prj.name}/**.h",
                "src/%{wks.name}/%{prj.name}/**.cpp",                
            }
            removefiles
            {               
            }
            includedirs
            {                   
                "3rdparty/getopt"               
            }
            links
            { 
                
            }
            libdirs
            {
                     
            }

    group "api_hook"

        project "injector"            
            kind "ConsoleApp" 
            characterset "MBCS"
            files
            {
                        
                "src/api_hook/injector/**.h",
                "src/api_hook/injector/**.cpp",                
            }            
            pchsource "src/api_hook/injector/stdafx.cpp"
            pchheader "stdafx.h"            
       
        project "iat_hook_dll"            
            kind "SharedLib" 
            characterset "MBCS"
            defines { "IAT_HOOK_DLL_EXPORTS" }
            files
            {
                "src/api_hook/share/**.h",   
                "src/api_hook/share/**.cpp",   
                "src/api_hook/iat_hook_dll/**.h",
                "src/api_hook/iat_hook_dll/**.cpp", 
                "src/api_hook/iat_hook_dll/**.def"
            }   
            includedirs
            {                   
                "src/api_hook/share"
            }
            pchsource "src/api_hook/iat_hook_dll/stdafx.cpp"
            pchheader "stdafx.h"  


        project "iat_hook_target"            
            kind "WindowedApp"
            characterset "MBCS"
           
            files
            {
               
                "src/api_hook/iat_hook_target/**.h",
                "src/api_hook/iat_hook_target/**.cpp",    
                "src/api_hook/iat_hook_target/**.rc", 
                "src/api_hook/iat_hook_target/**.rc2", 
            }   
            includedirs
            {                   
               
            }
            pchsource "src/api_hook/iat_hook_target/stdafx.cpp"
            pchheader "stdafx.h" 
            
        
        project "eat_hook_dll"            
            kind "SharedLib" 
            characterset "MBCS"
            defines { "EAT_HOOK_DLL_EXPORTS" }
            files
            {
                "src/api_hook/share/**.h",   
                "src/api_hook/share/**.cpp",   
                "src/api_hook/eat_hook_dll/**.h",
                "src/api_hook/eat_hook_dll/**.cpp",   
                "src/api_hook/eat_hook_dll/**.def"
            }   
            includedirs
            {                   
                "src/api_hook/share"
            }
            pchsource "src/api_hook/eat_hook_dll/stdafx.cpp"
            pchheader "stdafx.h"  

        project "HookDll"            
            kind "SharedLib" 
            --characterset "MBCS"
            defines { "HOOKDLL_EXPORTS" }
            files
            {
                   
                "src/api_hook/HookDll/**.h",
                "src/api_hook/HookDll/**.cpp",   
                "src/api_hook/HookDll/**.def"
            }   
            includedirs
            {                   
                
            }
            removefiles
            {      
                "src/api_hook/HookDll/dectours/uimports.cpp"
            }            
            pchsource "src/api_hook/HookDll/stdafx.cpp"
            pchheader "stdafx.h"  
            filter "files:src/api_hook/HookDll/dectours/**.cpp"
                flags { "NoPCH" }

        project "HookApiMainMFC"            
            kind "WindowedApp"
            flags { "MFC" }           
            files
            {
               
                "src/api_hook/HookApiMainMFC/**.h",
                "src/api_hook/HookApiMainMFC/**.cpp",    
                "src/api_hook/HookApiMainMFC/**.rc", 
                "src/api_hook/HookApiMainMFC/**.rc2", 
            }   
            includedirs
            {                   
               
            }
            pchsource "src/api_hook/HookApiMainMFC/stdafx.cpp"
            pchheader "stdafx.h" 

                


        project "pcid"            
            kind "SharedLib" 
            
            defines {  }
            files
            {
                "src/pcid/**.h",   
                "src/pcid/**.cpp",                
                "src/pcid/**.def"
            }   
            includedirs
            {                   
                "src/api_hook/share"
            }
            pchsource "src/pcid/stdafx.cpp"
            pchheader "stdafx.h"  
        

    group "Plot"

        project "plot"            
            kind "SharedLib" 
            characterset "MBCS"
            defines { "_AFXEXT" }
            flags { "MFC" }
            files
            {
                "src/clPlot/plot/**.h",   
                "src/clPlot/plot/**.cpp",   
                "src/clPlot/plot/**.rc",
                "src/clPlot/plot/**.rc2",
                "src/clPlot/plot/**.def"
            }   
            includedirs
            {                   
                
            }
            pchsource "src/clPlot/plot/StdAfx.cpp"
            pchheader "StdAfx.h"  

        project "demo"            
            kind "WindowedApp" 
            characterset "MBCS"
            defines {  }
            flags { "MFC" }
            files
            {
                "src/clPlot/demo/**.h",   
                "src/clPlot/demo/**.cpp",   
                "src/clPlot/demo/**.rc",
                "src/clPlot/demo/**.rc2",
              
            }   
            includedirs
            {                   
                
            }
            links
            {
                "plot.lib"
            }
            pchsource "src/clPlot/demo/StdAfx.cpp"
            pchheader "StdAfx.h"  