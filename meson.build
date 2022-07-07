project('Linux ImGui Overlay', 'cpp', default_options : ['default_library=static'])

ImGUILib = subproject('ImGUI').get_variable('libImGUIDep')

thread_dep = dependency('threads')

sources = [
    'Overlay.cpp',
    'Input.cpp']

buildType = get_option('LinuxOverlayBuildType')

if buildType == 'standalone'
    sources += ['main.cpp']
    executable('LinuxOverlay', sources, include_directories : incdir, dependencies : [thread_dep, ImGUILib] ) # [glfwLib, gl, thread_dep, ImGUILib] )
elif buildType == 'dependency'
    libLinuxOverlay = library('LinuxOverlay', sources, include_directories : incdir, dependencies : [thread_dep, ImGUILib] ) # [glfwLib, ImGUILib, gl, thread_dep])
    LinuxOverlayDep = declare_dependency(link_with : libLinuxOverlay) #, include_directories : incdir)
endif

