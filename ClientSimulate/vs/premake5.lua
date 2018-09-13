--
-- premake5 file to build RecastDemo
-- http://premake.github.io/
--

local action = _ACTION or ""
local outdir = action

workspace  "ClientSimulate"
	configurations { 
		"Debug",
		--"Release"
	}
	location (outdir)
    
project "ClientSimulate"
	language "C++"
	kind "ConsoleApp"
	files	{ 
		"../*.txt",
	}
    
project "lib_prj"
	language "C++"
	kind "ConsoleApp"


	includedirs { 
		"../external/include/",
		"../external/",
	}
	files	{ 
		"../lib_prj/**.cpp",  --**递归所有子目录，指定目录可用 "cc/*.cpp" 或者  "cc/**.cpp"
		"../lib_prj/**.cc",
		"../lib_prj/**.h",
		"../lib_prj/**.hpp",
		"../lib_prj/**.txt",
		"../external/include/event2/**.h", 
	}

project "main_prj"
	language "C++"
	kind "ConsoleApp"


	includedirs { 
		"../external/include/",
		"../lib_prj/",
	}
	files	{ 
		"../main_prj/**.cpp",  --**递归所有子目录，指定目录可用 "cc/*.cpp" 或者  "cc/**.cpp"
		"../main_prj/**.cc",
		"../main_prj/**.h",
		"../main_prj/**.hpp",
		"../main_prj/**.txt",
		"../lib_prj/*.h", 
	}