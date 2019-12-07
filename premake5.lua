workspace "cfx-workspace"	
	language "C++"
	
	architecture "x86_64"
	systemversion "latest"
	
	configurations { "Debug", "Release" }
	
	defines{}

	filter { "configurations:Debug" }
		symbols "On"
		defines{"DEBUG"}

	filter { "configurations:Release" }
        optimize "On"
		defines{"NDEBUG"}
	
	filter { }
	
	targetdir ("bin/")
    objdir ("bin/")
    
project "cfx"
	kind "StaticLib"
	
	newoption {
		trigger     = "with-opengl",
		description = "Force the use of OpenGL for rendering"
	 }

	 newaction {
		trigger     = "install",
		description = "Install cfx",
		execute = function ()
			print("Installing cfx")
				os.execute("install -d /usr/local/lib/")
				os.execute("install -m 644 ./bin/libcfx.a /usr/local/lib/")
				os.execute("install -d /usr/local/include/cfx")
				os.execute("install -m 644 ./include/*.h /usr/local/include/cfx/")
		end
	 }

	includedirs {"include", "."}

	files "src/*"
	files "include/*.h"

	libdirs "libs/"

	configuration "with-opengl"
		defines{"OPENGL_ACCELERATED"}
		files "src/gl/**"
	 	links {"glfw3"}

	configuration "not with-opengl"
		files "src/minifb/*"
		links {"minifb"}

	filter {}

project "basic-test"
	kind "ConsoleApp"
	
	includedirs {"include", "."}
	files "tests/basic.c"
	libdirs {"bin/", "libs/", "./modules/glfw/build/src/Release/"}

	configuration "with-opengl"
		defines{"OPENGL_ACCELERATED"}
		files "src/gl/*"
		links {"cfx","glfw3", "X11", "dl", "Xrandr", "Xxf86vm", "Xinerama", "Xcursor", "pthread"}

	configuration "not with-opengl"
		files "src/minifb/*"
		links {"cfx","minifb","X11"}

	filter {}

project "sine-test"
	kind "ConsoleApp"
	
	includedirs {"include", "."}
	files "tests/sine-test.c"
	libdirs "bin/"

	configuration "with-opengl"
		defines{"OPENGL_ACCELERATED"}
		files "src/gl/*"
	 	links {"cfx","glfw3", "X11", "dl", "Xrandr", "Xxf86vm", "Xinerama", "Xcursor", "pthread"}

	configuration "not with-opengl"
		files "src/minifb/*"
		links {"cfx","minifb","X11"}

	links {"cfx","X11"}

	filter {}