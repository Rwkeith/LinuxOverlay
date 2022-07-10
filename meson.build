project('Linux ImGui Overlay', 'cpp', default_options : ['default_library=static'])

linuxOverlayInc = include_directories('./include')

subproject('ImGUI')

threadDep = dependency('threads')
glfwDep = dependency('glfw')
imguiDep = dependency('imgui')

sources = [
    'Overlay.cpp',
    'Input.cpp']

buildType = get_option('LinuxOverlayBuildType')

if buildType == 'standalone'
    srcResources = join_paths(meson.source_root(), 'resources')
    sources += ['main.cpp']
    executable('LinuxOverlay', sources, dependencies : [threadDep, imguiDep, glfwDep], include_directories : [linuxOverlayInc])
elif buildType == 'dependency'
    srcResources = join_paths(meson.current_source_dir(), 'resources')
    libLinuxOverlay = library('LinuxOverlay', sources, dependencies : [threadDep, imguiDep, glfwDep] )
    LinuxOverlayDep = declare_dependency(link_with : libLinuxOverlay, include_directories : [linuxOverlayInc])
    meson.override_dependency('linuxoverlay', LinuxOverlayDep)
endif

destResources = meson.build_root()
message('Copying @0@ to @1@ ...'.format(srcResources, destResources))
run_command('cp', '-R', srcResources, destResources, check : false)
