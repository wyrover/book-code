includeexternal ("premake5-include.lua")

workspace "csharp"
    location "build/%{_ACTION}/%{wks.name}" 
    configurations { "Debug", "Release" }

    filter { "kind:ConsoleApp or WindowedApp or SharedLib" }
        targetdir "bin/%{_ACTION}/%{wks.name}"         
                
        project "WindowsFormsApplication1"            
            kind "WindowedApp"                         
            language "C#"               
            files
            {
               
                "src/%{wks.name}/%{prj.name}/**.cs",
                "src/%{wks.name}/%{prj.name}/Properties/**",
                "src/%{wks.name}/%{prj.name}/**.config",
              
            }
            removefiles
            {          
                
            
            }
            links {
                "Microsoft.CSharp",
                "System",
                "System.Core",
                "System.Data",
                "System.Data.DataSetExtensions",
                "System.Deployment",
                "System.Drawing",
                "System.Net.Http",
                "System.Windows.Forms",
                "System.Xml",
                "System.Xml.Linq"
            }
                    

        
        