find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

find_package(spdlog CONFIG REQUIRED)

find_package(nlohmann_json CONFIG REQUIRED)

find_package(Eigen3 CONFIG REQUIRED)
find_package(MKL CONFIG REQUIRED)
target_link_libraries(Eigen3::Eigen INTERFACE MKL::MKL)
target_compile_definitions(Eigen3::Eigen INTERFACE EIGEN_USE_MKL_ALL)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
