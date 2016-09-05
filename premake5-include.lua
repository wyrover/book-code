--[========================================================================[
  Helper functions for premake projects
  usage: 
    StartProject("MyProjectName", "ConsoleApp") -- sets up a new Project
    AddProjectSource("Source/Path")             -- adds source files to project
--]========================================================================]

-- Handle no valid action.
if not _ACTION then error("No action defined!") end



-- DEFAULT GLOBAL CONFIGS --
----------------------------
-- NOTE: DO NOT MODIFY HERE. Instead copy into premake5.lua then modify there.

-- The path from premake5.lua to get to the project root.
ROOT = "./" 
-- The folder that will contain the project/solution files
WORKSPACE = ROOT .. "project_" .. os.get() .. "_" .. _ACTION .. "/"
-- If compile/link warnings should be treated as errors
TREAT_WARNINGS_AS_ERRORS = false;




function StartProject(proj_name, proj_kind)
  project(proj_name)
    kind(proj_kind)
    targetname(proj_name)            -- The name of our executable
    language   "C++"
    -- The binaries ar output inside the project. If you want them in the root, just prepend: ROOT ..
    targetdir(WORKSPACE .. "bin_" .. os.get() .. "_" .. proj_name .."_%{cfg.buildcfg}_%{cfg.platform}/");

  -- ARCHITECTURE/CONFIGURATIONS --
  ---------------------------------
    filter { "platforms:*32" } architecture "x86"
    filter { "platforms:*64" } architecture "x64"
    
    filter { "configurations:Debug" }   defines { "DEBUG" }  flags { "Symbols" }
    filter { "configurations:Release" } defines { "NDEBUG" } optimize "On"

    filter {} -- clear filter


  -- COMPILER/LINKER --
  ---------------------
    if TREAT_WARNINGS_AS_ERRORS then flags { "FatalWarnings" } end -- All warnings on.

    local using_clang = false      -- workaround for premake issue #257

    filter { "action:gmake" }
      toolset "clang"
      using_clang = true
      buildoptions { "-std=c++11" }

    filter { "system:windows", "action:vs*"}
      flags         { "MultiProcessorCompile", "NoMinimalRebuild" }
      linkoptions   { "/ignore:4099" }      -- Ignore library pdb warnings when running in debug

    filter {"system:linux"}
      -- http://libcxx.llvm.org/
      if(using_clang) then -- workaround for premake issue #257
        buildoptions { "-stdlib=libc++" } 
        linkoptions  { "-stdlib=libc++" }
      end

    filter {} -- reset filter
end


-- exclude relevant files from the project
local function FilterProjectSource()
  -- Exclude template files from project (so they don't accidentally get compiled)
  filter { "files:**.tpp" }
    flags {"ExcludeFromBuild"}

  -- Exclude files for other operating systems (so they don't accidentally get compiled)
  filter { "system:macosx", "files:**_windows.* or files:**_linux.*" }
    flags { "ExcludeFromBuild" }
  filter { "system:windows", "files:**_macosx.* or files:**_linux.*" }
    flags { "ExcludeFromBuild"}
  filter { "system:linux", "files:**_macosx.* or files:**_windows.*" }
    flags { "ExcludeFromBuild"}
    
  filter {} -- clear filter!
end


-- add files to be included (and possibly compiled)
function AddProjectSource(src_path, is_included)
  
  if(is_included == nil ) then is_included = true end

  files
  {
    src_path .. "**.cpp",
    src_path .. "**.hpp",
    src_path .. "**.h",
    src_path .. "**.c",
    src_path .. "**.tpp",
  }

  FilterProjectSource()

  -- Include the root to allow for nice absolute include paths
  if(is_included) then
    includedirs { src_path }
  end
end