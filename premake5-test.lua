local p = premake

workspace "test"
    language "C++"
    location "build/%{_ACTION}"
    targetdir "bin"

    configurations { "Debug", "Release", "TRACE", "TRACE_MT" }
    platforms { "Win32", "x64" }    

    filter { "kind:StaticLib", "platforms:Win32" }
        targetdir "lib/x86" 
    filter { "kind:StaticLib", "platforms:x64" }
        targetdir "lib/x64" 
    filter { "kind:SharedLib", "platforms:Win32" }
        implibdir "lib/x86" 
    filter { "kind:SharedLib", "platforms:x64" }
        implibdir "lib/x64" 
    filter { "kind:ConsoleApp or WindowedApp or SharedLib", "platforms:Win32" }
        targetdir "bin/x86" 
    filter { "kind:ConsoleApp or WindowedApp or SharedLib", "platforms:x64" }
        targetdir "bin/x64" 

    filter { "platforms:Win32" }
        system "Windows"
        architecture "x32"
        libdirs 
        {
            "lib/x86",
            "lib/x86/boost",
            "bin/x86"            
        }
    
    filter { "platforms:x64" }
        system "Windows"
        architecture "x64"   
        libdirs
        {
            "lib/x64",
            "lib/x64/boost",
            "bin/x64"
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
        links { "tracetool.lib" }         

    filter "configurations:TRACE_MT"
        defines { "NDEBUG", "TRACE_TOOL" }
        flags { "Symbols" }
        optimize "On"  
        buildoptions { "/Od" }  
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

    function create_project(name)        
        project(name)          
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
    end
   
    group "09 底层"

                 
            
--        project "test-semaphore"          
--            kind "WindowedApp"                            
--            characterset "MBCS"
--            flags { "NoManifest", "WinMain", "StaticRuntime" }       
--            defines {  }
--            files
--            {                                  
--                "src/test/%{prj.name}/**.h",
--                "src/test/%{prj.name}/**.cpp", 
--                "src/test/%{prj.name}/**.rc" 
--            }
--            removefiles
--            {               
--            }
--            includedirs
--            {                   
--                "3rdparty"   
--            }      -        
            

            matches = os.matchdirs("src/test/*")
            for i = #matches, 1, -1 do
                --p.w(path.getname(matches[i]))  
                create_project(path.getname(matches[i]))
            end
        






        