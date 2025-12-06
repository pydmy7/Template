set(target testlibavoid)
add_executable(${target}
    SchematicHelper.cpp
    main.cpp
)

target_link_libraries(${target} PRIVATE libavoid)

copyDll(${target})
generatePath(${target} "${PROJECT_SOURCE_DIR}/target_path.toml")

target_link_libraries(${target} PRIVATE Qt5::Core Qt5::Widgets Qt5::Gui)
set_target_properties(${target} PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
winDeployQt(${target})
