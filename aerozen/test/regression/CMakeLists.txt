set(TEST_TYPE "REGRESSION")

aerozen_get_sources(tests)

aerozen_build_tests(TYPE REGRESSION SOURCES ${tests}
  LIB_DEPS ${EXTRA_TEST_LIB_DEPS})
