includeexternal ("premake-vs-include.lua")

workspace "boost-test"
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"    
    if _ACTION == "vs2015" then
        toolset "v140_xp"
    end    
   
    group "boost-test"  

        matches = os.matchdirs("src/test/boost/*")
        for i = #matches, 1, -1 do
            --p.w(path.getname(matches[i]))  
            create_console_charset_project(path.getname(matches[i]), "src/test/boost", "mbcs")
        end
        






        