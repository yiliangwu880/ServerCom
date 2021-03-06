--
-- premake5 file to build RecastDemo
-- http://premake.github.io/
--

local action = _ACTION or ""
local outdir = action

workspace  "sulustion"
	configurations { 
		"Debug",
		--"Release"
	}
	location (outdir)


project "prj"
	language "C++"
	kind "ConsoleApp"


	includedirs { 
		"../dep/gflags/include/",
	}
	files	{ 
		"../src/**.cpp",  --**递归所有子目录，指定目录可用 "cc/*.cpp" 或者  "cc/**.cpp"
		"../src/**.h",
		"../src/**.cc",
		"../dep/**.h",
	}
