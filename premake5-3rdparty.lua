includeexternal ("premake5-include.lua")

workspace "3rdparty"
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
        implibsuffix  "_impl"
    filter { "kind:SharedLib", "platforms:x64" }
        implibdir "lib/x64/%{_ACTION}" 
        implibsuffix  "_impl"
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

    group "pugixml"

        project "pugixml"                     
            kind "StaticLib"                    
            files
            {
                "3rdparty/pugixml/**.hpp",
                "3rdparty/pugixml/**.cpp"                
            }             

        project "pugixml_mt"              
            kind "StaticLib"          
            flags { "StaticRuntime" }           
            files
            {
                "3rdparty/pugixml/**.hpp",
                "3rdparty/pugixml/**.cpp"                      
            }  

    group "libpng"    

        project "libpng"                     
            kind "StaticLib"                    
            files
            {
                "3rdparty/libpng-libpng16/*.h",
                "3rdparty/libpng-libpng16/*.c",   
                "3rdparty/libpng-libpng16/scripts/pngwin.rc"   
            }             
            removefiles
            {   
                "3rdparty/libpng-libpng16/example.c",

            }
            includedirs
            {
                "3rdparty/zlib"
            }

        project "libpng_mt"              
            kind "StaticLib"          
            flags { "StaticRuntime" }           
            files
            {
                "3rdparty/libpng-libpng16/*.h",
                "3rdparty/libpng-libpng16/*.c",
                "3rdparty/libpng-libpng16/scripts/pngwin.rc"  
            }  
            removefiles
            {   
                "3rdparty/libpng-libpng16/example.c",

            }
            includedirs
            {
                "3rdparty/zlib"
            }

        project "libpng_dll"         
            targetname "libpng"    
            kind "SharedLib"             
            defines { "PNG_BUILD_DLL", "ZLIB_DLL" }
            flags { "NoManifest" }
            files
            {
                "3rdparty/libpng-libpng16/*.h",
                "3rdparty/libpng-libpng16/*.c",
                "3rdparty/libpng-libpng16/scripts/pngwin.rc"  
            }  
            removefiles
            {   
                "3rdparty/libpng-libpng16/example.c",

            }
            includedirs
            {
                "3rdparty/zlib"
            }
            links
            {
                "zlib1_impl.lib"
            }

        project "libpng_dll_mt"  
            targetname "libpng"    
            implibname "libpng_mt"
            kind "SharedLib"                  
            flags { "StaticRuntime", "NoManifest" }
            files
            {
                "3rdparty/libpng-libpng16/*.h",
                "3rdparty/libpng-libpng16/*.c",
                "3rdparty/libpng-libpng16/scripts/pngwin.rc"  
            }  
            removefiles
            {   
                "3rdparty/libpng-libpng16/example.c",

            }
            includedirs
            {
                "3rdparty/zlib"
            }
            links
            {
                "zlib1_mt_impl.lib"
            }

    group "tidylib"    

        project "tidylib"                     
            kind "StaticLib"                    
            files
            {
                "3rdparty/tidylib/**.h",
                "3rdparty/tidylib/**.c",   
                 
            }             
            removefiles
            {   
                --"3rdparty/libpng-libpng16/example.c",

            }
            includedirs
            {
                "3rdparty/tidylib/include"
            }
            

        project "tidylib_mt"              
            kind "StaticLib"          
            flags { "StaticRuntime" }           
            files
            {
                "3rdparty/tidylib/**.h",
                "3rdparty/tidylib/**.c", 
            }  
            removefiles
            {   
                --"3rdparty/libpng-libpng16/example.c",

            }
            includedirs
            {
                "3rdparty/tidylib/include"
            }
            
            
    group "tracetool"

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

    group "getopt"

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


    group "jsoncpp"

        project "jsoncpp"            
            kind "StaticLib"                     
            files
            {
                "3rdparty/jsoncpp/include/**.h",
                "3rdparty/jsoncpp/**.cpp"      
                               
            } 
            includedirs
            {          
                "3rdparty/jsoncpp/include"
            }   
            
        project "jsoncpp_mt"            
            kind "StaticLib"      
           
            flags { "StaticRuntime" }
            files
            {
                "3rdparty/jsoncpp/include/**.h",
                "3rdparty/jsoncpp/**.cpp"      
                               
            } 
            includedirs
            {          
                "3rdparty/jsoncpp/include"
            }

    group "bzip2"

        project "bzip2"            
            kind "StaticLib"        
            defines { "BZ_NO_STDIO2" }                       
            files
            {
                "3rdparty/bzip2/*.h",
                "3rdparty/bzip2/*.c"                              
            } 
            removefiles
            {   
                "3rdparty/bzip2/bzip2recover.c",
                "3rdparty/bzip2/dlltest.c",
                "3rdparty/bzip2/mk251.c",
                "3rdparty/bzip2/spewG.c",
                "3rdparty/bzip2/unzcrash.c"
            }
            
        project "bzip2_mt"            
            kind "StaticLib"     
            defines { "BZ_NO_STDIO2" }
            flags { "StaticRuntime" }
            files
            {
                "3rdparty/bzip2/*.h",
                "3rdparty/bzip2/*.c"      
                               
            }
            removefiles
            {   
                "3rdparty/bzip2/bzip2recover.c",
                "3rdparty/bzip2/dlltest.c",
                "3rdparty/bzip2/mk251.c",
                "3rdparty/bzip2/spewG.c",
                "3rdparty/bzip2/unzcrash.c"
            }

    group "zlib"

        project "zlib"            
            kind "StaticLib"        
                       
            files
            {
                "3rdparty/zlib/*.h",
                "3rdparty/zlib/*.c"      
                               
            } 
            
        project "zlib_mt"            
            kind "StaticLib"     
            
            flags { "StaticRuntime" }
            files
            {
                "3rdparty/zlib/*.h",
                "3rdparty/zlib/*.c"      
                               
            }

        project "zlib_dll"         
            targetname "zlib1"    
            kind "SharedLib"                  
            flags { "NoManifest" }
            files
            {
                "3rdparty/zlib/*.h",
                "3rdparty/zlib/*.c",
                "3rdparty/zlib/win32/*.def",
                "3rdparty/zlib/win32/*.rc"
                               
            }

        project "zlib_dll_mt"  
            targetname "zlib1"    
            implibname "zlib1_mt"
            kind "SharedLib"                  
            flags { "StaticRuntime", "NoManifest" }
            files
            {
                "3rdparty/zlib/*.h",
                "3rdparty/zlib/*.c",
                "3rdparty/zlib/win32/*.def",
                "3rdparty/zlib/win32/*.rc"                               
            }

    group "unzip"

        project "unzip"            
            kind "StaticLib"        
                       
            files
            {
                "3rdparty/unzip101e/*.h",
                "3rdparty/unzip101e/*.c"      
                               
            } 
            includedirs
            {
                "3rdparty/zlib"
            }
            
            
        project "unzip_mt"            
            kind "StaticLib"     
            
            flags { "StaticRuntime" }
            files
            {
                "3rdparty/unzip101e/*.h",
                "3rdparty/unzip101e/*.c"      
                               
            }
            includedirs
            {
                "3rdparty/zlib"
            }
            


    group "unrar"

        project "unrar"            
            kind "StaticLib"        
            defines { "UNRAR" }
            files
            {
                "3rdparty/unrar/rar.hpp",
                "3rdparty/unrar/**.cpp"      
                               
            } 
            removefiles
            {
                "3rdparty/unrar/arccmt.cpp",
                "3rdparty/unrar/beosea.cpp",
                "3rdparty/unrar/coder.cpp",
                "3rdparty/unrar/dll.cpp",
                "3rdparty/unrar/log.cpp",
                "3rdparty/unrar/model.cpp",
                "3rdparty/unrar/os2ea.cpp",
                "3rdparty/unrar/rarvmtbl.cpp",
                "3rdparty/unrar/suballoc.cpp",
                "3rdparty/unrar/unios2.cpp",
                "3rdparty/unrar/unpack15.cpp",
                "3rdparty/unrar/unpack20.cpp",
                "3rdparty/unrar/uowners.cpp",
                "3rdparty/unrar/win32acl.cpp",
                "3rdparty/unrar/win32stm.cpp",

            }     
            includedirs
            {          
                "3rdparty/unrar"
            }   

        project "unrar_mt"            
            kind "StaticLib"        
            defines { "UNRAR" }
            flags { "StaticRuntime" }
            files
            {
                "3rdparty/unrar/rar.hpp",
                "3rdparty/unrar/**.cpp"      
                               
            } 
            removefiles
            {
                "3rdparty/unrar/arccmt.cpp",
                "3rdparty/unrar/beosea.cpp",
                "3rdparty/unrar/coder.cpp",
                "3rdparty/unrar/dll.cpp",
                "3rdparty/unrar/log.cpp",
                "3rdparty/unrar/model.cpp",
                "3rdparty/unrar/os2ea.cpp",
                "3rdparty/unrar/rarvmtbl.cpp",
                "3rdparty/unrar/suballoc.cpp",
                "3rdparty/unrar/unios2.cpp",
                "3rdparty/unrar/unpack15.cpp",
                "3rdparty/unrar/unpack20.cpp",
                "3rdparty/unrar/uowners.cpp",
                "3rdparty/unrar/win32acl.cpp",
                "3rdparty/unrar/win32stm.cpp",

            }     
            includedirs
            {          
                "3rdparty/unrar"
            }   

        project "unrar_dll"         
            targetname "unrar"                
            kind "SharedLib"        
            defines { "RARDLL", "UNRAR", "SILENT" }   
            flags { "NoManifest" }
            files
            {
                "3rdparty/unrar/rar.hpp",
                "3rdparty/unrar/**.cpp",
                "3rdparty/unrar/**.rc"  
                               
            } 
            removefiles
            {
                "3rdparty/unrar/arccmt.cpp",
                "3rdparty/unrar/beosea.cpp",
                "3rdparty/unrar/coder.cpp",                
                "3rdparty/unrar/log.cpp",
                "3rdparty/unrar/model.cpp",
                "3rdparty/unrar/os2ea.cpp",
                "3rdparty/unrar/rarvmtbl.cpp",
                "3rdparty/unrar/suballoc.cpp",
                "3rdparty/unrar/unios2.cpp",
                "3rdparty/unrar/unpack15.cpp",
                "3rdparty/unrar/unpack20.cpp",
                "3rdparty/unrar/uowners.cpp",
                "3rdparty/unrar/win32acl.cpp",
                "3rdparty/unrar/win32stm.cpp",

            }     
            includedirs
            {          
                "3rdparty/unrar"
            }   
            
        project "unrar_dll_mt"     
            targetname "unrar"    
            implibname "unrar_mt"
            kind "SharedLib"        
            defines { "RARDLL", "UNRAR", "SILENT" }
            flags { "StaticRuntime", "NoManifest" }
            files
            {
                "3rdparty/unrar/rar.hpp",
                "3rdparty/unrar/**.cpp",
                "3rdparty/unrar/**.rc"  
                               
            } 
            removefiles
            {
                "3rdparty/unrar/arccmt.cpp",
                "3rdparty/unrar/beosea.cpp",
                "3rdparty/unrar/coder.cpp",                
                "3rdparty/unrar/log.cpp",
                "3rdparty/unrar/model.cpp",
                "3rdparty/unrar/os2ea.cpp",
                "3rdparty/unrar/rarvmtbl.cpp",
                "3rdparty/unrar/suballoc.cpp",
                "3rdparty/unrar/unios2.cpp",
                "3rdparty/unrar/unpack15.cpp",
                "3rdparty/unrar/unpack20.cpp",
                "3rdparty/unrar/uowners.cpp",
                "3rdparty/unrar/win32acl.cpp",
                "3rdparty/unrar/win32stm.cpp",

            }     
            includedirs
            {          
                "3rdparty/unrar"
            } 
            
    group "libiconv"

        project "libiconv"            
            kind "StaticLib"        
                       
            files
            {
                "3rdparty/libiconv/**.h",
                "3rdparty/libiconv/**.c",
                "3rdparty/libiconv/**.rc"
                               
            } 
            
        project "libiconv_mt"            
            kind "StaticLib"     
            
            flags { "StaticRuntime" }
            files
            {
                "3rdparty/libiconv/**.h",
                "3rdparty/libiconv/**.c",
                "3rdparty/libiconv/**.rc"
                               
            }

        project "libiconv_dll"    
            targetname "libiconv" 
            kind "SharedLib"           
            defines { "_USRDLL", "LIBICONV_EXPORTS", "BUILDING_LIBICONV", "BUILDING_LIBCHARSET" }
            flags { "NoManifest" }
            files
            {
                "3rdparty/libiconv/**.h",
                "3rdparty/libiconv/**.c",
                "3rdparty/libiconv/**.rc"
                
                               
            }

        project "libiconv_dll_mt"  
            targetname "libiconv"    
            implibname "libiconv_mt"
            kind "SharedLib"            
            defines { "_USRDLL", "LIBICONV_EXPORTS", "BUILDING_LIBICONV", "BUILDING_LIBCHARSET" }
            flags { "StaticRuntime", "NoManifest" }
            files
            {
                "3rdparty/libiconv/**.h",
                "3rdparty/libiconv/**.c",
                "3rdparty/libiconv/**.rc"                           
            }

    group "libxml2"

        project "libxml2"            
            kind "StaticLib"        
                       
            files
            {
                "3rdparty/libxml2/**.h",
                "3rdparty/libxml2/**.c",
                "3rdparty/libxml2/**.rc"
                               
            } 
            removefiles
            {
                "3rdparty/libxml2/run*.c",
                "3rdparty/libxml2/test*.c",
                "3rdparty/libxml2/trio.c",
                "3rdparty/libxml2/trionan.c",
                "3rdparty/libxml2/triostr.c",
                "3rdparty/libxml2/xmlcatalog.c",
                "3rdparty/libxml2/xmllint.c",
                "3rdparty/libxml2/xzlib.c",
                
            }  
            includedirs
            {
                "3rdparty/libxml2/include",
                "3rdparty/libiconv/include"
            }
            
        project "libxml2_mt"            
            kind "StaticLib"     
            
            flags { "StaticRuntime" }
            files
            {
                "3rdparty/libxml2/**.h",
                "3rdparty/libxml2/**.c",
                "3rdparty/libxml2/**.rc"
                               
            }
            removefiles
            {
                "3rdparty/libxml2/run*.c",
                "3rdparty/libxml2/test*.c",
                "3rdparty/libxml2/trio.c",
                "3rdparty/libxml2/trionan.c",
                "3rdparty/libxml2/triostr.c",
                "3rdparty/libxml2/xmlcatalog.c",
                "3rdparty/libxml2/xmllint.c",
                "3rdparty/libxml2/xzlib.c",
                
            }  
            includedirs
            {
                "3rdparty/libxml2/include",
                "3rdparty/libiconv/include"
            }

    group "libmobi"

        project "libmobi"            
            kind "StaticLib"        
                       
            files
            {
                "3rdparty/libmobi/src/*.h",
                "3rdparty/libmobi/src/*.c"      
                               
            } 
            removefiles
            {
                "3rdparty/libmobi/src/miniz.c"
            }
            includedirs
            {
                "3rdparty/include",
                "3rdparty/zlib",
                "3rdparty/libiconv/include",
                "3rdparty/libxml2/include",
                "3rdparty/unzip101e",
                "3rdparty/tidylib/include"
            }
            
            
        project "libmobi_mt"            
            kind "StaticLib"     
            
            flags { "StaticRuntime" }
            files
            {
                "3rdparty/libmobi/src/*.h",
                "3rdparty/libmobi/src/*.c"      
                               
            }
            removefiles
            {
                "3rdparty/libmobi/src/miniz.c"
            }
            includedirs
            {
                "3rdparty/include",
                "3rdparty/zlib",
                "3rdparty/libiconv/include",
                "3rdparty/libxml2/include",
                "3rdparty/unzip101e",
                "3rdparty/tidylib/include"
            }

    group "libcnary"

        project "libcnary"            
            kind "StaticLib"        
                       
            files
            {
                "3rdparty/libcnary/**.h",
                "3rdparty/libcnary/**.c"      
                               
            }             
            includedirs
            {
                "3rdparty/libcnary/include"
                
            }
            
            
        project "libcnary_mt"            
            kind "StaticLib"     
            
            flags { "StaticRuntime" }
            files
            {
                "3rdparty/libcnary/**.h",
                "3rdparty/libcnary/**.c"      
                               
            }             
            includedirs
            {
                "3rdparty/libcnary/include"
                
            }
            

            
            

    
    group "lua"

        project "liblua"            
            kind "StaticLib"      
            --defines { "STATIC_GETOPT" }
            --flags { "StaticRuntime" }
            files
            {
                "3rdparty/lua-5.3.3/**.h",
                "3rdparty/lua-5.3.3/**.c"                               
            } 
            removefiles
            {
                "3rdparty/lua-5.3.3/luac.c",
                "3rdparty/lua-5.3.3/lua.c"
            }  

        project "liblua_mt"            
            kind "StaticLib"      
            --defines { "STATIC_GETOPT" }
            flags { "StaticRuntime" }
            files
            {
                "3rdparty/lua-5.3.3/**.h",
                "3rdparty/lua-5.3.3/**.c"                               
            } 
            removefiles
            {
                "3rdparty/lua-5.3.3/luac.c",
                "3rdparty/lua-5.3.3/lua.c"
            }  
            
        project "lua"            
            kind "ConsoleApp"                           
            flags { "NoManifest", "WinMain", "StaticRuntime" }    
            files
            {
                "3rdparty/lua-5.3.3/**.h",
                "3rdparty/lua-5.3.3/**.c"                               
            } 
            removefiles
            {
                "3rdparty/lua-5.3.3/luac.c"                
            }  

        project "luac"            
            kind "ConsoleApp"                  
            flags { "NoManifest", "WinMain", "StaticRuntime" } 
            files
            {
                "3rdparty/lua-5.3.3/**.h",
                "3rdparty/lua-5.3.3/**.c"                               
            } 
            removefiles
            {
                "3rdparty/lua-5.3.3/lua.c"                
            }  
             
    group "lua-bridge-sample"
         
        project "test01"       
            targetdir "bin/x86/%{_ACTION}/lua-bridge-sample"
            kind "ConsoleApp"                  
            flags { "NoManifest", "WinMain", "StaticRuntime" } 
            files
            {
                "lua-bridge-sample/%{prj.name}/**.cpp",
                
            } 
            removefiles
            {
                
    
            }     
            includedirs
            {          
                
                "3rdparty/LuaBridge",
                "3rdparty"
            }                
            links
            {
                "liblua_mt.lib"              
            }

        project "test02"       
            targetdir "bin/x86/%{_ACTION}/lua-bridge-sample"
            kind "ConsoleApp"                  
            flags { "NoManifest", "WinMain", "StaticRuntime" } 
            files
            {
                "lua-bridge-sample/%{prj.name}/**.cpp",
                
            } 
            removefiles
            {
                
    
            }     
            includedirs
            {          
                
                "3rdparty/LuaBridge",
                "3rdparty",
                "lua-bridge-sample/shell"
            }                
            links
            {
                "liblua_mt.lib",
                "shell.lib"
            }

        project "test03"       
            targetdir "bin/x86/%{_ACTION}/lua-bridge-sample"
            kind "ConsoleApp"                  
            flags { "NoManifest", "WinMain", "StaticRuntime" } 
            defines { "PSAPI_VERSION=1", "DISABLE_INVALID_NUMBERS", "MISSING_ISINF", "ENABLE_CJSON_GLOBAL" }
            files
            {
                "lua-bridge-sample/%{prj.name}/**.cpp",
                "lua-bridge-sample/%{prj.name}/**.h",
                "lua-bridge-sample/%{prj.name}/**.c"
                
            } 
            removefiles
            {
                
    
            }     
            includedirs
            {          
                
                "3rdparty/LuaBridge",
                "3rdparty/lua-5.3.3",
                "3rdparty"             
            }                
            links
            {
                "kernel32.lib",
                "user32.lib",
                "psapi.lib",
                "advapi32.lib",
                "shell32.lib",
                "Mpr.lib",
                "liblua_mt.lib"
                
            }
            libdirs
            {
                "lib/x86/%{_ACTION}/boost-1_56"
            }


        project "lua-iconv"       
            targetdir "bin/x86/%{_ACTION}/lua-bridge-sample"
            kind "SharedLib"                  
            flags { "StaticRuntime" } 
            files
            {
                "lua-bridge-sample/%{prj.name}/**.c",
                "lua-bridge-sample/%{prj.name}/**.h"
                
            } 
            removefiles
            {
                
    
            }     
            includedirs
            {               
                
                "3rdparty/lua-5.3.3",
                "lua-bridge-sample/lua-iconv"
            }                
            links
            {
                "liblua_mt.lib"              
            }
        
        project "shell"       
            targetdir "bin/x86/%{_ACTION}/lua-bridge-sample"
            kind "SharedLib"                  
            flags { "StaticRuntime", "NoManifest" } 
            files
            {
                "lua-bridge-sample/%{prj.name}/**.cpp",
                "lua-bridge-sample/%{prj.name}/**.h"
                
            } 
            removefiles
            {
                
    
            }     
            includedirs
            {               
                
                "3rdparty/lua-5.3.3",
                --"lua-bridge-sample/lua-iconv"
            }                
            links
            {
                "liblua_mt.lib",
                "Shlwapi.lib"
            }
   
        
        project "winreg"       
            targetdir "bin/x86/%{_ACTION}/lua-bridge-sample"
            kind "SharedLib"                  
            flags { "StaticRuntime", "NoManifest" } 
            files
            {
                "lua-bridge-sample/%{prj.name}/**.c",
                "lua-bridge-sample/%{prj.name}/**.h"
                
            } 
            removefiles
            {
                
    
            }     
            includedirs
            {               
                
                "3rdparty/lua-5.3.3",
                --"lua-bridge-sample/lua-iconv"
            }                
            links
            {
                "liblua_mt.lib",
                --"Shlwapi.lib"
            }
        






        