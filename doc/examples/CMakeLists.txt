  IF(PDFLATEX_COMPILER)
   
    FILE(GLOB_RECURSE DOCFILES *.tex *.png)

    ADD_CUSTOM_COMMAND(
      OUTPUT ${EXAMPLES_DOC_BUILD_DIR}/openfluid_example-primitives_en.tex
      DEPENDS ${DOCFILES}                  
      COMMAND ${CMAKE_COMMAND} -E remove_directory ${EXAMPLES_DOC_BUILD_DIR}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${EXAMPLES_DOC_BUILD_DIR}
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR} ${EXAMPLES_DOC_BUILD_DIR}
     )
   
  
    IF(PDFLATEX_COMPILER)  
      COMPILE_PDFLATEX_DOC("openfluid_example-primitives_en" ${EXAMPLES_DOC_BUILD_DIR} ${EXAMPLES_DOCPDF_OUTPUT_PATH} ${EXAMPLES_DOCPDF_INSTALL_PATH})
    ENDIF(PDFLATEX_COMPILER)
  
 
  ENDIF(PDFLATEX_COMPILER)