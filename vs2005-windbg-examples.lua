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
        create_mfc_project("MySpy", "windbg-examples")
        create_mfc_project("PEShadow", "windbg-examples")
        create_mfc_project("FileRecover", "windbg-examples")
        create_console_project("oless", "windbg-examples")  
        create_console_project("GetChildProcessOutput", "windbg-examples")   
        create_mfc_project("ReadEnvironmentSample", "windbg-examples", "mbcs")
        

    group "DllInject"
        create_console_project("DllInject", "windbg-examples/DllInject") 
        create_dll_project("InjectDLL", "windbg-examples/DllInject")

    group "XPEInfo"
        create_mfc_project("XPEInfo_win", "windbg-examples/XPEInfo")
        create_console_project("XPEInfo_console", "windbg-examples/XPEInfo")

    group "AntiSpy"
        create_sys_project("AntiSpy_Sys", "windbg-examples")
        create_mfc_project("AntiSpy", "windbg-examples")

    group "detours"
        project "detours"            
            kind "StaticLib"    
            defines 
            { 
                "ASMJIT_EMBED",
                "ASMJIT_BUILD_X86",               
                "_LIB"
            }
            files
            {
                "windbg-examples/detours/detours/**.h",
                "windbg-examples/detours/detours/**.cpp",
                
                
            }             
            removefiles
            {
                "windbg-examples/detours/detours/asmjit/asmjit/test/**", 
                "windbg-examples/detours/detours/asmjit/asmtk/test/**", 
            }
            includedirs
            {          
                "windbg-examples/detours/detours/asmjit/asmjit/src",
                "windbg-examples/detours/detours/asmjit/asmtk/src", 
            }                
            links
            {
                
            }
            libdirs
            {
               
            }

    group "crack"

        create_console_project("virtual-mem-address", "windbg-examples")


        matches = os.matchdirs("windbg-examples/crack-*")
        for i = #matches, 1, -1 do
            --p.w(path.getname(matches[i]))  
            create_console_project(path.getname(matches[i]), "windbg-examples")
        end


        