# examples installation
INSTALL(DIRECTORY "src" 
        DESTINATION "${EXAMPLES_INSTALL_PATH}" PATTERN ".svn" EXCLUDE)
  
INSTALL(DIRECTORY "datasets" 
        DESTINATION "${EXAMPLES_INSTALL_PATH}" PATTERN ".svn" EXCLUDE)
        
IF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT (CMAKE_BUILD_TYPE MATCHES "Release"))  
