
ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src"
                    ${Boost_INCLUDE_DIRS} ${GLIBMM_INCLUDE_DIRS} ${GDAL_INCLUDE_DIRS}
                    )
                    
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS} ${GLIBMM_LIBRARY_DIRS} ${GDAL_LIBRARY_DIRS})


ADD_SUBDIRECTORY(openfluid)

ADD_SUBDIRECTORY(apps)

ADD_SUBDIRECTORY(observers)

IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))  


