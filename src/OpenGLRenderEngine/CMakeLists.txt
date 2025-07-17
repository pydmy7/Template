set(target OpenGLRenderEngine)
add_executable(${target})
deploy(${target})

target_link_libraries(${target} PRIVATE
    OpenGL::GL glad::glad glfw
    imgui::imgui implot::implot
)
