workspace "pattern-examples"
    language "C++"
    location "build/%{_ACTION}"    
    flags { "StaticRuntime", "C++14" }
    buildoptions
    {
        --"-finput-charset=GBK",
        --"-fexec-charset=GBK",
        --"-fwide-exec-charset=UTF-16LE"
    }
    linkoptions
    {
        --"-static-libgcc",
        --"-static-libstdc++"
        "-static"
       
    }
		

    configurations { "Debug", "Release", "TRACE", "TRACE_MT" }
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
        architecture "x32"
        libdirs 
        {
            "lib/x86/%{_ACTION}",
            "lib/x86/%{_ACTION}/boost-1_56",
            "lib/x86/%{_ACTION}/boost-1_60",
            "bin/x86/%{_ACTION}"            
        }
    
    filter { "platforms:x64" }        
        architecture "x64"   
        libdirs
        {
            "lib/x64/%{_ACTION}",
            "lib/x86/%{_ACTION}/boost-1_56",
            "lib/x64/%{_ACTION}/boost-1_60",
            "bin/x64/%{_ACTION}"
        }
   
    filter "configurations:Debug"        
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
            "_WIN32_WINNT=0x601",           -- 支持到 win7
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


    function create_console_project(name, dir)
        project(name)                   
        kind "ConsoleApp"          
        files
        {
            dir .. "/%{prj.name}/**.h",
            dir .. "/%{prj.name}/**.cpp", 
            dir .. "/%{prj.name}/**.c", 
            dir .. "/%{prj.name}/**.rc",
            dir .. "/%{prj.name}/**.rc2"         
        }
        removefiles
        {               
        }
        includedirs
        {   
            "3rdparty"
        }
        vpaths 
        {
            ["Headers"] = { "**.h", "**.hxx", "**.hpp" },
            ["Sources"] = { "**.cpp", "**.c", "**.rc", "**.idl", "**.def" },
            ["Resource"] = { "**.rc2" },
        }
       
    end

    function create_dll_project(name, dir)
        project(name)
		kind "SharedLib"
		language "C++"        
        defines { "BUILDING_DLL" }  
		files 
        { 
            dir .. "/%{prj.name}/**.h",
            dir .. "/%{prj.name}/**.cpp"            
        }
        links
        {
            "DevCppDLL"
        }
    end
    
        
--	project "test-DevCppDLL"
--		kind "ConsoleApp"
--		language "C++"        
--        --defines { "BUILDING_DLL" }        
--        --targetdir "bin/x86/%{_ACTION}/%{wks.name}" 
--        --implibdir "lib/x86/%{_ACTION}"
--		files 
--        { 
--            "gmake-examples/test-DevCppDLL/**.h", 
--            "gmake-examples/test-DevCppDLL/**.cpp" 
--        }	
--        links
--        {
--            "DevCppDLL"
--        }


    
    --create_console_project("singleton", "pattern-examples") 
    --create_console_project("factory", "pattern-examples") 

    matches = os.matchdirs("pattern-examples/*")
    for i = #matches, 1, -1 do        
        create_console_project(path.getname(matches[i]), "pattern-examples")
    end
            