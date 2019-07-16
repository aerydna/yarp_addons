# Copyright (C) 2006-2019 Istituto Italiano di Tecnologia (IIT)
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

yarp_prepare_plugin(oculusheadset
                    CATEGORY device
                    TYPE yarp::dev::OVRHeadset
                    INCLUDE OVRHeadset.h)

if(NOT SKIP_ovrheadset)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  set(yarp_ovrheadset_SRCS OVRHeadset.cpp
                           TextureBuffer.cpp
                           TextureStatic.cpp
                           TextureBattery.cpp
                           InputCallback.cpp
                           GLDebug.cpp)
  set(yarp_ovrheadset_IMGS img-yarp-robot-64.h
                           img-crosshairs.h
                           img-battery-100.h
                           img-battery-080.h
                           img-battery-060.h
                           img-battery-040.h
                           img-battery-caution.h
                           img-battery-low.h
                           img-battery-charging.h
                           img-battery-charging-080.h
                           img-battery-charging-060.h
                           img-battery-charging-040.h
                           img-battery-charging-caution.h
                           img-battery-charging-low.h
                           img-battery-missing.h)
  set(yarp_ovrheadset_HDRS ImageType.h
                           OVRHeadset.h
                           TextureBuffer.h
                           TextureStatic.h
                           TextureBattery.h
                           InputCallback.h
                           GLDebug.h
                           ${yarp_ovrheadset_IMGS})

  source_group("Texture Images" FILES ${yarp_ovrheadset_IMGS})

  yarp_add_plugin(yarp_ovrheadset ${yarp_ovrheadset_SRCS}
                                  ${yarp_ovrheadset_HDRS})

  target_include_directories(yarp_ovrheadset PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libovr/include)
  target_include_directories(yarp_ovrheadset PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glew/include)
  target_include_directories(yarp_ovrheadset PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/glfw3/include/GLFW)
  target_include_directories(yarp_ovrheadset PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libovr/include/extras)
  target_include_directories(yarp_ovrheadset PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libovr/include/kernel)
  target_link_libraries(yarp_ovrheadset PRIVATE YARP::YARP_OS
                                                YARP::YARP_sig
                                                YARP::YARP_dev
                                                YARP::YARP_math)
  target_link_libraries(yarp_ovrheadset PRIVATE AerydnaYarpAddons)
  list(APPEND YARP_${YARP_PLUGIN_MASTER}_PRIVATE_DEPS YARP_OS
                                                      YARP_sig
                                                      YARP_dev
                                                      YARP_math)
  
  

  yarp_install(TARGETS yarp_ovrheadset
               EXPORT YARP_${YARP_PLUGIN_MASTER}
               COMPONENT ${YARP_PLUGIN_MASTER}
               LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
               ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR}
               YARP_INI DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR})

  set(YARP_${YARP_PLUGIN_MASTER}_PRIVATE_DEPS ${YARP_${YARP_PLUGIN_MASTER}_PRIVATE_DEPS} PARENT_SCOPE)

  set_property(TARGET yarp_ovrheadset PROPERTY FOLDER "Plugins/Device")
endif()
