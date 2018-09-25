#pragma once
#ifndef VYBIND11_VYBIND11_HPP
#define VYBIND11_VYBIND11_HPP

// std
#include <cstdint>
#include <iostream>

// vigra
#include <vigra/multi_array.hxx>
#include <vigra/tinyvector.hxx>
// pybind11
#include "pybind11/pybind11.h"
#include <pybind11/numpy.h>


namespace vybind11 {

    enum class CopyPolicy { 
        CopyData, 
        ViewData,
        DeleteFromPython
    };

    namespace detail_convert
    {
        template<class T, class STRIDE>
        std::vector<int> makeStrides(const STRIDE & s)
        {
            std::vector<int> r(s.begin(), s.end());
            for(std::size_t i=0; i<s.size(); ++i)
            {
                r[i] *= sizeof(T);
            }
            return r;
        }
    }

    template<unsigned int N, class T, class S>
    pybind11::array_t<T> viewToNumpy
    (
        vigra::MultiArrayView<N, T, S> & view,
        const CopyPolicy copyPolicy = CopyPolicy::CopyData
    )
    {
        if(copyPolicy == CopyPolicy::CopyData)
        {
            typedef vigra::MultiArray<N, T>  ArrayCopy;

            ArrayCopy * arrayCopyPtr = new ArrayCopy(view);

            pybind11::capsule free_when_done(arrayCopyPtr, [](void *f) {
                ArrayCopy * arrayCopyPtr = reinterpret_cast<ArrayCopy *>(f);
                delete arrayCopyPtr;
            });

            return pybind11::array_t<T>(
                std::vector<int>(view.shape().begin(), view.shape().end()),
                detail_convert::makeStrides<T>(arrayCopyPtr->stride()), 
                arrayCopyPtr->data(),
                free_when_done
            ); 
        }
        else if(copyPolicy == CopyPolicy::ViewData)
        {
            return pybind11::array_t<T>(
                std::vector<int>(view.shape().begin(), view.shape().end()),
                detail_convert::makeStrides<T>(view.stride()), 
                view.data()
            ); 
        }
        else if(copyPolicy == CopyPolicy::DeleteFromPython)
        {
            return pybind11::array_t<T>(
                std::vector<int>(view.shape().begin(), view.shape().end()),
                detail_convert::makeStrides<T>(view.stride()), 
                view.data()
            ); 

            pybind11::capsule free_when_done(view.data(), [](void *f) {
                T * ptr = reinterpret_cast<T *>(f);
                delete[] ptr;
            });

        }
    }


    template<unsigned int N, class T, int PY_FORECAST_FLAGS>
    vigra::MultiArrayView<N, T> numpyToView(pybind11::array_t<T, PY_FORECAST_FLAGS> & array)
    {

        pybind11::buffer_info info = array.request();

        /* Some sanity checks ... */
        if (info.format != pybind11::format_descriptor<T>::format())
            throw std::runtime_error("Incompatible format: expected a double array!");

        if (info.ndim != N)
            throw std::runtime_error("Incompatible buffer dimension!");

        vigra::TinyVector<int, N> shape;
        vigra::TinyVector<int, N> stride;

        for(std::size_t i=0; i<N; ++i)
        {
            shape[i] = info.shape[i];
            stride[i] = info.strides[i] / sizeof(T);
        }

        return vigra::MultiArrayView<N,T>(shape, stride,
            static_cast<T*>(info.ptr));

    }


} // end namespace vybind11


#endif // VYBIND11_VYBIND11_HPP