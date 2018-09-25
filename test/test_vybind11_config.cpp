#include <doctest.h>

#include "vybind11/vybind11.hpp"
#include "vybind11/vybind11_config.hpp"



TEST_SUITE_BEGIN("core");

TEST_CASE("check version"){

    #ifndef VYBIND11_VERSION_MAJOR
        #error "VYBIND11_VERSION_MAJOR is undefined"
    #endif
    

    #ifndef VYBIND11_VERSION_MINOR
        #error "VYBIND11_VERSION_MINOR is undefined"
    #endif


    #ifndef VYBIND11_VERSION_PATCH
        #error "VYBIND11_VERSION_PATCH is undefined"
    #endif

    CHECK_GE(VYBIND11_VERSION_MAJOR , 0);
    CHECK_GE(VYBIND11_VERSION_MINOR , 0);
    CHECK_GE(VYBIND11_VERSION_PATCH , 0);
}



TEST_SUITE_END(); // end of testsuite core
