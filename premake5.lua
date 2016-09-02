workspace "smb"
    language "C++"
    location "build/%{_ACTION}"
    targetdir "bin"

    configurations { "Debug", "Release", "TRACE", "TRACE_MT" }
    platforms { "Win32", "Win64" }

    filter { "platforms:Win32" }
        system "Windows"
        architecture "x32"
    
    filter { "platforms:Win64" }
        system "Windows"
        architecture "x64"    

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
        links
        {
            "tracetool.lib"
        }
        libdirs
        {
            "product/win32"
        }

    filter "configurations:TRACE_MT"
        defines { "NDEBUG", "TRACE_TOOL" }
        flags { "Symbols" }
        optimize "On"  
        buildoptions { "/Od" }  
        links
        {
            "tracetool_mt.lib"
        }
        libdirs
        {
            "product/win32"
        }

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
   
    

        project "execserver"
            location "build/%{_ACTION}"
            kind "ConsoleApp"        
            characterset "MBCS"
            defines { }               
            --buildoptions { "/EHsc", "/Zm200" }
            flags { "NoManifest", "StaticRuntime" }
            targetdir "bin"   
            files
            {
                "execserver/**.h",
                "execserver/**.cpp",
                "smbexec/connection.h",
                "smbexec/connection.cpp", 
                "smbexec/utilities.h",
                "smbexec/utilities.cpp"
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
                "lib"
            }


        project "smbexec"
            location "build/%{_ACTION}"
            kind "ConsoleApp"        
            characterset "MBCS"
            defines { }               
            --buildoptions { "/EHsc", "/Zm200" }
            flags { "NoManifest", "StaticRuntime" }
            targetdir "bin"   
            files
            {
                "smbexec/**.h",
                "smbexec/**.cpp"
               
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
                "lib"
            }
        




        