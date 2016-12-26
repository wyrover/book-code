includeexternal ("premake5-include.lua")

workspace "sqlite3"
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"    

    configurations { "Debug AES128", "Release AES128", "Debug AES256", "Release AES256" }
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
            "_CRT_NONSTDC_NO_DEPRECATE",
            "STRSAFE_NO_DEPRECATE",
            "_CRT_NON_CONFORMING_SWPRINTFS"
        }
        buildoptions
        {
            "/wd4267",                      -- 关闭 64 位检测
            "/wd4996"
        }          
      
    
     
        -- SQLite3 static library
        project "sqlite3lib"
            language "C++"
            kind "StaticLib"

            files { 
                "3rdparty/sqlite3/secure/src/sqlite3secure.c", 
                "3rdparty/sqlite3/secure/src/*.h" 
            }
            vpaths {
                ["Header Files"] = { "**.h" },
                ["Source Files"] = { "**/sqlite3secure.c", "**.def", "**.rc" }
            }
            flags { "Unicode", "StaticRuntime" }    
            targetname "sqlite3"

            defines {
                "_LIB",
                "THREADSAFE=1",
                "SQLITE_SOUNDEX",
                "SQLITE_ENABLE_COLUMN_METADATA",
                "SQLITE_HAS_CODEC",
                "SQLITE_SECURE_DELETE",
                "SQLITE_ENABLE_FTS3",
                "SQLITE_ENABLE_FTS3_PARENTHESIS",
                "SQLITE_ENABLE_FTS4",
                "SQLITE_ENABLE_FTS5",
                "SQLITE_ENABLE_JSON1",
                "SQLITE_ENABLE_RTREE",
                "SQLITE_CORE",
                "SQLITE_ENABLE_EXTFUNC",
                "SQLITE_ENABLE_CSV",
                "SQLITE_USE_URI",
                "SQLITE_USER_AUTHENTICATION"
            }

            -- Encryption type
            filter { "configurations:*AES128" }
                defines {
                  "CODEC_TYPE=CODEC_TYPE_AES128"
                }
            filter { "configurations:*AES256" }
                defines {
                  "CODEC_TYPE=CODEC_TYPE_AES256"
                }



    -- SQLite3 shared library
    project "sqlite3dll"
        language "C++"
        kind "SharedLib"

        files { 
            "3rdparty/sqlite3/secure/src/sqlite3secure.c", 
            "3rdparty/sqlite3/secure/src/*.h", 
            "3rdparty/sqlite3/secure/src/sqlite3.def", 
            "3rdparty/sqlite3/secure/src/sqlite3.rc" 
        }
        filter {}
        vpaths {
            ["Header Files"] = { "**.h" },
            ["Source Files"] = { "**/sqlite3secure.c", "**.def", "**.rc" }
        }
        flags { "Unicode", "StaticRuntime" }  
        targetname "sqlite3"
        defines 
        {
            "_USRDLL",
            "THREADSAFE=1",
            "SQLITE_SOUNDEX",
            "SQLITE_ENABLE_COLUMN_METADATA",
            "SQLITE_HAS_CODEC",
            "SQLITE_SECURE_DELETE",
            "SQLITE_ENABLE_FTS3",
            "SQLITE_ENABLE_FTS3_PARENTHESIS",
            "SQLITE_ENABLE_FTS4",
            "SQLITE_ENABLE_FTS5",
            "SQLITE_ENABLE_JSON1",
            "SQLITE_ENABLE_RTREE",
            "SQLITE_CORE",
            "SQLITE_ENABLE_EXTFUNC",
            "SQLITE_ENABLE_CSV",
            "SQLITE_USE_URI",
            "SQLITE_USER_AUTHENTICATION"
        }

        -- Encryption type
        filter { "configurations:*AES128" }
            defines {
              "CODEC_TYPE=CODEC_TYPE_AES128"
            }
        filter { "configurations:*AES256" }
            defines {
              "CODEC_TYPE=CODEC_TYPE_AES256"
            }



    -- SQLite3 Shell    
    project "sqlite3shell"
        kind "ConsoleApp"
        language "C++"
        vpaths {
            ["Header Files"] = { "**.h" },
            ["Source Files"] = { "**.c", "**.rc" }
        }
        files 
        { 
            "3rdparty/sqlite3/secure/src/sqlite3.h", 
            "3rdparty/sqlite3/secure/src/shell.c", 
            "3rdparty/sqlite3/secure/src/sqlite3shell.rc" 
        }
        flags { "Unicode", "StaticRuntime" }  
        links { "sqlite3lib" }  

        defines {
            "SQLITE_SHELL_IS_UTF8",
            "SQLITE_HAS_CODEC",
            "SQLITE_USER_AUTHENTICATION"
        }


    group "examples"
        
        project "encrypt-example"            
            kind "ConsoleApp"                         
            flags { "NoManifest", "WinMain", "StaticRuntime" }  
            --defines { "STATIC_GETOPT" }
            defines 
            {
                "SQLITE_SHELL_IS_UTF8",
                "SQLITE_HAS_CODEC",
                "SQLITE_USER_AUTHENTICATION"
            }
            files
            {
               
                "3rdparty/sqlite3/example/**.cpp"                            
            }
            removefiles
            {               
            }
            includedirs
            {                   
                "3rdparty/sqlite3/include"    
            }
            links
            { 
                "sqlite3.lib"
            }
            libdirs
            {
                     
            }



        