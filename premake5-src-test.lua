includeexternal ("premake-vs-include.lua")

workspace "test" 
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"    

    group "02 test"          
        
        project "api-test"                    
            kind "ConsoleApp"                                     
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


        create_console_project("boost-write-sharedmemery-test", "src/test")             
        create_console_project("boost-read-sharedmemery-test", "src/test")       
        
        project "msgpack-test"            
            kind "ConsoleApp"            
            files
            {
                "src/test/msgpack-test/**.h",
                "src/test/msgpack-test/**.cpp"
                      
            }            
            includedirs
            {              
                "3rdparty/msgpack-c/include",
                "3rdparty"
            }
            links
            { 
                "msgpack_mt.lib"
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


        create_wtl_project("wtl_dialog_template", "src/test")
        create_wtl_project("dectet-process", "src/test")   
        

                  
        
        project "clean-driverstore"          
            kind "ConsoleApp"    
                 
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
            
        project "openssl-tool"            
            kind "ConsoleApp"                                        
            files
            {
                "src/test/openssl-tool/**.h",
                "src/test/openssl-tool/**.cpp",
                "src/test/openssl-tool/app.rc",
                "3rdparty/getopt/**.h",
                "3rdparty/getopt/**.c"                            
            }            
            includedirs
            {              
                "3rdparty/getopt",      
                "3rdparty/msgpack-c/include",
                "3rdparty/openssl-1.0.2h/include",
                "include",
                "include/buildcfg"
            }
            links
            { 
                "msgpack.lib",
                "libeay32.lib",
                "ssleay32.lib"
            }
            libdirs
            {
                "product/win32",
                "3rdparty/openssl-1.0.2h/lib"
            } 
          
           
        project "dll_load_dll_template"            
            kind "SharedLib"           
            files
            {
                "src/test/%{prj.name}/*.h",
                "src/test/%{prj.name}/*.c",
                "src/test/%{prj.name}/*.cpp",
                "src/test/%{prj.name}/*.rc"  
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

        






        