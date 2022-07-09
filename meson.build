project('Linux ImGui Overlay', 'cpp', default_options : ['default_library=static'])

ImGUILib = subproject('ImGUI').get_variable('libImGUIDep')

thread_dep = dependency('threads')

linuxOverlayInc = include_directories('./include')
glfwInc = subproject('glfw').get_variable('glfwIncs')
glfwLib = subproject('glfw').get_variable('glfw_dep')

sources = [
    'Overlay.cpp',
    'Input.cpp']

srcResources = join_paths(meson.source_root(), 'resources')
destResources = meson.build_root()
message('Copying @0@ to @1@ ...'.format(srcResources, destResources))

run_command('cp', '-R', srcResources, destResources, check : false)

buildType = get_option('LinuxOverlayBuildType')

if buildType == 'standalone'
    sources += ['main.cpp']
    executable('LinuxOverlay', sources, dependencies : [thread_dep, ImGUILib, glfwLib], include_directories : [linuxOverlayInc, glfwInc])
elif buildType == 'dependency'
    libLinuxOverlay = library('LinuxOverlay', sources, dependencies : [thread_dep, ImGUILib, glfwLib] )
    LinuxOverlayDep = declare_dependency(link_with : libLinuxOverlay, include_directories : [linuxOverlayInc, glfwInc])
endif
