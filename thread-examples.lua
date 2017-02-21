includeexternal ("premake-vs-include.lua")

workspace "thread-examples"
    language "C++"
    location "build/%{_ACTION}/%{wks.name}"    
    if _ACTION == "vs2015" then
        toolset "v140_xp"
    end

    

    
   
    group "thread-examples"  

        matches = os.matchdirs("thread_examples/*")
        for i = #matches, 1, -1 do
            --p.w(path.getname(matches[i]))  
            create_console_project(path.getname(matches[i]), "thread_examples")
        end
        






        