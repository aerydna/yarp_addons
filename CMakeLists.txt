cmake_minimum_required(VERSION 3.13)

project(AerydnaYarpAddons
        LANGUAGES CXX C
        VERSION 0.0.0.0)

if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
option(BUILD_SHARED_LIBS "Build libraries as shared as opposed to static" ON)

find_package(YARP REQUIRED COMPONENTS dev OS math)
find_package(YCM REQUIRED)

set(HDRs src/IFrameSet.h
         src/IFrameSource.h
         src/FrameTransform.h
         src/api.h)
set(CPPs src/ImplementIFrameSource.cpp)

add_library(${PROJECT_NAME} ${CPPs})
target_link_libraries(${PROJECT_NAME} YARP::YARP_OS YARP::YARP_dev YARP::YARP_math YARP::YARP_init)
target_compile_definitions(${PROJECT_NAME} PRIVATE YARP_ADDONS_EXPORTS)

install(TARGETS ${PROJECT_NAME}
        EXPORT  ${PROJECT_NAME}
        LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
        ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
        RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME}" COMPONENT dev)

install(FILES ${HDRs}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")
list(APPEND CMAKE_MODULE_PATH ${YCM_MODULE_PATH})

include(InstallBasicPackageFiles)
install_basic_package_files(${PROJECT_NAME}
                            VERSION ${${PROJECT_NAME}_VERSION}
                            COMPATIBILITY AnyNewerVersion
                            VARS_PREFIX ${PROJECT_NAME}
                            NO_CHECK_REQUIRED_COMPONENTS_MACRO)

