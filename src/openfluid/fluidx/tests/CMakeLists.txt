INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")


SET(UNITTEST_LINK_LIBRARIES ${Boost_REGEX_LIBRARIES} openfluid-core openfluid-base openfluid-fluidx openfluid-machine)

DISCOVER_OPENFLUID_UNITTESTS(api)