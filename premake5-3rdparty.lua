includeexternal ("premake-vs-include.lua")


workspace "3rdparty"
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"    

    
   
    group "09 底层"

        project "cactus"
            
            kind "SharedLib"            
            defines { "_WINDOWS", "_USRDLL", "DLL_EXPORTS", "CACTUS_DLL", "CACTUS_EXPORTS" }               
            buildoptions { "/EHsc", "/Zm200" }           
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

        project "cryptopp"            
            kind "StaticLib"            
            characterset "MBCS"
            defines { "_WINDOWS", "USE_PRECOMPILED_HEADERS", "WIN32" }                                    
            
            filter { "platforms:Win32" }
               -- defines { "WIN32", "_WINDOWS" }                 
                files
                {
                    "3rdparty/cryptopp565/**.h",
                    "3rdparty/cryptopp565/**.c",
                    "3rdparty/cryptopp565/**.cpp",
                    "3rdparty/cryptopp565/**.asm"                    
                }   
                removefiles
                {
                    "3rdparty/cryptopp565/test.cpp",
                    "3rdparty/cryptopp565/fipstest.cpp",
                    "3rdparty/cryptopp565/fipsalgt.cpp",
                    "3rdparty/cryptopp565/regtest.cpp",
                    "3rdparty/cryptopp565/bench*.cpp",
                    "3rdparty/cryptopp565/eccrypto.cpp",
                    "3rdparty/cryptopp565/eprecomp.cpp",                    
                    "3rdparty/cryptopp565/dlltest.cpp",
                    "3rdparty/cryptopp565/datatest.cpp",
                    "3rdparty/cryptopp565/validat*.cpp",
                    "3rdparty/cryptopp565/x64dll.asm",
                    "3rdparty/cryptopp565/x64masm.asm" 
                }
            filter "platforms:x64"
               -- defines { "WIN32", "_WINDOWS", "_USRDLL", "OLSDLL_EXPORTS", "OLS_WIN_RING0" }
                files
                {
                    "3rdparty/cryptopp565/**.h",
                    "3rdparty/cryptopp565/**.c",
                    "3rdparty/cryptopp565/**.cpp",
                    "3rdparty/cryptopp565/**.asm"                   
                }
                removefiles
                {
                    "3rdparty/cryptopp565/test.cpp",
                    "3rdparty/cryptopp565/fipstest.cpp",
                    "3rdparty/cryptopp565/fipsalgt.cpp",
                    "3rdparty/cryptopp565/regtest.cpp",
                    "3rdparty/cryptopp565/bench*.cpp",
                    "3rdparty/cryptopp565/eccrypto.cpp",
                    "3rdparty/cryptopp565/eprecomp.cpp",                    
                    "3rdparty/cryptopp565/dlltest.cpp",
                    "3rdparty/cryptopp565/datatest.cpp",
                    "3rdparty/cryptopp565/validat*.cpp"
                    
                }
                          
            filter { "files:**.asm", "platforms:Win32" }            
                buildmessage "Compiling %{file.relpath}"
                buildcommands 
                {
                    "ml -c \"-Fl%{cfg.objdir}/%{file.basename}.lst\" \"-Fo%{cfg.objdir}/%{file.basename}.obj\" \"%{file.relpath}\""
                }
                buildoutputs
                {
                    "%{cfg.objdir}/%{file.basename}.obj"
                }

            filter { "files:**.asm", "platforms:x64" }            
                buildmessage "Compiling %{file.relpath}"
                buildcommands 
                {
                    "ml64 -c \"-Fl%{cfg.objdir}/%{file.basename}.lst\" \"-Fo%{cfg.objdir}/%{file.basename}.obj\" \"%{file.relpath}\""
                }
                buildoutputs
                {
                    "%{cfg.objdir}/%{file.basename}.obj"
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

        
            
            
    

        project "tracetool"
            removeconfigurations "TRACE*"            
            kind "StaticLib"                    
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
        

    group "zlib"

        project "zlib"            
            kind "StaticLib"        
                       
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
            
        
            
    group "libiconv"

        project "libiconv"            
            kind "StaticLib"        
                       
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
            
            
        

    group "gtest"

        project "gtest"            
            kind "StaticLib"     
            defines { "GTEST_HAS_PTHREAD=0", "_HAS_EXCEPTIONS=1" }            
            files
            {
            
                "3rdparty/googletest/googletest/src/gtest-all.cc"
                     
                               
            }             
            includedirs
            {
               
                "3rdparty/googletest/googletest/include",
                "3rdparty/googletest/googletest"
            }
            
        
    

        project "gtest_main"            
            kind "StaticLib"     
            defines { "GTEST_HAS_PTHREAD=0", "_HAS_EXCEPTIONS=1" }            
            files
            {
            
                "3rdparty/googletest/googletest/src/gtest-all.cc",
                "3rdparty/googletest/googletest/src/gtest_main.cc"
                  
                               
            }             
            includedirs
            {
               
                "3rdparty/googletest/googletest/include",
                "3rdparty/googletest/googletest"
            }
            
        
            
    
   

        project "gmock"            
            kind "StaticLib"     
            defines { "GTEST_HAS_PTHREAD=0", "_HAS_EXCEPTIONS=1" }            
            files
            {
            
                "3rdparty/googletest/googletest/src/gtest-all.cc",
                "3rdparty/googletest/googlemock/src/gmock-all.cc"      
                               
            }             
            includedirs
            {
               
                "3rdparty/googletest/googletest/include",
                "3rdparty/googletest/googletest",
                "3rdparty/googletest/googlemock/include",
                "3rdparty/googletest/googlemock"

            }
            
        
    
    

        project "gmock_main"            
            kind "StaticLib"     
            defines { "GTEST_HAS_PTHREAD=0", "_HAS_EXCEPTIONS=1" }            
            files
            {
            
                
                "3rdparty/googletest/googletest/src/gtest-all.cc",
                "3rdparty/googletest/googlemock/src/gmock-all.cc",
                "3rdparty/googletest/googlemock/src/gmock_main.cc"
                               
            }             
            includedirs
            {       
               
                "3rdparty/googletest/googletest/include",
                "3rdparty/googletest/googletest",
                "3rdparty/googletest/googlemock/include",
                "3rdparty/googletest/googlemock"

            }

        

    group "libprotobuf"

        project "libprotobuf"            
            kind "StaticLib"     
            --defines { "GTEST_HAS_PTHREAD=0", "_HAS_EXCEPTIONS=1" }            
            files
            {
            
                
                "3rdparty/protobuf/src/google/protobuf/any.cc",
                "3rdparty/protobuf/src/google/protobuf/any.pb.cc",
                "3rdparty/protobuf/src/google/protobuf/api.pb.cc",
                "3rdparty/protobuf/src/google/protobuf/compiler/importer.cc",
                "3rdparty/protobuf/src/google/protobuf/compiler/parser.cc",
                "3rdparty/protobuf/src/google/protobuf/descriptor.cc",
                "3rdparty/protobuf/src/google/protobuf/descriptor.pb.cc",
                "3rdparty/protobuf/src/google/protobuf/descriptor_database.cc",
                "3rdparty/protobuf/src/google/protobuf/duration.pb.cc",
                "3rdparty/protobuf/src/google/protobuf/dynamic_message.cc",
                "3rdparty/protobuf/src/google/protobuf/empty.pb.cc",
                "3rdparty/protobuf/src/google/protobuf/extension_set_heavy.cc",
                "3rdparty/protobuf/src/google/protobuf/field_mask.pb.cc",
                "3rdparty/protobuf/src/google/protobuf/generated_message_reflection.cc",
                "3rdparty/protobuf/src/google/protobuf/io/gzip_stream.cc",
                "3rdparty/protobuf/src/google/protobuf/io/printer.cc",
                "3rdparty/protobuf/src/google/protobuf/io/strtod.cc",
                "3rdparty/protobuf/src/google/protobuf/io/tokenizer.cc",
                "3rdparty/protobuf/src/google/protobuf/io/zero_copy_stream_impl.cc",
                "3rdparty/protobuf/src/google/protobuf/map_field.cc",
                "3rdparty/protobuf/src/google/protobuf/message.cc",
                "3rdparty/protobuf/src/google/protobuf/reflection_ops.cc",
                "3rdparty/protobuf/src/google/protobuf/service.cc",
                "3rdparty/protobuf/src/google/protobuf/source_context.pb.cc",
                "3rdparty/protobuf/src/google/protobuf/struct.pb.cc",
                "3rdparty/protobuf/src/google/protobuf/stubs/mathlimits.cc",
                "3rdparty/protobuf/src/google/protobuf/stubs/substitute.cc",
                "3rdparty/protobuf/src/google/protobuf/text_format.cc",
                "3rdparty/protobuf/src/google/protobuf/timestamp.pb.cc",
                "3rdparty/protobuf/src/google/protobuf/type.pb.cc",
                "3rdparty/protobuf/src/google/protobuf/unknown_field_set.cc",
                "3rdparty/protobuf/src/google/protobuf/util/field_comparator.cc",
                "3rdparty/protobuf/src/google/protobuf/util/field_mask_util.cc",
                "3rdparty/protobuf/src/google/protobuf/util/internal/datapiece.cc",
                "3rdparty/protobuf/src/google/protobuf/util/internal/default_value_objectwriter.cc",
                "3rdparty/protobuf/src/google/protobuf/util/internal/error_listener.cc",
                "3rdparty/protobuf/src/google/protobuf/util/internal/field_mask_utility.cc",
                "3rdparty/protobuf/src/google/protobuf/util/internal/json_escaping.cc",
                "3rdparty/protobuf/src/google/protobuf/util/internal/json_objectwriter.cc",
                "3rdparty/protobuf/src/google/protobuf/util/internal/json_stream_parser.cc",
                "3rdparty/protobuf/src/google/protobuf/util/internal/object_writer.cc",
                "3rdparty/protobuf/src/google/protobuf/util/internal/proto_writer.cc",
                "3rdparty/protobuf/src/google/protobuf/util/internal/protostream_objectsource.cc",
                "3rdparty/protobuf/src/google/protobuf/util/internal/protostream_objectwriter.cc",
                "3rdparty/protobuf/src/google/protobuf/util/internal/type_info.cc",
                "3rdparty/protobuf/src/google/protobuf/util/internal/type_info_test_helper.cc",
                "3rdparty/protobuf/src/google/protobuf/util/internal/utility.cc",
                "3rdparty/protobuf/src/google/protobuf/util/json_util.cc",
                "3rdparty/protobuf/src/google/protobuf/util/message_differencer.cc",
                "3rdparty/protobuf/src/google/protobuf/util/time_util.cc",
                "3rdparty/protobuf/src/google/protobuf/util/type_resolver_util.cc",
                "3rdparty/protobuf/src/google/protobuf/wire_format.cc",
                "3rdparty/protobuf/src/google/protobuf/wrappers.pb.cc",
                
                               
            }            
            
            includedirs
            {       
                "3rdparty/protobuf/src",
                "3rdparty/googletest/googletest/include",
                "3rdparty/googletest/googletest",
                "3rdparty/googletest/googlemock/include",
                "3rdparty/googletest/googlemock"

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

          
            
        project "lua"            
            kind "ConsoleApp"          
               
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
            
            files
            {
                "3rdparty/lua-5.3.3/**.h",
                "3rdparty/lua-5.3.3/**.c"                               
            } 
            removefiles
            {
                "3rdparty/lua-5.3.3/lua.c"                
            }  


    group "pe-parse"
        project "pe-parse"            
            kind "StaticLib"        
                       
            files
            {
                "3rdparty/pe-parse/parser-library/*.h",
                "3rdparty/pe-parse/parser-library/*.c",
                "3rdparty/pe-parse/parser-library/*.cpp" 
                               
            } 
            includedirs
            {            
                "3rdparty/boost_1_60_0"    
            }  
             
    group "lua-bridge-sample"
         
        project "test01"       
            targetdir "bin/x86/%{_ACTION}/lua-bridge-sample"
            kind "ConsoleApp"                 
             
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
    
    group "PE"   
        project "PEFile"                   
            kind "ConsoleApp"                 
            
            files
            {
                "3rdparty/PEFile/**.cpp",             
            } 

        project "impdef"                   
            kind "ConsoleApp"                  
            
            files
            {
                "3rdparty/pe-file/**.c",               
            } 

        project "bin-to-hex"                   
            kind "ConsoleApp"                  
           
            files
            {
                "3rdparty/bin-to-hex/**.cpp",               
            }
            
        project "oless"                   
            kind "ConsoleApp"                  
           
            files
            {
                "3rdparty/oless/**.h",  
                "3rdparty/oless/**.hpp",  
                "3rdparty/oless/**.cpp",               
            }


workspace "pe-master"
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"    

        project "pe-master"                   
            kind "SharedLib"                  
           
            files
            {
                "3rdparty/%{wks.name}/%{prj.name}/**.h",  
                "3rdparty/%{wks.name}/%{prj.name}/**.hpp",  
                "3rdparty/%{wks.name}/%{prj.name}/**.cpp", 
                "3rdparty/%{wks.name}/%{prj.name}/**.c",
                "3rdparty/%{wks.name}/%{prj.name}/**.def",
            }


workspace "cairo"
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"    

        project "test_wtl_cairo"                   
            kind "WindowedApp"                  
           
            files
            {
                "src/test_wtl_cairo/**.h",            
                "src/test_wtl_cairo/**.cpp", 
                "src/test_wtl_cairo/**.c",
                "src/test_wtl_cairo/**.rc",
            }
            includedirs
            {               
                "E:/cairo-ft-c951bb2e0d21a1bd46a348f61a59d3e15ff96399/cairo/src",
                "3rdparty/wtl",
               
            }                
          
                    
workspace "sfml"
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"    

    create_sfml_project("win32", "src/sfml-examples")
    create_sfml_project("mouse_button", "src/sfml-examples")
    create_sfml_project("shape", "src/sfml-examples")
    create_sfml_project("01", "src/sfml-examples")    
    create_sfml_project("opengl_01", "src/sfml-examples")  
    create_sfml_project("dragable_window", "src/sfml-examples") 
    create_sfml_project("fps", "src/sfml-examples")
    create_sfml_project("layered_window", "src/sfml-examples")
    create_sfml_project("opengl", "src/sfml-examples")
    links
    {
        "opengl32.lib"
    }

        
workspace "test"
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"                
    
    create_mfc_console_project("test_filesystem", "src")
    create_console_project("test_unix_time", "src")       
    

workspace "glfw"
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"  

    group "Tutorial1"
        create_glfw_console_project("Tutorial1", "src/glfw")

    group "Tutorial2"
        create_glfw_console_project("ecg", "src/glfw/Tutorial2")
        create_glfw_console_project("gaussian", "src/glfw/Tutorial2")
        create_glfw_console_project("line", "src/glfw/Tutorial2")
        create_glfw_console_project("point", "src/glfw/Tutorial2")
        create_glfw_console_project("sinusoid", "src/glfw/Tutorial2")
        create_glfw_console_project("triangle", "src/glfw/Tutorial2")       
    group "Tutorial3"
        create_glfw_console_project("mcml", "src/glfw/Tutorial3")
        create_glfw_console_project("gaussian2", "src/glfw/Tutorial3")
    group "Tutorial4"
        create_glfw_console_project("code_image", "src/glfw/Tutorial4")
        