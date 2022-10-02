include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

########################## GLFW ##########################

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.3.8
)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate the GLFW installation target" OFF)
FetchContent_MakeAvailable(glfw)

########################## GLAD ##########################

add_subdirectory(glad)

########################## GLM ##########################

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 0.9.9.8
)
option(GLM_TEST_ENABLE "Enable GLM tests" OFF)
FetchContent_MakeAvailable(glm)

########################## ImGui ##########################

add_subdirectory(imgui)

########################## Thread-pool ##########################

add_subdirectory(thread-pool)

########################## stbi ##########################

add_subdirectory(stbi)
