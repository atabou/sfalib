FIND_PROGRAM(GCOV_PATH gcov)
FIND_PROGRAM(LCOV_PATH lcov)
FIND_PROGRAM(GENHTML_PATH genhtml)


IF(NOT GCOV_PATH)
    MESSAGE(FATAL_ERROR "gcov not found")
ENDIF()


IF(NOT LCOV_PATH)
    MESSAGE(FATAL_ERROR "lcov not found! Aborting")
ENDIF()


IF(NOT GENHTML_PATH)
    MESSAGE(FATAL_ERROR "genhtml not found! Aborting...")
ENDIF()


IF (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")

    MESSAGE(WARNING "Code coverage results with an optimized (non-Debug) build may be misleading")

ENDIF()


SET(CMAKE_EXE_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used for linking binaries during coverage builds."
    FORCE)


SET(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used by the shared libraries linker during coverage builds."
    FORCE)


IF(CMAKE_COMPILER_IS_GNUCXX)

    SET(CMAKE_CXX_FLAGS_COVERAGE
        "-pg -O0 --coverage -fprofile-arcs -ftest-coverage"
        CACHE STRING "Fags used by C++ compiler during coverage builds."
        FORCE)


    MARK_AS_ADVANCED(
        CMAKE_CXX_FLAGS_COVERAGE
        CMAKE_EXE_LINKER_FLAGS_COVERAGE
        CMAKE_SHARED_LINKER_FLAGS_COVERAGE)


    FUNCTION(SETUP_TARGET_FOR_COVERAGE _targetname _testrunner _outputname)

        ADD_CUSTOM_TARGET(${_targetname}

            # Cleanup lcov
            ${LCOV_PATH} --directory . --zerocounters

            # Run tests
            COMMAND ${_testrunner} ${ARGV3}
            COMMAND pwd	
            # Capturing lcov counters and generating report
            COMMAND ${LCOV_PATH} --directory . --capture --output-file ${_outputname}.info
            COMMAND ${LCOV_PATH} --remove ${_outputname}.info '*/build/*' '*/tests/*' '/usr/*' --output-file ${_outputname}.info.cleaned
            COMMAND ${GENHTML_PATH} -o ${_outputname} ${_outputname}.info.cleaned
            # COMMAND ${CMAKE_COMMAND} -E remove ${_outputname}.info ${_outputname}.info.cleaned

            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}

            COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report."

            )

        # Show info where to find the report
        ADD_CUSTOM_COMMAND(TARGET ${_targetname} POST_BUILD
            COMMAND ;
            COMMENT "Open ./${_outputname}/index.html in your browser to view the coverage report."
            )

    ENDFUNCTION()


ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

    SET(CMAKE_CXX_FLAGS_COVERAGE
        "-pg -O0 -fprofile-instr-generate -fcoverage-mapping"
        CACHE STRING "Fags used by C++ compiler during coverage builds."
        FORCE)
    
    MARK_AS_ADVANCED(
        CMAKE_CXX_FLAGS_COVERAGE
        CMAKE_EXE_LINKER_FLAGS_COVERAGE
        CMAKE_SHARED_LINKER_FLAGS_COVERAGE)

    FUNCTION(SETUP_TARGET_FOR_COVERAGE _targetname _testrunner _outputname)

        # Enable LLVM code coverage output
        set(LLVM_PROFILE_DATA_DIR "${CMAKE_BINARY_DIR}/profdata")
        set(LLVM_PROFILE_FILE_NAME "coverage-tmp.profraw")

        # Create the directory for code coverage data
        file(MAKE_DIRECTORY ${LLVM_PROFILE_DATA_DIR})

        ADD_CUSTOM_TARGET(${_targetname}
       
            COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE=${LLVM_PROFILE_DATA_DIR}/${LLVM_PROFILE_FILE_NAME}
                    tests/tests
            COMMAND llvm-profdata merge -sparse ${LLVM_PROFILE_DATA_DIR}/${LLVM_PROFILE_FILE_NAME} -o ${LLVM_PROFILE_DATA_DIR}/coverage.profdata
            COMMAND llvm-cov show tests/tests -format=html -o coverage_report  -instr-profile=${LLVM_PROFILE_DATA_DIR}/coverage.profdata --show-line-counts-or-regions --show-instantiation-summary --show-region-summary --ignore-filename-regex=build/ --ignore-filename-regex=tests/
            DEPENDS tests/tests
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}

        )   


        # Show info where to find the report
        ADD_CUSTOM_COMMAND(TARGET ${_targetname} POST_BUILD
            COMMAND ;
            COMMENT "Open ./${_outputname}/index.html in your browser to view the coverage report."
        )




    ENDFUNCTION()

ELSE()

    MESSAGE(FATAL_ERROR "Compiler not recognized. Cannot run coverage.")

ENDIF()




