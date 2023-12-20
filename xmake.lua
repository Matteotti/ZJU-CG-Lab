set_project("ZJU-CG-LAB")

add_rules("mode.debug", "mode.release")
set_policy("build.warning", true)
set_languages("cxx17")
set_allowedplats("windows", "linux")
add_requires("imgui", {configs = {glfw_opengl3 = true}})
add_requires("glfw", "glad", "glm", "stb", "yaml-cpp")

if is_plat("windows") then
    add_defines("_CRT_SECURE_NO_WARNINGS")
end

target("EngineRuntime")
    set_kind("static")
    set_warnings("all", "extra")
    add_includedirs("src/Engine", {public = true})
    add_files("src/Engine/**.cpp")
    add_packages("glfw", "glad", "glm", "stb", "yaml-cpp")
    after_build(
        function (target)
            os.cp("assets", "bin/")
        end)
target_end()

target("Test")
    set_kind("binary")
    set_warnings("all", "extra")
    add_includedirs("src/Test")
    add_files("src/Test/**.cpp")
    add_deps("EngineRuntime")
    after_build(
        function (target)
            os.cp(target:targetfile(), "bin/")
            os.cp("assets", "bin/")
        end)
target_end()

target("Editor")
    set_kind("binary")
    set_warnings("all", "extra")
    add_includedirs("src/Editor")
    add_files("src/Editor/**.cpp")
    add_packages("imgui", "glad")
    add_deps("EngineRuntime")
    after_build(
        function (target)
            os.cp(target:targetfile(), "bin/")
            os.cp("assets", "bin/")
        end)
target_end()
