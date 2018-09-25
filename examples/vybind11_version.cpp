#include <iostream>

#include "vybind11/vybind11.hpp"
#include "vybind11/vybind11_config.hpp"

int main(int argc, char *argv[]){
    std::cout<<"VYBIND11_VERSION_MAJOR "<<VYBIND11_VERSION_MAJOR<<"\n";
    std::cout<<"VYBIND11_VERSION_MINOR "<<VYBIND11_VERSION_MINOR<<"\n";
    std::cout<<"VYBIND11_VERSION_PATCH "<<VYBIND11_VERSION_PATCH<<"\n";
}