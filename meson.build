project('Linux ImGui Overlay', 'cpp', default_options : ['default_library=static'])

ImGUILib = subproject('ImGUI').get_variable('libImGUIDep')

thread_dep = dependency('threads')

linuxOverlayInc = include_directories('./include')
glfwInc = subproject('glfw').get_variable('glfwIncs')
glfwLib = subproject('glfw').get_variable('glfw_dep')

sources = [
    'Overlay.cpp',
    'Input.cpp']

buildType = get_option('LinuxOverlayBuildType')

if buildType == 'standalone'
    sources += ['main.cpp']
    executable('LinuxOverlay', sources, dependencies : [thread_dep, ImGUILib] )
elif buildType == 'dependency'
    libLinuxOverlay = library('LinuxOverlay', sources, dependencies : [thread_dep, ImGUILib, glfwLib] )
    LinuxOverlayDep = declare_dependency(link_with : libLinuxOverlay, include_directories : [linuxOverlayInc, glfwInc])
endif
