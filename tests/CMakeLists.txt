# Fetch Catch2
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)

FetchContent_MakeAvailable(Catch2)

file(GLOB ${PROJECT_NAME}_TEST_SOURCE CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Add tests
add_executable(${PROJECT_NAME}_tests ${${PROJECT_NAME}_TEST_SOURCE})

# Link required libraries
target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE
        Catch2::Catch2
        ${PROJECT_NAME}
)

# Include directories
target_include_directories(${PROJECT_NAME}_tests
    PRIVATE
        ${catch2_SOURCE_DIR}/single_include
        ${${PROJECT_NAME}_INCLUDE_DIR}
)

# Set test properties
set_target_properties(${PROJECT_NAME}_tests PROPERTIES
    TEST_PREFIX "test_"
    TEST_SUFFIX ""
)

# Discover tests
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
include(Catch)
catch_discover_tests(${PROJECT_NAME}_tests)

# Create coverage target
if(NOT ${PROJECT_NAME}_IS_SUBMODULE)

    find_program(LCOV lcov)
    find_program(GENHTML genhtml)

    #find correct coverage system
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        find_program(GCOV gcov)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        find_program(GCOV llvm-cov)
    endif()

    #prepare for coverage report
    if(GCOV AND LCOV AND GENHTML)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
            target_compile_options(${PROJECT_NAME} PRIVATE -fprofile-arcs -ftest-coverage)
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            target_compile_options(${PROJECT_NAME} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        endif()
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
            target_compile_options(${PROJECT_NAME}_tests PRIVATE -fprofile-arcs -ftest-coverage)
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
            target_compile_options(${PROJECT_NAME}_tests PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        endif()


        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
            add_custom_target(coverage
                COMMAND ${CMAKE_MAKE_PROGRAM} test
                COMMAND ${GCOV} -r -pb tests/CMakeFiles/${PROJECT_NAME}_tests.dir/*.gcno CMakeFiles/${PROJECT_NAME}.dir/source/*.gcno
                COMMAND ${LCOV} --capture --directory . --output-file coverage.info
                COMMAND ${LCOV} --remove coverage.info '*catch2*' '*test_*' '/usr/*' --output-file coverage.info
                COMMAND ${GENHTML} coverage.info --output-directory out
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Running GCOV and generating report"
            )
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            set(PROFRAW_FILE "${CMAKE_BINARY_DIR}/tests/${PROJECT_NAME}-%p.profraw")
            set(PROFDATA_FILE "${CMAKE_BINARY_DIR}/tests/coverage.profdata")
            set(DEFAULT_PROFRAW_FILE "${CMAKE_BINARY_DIR}/tests/default.profraw")

            add_custom_target(coverage
                COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE="${PROFRAW_FILE}" ${CMAKE_MAKE_PROGRAM} test
                COMMAND llvm-profdata merge -sparse ${CMAKE_BINARY_DIR}/tests/${PROJECT_NAME}-*.profraw ${DEFAULT_PROFRAW_FILE} -o ${PROFDATA_FILE}
                COMMAND ${GCOV} export -format=lcov ${CMAKE_BINARY_DIR}/tests/${PROJECT_NAME}_tests -instr-profile=${PROFDATA_FILE} > coverage.lcov
                COMMAND ${LCOV} --remove coverage.lcov '*catch2*' '*test_*' '/usr/*' --output-file coverage.lcov
                COMMAND ${LCOV} --summary coverage.lcov
                COMMAND ${GENHTML} coverage.lcov --output-directory out
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Running llvm-cov and generating report"
            )
        endif()
    else()
        if(NOT GCOV)
            if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
                message(WARNING "llvm-cov not found. No coverage report will be generated.")
            else()
                message(WARNING "gcov not found. No coverage report will be generated.")
            endif()
        endif()
        if(NOT LCOV)
            message(WARNING "lcov not found. No coverage report will be generated.")
        endif()
        if(NOT GENHTML)
            message(WARNING "genhtml not found. No coverage report will be generated.")
        endif()
    endif()
endif()
