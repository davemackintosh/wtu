set_project("wtu")
add_rules("plugin.compile_commands.autoupdate", { outputdir = "." })

-- language specifics.
set_languages("c++23")
add_includedirs("include")
set_warnings("all", "error")

-- Dependencies.
add_requires(
	"vcpkg::sdl2[vulkan,wayland,alsa]",
	"vcpkg::vulkan-memory-allocator-hpp"
)

target("wtu")
do
	set_kind("binary")
	add_files("src/*.cpp")
	add_files("src/ecs/*.cpp")
	add_packages("vcpkg::sdl2", "vcpkg::vulkan-memory-allocator-hpp")
end
