BOOK_CODE_PATH = "H:/rover/rover-self-work/cpp/book-code"
THIRD_PARTY = "H:/rover/rover-self-work/cpp/book-code/3rdparty"
WORK_PATH = os.getcwd()
print(WORK_PATH)

includeexternal (BOOK_CODE_PATH .. "/premake-vs-include.lua")
    

workspace "gtest-gmock-glog-gflags"
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"    
    if _ACTION == "vs2015" then
        toolset "v140_xp"
    end

    
    group "gtest"

        project "gtest"            
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

    


    

    