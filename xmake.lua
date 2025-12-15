set_project("cpp-quick-starter")
set_version("0.1.0")

set_languages("cxx20")
add_rules("mode.debug", "mode.release")

option("build_tests")
  set_default(true)
  set_showmenu(true)
  set_description("Build unit/integration tests")
option_end()

option("build_benchmarks")
  set_default(false)
  set_showmenu(true)
  set_description("Build benchmarks")
option_end()

option("build_examples")
  set_default(true)
  set_showmenu(true)
  set_description("Build examples")
option_end()

target("cpp_quick_starter")
  set_kind("static")
  add_headerfiles("include/(project_name/**.hpp)")
  add_files("src/core/**.cpp", "src/utils/**.cpp")
  add_includedirs("include", {public = true})
  if is_mode("debug") then
    add_defines("CPP_QUICK_STARTER_DEBUG", {public = true})
  end

  if is_plat("windows") then
    add_cxflags("/W4")
  else
    add_cxflags("-Wall", "-Wextra", "-Wpedantic")
  end

target_end()

target("cpp_quick_starter_app")
  set_kind("binary")
  add_files("src/main.cpp")
  add_deps("cpp_quick_starter")

target_end()

if has_config("build_examples") then
  target("example_01")
    set_kind("binary")
    add_files("examples/example_01.cpp")
    add_deps("cpp_quick_starter")

  target_end()
end

if has_config("build_tests") then
  add_requires("gtest")

  target("unit_tests")
    set_kind("binary")
    add_files("tests/unit/**.cpp")
    add_deps("cpp_quick_starter")
    add_packages("gtest")

  target_end()

  target("integration_tests")
    set_kind("binary")
    add_files("tests/integration/**.cpp")
    add_deps("cpp_quick_starter")
    add_packages("gtest")

  target_end()
end

if has_config("build_benchmarks") then
  add_requires("benchmark")

  target("benchmarks")
    set_kind("binary")
    add_files("benchmarks/**.cpp")
    add_deps("cpp_quick_starter")
    add_packages("benchmark")

  target_end()
end

task("docs")
  set_menu {
    usage = "xmake docs",
    description = "Build MkDocs documentation",
    options = {
      {"o", "outputdir", "kv", "Site output directory", "build/mkdocs"}
    }
  }
  on_run(function ()
    import("core.base.option")
    import("lib.detect.find_tool")
    local uv = find_tool("uv")
    local mkdocs = find_tool("mkdocs")
    local cmd = nil
    local argv = {}
    if uv then
      cmd = uv.program
      argv = {"run", "mkdocs"}
    elseif mkdocs then
      cmd = mkdocs.program
    else
      local python = find_tool("python") or find_tool("python3")
      if python then
        cmd = python.program
        argv = {"-m", "mkdocs"}
      end
    end
    if not cmd then
      raise("mkdocs not found. Install via: pip install mkdocs")
    end
    local outputdir = option.get("outputdir") or "build/mkdocs"
    table.join2(argv, {"build", "-f", "mkdocs.yml", "-d", outputdir})
    os.execv(cmd, argv)
  end)

task("docs-serve")
  set_menu {
    usage = "xmake docs-serve",
    description = "Serve MkDocs documentation locally",
    options = {}
  }
  on_run(function ()
    import("core.base.option")
    import("lib.detect.find_tool")
    local uv = find_tool("uv")
    local mkdocs = find_tool("mkdocs")
    local cmd = nil
    local argv = {}
    if uv then
      cmd = uv.program
      argv = {"run", "mkdocs"}
    elseif mkdocs then
      cmd = mkdocs.program
    else
      local python = find_tool("python") or find_tool("python3")
      if python then
        cmd = python.program
        argv = {"-m", "mkdocs"}
      end
    end
    if not cmd then
      raise("mkdocs not found. Install via: pip install mkdocs")
    end
    table.join2(argv, {"serve", "-f", "mkdocs.yml"})
    os.execv(cmd, argv)
  end)
