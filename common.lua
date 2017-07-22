group "gtest"


    project "gtest"            
        removeconfigurations "TRACE*"   
        kind "StaticLib"     
        defines { "GTEST_HAS_PTHREAD=0", "_HAS_EXCEPTIONS=1" }            
        files
        {
        
            "%{THIRD_PARTY}/googletest/googletest/src/gtest-all.cc"
                 
                           
        }             
        includedirs
        {
           
            "%{THIRD_PARTY}/googletest/googletest/include",
            "%{THIRD_PARTY}/googletest/googletest"
        }
        
    


    project "gtest_main"     
        removeconfigurations "TRACE*"   
        kind "StaticLib"     
        defines { "GTEST_HAS_PTHREAD=0", "_HAS_EXCEPTIONS=1" }            
        files
        {
        
            "%{THIRD_PARTY}/googletest/googletest/src/gtest-all.cc",
            "%{THIRD_PARTY}/googletest/googletest/src/gtest_main.cc"
              
                           
        }             
        includedirs
        {
           
            "%{THIRD_PARTY}/googletest/googletest/include",
            "%{THIRD_PARTY}/googletest/googletest"
        }
        
    
        



    project "gmock"         
        removeconfigurations "TRACE*"   
        kind "StaticLib"     
        defines { "GTEST_HAS_PTHREAD=0", "_HAS_EXCEPTIONS=1" }            
        files
        {
        
            "%{THIRD_PARTY}/googletest/googletest/src/gtest-all.cc",
            "%{THIRD_PARTY}/googletest/googlemock/src/gmock-all.cc"      
                           
        }             
        includedirs
        {
           
            "%{THIRD_PARTY}/googletest/googletest/include",
            "%{THIRD_PARTY}/googletest/googletest",
            "%{THIRD_PARTY}/googletest/googlemock/include",
            "%{THIRD_PARTY}/googletest/googlemock"

        }
        
    



    project "gmock_main"   
        removeconfigurations "TRACE*"   
        kind "StaticLib"     
        defines { "GTEST_HAS_PTHREAD=0", "_HAS_EXCEPTIONS=1" }            
        files
        {
        
            
            "%{THIRD_PARTY}/googletest/googletest/src/gtest-all.cc",
            "%{THIRD_PARTY}/googletest/googlemock/src/gmock-all.cc",
            "%{THIRD_PARTY}/googletest/googlemock/src/gmock_main.cc"
                           
        }             
        includedirs
        {       
           
            "%{THIRD_PARTY}/googletest/googletest/include",
            "%{THIRD_PARTY}/googletest/googletest",
            "%{THIRD_PARTY}/googletest/googlemock/include",
            "%{THIRD_PARTY}/googletest/googlemock"

        }

group "glog"
    project "glog"            
        removeconfigurations "TRACE*"   
        kind "StaticLib"
        defines { "GOOGLE_GLOG_DLL_DECL=", "HAVE_SNPRINTF" }
        files
        {
            "%{THIRD_PARTY}/glog-master/src/logging.cc",
            "%{THIRD_PARTY}/glog-master/src/windows/port.cc",
            "%{THIRD_PARTY}/glog-master/src/raw_logging.cc",
            "%{THIRD_PARTY}/glog-master/src/utilities.cc",
            "%{THIRD_PARTY}/glog-master/src/vlog_is_on.cc",
            ------------------------------------------------------------------------------------------------------------
            "%{THIRD_PARTY}/glog-master/src/base/commandlineflags.h",
            "%{THIRD_PARTY}/glog-master/src/windows/config.h",
            "%{THIRD_PARTY}/glog-master/src/base/googleinit.h",
            "%{THIRD_PARTY}/glog-master/src/windows/glog/log_severity.h",
            "%{THIRD_PARTY}/glog-master/src/windows/glog/logging.h",
            "%{THIRD_PARTY}/glog-master/src/base/mutex.h",
            "%{THIRD_PARTY}/glog-master/src/windows/port.h",
            "%{THIRD_PARTY}/glog-master/src/windows/glog/raw_logging.h",
            "%{THIRD_PARTY}/glog-master/src/windows/glog/stl_logging.h",
            "%{THIRD_PARTY}/glog-master/src/utilities.h",
            "%{THIRD_PARTY}/glog-master/src/windows/glog/vlog_is_on.h"
        }
        vpaths 
        { 
            ["Header Files"] = {"**.h", "**.hpp"},
            ["Source Files"] = {"**.c", "**.cpp"}
        }
        includedirs
        {
            "%{THIRD_PARTY}/glog-master/src/windows",
                        
        }

group "gflags"
    project "gflags"      
        removeconfigurations "TRACE*"   
        kind "StaticLib"
        characterset "MBCS"
        defines { "GFLAGS_IS_A_DLL=0" }
        files
        {
            "%{THIRD_PARTY}/gflags-master/src/**.cc",
            "%{THIRD_PARTY}/gflags-master/src/**.h",
            "%{THIRD_PARTY}/gflags-master/bin/include/gflags/**.h",
        }
        vpaths 
        { 
            ["Header Files"] = {"**.h", "**.hpp"},
            ["Source Files"] = {"**.c", "**.cpp", "**.cc"}
        }
        includedirs
        {
            "%{THIRD_PARTY}/gflags-master/bin/include/gflags",
            "%{THIRD_PARTY}/gflags-master/bin/include",
                        
        }

    project "gflags_nothreads"        
        removeconfigurations "TRACE*"   
        kind "StaticLib"
        characterset "MBCS"
        defines { "GFLAGS_IS_A_DLL=0", "NO_THREADS" }
        files
        {
            "%{THIRD_PARTY}/gflags-master/src/**.cc",
            "%{THIRD_PARTY}/gflags-master/src/**.h",
            "%{THIRD_PARTY}/gflags-master/bin/include/gflags/**.h",
        }
        vpaths 
        { 
            ["Header Files"] = {"**.h", "**.hpp"},
            ["Source Files"] = {"**.c", "**.cpp", "**.cc"}
        }
        includedirs
        {
            "%{THIRD_PARTY}/gflags-master/bin/include/gflags",
            "%{THIRD_PARTY}/gflags-master/bin/include",
                        
        }

group "tcmalloc"
    project "libtcmalloc_minimal"     
        removeconfigurations "TRACE*"   
        kind "SharedLib"
        characterset "MBCS"
        defines { "LIBTCMALLOC_MINIMAL_EXPORTS" }
        files
        {
            "%{THIRD_PARTY}/gperftools-master/src/central_freelist.cc",
            "%{THIRD_PARTY}/gperftools-master/src/base/dynamic_annotations.c",
            "%{THIRD_PARTY}/gperftools-master/src/heap-profile-table.cc",
            "%{THIRD_PARTY}/gperftools-master/src/symbolize.cc",
            "%{THIRD_PARTY}/gperftools-master/src/windows/ia32_modrm_map.cc",
            "%{THIRD_PARTY}/gperftools-master/src/windows/ia32_opcode_map.cc",
            "%{THIRD_PARTY}/gperftools-master/src/common.cc",
            "%{THIRD_PARTY}/gperftools-master/src/internal_logging.cc",
            "%{THIRD_PARTY}/gperftools-master/src/base/logging.cc",
            "%{THIRD_PARTY}/gperftools-master/src/base/low_level_alloc.cc",
            "%{THIRD_PARTY}/gperftools-master/src/malloc_extension.cc",
            "%{THIRD_PARTY}/gperftools-master/src/malloc_hook.cc",
            "%{THIRD_PARTY}/gperftools-master/src/memory_region_map.cc",
            "%{THIRD_PARTY}/gperftools-master/src/windows/mini_disassembler.cc",
            "%{THIRD_PARTY}/gperftools-master/src/page_heap.cc",
            "%{THIRD_PARTY}/gperftools-master/src/sampler.cc",
            "%{THIRD_PARTY}/gperftools-master/src/windows/patch_functions.cc",
            "%{THIRD_PARTY}/gperftools-master/src/windows/port.cc",
            "%{THIRD_PARTY}/gperftools-master/src/windows/preamble_patcher.cc",
            "%{THIRD_PARTY}/gperftools-master/src/windows/preamble_patcher_with_stub.cc",
            "%{THIRD_PARTY}/gperftools-master/src/windows/system-alloc.cc",
            "%{THIRD_PARTY}/gperftools-master/src/raw_printer.cc",
            "%{THIRD_PARTY}/gperftools-master/src/span.cc",
            "%{THIRD_PARTY}/gperftools-master/src/stacktrace.cc",
            "%{THIRD_PARTY}/gperftools-master/src/stack_trace_table.cc",
            "%{THIRD_PARTY}/gperftools-master/src/static_vars.cc",
            "%{THIRD_PARTY}/gperftools-master/src/base/spinlock.cc",
            "%{THIRD_PARTY}/gperftools-master/src/base/spinlock_internal.cc",
            "%{THIRD_PARTY}/gperftools-master/src/base/sysinfo.cc",
            "%{THIRD_PARTY}/gperftools-master/src/thread_cache.cc",
            "%{THIRD_PARTY}/gperftools-master/src/fake_stacktrace_scope.cc",
            ------------------------------------------------------------------------------------------------------------
            "%{THIRD_PARTY}/gperftools-master/src/addressmap-inl.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/basictypes.h",
            "%{THIRD_PARTY}/gperftools-master/src/central_freelist.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/commandlineflags.h",
            "%{THIRD_PARTY}/gperftools-master/src/windows/config.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/googleinit.h",
            "%{THIRD_PARTY}/gperftools-master/src/gperftools/heap-checker.h",
            "%{THIRD_PARTY}/gperftools-master/src/heap-profile-table.h",
            "%{THIRD_PARTY}/gperftools-master/src/symbolize.h",
            "%{THIRD_PARTY}/gperftools-master/src/gperftools/heap-profiler.h",
            "%{THIRD_PARTY}/gperftools-master/src/common.h",
            "%{THIRD_PARTY}/gperftools-master/src/internal_logging.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/linked_list.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/logging.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/low_level_alloc.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/atomicops-internals-arm-gcc.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/atomicops-internals-linuxppc.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/atomicops-internals-macosx.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/atomicops-internals-x86-msvc.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/atomicops-internals-x86.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/atomicops.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/spinlock.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/spinlock_internal.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/spinlock_linux-inl.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/spinlock_posix-inl.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/spinlock_win32-inl.h",
            "%{THIRD_PARTY}/gperftools-master/src/gperftools/malloc_extension.h",
            "%{THIRD_PARTY}/gperftools-master/src/gperftools/malloc_hook.h",
            "%{THIRD_PARTY}/gperftools-master/src/malloc_hook-inl.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/atomicops.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/atomicops-internals-x86-msvc.h",
            "%{THIRD_PARTY}/gperftools-master/src/memory_region_map.h",
            "%{THIRD_PARTY}/gperftools-master/src/windows/mini_disassembler.h",
            "%{THIRD_PARTY}/gperftools-master/src/windows/mini_disassembler_types.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/mutex.h",
            "%{THIRD_PARTY}/gperftools-master/src/packed-cache-inl.h",
            "%{THIRD_PARTY}/gperftools-master/src/page_heap.h",
            "%{THIRD_PARTY}/gperftools-master/src/page_heap_allocator.h",
            "%{THIRD_PARTY}/gperftools-master/src/pagemap.h",
            "%{THIRD_PARTY}/gperftools-master/src/windows/port.h",
            "%{THIRD_PARTY}/gperftools-master/src/windows/preamble_patcher.h",
            "%{THIRD_PARTY}/gperftools-master/src/gperftools/profiler.h",
            "%{THIRD_PARTY}/gperftools-master/src/raw_printer.h",
            "%{THIRD_PARTY}/gperftools-master/src/sampler.h",
            "%{THIRD_PARTY}/gperftools-master/src/span.h",
            "%{THIRD_PARTY}/gperftools-master/src/gperftools/stacktrace.h",
            "%{THIRD_PARTY}/gperftools-master/src/stacktrace_config.h",
            "%{THIRD_PARTY}/gperftools-master/src/stacktrace_win32-inl.h",
            "%{THIRD_PARTY}/gperftools-master/src/stack_trace_table.h",
            "%{THIRD_PARTY}/gperftools-master/src/static_vars.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/stl_allocator.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/sysinfo.h",
            "%{THIRD_PARTY}/gperftools-master/src/system-alloc.h",
            "%{THIRD_PARTY}/gperftools-master/src/tcmalloc.h",
            "%{THIRD_PARTY}/gperftools-master/src/base/thread_annotations.h",
            "%{THIRD_PARTY}/gperftools-master/src/thread_cache.h",
        }
        vpaths 
        { 
            ["Header Files"] = {"**.h", "**.hpp"},
            ["Source Files"] = {"**.c", "**.cpp", "**.cc"}
        }
        includedirs
        {
            "%{THIRD_PARTY}/gperftools-master/src",
            "%{THIRD_PARTY}/gperftools-master/src/windows"
                        
        }


group "tracetool"

    project "tracetool"
        removeconfigurations "TRACE*"            
        kind "StaticLib"                    
        files
        {
            "%{THIRD_PARTY}/tracetool/**.h",
            "%{THIRD_PARTY}/tracetool/**.cpp"                
        }  

group "getopt"
        
    project "getopt"            
        kind "StaticLib"         
        defines { "STATIC_GETOPT" }            
        files
        {
            "%{THIRD_PARTY}/getopt/**.h",
            "%{THIRD_PARTY}/getopt/**.c"                            
        } 

group "jsoncpp"

    project "jsoncpp"            
        kind "StaticLib"                     
        files
        {
            "%{THIRD_PARTY}/jsoncpp/include/**.h",
            "%{THIRD_PARTY}/jsoncpp/**.cpp"      
                           
        } 
        includedirs
        {          
            "%{THIRD_PARTY}/jsoncpp/include"
        } 

group "zlib"

    project "zlib"            
        kind "StaticLib"                          
        files
        {
            "%{THIRD_PARTY}/zlib/*.h",
            "%{THIRD_PARTY}/zlib/*.c"      
                           
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
            "%{THIRD_PARTY}/zlib/*.h",
            "%{THIRD_PARTY}/zlib/*.c",
            "%{THIRD_PARTY}/zlib/win32/*.def",
            "%{THIRD_PARTY}/zlib/win32/*.rc"                               
        }        
        vpaths 
        { 
            ["Header Files"] = {"**.h", "**.hpp"},
            ["Source Files"] = {"**.c", "**.cpp"}
        }


group "libiconv"

    project "libiconv"            
        kind "StaticLib"                    
        files
        {
            "%{THIRD_PARTY}/libiconv/**.h",
            "%{THIRD_PARTY}/libiconv/**.c",
            "%{THIRD_PARTY}/libiconv/**.rc"
                           
        } 
        
    

    project "libiconv_dll"    
        targetname "libiconv" 
        kind "SharedLib"           
        defines { "_USRDLL", "LIBICONV_EXPORTS", "BUILDING_LIBICONV", "BUILDING_LIBCHARSET" }
        flags { "NoManifest" }
        files
        {
            "%{THIRD_PARTY}/libiconv/**.h",
            "%{THIRD_PARTY}/libiconv/**.c",
            "%{THIRD_PARTY}/libiconv/**.rc"
            
                           
        }   
        
group "lua"

    project "liblua"            
        kind "StaticLib"              
        files
        {
            "%{THIRD_PARTY}/lua-5.3.3/**.h",
            "%{THIRD_PARTY}/lua-5.3.3/**.c"                               
        } 
        removefiles
        {
            "%{THIRD_PARTY}/lua-5.3.3/luac.c",
            "%{THIRD_PARTY}/lua-5.3.3/lua.c"
        }  

      
        
    project "lua"            
        kind "ConsoleApp"          
           
        files
        {
            "%{THIRD_PARTY}/lua-5.3.3/**.h",
            "%{THIRD_PARTY}/lua-5.3.3/**.c"                               
        } 
        removefiles
        {
            "%{THIRD_PARTY}/lua-5.3.3/luac.c"                
        }  

    project "luac"            
        kind "ConsoleApp"                  
        
        files
        {
            "%{THIRD_PARTY}/lua-5.3.3/**.h",
            "%{THIRD_PARTY}/lua-5.3.3/**.c"                               
        } 
        removefiles
        {
            "%{THIRD_PARTY}/lua-5.3.3/lua.c"                
        } 




group "libcurl"
    project "libcurl"
        kind "SharedLib"
        defines { "BUILDING_LIBCURL", "USE_WINDOWS_SSPI", "USE_SCHANNEL", "USE_IPV6", "HTTP_ONLY" }
        files
        {
            "%{THIRD_PARTY}/curl-7.53.1/lib/amigaos.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/asyn-ares.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/asyn-thread.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/base64.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/conncache.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/connect.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/content_encoding.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/cookie.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_addrinfo.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_des.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_endian.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_fnmatch.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_gethostname.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_gssapi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_memrchr.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_multibyte.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_ntlm_core.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_ntlm_wb.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_rtmp.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_sasl.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_sspi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_threads.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/dict.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/dotdot.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/easy.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/escape.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/file.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/fileinfo.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/formdata.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/ftp.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/ftplistparser.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/getenv.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/getinfo.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/gopher.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hash.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hmac.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hostasyn.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hostcheck.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hostip.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hostip4.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hostip6.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hostsyn.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http2.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_chunks.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_digest.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_negotiate.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_ntlm.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_proxy.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/idn_win32.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/if2ip.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/imap.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/inet_ntop.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/inet_pton.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/krb5.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/ldap.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/llist.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/md4.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/md5.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/memdebug.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/mprintf.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/multi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/netrc.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/non-ascii.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/nonblock.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/openldap.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/parsedate.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/pingpong.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/pipeline.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/pop3.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/progress.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/rand.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/rtsp.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/security.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/select.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/sendf.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/share.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/slist.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/smb.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/smtp.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/socks.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/socks_gssapi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/socks_sspi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/speedcheck.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/splay.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/ssh.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strcase.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strdup.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strerror.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strtok.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strtoofft.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/system_win32.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/telnet.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/tftp.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/timeval.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/transfer.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/url.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/version.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/warnless.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/wildcard.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/x509asn1.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/cleartext.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/cram.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/digest.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/digest_sspi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/krb5_gssapi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/krb5_sspi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/ntlm.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/ntlm_sspi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/oauth2.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/spnego_gssapi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/spnego_sspi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/vauth.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/axtls.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/cyassl.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/darwinssl.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/gskit.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/gtls.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/mbedtls.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/nss.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/openssl.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/polarssl.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/polarssl_threadlock.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/schannel.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/vtls.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/amigaos.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/arpa_telnet.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/asyn.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/config-win32.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/conncache.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/connect.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/content_encoding.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/cookie.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_addrinfo.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_base64.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_des.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_endian.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_fnmatch.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_gethostname.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_gssapi.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_hmac.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_ldap.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_md4.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_md5.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_memory.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_memrchr.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_multibyte.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_ntlm_core.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_ntlm_wb.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_printf.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_rtmp.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_sasl.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_sec.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_setup.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_setup_once.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_sspi.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_threads.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curlx.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/dict.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/dotdot.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/easyif.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/escape.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/file.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/fileinfo.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/formdata.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/ftp.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/ftplistparser.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/getinfo.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/gopher.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hash.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hostcheck.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hostip.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http2.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_chunks.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_digest.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_negotiate.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_ntlm.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_proxy.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/if2ip.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/imap.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/inet_ntop.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/inet_pton.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/llist.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/memdebug.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/multihandle.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/multiif.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/netrc.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/non-ascii.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/nonblock.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/parsedate.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/pingpong.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/pipeline.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/pop3.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/progress.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/rand.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/rtsp.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/select.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/sendf.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/setup-vms.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/share.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/sigpipe.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/slist.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/smb.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/smtp.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/sockaddr.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/socks.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/speedcheck.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/splay.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/ssh.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strcase.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strdup.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strerror.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strtok.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strtoofft.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/system_win32.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/telnet.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/tftp.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/timeval.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/transfer.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/url.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/urldata.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/warnless.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/wildcard.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/x509asn1.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/digest.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/ntlm.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/vauth.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/axtls.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/cyassl.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/darwinssl.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/gskit.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/gtls.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/mbedtls.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/nssg.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/openssl.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/polarssl.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/polarssl_threadlock.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/schannel.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/vtls.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/libcurl.rc"
        }
        vpaths 
        { 
            ["Header Files"] = {"**.h", "**.hpp"},
            ["Source Files"] = {"**.c", "**.cpp"}
        }
        includedirs
        {
            "%{THIRD_PARTY}/curl-7.53.1/include",
            "%{THIRD_PARTY}/curl-7.53.1/lib"
        }
        links
        {
            "ws2_32.lib",
            "wldap32.lib",
            "Crypt32.lib"
        }


    project "libcurl_static"
        kind "StaticLib"
        defines { "BUILDING_LIBCURL", "USE_WINDOWS_SSPI", "USE_SCHANNEL", "USE_IPV6", "HTTP_ONLY" }
        files
        {
            "%{THIRD_PARTY}/curl-7.53.1/lib/amigaos.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/asyn-ares.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/asyn-thread.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/base64.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/conncache.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/connect.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/content_encoding.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/cookie.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_addrinfo.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_des.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_endian.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_fnmatch.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_gethostname.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_gssapi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_memrchr.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_multibyte.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_ntlm_core.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_ntlm_wb.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_rtmp.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_sasl.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_sspi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_threads.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/dict.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/dotdot.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/easy.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/escape.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/file.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/fileinfo.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/formdata.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/ftp.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/ftplistparser.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/getenv.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/getinfo.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/gopher.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hash.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hmac.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hostasyn.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hostcheck.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hostip.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hostip4.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hostip6.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hostsyn.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http2.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_chunks.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_digest.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_negotiate.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_ntlm.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_proxy.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/idn_win32.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/if2ip.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/imap.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/inet_ntop.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/inet_pton.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/krb5.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/ldap.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/llist.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/md4.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/md5.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/memdebug.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/mprintf.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/multi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/netrc.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/non-ascii.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/nonblock.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/openldap.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/parsedate.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/pingpong.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/pipeline.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/pop3.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/progress.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/rand.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/rtsp.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/security.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/select.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/sendf.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/share.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/slist.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/smb.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/smtp.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/socks.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/socks_gssapi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/socks_sspi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/speedcheck.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/splay.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/ssh.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strcase.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strdup.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strerror.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strtok.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strtoofft.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/system_win32.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/telnet.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/tftp.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/timeval.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/transfer.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/url.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/version.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/warnless.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/wildcard.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/x509asn1.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/cleartext.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/cram.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/digest.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/digest_sspi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/krb5_gssapi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/krb5_sspi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/ntlm.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/ntlm_sspi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/oauth2.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/spnego_gssapi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/spnego_sspi.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/vauth.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/axtls.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/cyassl.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/darwinssl.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/gskit.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/gtls.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/mbedtls.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/nss.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/openssl.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/polarssl.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/polarssl_threadlock.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/schannel.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/vtls.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/amigaos.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/arpa_telnet.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/asyn.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/config-win32.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/conncache.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/connect.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/content_encoding.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/cookie.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_addrinfo.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_base64.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_des.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_endian.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_fnmatch.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_gethostname.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_gssapi.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_hmac.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_ldap.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_md4.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_md5.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_memory.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_memrchr.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_multibyte.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_ntlm_core.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_ntlm_wb.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_printf.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_rtmp.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_sasl.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_sec.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_setup.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_setup_once.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_sspi.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_threads.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curlx.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/dict.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/dotdot.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/easyif.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/escape.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/file.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/fileinfo.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/formdata.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/ftp.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/ftplistparser.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/getinfo.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/gopher.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hash.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hostcheck.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/hostip.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http2.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_chunks.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_digest.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_negotiate.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_ntlm.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/http_proxy.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/if2ip.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/imap.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/inet_ntop.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/inet_pton.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/llist.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/memdebug.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/multihandle.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/multiif.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/netrc.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/non-ascii.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/nonblock.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/parsedate.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/pingpong.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/pipeline.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/pop3.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/progress.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/rand.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/rtsp.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/select.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/sendf.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/setup-vms.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/share.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/sigpipe.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/slist.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/smb.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/smtp.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/sockaddr.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/socks.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/speedcheck.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/splay.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/ssh.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strcase.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strdup.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strerror.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strtok.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strtoofft.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/system_win32.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/telnet.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/tftp.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/timeval.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/transfer.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/url.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/urldata.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/warnless.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/wildcard.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/x509asn1.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/digest.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/ntlm.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vauth/vauth.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/axtls.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/cyassl.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/darwinssl.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/gskit.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/gtls.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/mbedtls.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/nssg.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/openssl.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/polarssl.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/polarssl_threadlock.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/schannel.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/vtls/vtls.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/libcurl.rc"
        }
        vpaths 
        { 
            ["Header Files"] = {"**.h", "**.hpp"},
            ["Source Files"] = {"**.c", "**.cpp"}
        }
        includedirs
        {
            "%{THIRD_PARTY}/curl-7.53.1/include",
            "%{THIRD_PARTY}/curl-7.53.1/lib"
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
            "%{THIRD_PARTY}/curl-7.53.1/lib/nonblock.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strtoofft.c",
            "%{THIRD_PARTY}/curl-7.53.1/lib/warnless.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/slist_wc.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_binmode.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_bname.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_cb_dbg.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_cb_hdr.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_cb_prg.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_cb_rea.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_cb_see.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_cb_wrt.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_cfgable.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_convert.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_dirhie.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_doswin.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_easysrc.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_formparse.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_getparam.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_getpass.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_help.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_helpers.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_homedir.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_hugehelp.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_libinfo.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_main.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_metalink.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_mfiles.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_msgs.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_operate.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_operhlp.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_panykey.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_paramhlp.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_parsecfg.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_setopt.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_sleep.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_strdup.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_urlglob.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_util.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_vms.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_writeenv.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_writeout.c",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_xattr.c",
            ------------------------------------------------------------------------------------------------------------
            "%{THIRD_PARTY}/curl-7.53.1/lib/config-win32.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/curl_setup.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/nonblock.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/strtoofft.h",
            "%{THIRD_PARTY}/curl-7.53.1/lib/warnless.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/slist_wc.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_binmode.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_bname.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_cb_dbg.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_cb_hdr.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_cb_prg.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_cb_rea.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_cb_see.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_cb_wrt.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_cfgable.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_convert.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_dirhie.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_doswin.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_easysrc.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_formparse.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_getparam.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_getpass.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_helpers.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_help.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_homedir.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_hugehelp.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_libinfo.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_main.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_metalink.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_mfiles.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_msgs.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_operate.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_operhlp.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_panykey.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_paramhlp.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_parsecfg.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_sdecls.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_setopt.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_setup.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_sleep.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_strdup.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_urlglob.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_util.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_version.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_vms.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_writeenv.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_writeout.h",
            "%{THIRD_PARTY}/curl-7.53.1/src/tool_xattr.h",
            ------------------------------------------------------------------------------------------------------------
            "%{THIRD_PARTY}/curl-7.53.1/src/curl.rc",    
        }            
        vpaths 
        { 
            ["Header Files"] = {"**.h", "**.hpp"},
            ["Source Files"] = {"**.c", "**.cpp"}
        }
        includedirs
        {
            "%{THIRD_PARTY}/curl-7.53.1/include",
            "%{THIRD_PARTY}/curl-7.53.1/lib"
        }
        links
        {
            "libcurl",
            "ws2_32.lib",
            "wldap32.lib"
        }   


group "xml"

    project "pugixml"                     
        kind "StaticLib"                    
        files
        {
            "%{THIRD_PARTY}/pugixml/**.hpp",
            "%{THIRD_PARTY}/pugixml/**.cpp"                
        }  

    project "tinyxml"            
        kind "SharedLib"            
        defines { "_WINDOWS", "_DLL", "TINY_EXPORTS", "TINYXML_DLL" }      
        flags { "NoManifest" }                        
        files
        {
            "%{THIRD_PARTY}/tinyxml/**.h",
            "%{THIRD_PARTY}/tinyxml/**.cpp",
            "include/buildcfg/version.rc",
            "include/buildcfg/versionno.rc2",                
            "include/buildcfg/buildcfg.h"
        }             
        
    project "tinyxml2"            
        kind "StaticLib"      
        files
        {
            "%{THIRD_PARTY}/tinyxml2/**.h",
            "%{THIRD_PARTY}/tinyxml2/**.cpp",            
        }

group "libQREncode"

    project "libQREncode"            
        kind "StaticLib"            
        defines { "_WINDOWS", "HAVE_CONFIG_H" }                                    
        files
        {
            "%{THIRD_PARTY}/libQREncode/**.h",
            "%{THIRD_PARTY}/libQREncode/**.cpp",  
            "%{THIRD_PARTY}/libQREncode/**.c"
        }             
        removefiles
        {
            "%{THIRD_PARTY}/libQREncode/qrenc.c",
            "%{THIRD_PARTY}/libQREncode/libQREncode.h",
            "%{THIRD_PARTY}/libQREncode/libQREncode.cpp"
        }

group "cryptopp"

    project "cryptopp565"            
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
                "%{THIRD_PARTY}/cryptopp565/**.h",
                "%{THIRD_PARTY}/cryptopp565/**.c",
                "%{THIRD_PARTY}/cryptopp565/**.cpp",
                "%{THIRD_PARTY}/cryptopp565/**.asm"                    
            }   
            removefiles
            {
                "%{THIRD_PARTY}/cryptopp565/test.cpp",
                "%{THIRD_PARTY}/cryptopp565/fipstest.cpp",
                "%{THIRD_PARTY}/cryptopp565/fipsalgt.cpp",
                "%{THIRD_PARTY}/cryptopp565/regtest.cpp",
                "%{THIRD_PARTY}/cryptopp565/bench*.cpp",
                "%{THIRD_PARTY}/cryptopp565/eccrypto.cpp",
                "%{THIRD_PARTY}/cryptopp565/eprecomp.cpp",                    
                "%{THIRD_PARTY}/cryptopp565/dlltest.cpp",
                "%{THIRD_PARTY}/cryptopp565/datatest.cpp",
                "%{THIRD_PARTY}/cryptopp565/validat*.cpp",
                "%{THIRD_PARTY}/cryptopp565/x64dll.asm",
                "%{THIRD_PARTY}/cryptopp565/x64masm.asm" 
            }
        filter "platforms:x64"
           -- defines { "WIN32", "_WINDOWS", "_USRDLL", "OLSDLL_EXPORTS", "OLS_WIN_RING0" }
            files
            {
                "%{THIRD_PARTY}/cryptopp565/**.h",
                "%{THIRD_PARTY}/cryptopp565/**.c",
                "%{THIRD_PARTY}/cryptopp565/**.cpp",
                "%{THIRD_PARTY}/cryptopp565/**.asm"                   
            }
            removefiles
            {
                "%{THIRD_PARTY}/cryptopp565/test.cpp",
                "%{THIRD_PARTY}/cryptopp565/fipstest.cpp",
                "%{THIRD_PARTY}/cryptopp565/fipsalgt.cpp",
                "%{THIRD_PARTY}/cryptopp565/regtest.cpp",
                "%{THIRD_PARTY}/cryptopp565/bench*.cpp",
                "%{THIRD_PARTY}/cryptopp565/eccrypto.cpp",
                "%{THIRD_PARTY}/cryptopp565/eprecomp.cpp",                    
                "%{THIRD_PARTY}/cryptopp565/dlltest.cpp",
                "%{THIRD_PARTY}/cryptopp565/datatest.cpp",
                "%{THIRD_PARTY}/cryptopp565/validat*.cpp"
                
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
          

    project "cryptopp"            
        kind "StaticLib"            
        characterset "MBCS"
        defines { "_WINDOWS", "USE_PRECOMPILED_HEADERS", "WIN32" }                                    
        
        filter { "platforms:Win32" }
           -- defines { "WIN32", "_WINDOWS" }                 
            files
            {
                "%{THIRD_PARTY}/cryptopp/**.h",
                "%{THIRD_PARTY}/cryptopp/**.c",
                "%{THIRD_PARTY}/cryptopp/**.cpp",
                "%{THIRD_PARTY}/cryptopp/**.asm"                    
            }   
            removefiles
            {
                "%{THIRD_PARTY}/cryptopp/cryptlib_bds.cpp",
                "%{THIRD_PARTY}/cryptopp/test.cpp",
                "%{THIRD_PARTY}/cryptopp/fipstest.cpp",
                "%{THIRD_PARTY}/cryptopp/fipsalgt.cpp",
                "%{THIRD_PARTY}/cryptopp/regtest.cpp",
                "%{THIRD_PARTY}/cryptopp/bench*.cpp",
                "%{THIRD_PARTY}/cryptopp/eccrypto.cpp",
                "%{THIRD_PARTY}/cryptopp/eprecomp.cpp",                    
                "%{THIRD_PARTY}/cryptopp/dlltest.cpp",
                "%{THIRD_PARTY}/cryptopp/datatest.cpp",
                "%{THIRD_PARTY}/cryptopp/validat*.cpp",
                "%{THIRD_PARTY}/cryptopp/x64dll.asm",
                "%{THIRD_PARTY}/cryptopp/x64masm.asm" 
            }
            pchsource "%{THIRD_PARTY}/cryptopp/pch.cpp"
            pchheader "pch.h"   
            filter "files:%{THIRD_PARTY}/cryptopp/iterhash.cpp"
                flags { "NoPCH" }
            filter "files:%{THIRD_PARTY}/cryptopp/dll.cpp"
                flags { "NoPCH" }
            filter "files:%{THIRD_PARTY}/cryptopp/adhoc.cpp"
                flags { "NoPCH" }

        filter "platforms:x64"
           -- defines { "WIN32", "_WINDOWS", "_USRDLL", "OLSDLL_EXPORTS", "OLS_WIN_RING0" }
            files
            {
                "%{THIRD_PARTY}/cryptopp/**.h",
                "%{THIRD_PARTY}/cryptopp/**.c",
                "%{THIRD_PARTY}/cryptopp/**.cpp",
                "%{THIRD_PARTY}/cryptopp/**.asm"                   
            }
            removefiles
            {
                "%{THIRD_PARTY}/cryptopp/cryptlib_bds.cpp",
                "%{THIRD_PARTY}/cryptopp/test.cpp",
                "%{THIRD_PARTY}/cryptopp/fipstest.cpp",
                "%{THIRD_PARTY}/cryptopp/fipsalgt.cpp",
                "%{THIRD_PARTY}/cryptopp/regtest.cpp",
                "%{THIRD_PARTY}/cryptopp/bench*.cpp",
                "%{THIRD_PARTY}/cryptopp/eccrypto.cpp",
                "%{THIRD_PARTY}/cryptopp/eprecomp.cpp",                    
                "%{THIRD_PARTY}/cryptopp/dlltest.cpp",
                "%{THIRD_PARTY}/cryptopp/datatest.cpp",
                "%{THIRD_PARTY}/cryptopp/validat*.cpp"
                
            }            
            pchsource "%{THIRD_PARTY}/cryptopp/pch.cpp"
            pchheader "pch.h"   
            filter "files:%{THIRD_PARTY}/cryptopp/iterhash.cpp"
                flags { "NoPCH" }
            filter "files:%{THIRD_PARTY}/cryptopp/dll.cpp"
                flags { "NoPCH" }
            filter "files:%{THIRD_PARTY}/cryptopp/adhoc.cpp"
                flags { "NoPCH" } 
                   
                      
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


group "cactus"

    project "cactus"            
        kind "SharedLib"            
        defines { "_WINDOWS", "_USRDLL", "DLL_EXPORTS", "CACTUS_DLL", "CACTUS_EXPORTS" }               
        buildoptions { "/EHsc", "/Zm200" }   
        flags { "NoManifest" }
        files
        {
            "%{THIRD_PARTY}/cactus/**.h",
            "%{THIRD_PARTY}/cactus/**.cpp",  
            "%{THIRD_PARTY}/cactus/**.c",
            "%{BOOK_CODE_PATH}/include/buildcfg/vs2015/dll_version.rc",
            "%{BOOK_CODE_PATH}/include/buildcfg/vs2015/versionno.rc2",                
            "%{BOOK_CODE_PATH}/include/buildcfg/vs2015/buildcfg.h"
        }             
        removefiles
        {
            --"%{THIRD_PARTY}/cactus/mongoose/**",
            "%{THIRD_PARTY}/cactus/civetweb/**",
            "%{THIRD_PARTY}/cactus/cactus_sqlite.cpp",
            "%{THIRD_PARTY}/cactus/cactus_curl.cpp",
            "%{THIRD_PARTY}/cactus/cactus_rest.cpp",
            "%{THIRD_PARTY}/cactus/7zHelper.cpp",
            "%{THIRD_PARTY}/cactus/cactus_7z.cpp",
            "%{THIRD_PARTY}/cactus/cactus_cmdline.cpp",
            "%{THIRD_PARTY}/cactus/cactus_szlib.cpp",
            "%{THIRD_PARTY}/cactus/cactus_xunlei.cpp",
            "%{THIRD_PARTY}/cactus/log_impl.cpp",
            "%{THIRD_PARTY}/cactus/PropVariant.cpp"
        }
        includedirs
        {            
            "3rdparty"    
        }                
        

group "duilib"

    project "duilib"        
        kind "SharedLib"            
        defines { "_WINDOWS", "UILIB_EXPORTS", "DUILIB_DLL" }               
        flags { "NoManifest" }                       
        files
        {
            "%{THIRD_PARTY}/DuiLib/**.h",
            "%{THIRD_PARTY}/DuiLib/**.cpp",  
            "%{THIRD_PARTY}/DuiLib/**.c",
            "%{BOOK_CODE_PATH}/include/buildcfg/vs2015/dll_version.rc",
            "%{BOOK_CODE_PATH}/include/buildcfg/vs2015/versionno.rc2",                
            "%{BOOK_CODE_PATH}/include/buildcfg/vs2015/buildcfg.h"
        }                    
        includedirs
        {          
            "%{THIRD_PARTY}/DuiLib", 
            "%{THIRD_PARTY}",     
        }            
        links
        {
            "cactus.lib"              
        }        
        pchsource "%{THIRD_PARTY}/DuiLib/StdAfx.cpp"
        pchheader "StdAfx.h"
        filter "files:**.c"
            flags { "NoPCH" }
        filter "files:%{THIRD_PARTY}/DuiLib/Utils/XUnzip.cpp"
            flags { "NoPCH" }

--    function include_win32_common()
--        files { 
--            "api/win32_common.h",
--            "api/win32_common.cpp"
--        }
--        filter "files:api/win32_common.cpp"
--            flags { "NoPCH" }    
--    end
