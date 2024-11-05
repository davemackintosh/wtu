set_project("wtu")
add_rules("plugin.compile_commands.autoupdate", { outputdir = "." })

if is_mode("debug") then
	set_symbols("debug")
	set_optimize("none")
	add_defines("DEBUG")
end

-- language specifics.
set_languages("c++23")
add_includedirs("include")
set_warnings("all", "error")

local sdl_features = {
	"vulkan"
}
if is_plat("linux") then
	table.insert(sdl_features, "wayland")
	table.insert(sdl_features, "alsa")
end

-- Dependencies.
add_requires(
	"vcpkg::sdl2", { configs = { features = sdl_features } },
	"vcpkg::vulkan-memory-allocator-hpp"
)

add_requires("vcpkg::catch2")

target("wtu-core")
do
	set_kind("shared")
	add_files("src/*.cpp")
	add_files("src/ecs/*.cpp")
	add_packages("vcpkg::sdl2", "vcpkg::vulkan-memory-allocator-hpp")
end
target_end()

target("wtu-linux")
do
	set_kind("binary")
	add_files("src/platforms/linux/*.cpp")
	add_deps("wtu-core")
end
target_end()

target("test_wtu")
do
	set_kind("binary")
	add_files("test/*.cpp")
	add_packages("vcpkg::catch2")
	add_deps("wtu-core")
end
target_end()
