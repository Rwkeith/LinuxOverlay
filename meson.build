project('ImGui Overlay', 'cpp')

incdir = include_directories('/home/keith/git/imgui/backends', '/home/keith/git/imgui')

glfw_lib = subproject('glfw').get_variable('glfw_dep')
gl = dependency('gl')
thread_dep = dependency('threads')

files = files(
    'main.cpp',
    '/home/keith/git/imgui/imgui.cpp',
    '/home/keith/git/imgui/imgui_draw.cpp',
    '/home/keith/git/imgui/imgui_tables.cpp',
    '/home/keith/git/imgui/imgui_demo.cpp',
    '/home/keith/git/imgui/imgui_widgets.cpp',
    '/home/keith/git/imgui/backends/imgui_impl_glfw.cpp',
    '/home/keith/git/imgui/backends/imgui_impl_opengl3.cpp',
)

executable('example', files, include_directories : incdir, dependencies : [glfw_lib, gl, thread_dep] )

