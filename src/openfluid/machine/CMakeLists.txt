
ADD_DEFINITIONS(${LibXML2_CFLAGS})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${LibXML2_INCLUDE_DIRS})
LINK_DIRECTORIES(${LibXML2_LIBRARY_DIRS})


FILE(GLOB OPENFLUID_MACHINE_CPP *.cpp)
FILE(GLOB OPENFLUID_MACHINE_HPP *.hpp)


ADD_LIBRARY(openfluid-machine SHARED ${OPENFLUID_MACHINE_CPP} ${OPENFLUID_MACHINE_HPP})

SET_TARGET_PROPERTIES(openfluid-machine PROPERTIES VERSION "${VERSION_MAJOR}.${VERSION_MINOR}" 
                                        SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
                                        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        RUNTIME_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}"
                                        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUTPUT_PATH}")

TARGET_LINK_LIBRARIES(openfluid-machine
                      openfluid-io 
                      openfluid-tools
                      openfluid-core
                      openfluid-base
                      openfluid-ware
                      openfluid-fluidx                    
                      ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY})


INSTALL(TARGETS openfluid-machine
        RUNTIME DESTINATION ${BIN_INSTALL_PATH}
        LIBRARY DESTINATION ${LIB_INSTALL_PATH}
        ARCHIVE DESTINATION ${LIB_INSTALL_PATH})
   

INSTALL(FILES ${OPENFLUID_MACHINE_HPP} DESTINATION ${INCLUDE_OPENFLUID_INSTALL_PATH}/machine)


IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))  


   