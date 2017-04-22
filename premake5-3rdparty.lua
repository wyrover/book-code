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
            vpaths 
            { 
                ["Header Files"] = {"**.h", "**.hpp"},
                ["Source Files"] = {"**.c", "**.cpp"}
            }
            
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

        project "msgpack"            
            kind "StaticLib"                             
            files
            {
                "3rdparty/msgpack-c/include/msgpack.hpp",
                "3rdparty/msgpack-c/src/**.c"                                   
            } 
            includedirs
            {          
                "3rdparty/msgpack-c/include",
                "3rdparty"    
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
            
    group "libcurl"
        project "libcurl"
            kind "SharedLib"
            defines { "BUILDING_LIBCURL", "USE_WINDOWS_SSPI", "USE_SCHANNEL", "USE_IPV6", "HTTP_ONLY" }
            files
            {
                "3rdparty/curl-7.53.1/lib/amigaos.c",
                "3rdparty/curl-7.53.1/lib/asyn-ares.c",
                "3rdparty/curl-7.53.1/lib/asyn-thread.c",
                "3rdparty/curl-7.53.1/lib/base64.c",
                "3rdparty/curl-7.53.1/lib/conncache.c",
                "3rdparty/curl-7.53.1/lib/connect.c",
                "3rdparty/curl-7.53.1/lib/content_encoding.c",
                "3rdparty/curl-7.53.1/lib/cookie.c",
                "3rdparty/curl-7.53.1/lib/curl_addrinfo.c",
                "3rdparty/curl-7.53.1/lib/curl_des.c",
                "3rdparty/curl-7.53.1/lib/curl_endian.c",
                "3rdparty/curl-7.53.1/lib/curl_fnmatch.c",
                "3rdparty/curl-7.53.1/lib/curl_gethostname.c",
                "3rdparty/curl-7.53.1/lib/curl_gssapi.c",
                "3rdparty/curl-7.53.1/lib/curl_memrchr.c",
                "3rdparty/curl-7.53.1/lib/curl_multibyte.c",
                "3rdparty/curl-7.53.1/lib/curl_ntlm_core.c",
                "3rdparty/curl-7.53.1/lib/curl_ntlm_wb.c",
                "3rdparty/curl-7.53.1/lib/curl_rtmp.c",
                "3rdparty/curl-7.53.1/lib/curl_sasl.c",
                "3rdparty/curl-7.53.1/lib/curl_sspi.c",
                "3rdparty/curl-7.53.1/lib/curl_threads.c",
                "3rdparty/curl-7.53.1/lib/dict.c",
                "3rdparty/curl-7.53.1/lib/dotdot.c",
                "3rdparty/curl-7.53.1/lib/easy.c",
                "3rdparty/curl-7.53.1/lib/escape.c",
                "3rdparty/curl-7.53.1/lib/file.c",
                "3rdparty/curl-7.53.1/lib/fileinfo.c",
                "3rdparty/curl-7.53.1/lib/formdata.c",
                "3rdparty/curl-7.53.1/lib/ftp.c",
                "3rdparty/curl-7.53.1/lib/ftplistparser.c",
                "3rdparty/curl-7.53.1/lib/getenv.c",
                "3rdparty/curl-7.53.1/lib/getinfo.c",
                "3rdparty/curl-7.53.1/lib/gopher.c",
                "3rdparty/curl-7.53.1/lib/hash.c",
                "3rdparty/curl-7.53.1/lib/hmac.c",
                "3rdparty/curl-7.53.1/lib/hostasyn.c",
                "3rdparty/curl-7.53.1/lib/hostcheck.c",
                "3rdparty/curl-7.53.1/lib/hostip.c",
                "3rdparty/curl-7.53.1/lib/hostip4.c",
                "3rdparty/curl-7.53.1/lib/hostip6.c",
                "3rdparty/curl-7.53.1/lib/hostsyn.c",
                "3rdparty/curl-7.53.1/lib/http.c",
                "3rdparty/curl-7.53.1/lib/http2.c",
                "3rdparty/curl-7.53.1/lib/http_chunks.c",
                "3rdparty/curl-7.53.1/lib/http_digest.c",
                "3rdparty/curl-7.53.1/lib/http_negotiate.c",
                "3rdparty/curl-7.53.1/lib/http_ntlm.c",
                "3rdparty/curl-7.53.1/lib/http_proxy.c",
                "3rdparty/curl-7.53.1/lib/idn_win32.c",
                "3rdparty/curl-7.53.1/lib/if2ip.c",
                "3rdparty/curl-7.53.1/lib/imap.c",
                "3rdparty/curl-7.53.1/lib/inet_ntop.c",
                "3rdparty/curl-7.53.1/lib/inet_pton.c",
                "3rdparty/curl-7.53.1/lib/krb5.c",
                "3rdparty/curl-7.53.1/lib/ldap.c",
                "3rdparty/curl-7.53.1/lib/llist.c",
                "3rdparty/curl-7.53.1/lib/md4.c",
                "3rdparty/curl-7.53.1/lib/md5.c",
                "3rdparty/curl-7.53.1/lib/memdebug.c",
                "3rdparty/curl-7.53.1/lib/mprintf.c",
                "3rdparty/curl-7.53.1/lib/multi.c",
                "3rdparty/curl-7.53.1/lib/netrc.c",
                "3rdparty/curl-7.53.1/lib/non-ascii.c",
                "3rdparty/curl-7.53.1/lib/nonblock.c",
                "3rdparty/curl-7.53.1/lib/openldap.c",
                "3rdparty/curl-7.53.1/lib/parsedate.c",
                "3rdparty/curl-7.53.1/lib/pingpong.c",
                "3rdparty/curl-7.53.1/lib/pipeline.c",
                "3rdparty/curl-7.53.1/lib/pop3.c",
                "3rdparty/curl-7.53.1/lib/progress.c",
                "3rdparty/curl-7.53.1/lib/rand.c",
                "3rdparty/curl-7.53.1/lib/rtsp.c",
                "3rdparty/curl-7.53.1/lib/security.c",
                "3rdparty/curl-7.53.1/lib/select.c",
                "3rdparty/curl-7.53.1/lib/sendf.c",
                "3rdparty/curl-7.53.1/lib/share.c",
                "3rdparty/curl-7.53.1/lib/slist.c",
                "3rdparty/curl-7.53.1/lib/smb.c",
                "3rdparty/curl-7.53.1/lib/smtp.c",
                "3rdparty/curl-7.53.1/lib/socks.c",
                "3rdparty/curl-7.53.1/lib/socks_gssapi.c",
                "3rdparty/curl-7.53.1/lib/socks_sspi.c",
                "3rdparty/curl-7.53.1/lib/speedcheck.c",
                "3rdparty/curl-7.53.1/lib/splay.c",
                "3rdparty/curl-7.53.1/lib/ssh.c",
                "3rdparty/curl-7.53.1/lib/strcase.c",
                "3rdparty/curl-7.53.1/lib/strdup.c",
                "3rdparty/curl-7.53.1/lib/strerror.c",
                "3rdparty/curl-7.53.1/lib/strtok.c",
                "3rdparty/curl-7.53.1/lib/strtoofft.c",
                "3rdparty/curl-7.53.1/lib/system_win32.c",
                "3rdparty/curl-7.53.1/lib/telnet.c",
                "3rdparty/curl-7.53.1/lib/tftp.c",
                "3rdparty/curl-7.53.1/lib/timeval.c",
                "3rdparty/curl-7.53.1/lib/transfer.c",
                "3rdparty/curl-7.53.1/lib/url.c",
                "3rdparty/curl-7.53.1/lib/version.c",
                "3rdparty/curl-7.53.1/lib/warnless.c",
                "3rdparty/curl-7.53.1/lib/wildcard.c",
                "3rdparty/curl-7.53.1/lib/x509asn1.c",
                "3rdparty/curl-7.53.1/lib/vauth/cleartext.c",
                "3rdparty/curl-7.53.1/lib/vauth/cram.c",
                "3rdparty/curl-7.53.1/lib/vauth/digest.c",
                "3rdparty/curl-7.53.1/lib/vauth/digest_sspi.c",
                "3rdparty/curl-7.53.1/lib/vauth/krb5_gssapi.c",
                "3rdparty/curl-7.53.1/lib/vauth/krb5_sspi.c",
                "3rdparty/curl-7.53.1/lib/vauth/ntlm.c",
                "3rdparty/curl-7.53.1/lib/vauth/ntlm_sspi.c",
                "3rdparty/curl-7.53.1/lib/vauth/oauth2.c",
                "3rdparty/curl-7.53.1/lib/vauth/spnego_gssapi.c",
                "3rdparty/curl-7.53.1/lib/vauth/spnego_sspi.c",
                "3rdparty/curl-7.53.1/lib/vauth/vauth.c",
                "3rdparty/curl-7.53.1/lib/vtls/axtls.c",
                "3rdparty/curl-7.53.1/lib/vtls/cyassl.c",
                "3rdparty/curl-7.53.1/lib/vtls/darwinssl.c",
                "3rdparty/curl-7.53.1/lib/vtls/gskit.c",
                "3rdparty/curl-7.53.1/lib/vtls/gtls.c",
                "3rdparty/curl-7.53.1/lib/vtls/mbedtls.c",
                "3rdparty/curl-7.53.1/lib/vtls/nss.c",
                "3rdparty/curl-7.53.1/lib/vtls/openssl.c",
                "3rdparty/curl-7.53.1/lib/vtls/polarssl.c",
                "3rdparty/curl-7.53.1/lib/vtls/polarssl_threadlock.c",
                "3rdparty/curl-7.53.1/lib/vtls/schannel.c",
                "3rdparty/curl-7.53.1/lib/vtls/vtls.c",
                "3rdparty/curl-7.53.1/lib/amigaos.h",
                "3rdparty/curl-7.53.1/lib/arpa_telnet.h",
                "3rdparty/curl-7.53.1/lib/asyn.h",
                "3rdparty/curl-7.53.1/lib/config-win32.h",
                "3rdparty/curl-7.53.1/lib/conncache.h",
                "3rdparty/curl-7.53.1/lib/connect.h",
                "3rdparty/curl-7.53.1/lib/content_encoding.h",
                "3rdparty/curl-7.53.1/lib/cookie.h",
                "3rdparty/curl-7.53.1/lib/curl_addrinfo.h",
                "3rdparty/curl-7.53.1/lib/curl_base64.h",
                "3rdparty/curl-7.53.1/lib/curl_des.h",
                "3rdparty/curl-7.53.1/lib/curl_endian.h",
                "3rdparty/curl-7.53.1/lib/curl_fnmatch.h",
                "3rdparty/curl-7.53.1/lib/curl_gethostname.h",
                "3rdparty/curl-7.53.1/lib/curl_gssapi.h",
                "3rdparty/curl-7.53.1/lib/curl_hmac.h",
                "3rdparty/curl-7.53.1/lib/curl_ldap.h",
                "3rdparty/curl-7.53.1/lib/curl_md4.h",
                "3rdparty/curl-7.53.1/lib/curl_md5.h",
                "3rdparty/curl-7.53.1/lib/curl_memory.h",
                "3rdparty/curl-7.53.1/lib/curl_memrchr.h",
                "3rdparty/curl-7.53.1/lib/curl_multibyte.h",
                "3rdparty/curl-7.53.1/lib/curl_ntlm_core.h",
                "3rdparty/curl-7.53.1/lib/curl_ntlm_wb.h",
                "3rdparty/curl-7.53.1/lib/curl_printf.h",
                "3rdparty/curl-7.53.1/lib/curl_rtmp.h",
                "3rdparty/curl-7.53.1/lib/curl_sasl.h",
                "3rdparty/curl-7.53.1/lib/curl_sec.h",
                "3rdparty/curl-7.53.1/lib/curl_setup.h",
                "3rdparty/curl-7.53.1/lib/curl_setup_once.h",
                "3rdparty/curl-7.53.1/lib/curl_sspi.h",
                "3rdparty/curl-7.53.1/lib/curl_threads.h",
                "3rdparty/curl-7.53.1/lib/curlx.h",
                "3rdparty/curl-7.53.1/lib/dict.h",
                "3rdparty/curl-7.53.1/lib/dotdot.h",
                "3rdparty/curl-7.53.1/lib/easyif.h",
                "3rdparty/curl-7.53.1/lib/escape.h",
                "3rdparty/curl-7.53.1/lib/file.h",
                "3rdparty/curl-7.53.1/lib/fileinfo.h",
                "3rdparty/curl-7.53.1/lib/formdata.h",
                "3rdparty/curl-7.53.1/lib/ftp.h",
                "3rdparty/curl-7.53.1/lib/ftplistparser.h",
                "3rdparty/curl-7.53.1/lib/getinfo.h",
                "3rdparty/curl-7.53.1/lib/gopher.h",
                "3rdparty/curl-7.53.1/lib/hash.h",
                "3rdparty/curl-7.53.1/lib/hostcheck.h",
                "3rdparty/curl-7.53.1/lib/hostip.h",
                "3rdparty/curl-7.53.1/lib/http.h",
                "3rdparty/curl-7.53.1/lib/http2.h",
                "3rdparty/curl-7.53.1/lib/http_chunks.h",
                "3rdparty/curl-7.53.1/lib/http_digest.h",
                "3rdparty/curl-7.53.1/lib/http_negotiate.h",
                "3rdparty/curl-7.53.1/lib/http_ntlm.h",
                "3rdparty/curl-7.53.1/lib/http_proxy.h",
                "3rdparty/curl-7.53.1/lib/if2ip.h",
                "3rdparty/curl-7.53.1/lib/imap.h",
                "3rdparty/curl-7.53.1/lib/inet_ntop.h",
                "3rdparty/curl-7.53.1/lib/inet_pton.h",
                "3rdparty/curl-7.53.1/lib/llist.h",
                "3rdparty/curl-7.53.1/lib/memdebug.h",
                "3rdparty/curl-7.53.1/lib/multihandle.h",
                "3rdparty/curl-7.53.1/lib/multiif.h",
                "3rdparty/curl-7.53.1/lib/netrc.h",
                "3rdparty/curl-7.53.1/lib/non-ascii.h",
                "3rdparty/curl-7.53.1/lib/nonblock.h",
                "3rdparty/curl-7.53.1/lib/parsedate.h",
                "3rdparty/curl-7.53.1/lib/pingpong.h",
                "3rdparty/curl-7.53.1/lib/pipeline.h",
                "3rdparty/curl-7.53.1/lib/pop3.h",
                "3rdparty/curl-7.53.1/lib/progress.h",
                "3rdparty/curl-7.53.1/lib/rand.h",
                "3rdparty/curl-7.53.1/lib/rtsp.h",
                "3rdparty/curl-7.53.1/lib/select.h",
                "3rdparty/curl-7.53.1/lib/sendf.h",
                "3rdparty/curl-7.53.1/lib/setup-vms.h",
                "3rdparty/curl-7.53.1/lib/share.h",
                "3rdparty/curl-7.53.1/lib/sigpipe.h",
                "3rdparty/curl-7.53.1/lib/slist.h",
                "3rdparty/curl-7.53.1/lib/smb.h",
                "3rdparty/curl-7.53.1/lib/smtp.h",
                "3rdparty/curl-7.53.1/lib/sockaddr.h",
                "3rdparty/curl-7.53.1/lib/socks.h",
                "3rdparty/curl-7.53.1/lib/speedcheck.h",
                "3rdparty/curl-7.53.1/lib/splay.h",
                "3rdparty/curl-7.53.1/lib/ssh.h",
                "3rdparty/curl-7.53.1/lib/strcase.h",
                "3rdparty/curl-7.53.1/lib/strdup.h",
                "3rdparty/curl-7.53.1/lib/strerror.h",
                "3rdparty/curl-7.53.1/lib/strtok.h",
                "3rdparty/curl-7.53.1/lib/strtoofft.h",
                "3rdparty/curl-7.53.1/lib/system_win32.h",
                "3rdparty/curl-7.53.1/lib/telnet.h",
                "3rdparty/curl-7.53.1/lib/tftp.h",
                "3rdparty/curl-7.53.1/lib/timeval.h",
                "3rdparty/curl-7.53.1/lib/transfer.h",
                "3rdparty/curl-7.53.1/lib/url.h",
                "3rdparty/curl-7.53.1/lib/urldata.h",
                "3rdparty/curl-7.53.1/lib/warnless.h",
                "3rdparty/curl-7.53.1/lib/wildcard.h",
                "3rdparty/curl-7.53.1/lib/x509asn1.h",
                "3rdparty/curl-7.53.1/lib/vauth/digest.h",
                "3rdparty/curl-7.53.1/lib/vauth/ntlm.h",
                "3rdparty/curl-7.53.1/lib/vauth/vauth.h",
                "3rdparty/curl-7.53.1/lib/vtls/axtls.h",
                "3rdparty/curl-7.53.1/lib/vtls/cyassl.h",
                "3rdparty/curl-7.53.1/lib/vtls/darwinssl.h",
                "3rdparty/curl-7.53.1/lib/vtls/gskit.h",
                "3rdparty/curl-7.53.1/lib/vtls/gtls.h",
                "3rdparty/curl-7.53.1/lib/vtls/mbedtls.h",
                "3rdparty/curl-7.53.1/lib/vtls/nssg.h",
                "3rdparty/curl-7.53.1/lib/vtls/openssl.h",
                "3rdparty/curl-7.53.1/lib/vtls/polarssl.h",
                "3rdparty/curl-7.53.1/lib/vtls/polarssl_threadlock.h",
                "3rdparty/curl-7.53.1/lib/vtls/schannel.h",
                "3rdparty/curl-7.53.1/lib/vtls/vtls.h",
                "3rdparty/curl-7.53.1/lib/libcurl.rc"
            }
            vpaths 
            { 
                ["Header Files"] = {"**.h", "**.hpp"},
                ["Source Files"] = {"**.c", "**.cpp"}
            }
            includedirs
            {
                "3rdparty/curl-7.53.1/include",
                "3rdparty/curl-7.53.1/lib"
            }
            links
            {
                "ws2_32.lib",
                "wldap32.lib",
                "Crypt32.lib"
            }

        

       
        project "curl"          
            kind "ConsoleApp"                                             
            files
            {                                  
                "3rdparty/curl-7.53.1/lib/nonblock.c",
                "3rdparty/curl-7.53.1/lib/strtoofft.c",
                "3rdparty/curl-7.53.1/lib/warnless.c",
                "3rdparty/curl-7.53.1/src/slist_wc.c",
                "3rdparty/curl-7.53.1/src/tool_binmode.c",
                "3rdparty/curl-7.53.1/src/tool_bname.c",
                "3rdparty/curl-7.53.1/src/tool_cb_dbg.c",
                "3rdparty/curl-7.53.1/src/tool_cb_hdr.c",
                "3rdparty/curl-7.53.1/src/tool_cb_prg.c",
                "3rdparty/curl-7.53.1/src/tool_cb_rea.c",
                "3rdparty/curl-7.53.1/src/tool_cb_see.c",
                "3rdparty/curl-7.53.1/src/tool_cb_wrt.c",
                "3rdparty/curl-7.53.1/src/tool_cfgable.c",
                "3rdparty/curl-7.53.1/src/tool_convert.c",
                "3rdparty/curl-7.53.1/src/tool_dirhie.c",
                "3rdparty/curl-7.53.1/src/tool_doswin.c",
                "3rdparty/curl-7.53.1/src/tool_easysrc.c",
                "3rdparty/curl-7.53.1/src/tool_formparse.c",
                "3rdparty/curl-7.53.1/src/tool_getparam.c",
                "3rdparty/curl-7.53.1/src/tool_getpass.c",
                "3rdparty/curl-7.53.1/src/tool_help.c",
                "3rdparty/curl-7.53.1/src/tool_helpers.c",
                "3rdparty/curl-7.53.1/src/tool_homedir.c",
                "3rdparty/curl-7.53.1/src/tool_hugehelp.c",
                "3rdparty/curl-7.53.1/src/tool_libinfo.c",
                "3rdparty/curl-7.53.1/src/tool_main.c",
                "3rdparty/curl-7.53.1/src/tool_metalink.c",
                "3rdparty/curl-7.53.1/src/tool_mfiles.c",
                "3rdparty/curl-7.53.1/src/tool_msgs.c",
                "3rdparty/curl-7.53.1/src/tool_operate.c",
                "3rdparty/curl-7.53.1/src/tool_operhlp.c",
                "3rdparty/curl-7.53.1/src/tool_panykey.c",
                "3rdparty/curl-7.53.1/src/tool_paramhlp.c",
                "3rdparty/curl-7.53.1/src/tool_parsecfg.c",
                "3rdparty/curl-7.53.1/src/tool_setopt.c",
                "3rdparty/curl-7.53.1/src/tool_sleep.c",
                "3rdparty/curl-7.53.1/src/tool_strdup.c",
                "3rdparty/curl-7.53.1/src/tool_urlglob.c",
                "3rdparty/curl-7.53.1/src/tool_util.c",
                "3rdparty/curl-7.53.1/src/tool_vms.c",
                "3rdparty/curl-7.53.1/src/tool_writeenv.c",
                "3rdparty/curl-7.53.1/src/tool_writeout.c",
                "3rdparty/curl-7.53.1/src/tool_xattr.c",
                ------------------------------------------------------------------------------------------------------------
                "3rdparty/curl-7.53.1/lib/config-win32.h",
                "3rdparty/curl-7.53.1/lib/curl_setup.h",
                "3rdparty/curl-7.53.1/lib/nonblock.h",
                "3rdparty/curl-7.53.1/lib/strtoofft.h",
                "3rdparty/curl-7.53.1/lib/warnless.h",
                "3rdparty/curl-7.53.1/src/slist_wc.h",
                "3rdparty/curl-7.53.1/src/tool_binmode.h",
                "3rdparty/curl-7.53.1/src/tool_bname.h",
                "3rdparty/curl-7.53.1/src/tool_cb_dbg.h",
                "3rdparty/curl-7.53.1/src/tool_cb_hdr.h",
                "3rdparty/curl-7.53.1/src/tool_cb_prg.h",
                "3rdparty/curl-7.53.1/src/tool_cb_rea.h",
                "3rdparty/curl-7.53.1/src/tool_cb_see.h",
                "3rdparty/curl-7.53.1/src/tool_cb_wrt.h",
                "3rdparty/curl-7.53.1/src/tool_cfgable.h",
                "3rdparty/curl-7.53.1/src/tool_convert.h",
                "3rdparty/curl-7.53.1/src/tool_dirhie.h",
                "3rdparty/curl-7.53.1/src/tool_doswin.h",
                "3rdparty/curl-7.53.1/src/tool_easysrc.h",
                "3rdparty/curl-7.53.1/src/tool_formparse.h",
                "3rdparty/curl-7.53.1/src/tool_getparam.h",
                "3rdparty/curl-7.53.1/src/tool_getpass.h",
                "3rdparty/curl-7.53.1/src/tool_helpers.h",
                "3rdparty/curl-7.53.1/src/tool_help.h",
                "3rdparty/curl-7.53.1/src/tool_homedir.h",
                "3rdparty/curl-7.53.1/src/tool_hugehelp.h",
                "3rdparty/curl-7.53.1/src/tool_libinfo.h",
                "3rdparty/curl-7.53.1/src/tool_main.h",
                "3rdparty/curl-7.53.1/src/tool_metalink.h",
                "3rdparty/curl-7.53.1/src/tool_mfiles.h",
                "3rdparty/curl-7.53.1/src/tool_msgs.h",
                "3rdparty/curl-7.53.1/src/tool_operate.h",
                "3rdparty/curl-7.53.1/src/tool_operhlp.h",
                "3rdparty/curl-7.53.1/src/tool_panykey.h",
                "3rdparty/curl-7.53.1/src/tool_paramhlp.h",
                "3rdparty/curl-7.53.1/src/tool_parsecfg.h",
                "3rdparty/curl-7.53.1/src/tool_sdecls.h",
                "3rdparty/curl-7.53.1/src/tool_setopt.h",
                "3rdparty/curl-7.53.1/src/tool_setup.h",
                "3rdparty/curl-7.53.1/src/tool_sleep.h",
                "3rdparty/curl-7.53.1/src/tool_strdup.h",
                "3rdparty/curl-7.53.1/src/tool_urlglob.h",
                "3rdparty/curl-7.53.1/src/tool_util.h",
                "3rdparty/curl-7.53.1/src/tool_version.h",
                "3rdparty/curl-7.53.1/src/tool_vms.h",
                "3rdparty/curl-7.53.1/src/tool_writeenv.h",
                "3rdparty/curl-7.53.1/src/tool_writeout.h",
                "3rdparty/curl-7.53.1/src/tool_xattr.h",
                ------------------------------------------------------------------------------------------------------------
                "3rdparty/curl-7.53.1/src/curl.rc",    
            }            
            vpaths 
            { 
                ["Header Files"] = {"**.h", "**.hpp"},
                ["Source Files"] = {"**.c", "**.cpp"}
            }
            includedirs
            {
                "3rdparty/curl-7.53.1/include",
                "3rdparty/curl-7.53.1/lib"
            }
            links
            {
                "libcurl",
                "ws2_32.lib",
                "wldap32.lib"
            }    
            


    group "zlib"

        project "zlib"            
            kind "StaticLib"                          
            files
            {
                "3rdparty/zlib/*.h",
                "3rdparty/zlib/*.c"      
                               
            }      
            vpaths 
            { 
                ["Header Files"] = {"**.h", "**.hpp"},
                ["Source Files"] = {"**.c", "**.cpp"}
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
            vpaths 
            { 
                ["Header Files"] = {"**.h", "**.hpp"},
                ["Source Files"] = {"**.c", "**.cpp"}
            }

        create_console_project("test-zlib", "src")              
            links
            {
                "zlib",
                "cryptopp"
            }
            includedirs
            {
                "3rdparty/cryptopp565"
            }


        project "7zpp"            
            kind "StaticLib"                          
            files
            {
                "3rdparty/7zpp/*.h",
                "3rdparty/7zpp/*.cpp"      
                               
            }      
            vpaths 
            { 
                ["Header Files"] = {"**.h", "**.hpp"},
                ["Source Files"] = {"**.c", "**.cpp"}
            }
            links
            {
                "7z.lib"   
            }
            libdirs
            {
                "H:/rover/rover-self-work/cpp/book-code/3rdparty/7z1604-vc9/CPP/7zip/Bundles/Format7zF/static"
            }
            includedirs
            {
                "H:/rover/rover-self-work/cpp/book-code/3rdparty/7z1604-vc9/C",
                "H:/rover/rover-self-work/cpp/book-code/3rdparty/7z1604-vc9/CPP"
            }
            has_stdafx("7zpp", "3rdparty")

        create_console_project("test-7z", "src")   
            files
            {
                "3rdparty/win32_common/**"
            }
            links
            {
                "7z.lib",   
                "7zpp",
                
            }
            libdirs
            {
                "H:/rover/rover-self-work/cpp/book-code/3rdparty/7z1604-vc9/CPP/7zip/Bundles/Format7zF/static"
            }
            includedirs
            {
                "H:/rover/rover-self-work/cpp/book-code/3rdparty/7z1604-vc9/CPP"
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
    
    


 
        
