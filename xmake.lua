set_project("student-attendance-system")
set_version("1.0.0")

set_languages("cxx20")
add_rules("mode.debug", "mode.release")

option("build_server")
  set_default(true)
  set_showmenu(true)
  set_description("Build attendance server")
option_end()

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

-- Main app (help only)
target("student_attendance_app")
  set_kind("binary")
  add_files("src/main.cpp")

  if is_plat("windows") then
    add_cxflags("/W4")
  else
    add_cxflags("-Wall", "-Wextra", "-Wpedantic")
  end

target_end()

-- Server with Drogon
if has_config("build_server") then
  add_requires("drogon", {configs = {mysql = false, postgresql = false, sqlite3 = true}})
  add_requires("jsoncpp")

  target("student_attendance_server_lib")
    set_kind("static")
    add_headerfiles("include/(student_attendance/**.h)")
    add_files(
      "src/db/**.cc",
      "src/models/**.cc",
      "src/services/**.cc",
      "src/controllers/**.cc"
    )
    add_includedirs("include", {public = true})
    add_packages("drogon", "jsoncpp", {public = true})

  target_end()

  target("student_attendance_server")
    set_kind("binary")
    add_files("src/server_main.cpp")
    add_deps("student_attendance_server_lib")

    after_build(function (target)
      os.cp("$(projectdir)/config.json", target:targetdir())
    end)

  target_end()
end

if has_config("build_tests") and has_config("build_server") then
  add_requires("gtest")

  target("api_tests")
    set_kind("binary")
    add_files("tests/api/**.cpp")
    add_deps("student_attendance_server_lib")
    add_packages("gtest")

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
