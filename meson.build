project('Linux ImGui Overlay', 'cpp')

incdir = include_directories('/home/keith/git/imgui/backends', '/home/keith/git/imgui')

glfw_lib = subproject('glfw').get_variable('glfw_dep')
gl = dependency('gl')
thread_dep = dependency('threads')

sources = [
    'Overlay.cpp',
    'Input.cpp',
    '/home/keith/git/imgui/imgui.cpp',
    '/home/keith/git/imgui/imgui_draw.cpp',
    '/home/keith/git/imgui/imgui_tables.cpp',
    '/home/keith/git/imgui/imgui_demo.cpp',
    '/home/keith/git/imgui/imgui_widgets.cpp',
    '/home/keith/git/imgui/backends/imgui_impl_glfw.cpp',
    '/home/keith/git/imgui/backends/imgui_impl_opengl3.cpp']

buildType = get_option('LinuxOverlayBuildType')

if buildType == 'standalone'
    sources += ['main.cpp']
    executable('LinuxOverlay', sources, include_directories : incdir, dependencies : [glfw_lib, gl, thread_dep] )
elif buildType == 'dependency'
    libLinuxOverlay = library('LinuxOverlay', sources, include_directories : incdir, dependencies: [glfw_lib, gl, thread_dep])
    LinuxOverlayDep = declare_dependency(link_with : libLinuxOverlay, include_directories : incdir)
endif

