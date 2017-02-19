includeexternal ("premake-vs-include.lua")

workspace "basic-test"
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"    
    if _ACTION == "vs2015" then
        toolset "v140_xp"
    end


    create_mfc_console_project("test_filesystem", "src")
    create_console_project("test_unix_time", "src")  
    create_console_project("test_enum_registry", "src")  
        