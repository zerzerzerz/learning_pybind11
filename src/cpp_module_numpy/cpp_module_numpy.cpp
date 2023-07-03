#include <iostream>
#include <string>
#include <pybind11/pybind11.h>

using namespace std;
namespace py = pybind11;

class Matrix {
public:
    Matrix(size_t rows, size_t cols) : m_rows(rows), m_cols(cols) {
        m_data = new float[rows*cols];
        for(int i=0; i<rows; ++i){
            for(int j=0; j<cols; ++j){
                m_data[i*cols+j] = 0;
            }
        }
    }
    ~Matrix(){delete [] m_data;}
    float *data() { return m_data; }
    size_t rows() const { return m_rows; }
    size_t cols() const { return m_cols; }
    float *m_data;
private:
    size_t m_rows, m_cols;
};

PYBIND11_MODULE(cpp_module_numpy, m){
    py::class_<Matrix>(m, "Matrix", py::buffer_protocol())
        .def(py::init<size_t, size_t>())
        .def("__repr__", [](const Matrix& m){
            string res = "";
            auto r = m.rows();
            auto c = m.cols();
            for(int i=0; i<r; ++i){
                for(int j=0; j<c; ++j){
                    res += to_string(m.m_data[i*c+j]) + " ";
                }
                res += "\n";
            }
            return res;
        })
        .def_buffer([](Matrix &mat) -> py::buffer_info {
            return py::buffer_info(
                mat.data(),                               /* Pointer to buffer */
                sizeof(float),                          /* Size of one scalar */
                py::format_descriptor<float>::format(), /* Python struct-style format descriptor */
                2,                                      /* Number of dimensions */
                { mat.rows(), mat.cols() },                 /* Buffer dimensions, which is the shape */
                { sizeof(float) * mat.cols(),             /* Strides (in bytes) for each index */
                sizeof(float) }
            );
        });
}
