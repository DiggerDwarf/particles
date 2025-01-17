#ifndef computeHandler_hpp_INCLUDED
#define computeHandler_hpp_INCLUDED

#define COMPUTE_INTEGRATE_SFML

#include <iostream>
#include <GL/glew.h>
#include <string>
#include <map>

#ifdef COMPUTE_INTEGRATE_SFML
#include <SFML/Graphics.hpp>
#endif

typedef struct __compute_shader {
    const GLuint                    m_programId;
    std::map<std::string, GLint>    m_uniformLocations;
    std::map<GLint, GLuint>         m_textureUniformHandles;
} ComputeShader;

ComputeShader compile_compute_shader(const char* code);

ComputeShader compile_compute_shader_file(const char* fileName);

GLint get_uniform_location(ComputeShader& shader, const std::string& name);

void set_uniform(ComputeShader& shader, const std::string& name, float value);
void set_uniform(ComputeShader& shader, const std::string& name, double value);
void set_uniform(ComputeShader& shader, const std::string& name, int value);
void set_uniform(ComputeShader& shader, const std::string& name, unsigned int value);
void set_uniform(ComputeShader& shader, const std::string& name, bool value);
void set_uniform(ComputeShader& shader, const std::string& name, float value0, float value1);
void set_uniform(ComputeShader& shader, const std::string& name, double value0, double value1);
void set_uniform(ComputeShader& shader, const std::string& name, int value0, int value1);
void set_uniform(ComputeShader& shader, const std::string& name, unsigned int value0, unsigned int value1);
void set_uniform(ComputeShader& shader, const std::string& name, bool value0, bool value1);
void set_uniform(ComputeShader& shader, const std::string& name, float value0, float value1, float value2);
void set_uniform(ComputeShader& shader, const std::string& name, double value0, double value1, double value2);
void set_uniform(ComputeShader& shader, const std::string& name, int value0, int value1, int value2);
void set_uniform(ComputeShader& shader, const std::string& name, unsigned int value0, unsigned int value1, unsigned int value2);
void set_uniform(ComputeShader& shader, const std::string& name, bool value0, bool value1, bool value2);
void set_uniform(ComputeShader& shader, const std::string& name, float value0, float value1, float value2, float value3);
void set_uniform(ComputeShader& shader, const std::string& name, double value0, double value1, double value2, double value3);
void set_uniform(ComputeShader& shader, const std::string& name, int value0, int value1, int value2, int value3);
void set_uniform(ComputeShader& shader, const std::string& name, unsigned int value0, unsigned int value1, unsigned int value2, unsigned int value3);
void set_uniform(ComputeShader& shader, const std::string& name, bool value0, bool value1, bool value2, bool value3);

void set_uniform_array_scalar(ComputeShader& shader, const std::string& name, const float* values, int arrayLength);
void set_uniform_array_scalar(ComputeShader& shader, const std::string& name, const double* values, int arrayLength);
void set_uniform_array_scalar(ComputeShader& shader, const std::string& name, const int* values, int arrayLength);
void set_uniform_array_scalar(ComputeShader& shader, const std::string& name, const unsigned int* values, int arrayLength);
void set_uniform_array_scalar(ComputeShader& shader, const std::string& name, const bool* values, int arrayLength);
void set_uniform_array_2_tuple(ComputeShader& shader, const std::string& name, const float* flattenedValues, int arrayLength);
void set_uniform_array_2_tuple(ComputeShader& shader, const std::string& name, const double* flattenedValues, int arrayLength);
void set_uniform_array_2_tuple(ComputeShader& shader, const std::string& name, const int* flattenedValues, int arrayLength);
void set_uniform_array_2_tuple(ComputeShader& shader, const std::string& name, const unsigned int* flattenedValues, int arrayLength);
void set_uniform_array_2_tuple(ComputeShader& shader, const std::string& name, const bool* flattenedValues, int arrayLength);
void set_uniform_array_3_tuple(ComputeShader& shader, const std::string& name, const float* flattenedValues, int arrayLength);
void set_uniform_array_3_tuple(ComputeShader& shader, const std::string& name, const double* flattenedValues, int arrayLength);
void set_uniform_array_3_tuple(ComputeShader& shader, const std::string& name, const int* flattenedValues, int arrayLength);
void set_uniform_array_3_tuple(ComputeShader& shader, const std::string& name, const unsigned int* flattenedValues, int arrayLength);
void set_uniform_array_3_tuple(ComputeShader& shader, const std::string& name, const bool* flattenedValues, int arrayLength);
void set_uniform_array_4_tuple(ComputeShader& shader, const std::string& name, const float* flattenedValues, int arrayLength);
void set_uniform_array_4_tuple(ComputeShader& shader, const std::string& name, const double* flattenedValues, int arrayLength);
void set_uniform_array_4_tuple(ComputeShader& shader, const std::string& name, const int* flattenedValues, int arrayLength);
void set_uniform_array_4_tuple(ComputeShader& shader, const std::string& name, const unsigned int* flattenedValues, int arrayLength);
void set_uniform_array_4_tuple(ComputeShader& shader, const std::string& name, const bool* flattenedValues, int arrayLength);

const std::map<std::pair<int, int>, PFNGLUNIFORMMATRIX2FVPROC> __computeHandler_matrix_functions_float = {
    std::pair(std::pair(2, 2), glUniformMatrix2fv),
    std::pair(std::pair(3, 3), glUniformMatrix3fv),
    std::pair(std::pair(4, 4), glUniformMatrix4fv),
    std::pair(std::pair(2, 3), glUniformMatrix2x3fv),
    std::pair(std::pair(3, 2), glUniformMatrix3x2fv),
    std::pair(std::pair(2, 4), glUniformMatrix2x4fv),
    std::pair(std::pair(4, 2), glUniformMatrix4x2fv),
    std::pair(std::pair(3, 4), glUniformMatrix3x4fv),
    std::pair(std::pair(4, 3), glUniformMatrix4x3fv),
};
const std::map<std::pair<int, int>, PFNGLUNIFORMMATRIX2DVPROC> __computeHandler_matrix_functions_double = {
    std::pair(std::pair(2, 2), glUniformMatrix2dv),
    std::pair(std::pair(3, 3), glUniformMatrix3dv),
    std::pair(std::pair(4, 4), glUniformMatrix4dv),
    std::pair(std::pair(2, 3), glUniformMatrix2x3dv),
    std::pair(std::pair(3, 2), glUniformMatrix3x2dv),
    std::pair(std::pair(2, 4), glUniformMatrix2x4dv),
    std::pair(std::pair(4, 2), glUniformMatrix4x2dv),
    std::pair(std::pair(3, 4), glUniformMatrix3x4dv),
    std::pair(std::pair(4, 3), glUniformMatrix4x3dv),
};

void set_uniform_matrix(ComputeShader& shader, const std::string& name, int x_size, int y_size, const float* flattennedValues, bool transpose);
void set_uniform_matrix(ComputeShader& shader, const std::string& name, int x_size, int y_size, const double* flattennedValues, bool transpose);
void set_uniform_array_matrix(ComputeShader& shader, const std::string& name, int x_size, int y_size, const float* flattennedValues, bool transpose, int arrayLength);
void set_uniform_array_matrix(ComputeShader& shader, const std::string& name, int x_size, int y_size, const double* flattennedValues, bool transpose, int arrayLength);

void set_uniform_texture(ComputeShader& shader, const std::string& name, unsigned int textureHandle);

#ifdef COMPUTE_INTEGRATE_SFML
template <typename Type> inline void set_uniform(ComputeShader& shader, const std::string& name, const sf::Vector2<Type>& value) {
    set_uniform(shader, name, value.x, value.y);
}
template <typename Type> inline void set_uniform(ComputeShader& shader, const std::string& name, const sf::Vector3<Type>& value) {
    set_uniform(shader, name, value.x, value.y, value.z);
}
template <typename Type> inline void set_uniform(ComputeShader& shader, const std::string& name, const sf::priv::Vector4<Type>& value) {
    set_uniform(shader, name, value.x, value.y, value.z, value.w);
}
inline void set_uniform(ComputeShader& shader, const std::string& name, sf::Color value) {
    set_uniform(shader, name, value.r/255.F, value.g/255.F, value.b/255.F, value.a/255.F);
}
template <typename Type> inline void set_uniform_array(ComputeShader& shader, const std::string& name, sf::Vector2<Type>* values, std::size_t arrayLength) {
    Type flattenedValues[arrayLength*2];
    for (size_t i = 0; i < arrayLength; i++)
    {
        flattenedValues[i*2    ] = values[i].x;
        flattenedValues[i*2 + 1] = values[i].y;
    }
    set_uniform_array_2_tuple(shader, name, flattenedValues, arrayLength);
}
template <typename Type> inline void set_uniform_array(ComputeShader& shader, const std::string& name, sf::Vector3<Type>* values, std::size_t arrayLength) {
    Type flattenedValues[arrayLength*2];
    for (size_t i = 0; i < arrayLength; i++)
    {
        flattenedValues[i*3    ] = values[i].x;
        flattenedValues[i*3 + 1] = values[i].y;
        flattenedValues[i*3 + 2] = values[i].z;
    }
    set_uniform_array_3_tuple(shader, name, flattenedValues, arrayLength);
}
template <typename Type> inline void set_uniform_array(ComputeShader& shader, const std::string& name, sf::priv::Vector4<Type>* values, std::size_t arrayLength) {
    Type flattenedValues[arrayLength*2];
    for (size_t i = 0; i < arrayLength; i++)
    {
        flattenedValues[i*4    ] = values[i].x;
        flattenedValues[i*4 + 1] = values[i].y;
        flattenedValues[i*4 + 2] = values[i].z;
        flattenedValues[i*4 + 3] = values[i].w;
    }
    set_uniform_array_4_tuple(shader, name, flattenedValues, arrayLength);
}
template <typename Type> inline void set_uniform_array(ComputeShader& shader, const std::string& name, sf::Color* values, std::size_t arrayLength) {
    Type flattenedValues[arrayLength*2];
    for (size_t i = 0; i < arrayLength; i++)
    {
        flattenedValues[i*4    ] = values[i].r / 255.F;
        flattenedValues[i*4 + 1] = values[i].g / 255.F;
        flattenedValues[i*4 + 2] = values[i].b / 255.F;
        flattenedValues[i*4 + 3] = values[i].a / 255.F;
    }
    set_uniform_array_4_tuple(shader, name, flattenedValues, arrayLength);
}
inline void set_uniform_texture(ComputeShader& shader, const std::string& name, const sf::Texture& texture) {
    set_uniform_texture(shader, name, texture.getNativeHandle());
}
#endif



void set_buffer(const void* data, std::size_t size, int bindIndex);


void execute_compute_shader(const ComputeShader& shader, GLuint x, GLuint y, GLuint z);

#endif // computeHandler_hpp_INCLUDED