
// our headers
#include "vybind11/vybind11.hpp"

// vigra
#include <vigra/multi_convolution.hxx> 

namespace py = pybind11;
//namespace vy = vybind11;



namespace vybind11 {


    


    void def_examples(py::module & module){

        module.def("return_numpy",[](){
            vigra::MultiArray<2, float> array(10, 10);
            return viewToNumpy(array, CopyPolicy::CopyData);
        });

        module.def("withCopy",[]
        (
            py::array_t<float> & data
        )
        {
            // non copying view 
            vigra::MultiArrayView<2, float> view = numpyToView<2>(data);

            // alloc output space
            vigra::MultiArray<2, float> ret(view.shape());

            // do alg
            vigra::gaussianGradientMagnitude(view, ret, 2.0);

            // return numpy COPY
            return viewToNumpy(ret, CopyPolicy::CopyData);

        });


    }


    void def_class(py::module & m)
    {
       // py::class_<MyClass>(m, "MyClass")
       //     .def(py::init<uint64_t>(),py::arg("size"))
       //     .def("hello_world", &MyClass::hello_world)
       // ;
    }

}


// Python Module and Docstrings
PYBIND11_MODULE(_vybind11 , module)
{


    module.doc() = R"pbdoc(
        _vybind11  python bindings

        .. currentmodule:: _vybind11 

        .. autosummary::
           :toctree: _generate
    )pbdoc";

    vybind11::def_examples(module);
    vybind11::def_class(module);

    module.attr("__version__") = "dev";
}