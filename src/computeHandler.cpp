#include "computeHandler.hpp"

ComputeShader compile_compute_shader(const char *code)
{
    // Create shader object
    GLuint shaderId = glCreateShader(GL_COMPUTE_SHADER);

    // Pass source code
    glShaderSource(shaderId, 1, &code, NULL);

    // Compile shader
    glCompileShader(shaderId);

    // Handle compilation errors
    GLint hasCompiled = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &hasCompiled);

    if (hasCompiled == GL_FALSE)
    {
        GLint errorLogSize = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorLogSize);

        char errorLog[errorLogSize] = {};
        glGetShaderInfoLog(shaderId, errorLogSize, NULL, errorLog);

        glDeleteShader(shaderId);

        std::cerr << "Error while compiling compute shader [ compile_compute_shader ] :\n";
        std::cerr << errorLog << std::endl;

        std::exit(1);
    }
    
    // Create a program object
    GLuint programId = glCreateProgram();

    glAttachShader(programId, shaderId);

    glLinkProgram(programId);
    

    // Handle linking errors
    GLint hasLinked = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, &hasLinked);

    if (hasLinked == GL_FALSE)
    {
        GLint errorLogSize = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &errorLogSize);

        char errorLog[errorLogSize] = {};
        glGetProgramInfoLog(programId, errorLogSize, NULL, errorLog);

        glDeleteProgram(programId);
        glDeleteShader(shaderId);

        std::cerr << "Error while linking compute shader [ compile_compute_shader ] :\n";
        std::cerr << errorLog << std::endl;

        std::exit(1);
    }

    // Clean up
    glDetachShader(programId, shaderId);
    glDeleteShader(shaderId);

    return ComputeShader{.m_programId= programId, .m_uniformLocations= std::map<std::string, GLint>()};
}

ComputeShader compile_compute_shader_file(const char *fileName)
{
    FILE* file = std::fopen(fileName, "r");

    if (file == NULL)
    {
        std::cerr << "Couldn't open designated file [ compile_compute_shader_file(" << fileName << ") ]" << std::endl;
        std::exit(1);
    }
    
    if (std::fseek(file, 0, SEEK_END) != 0)
    {
        std::cerr << "Error while parsing file [ compile_compute_shader_file(" << fileName << ") ]" << std::endl;
        std::exit(1);
    }

    std::fpos_t fileSize;
    if (std::fgetpos(file, &fileSize) != 0)
    {
        std::cerr << "Error while parsing file [ compile_compute_shader_file(" << fileName << ") ]" << std::endl;
        std::exit(1);
    }

    char code[fileSize] = {};

    if (std::fseek(file, 0, SEEK_SET) != 0)
    {
        std::cerr << "Error while parsing file [ compile_compute_shader_file(" << fileName << ") ]" << std::endl;
        std::exit(1);
    }
    if (std::fread(code, 1, fileSize, file) != fileSize)
    {
        std::cerr << "Error while parsing file [ compile_compute_shader_file(" << fileName << ") ]" << std::endl;
        std::exit(1);
    }

    return compile_compute_shader(code);
}

GLint get_uniform_location(ComputeShader& shader, const std::string& name)
{
    if (shader.m_programId != static_cast<GLuint>(glGetHandleARB(GL_PROGRAM_OBJECT_ARB)))
    {
        glUseProgramObjectARB(static_cast<GLhandleARB>(shader.m_programId));
    }

    std::map<std::string, GLint>::const_iterator uniformLocation;
    if ((uniformLocation = shader.m_uniformLocations.find(name)) != shader.m_uniformLocations.end())
        return uniformLocation->second;
    
    GLint location = glGetUniformLocation(shader.m_programId, name.c_str());
    shader.m_uniformLocations.insert(std::make_pair(name, location));

    if (location == -1)
        std::cerr << "Warning : uniform " << name << " was not found in shader and will not be set." << std::endl;

    return location;
}

#pragma region // set_uniform cluster

void set_uniform(ComputeShader& shader, const std::string& name, float value) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform1f(uniformLocation, value);
}
void set_uniform(ComputeShader& shader, const std::string& name, double value) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform1d(uniformLocation, value);
}
void set_uniform(ComputeShader& shader, const std::string& name, int value) {
    GLint uniformLocation = get_uniform_location(shader, name);
    glUniform1i(uniformLocation, value);
}
void set_uniform(ComputeShader& shader, const std::string& name, unsigned int value) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform1ui(uniformLocation, value);
}
void set_uniform(ComputeShader &shader, const std::string &name, bool value) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform1i(uniformLocation, (int)value);
}
void set_uniform(ComputeShader &shader, const std::string &name, float value0, float value1)
{
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform2f(uniformLocation, value0, value1);
}
void set_uniform(ComputeShader &shader, const std::string &name, double value0, double value1)
{
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform2d(uniformLocation, value0, value1);
}
void set_uniform(ComputeShader& shader, const std::string& name, int value0, int value1) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform2i(uniformLocation, value0, value1);
}
void set_uniform(ComputeShader& shader, const std::string& name, unsigned int value0, unsigned int value1) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform2ui(uniformLocation, value0, value1);
}
void set_uniform(ComputeShader &shader, const std::string &name, bool value0, bool value1) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform2i(uniformLocation, (int)value0, (int)value1);
}
void set_uniform(ComputeShader &shader, const std::string &name, float value0, float value1, float value2) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform3f(uniformLocation, value0, value1, value2);
}
void set_uniform(ComputeShader &shader, const std::string &name, double value0, double value1, double value2) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform3d(uniformLocation, value0, value1, value2);
}
void set_uniform(ComputeShader& shader, const std::string& name, int value0, int value1, int value2) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform3i(uniformLocation, value0, value1, value2);
}
void set_uniform(ComputeShader& shader, const std::string& name, unsigned int value0, unsigned int value1, unsigned int value2) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform3ui(uniformLocation, value0, value1, value2);
}
void set_uniform(ComputeShader &shader, const std::string &name, bool value0, bool value1, bool value2) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform3i(uniformLocation, (int)value0, (int)value1, (int)value2);
}
void set_uniform(ComputeShader &shader, const std::string &name, float value0, float value1, float value2, float value3) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform4f(uniformLocation, value0, value1, value2, value3);
}
void set_uniform(ComputeShader &shader, const std::string &name, double value0, double value1, double value2, double value3) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform4d(uniformLocation, value0, value1, value2, value3);
}
void set_uniform(ComputeShader& shader, const std::string& name, int value0, int value1, int value2, int value3) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform4i(uniformLocation, value0, value1, value2, value3);
}
void set_uniform(ComputeShader& shader, const std::string& name, unsigned int value0, unsigned int value1, unsigned int value2, unsigned int value3) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform4ui(uniformLocation, value0, value1, value2, value3);
}
void set_uniform(ComputeShader &shader, const std::string &name, bool value0, bool value1, bool value2, bool value3) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform4i(uniformLocation, (int)value0, (int)value1, (int)value2, (int)value3);
}

void set_uniform_array_scalar(ComputeShader &shader, const std::string &name, const float *values, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform1fv(uniformLocation, arrayLength, values);
}
void set_uniform_array_scalar(ComputeShader &shader, const std::string &name, const double *values, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform1dv(uniformLocation, arrayLength, values);
}
void set_uniform_array_scalar(ComputeShader &shader, const std::string &name, const int *values, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform1iv(uniformLocation, arrayLength, values);
}
void set_uniform_array_scalar(ComputeShader &shader, const std::string &name, const unsigned int *values, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform1uiv(uniformLocation, arrayLength, values);
}
void set_uniform_array_scalar(ComputeShader &shader, const std::string &name, const bool *values, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
    {
        int convertedValues[arrayLength];
        for (size_t i = 0; i < arrayLength; i++)
            convertedValues[i] = (int)values[i];
        
        glUniform1iv(uniformLocation, arrayLength, convertedValues);
    }
}
void set_uniform_array_2_tuple(ComputeShader &shader, const std::string &name, const float *flattenedValues, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform2fv(uniformLocation, arrayLength, flattenedValues);
}
void set_uniform_array_2_tuple(ComputeShader &shader, const std::string &name, const double *flattenedValues, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform2dv(uniformLocation, arrayLength, flattenedValues);
}
void set_uniform_array_2_tuple(ComputeShader &shader, const std::string &name, const int *flattenedValues, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform2iv(uniformLocation, arrayLength, flattenedValues);
}
void set_uniform_array_2_tuple(ComputeShader &shader, const std::string &name, const unsigned int *flattenedValues, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform2uiv(uniformLocation, arrayLength, flattenedValues);
}
void set_uniform_array_2_tuple(ComputeShader &shader, const std::string &name, const bool *flattenedValues, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
    {
        int convertedValues[arrayLength*2];
        for (size_t i = 0; i < arrayLength*2; i++)
            convertedValues[i] = (int)flattenedValues[i];
        
        glUniform2iv(uniformLocation, arrayLength, convertedValues);
    }
}
void set_uniform_array_3_tuple(ComputeShader &shader, const std::string &name, const float *flattenedValues, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform3fv(uniformLocation, arrayLength, flattenedValues);
}
void set_uniform_array_3_tuple(ComputeShader &shader, const std::string &name, const double *flattenedValues, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform3dv(uniformLocation, arrayLength, flattenedValues);
}
void set_uniform_array_3_tuple(ComputeShader &shader, const std::string &name, const int *flattenedValues, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform3iv(uniformLocation, arrayLength, flattenedValues);
}
void set_uniform_array_3_tuple(ComputeShader &shader, const std::string &name, const unsigned int *flattenedValues, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform3uiv(uniformLocation, arrayLength, flattenedValues);
}
void set_uniform_array_3_tuple(ComputeShader &shader, const std::string &name, const bool *flattenedValues, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
    {
        int convertedValues[arrayLength*3];
        for (size_t i = 0; i < arrayLength*3; i++)
            convertedValues[i] = (int)flattenedValues[i];
        
        glUniform3iv(uniformLocation, arrayLength, convertedValues);
    }
}
void set_uniform_array_4_tuple(ComputeShader &shader, const std::string &name, const float *flattenedValues, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform4fv(uniformLocation, arrayLength, flattenedValues);
}
void set_uniform_array_4_tuple(ComputeShader &shader, const std::string &name, const double *flattenedValues, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform4dv(uniformLocation, arrayLength, flattenedValues);
}
void set_uniform_array_4_tuple(ComputeShader &shader, const std::string &name, const int *flattenedValues, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform4iv(uniformLocation, arrayLength, flattenedValues);
}
void set_uniform_array_4_tuple(ComputeShader &shader, const std::string &name, const unsigned int *flattenedValues, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
        glUniform4uiv(uniformLocation, arrayLength, flattenedValues);
}
void set_uniform_array_4_tuple(ComputeShader &shader, const std::string &name, const bool *flattenedValues, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (uniformLocation != -1)
    {
        int convertedValues[arrayLength*4];
        for (size_t i = 0; i < arrayLength*4; i++)
            convertedValues[i] = (int)flattenedValues[i];
        
        glUniform4iv(uniformLocation, arrayLength, convertedValues);
    }
}

void set_uniform_matrix(ComputeShader &shader, const std::string &name, int x_size, int y_size, const float *flattennedValues, bool transpose) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (x_size < 2 || x_size > 4 || y_size < 2 || y_size > 4) {
        std::cerr << "Invalid dimensions for matrix : given " << x_size << 'x' << y_size << " when size must be between 2 and 4." << std::endl;
        return;
    }
    if (uniformLocation != -1)
        __computeHandler_matrix_functions_float.at(std::make_pair(x_size, y_size))(uniformLocation, 1, static_cast<GLboolean>(transpose), flattennedValues);
}
void set_uniform_matrix(ComputeShader &shader, const std::string &name, int x_size, int y_size, const double *flattennedValues, bool transpose) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (x_size < 2 || x_size > 4 || y_size < 2 || y_size > 4) {
        std::cerr << "Invalid dimensions for matrix : given " << x_size << 'x' << y_size << " when size must be between 2 and 4." << std::endl;
        return;
    }
    if (uniformLocation != -1)
        __computeHandler_matrix_functions_double.at(std::make_pair(x_size, y_size))(uniformLocation, 1, static_cast<GLboolean>(transpose), flattennedValues);
}

void set_uniform_array_matrix(ComputeShader &shader, const std::string &name, int x_size, int y_size, const float *flattennedValues, bool transpose, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (x_size < 2 || x_size > 4 || y_size < 2 || y_size > 4) {
        std::cerr << "Invalid dimensions for matrix : given " << x_size << 'x' << y_size << " when size must be between 2 and 4." << std::endl;
        return;
    }
    if (uniformLocation != -1)
        __computeHandler_matrix_functions_float.at(std::make_pair(x_size, y_size))(uniformLocation, arrayLength, static_cast<GLboolean>(transpose), flattennedValues);
}
void set_uniform_array_matrix(ComputeShader &shader, const std::string &name, int x_size, int y_size, const double *flattennedValues, bool transpose, int arrayLength) {
    GLint uniformLocation = get_uniform_location(shader, name);
    if (x_size < 2 || x_size > 4 || y_size < 2 || y_size > 4) {
        std::cerr << "Invalid dimensions for matrix : given " << x_size << 'x' << y_size << " when size must be between 2 and 4." << std::endl;
        return;
    }
    if (uniformLocation != -1)
        __computeHandler_matrix_functions_double.at(std::make_pair(x_size, y_size))(uniformLocation, arrayLength, static_cast<GLboolean>(transpose), flattennedValues);
}

void set_uniform_texture(ComputeShader &shader, const std::string &name, unsigned int textureHandle) {
    GLint uniformLocation = get_uniform_location(shader, name);

    if (uniformLocation != -1)
    {
        std::map<GLint, GLuint>::iterator it;
        if ((it = shader.m_textureUniformHandles.find(uniformLocation)) != shader.m_textureUniformHandles.end())
        {
            it->second = textureHandle;
        }
        else
        {
            shader.m_textureUniformHandles.insert(std::make_pair(uniformLocation, textureHandle));
        }
    }
}

#pragma endregion // set_uniform_cluster

void set_buffer(ComputeShader& shader, const void *data, std::size_t size, int bindIndex)
{
    glUseProgramObjectARB(shader.m_programId);

    GLuint bufferId;
    glGenBuffersARB(1, &bufferId);
    glBindBufferARB(GL_SHADER_STORAGE_BUFFER, bufferId);
    glBufferDataARB(GL_SHADER_STORAGE_BUFFER, size, data, )

}

void execute_compute_shader(const ComputeShader &shader, GLuint x, GLuint y, GLuint z)
{
    if (x >= GL_MAX_COMPUTE_WORK_GROUP_COUNT)
    {
        std::cerr << "Tried to launch " << x << " work groups in x axis when max is " << GL_MAX_COMPUTE_WORK_GROUP_COUNT << " [ execute_compute_shader ]" << std::endl;
        std::exit(1);
    }
    else if (y >= GL_MAX_COMPUTE_WORK_GROUP_COUNT)
    {
        std::cerr << "Tried to launch " << y << " work groups in y axis when max is " << GL_MAX_COMPUTE_WORK_GROUP_COUNT << " [ execute_compute_shader ]" << std::endl;
        std::exit(1);
    }
    else if (z >= GL_MAX_COMPUTE_WORK_GROUP_COUNT)
    {
        std::cerr << "Tried to launch " << z << " work groups in z axis when max is " << GL_MAX_COMPUTE_WORK_GROUP_COUNT << " [ execute_compute_shader ]" << std::endl;
        std::exit(1);
    }

    glUseProgram(shader.m_programId);

    GLenum errorCode = glGetError();
    if (errorCode != GL_NO_ERROR)
    {
        std::cerr << "An error has occured while trying to run a compute shader." << std::endl;
        glUseProgram(0);
        std::exit(1);
    }

    std::map<GLint, GLuint>::const_iterator it = shader.m_textureUniformHandles.begin();
    for (int i = 0; i < shader.m_textureUniformHandles.size(); ++i)
    {
        int index = i + 1;
        glUniform1i(it->first, index);
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, it->second);
        ++it;
    }

    glDispatchCompute(x, y, z);
}

