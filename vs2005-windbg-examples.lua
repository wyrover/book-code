includeexternal ("premake-vs-include.lua")

workspace "windbg-examples"
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"    
    if _ACTION == "vs2015" then
        toolset "v140_xp"
    end
      
    
    group "windbg-examples"                          
        
        create_wtl_project("wtl_dialog_template", "windbg-examples")

        project "dll-template"            
            kind "SharedLib"             
            defines { "BUILDING_DLL" }
            files
            {
                "windbg-examples/%{prj.name}/*.def",   
                "windbg-examples/%{prj.name}/**.h",
                "windbg-examples/%{prj.name}/**.cpp", 
                "windbg-examples/%{prj.name}/**.c"
                
            }   
            includedirs
            {                   
                
            }
                
        create_console_project("handleinfo", "windbg-examples")
        create_console_project("sysinfo", "windbg-examples")
        create_console_project("memstat", "windbg-examples")
        create_console_project("winsat", "windbg-examples")
        