--
-- premake5 file to build RecastDemo
-- http://premake.github.io/
--

local action = _ACTION or ""
local outdir = action

workspace  "redisClientPrj"
	configurations { 
		"Debug",
		--"Release"
	}
	location (outdir)


project "redisClient"
	language "C++"
	kind "ConsoleApp"


	includedirs { 
		"../dep/hiredis",
	}
	files	{ 
		"../src/**.cpp",  --**递归所有子目录，指定目录可用 "cc/*.cpp" 或者  "cc/**.cpp"
		"../src/**.cc",
		"../src/**.h",
		"../src/**.hpp",
		"../dep/**.h", 
	}

