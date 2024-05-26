include(ExternalProject)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

ExternalProject_Add(rbtree
    GIT_REPOSITORY https://github.com/bulatmain/RBTree.git
    GIT_TAG main
    # GIT_TAG std_func_comp_ver
    INSTALL_COMMAND cmake -E echo "Skipping install step."
    CMAKE_ARGS 
        -DCMAKE_INSTALL_PREFIX:PATH=${EXTERNAL_INSTALL_LOCATION} 
        -DRBTREE_TESTING=ON
)

ExternalProject_Get_Property(rbtree source_dir)
add_subdirectory(${source_dir} ${binary_dir})