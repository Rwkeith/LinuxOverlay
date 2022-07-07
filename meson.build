project('Linux ImGui Overlay', 'cpp')

#incdir = include_directories('/home/keith/git/imgui/backends', '/home/keith/git/imgui')

glfwLib = subproject('glfw').get_variable('glfw_dep')
ImGUILib = subproject('ImGUI').get_variable('libImGUIDep')

gl = dependency('gl')
thread_dep = dependency('threads')

sources = [
    'Overlay.cpp',
    'Input.cpp']

buildType = get_option('LinuxOverlayBuildType')

if buildType == 'standalone'
    sources += ['main.cpp']
    executable('LinuxOverlay', sources, include_directories : incdir, dependencies : [glfwLib, gl, thread_dep, ImGUILib] )
elif buildType == 'dependency'
    libLinuxOverlay = library('LinuxOverlay', sources, include_directories : incdir, dependencies: [glfwLib, ImGUILib, gl, thread_dep])
    LinuxOverlayDep = declare_dependency(link_with : libLinuxOverlay) #, include_directories : incdir)
endif

