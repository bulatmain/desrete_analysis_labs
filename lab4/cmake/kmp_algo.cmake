include(ExternalProject)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

ExternalProject_Add(kmp
    GIT_REPOSITORY https://github.com/bulatmain/Knuth-Morris-Pratt.git
    GIT_TAG main
    INSTALL_COMMAND cmake -E echo "Skipping install step."
    CMAKE_ARGS 
        -DCMAKE_INSTALL_PREFIX:PATH=${EXTERNAL_INSTALL_LOCATION} 
        -DRBTREE_TESTING=ON
)

ExternalProject_Get_Property(kmp source_dir)
ExternalProject_Get_Property(kmp binary_dir)
add_subdirectory(${source_dir} ${binary_dir})