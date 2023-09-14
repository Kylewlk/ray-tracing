//
// Created by DELL on 2022/3/4.
//

#include "Shader.h"
#include "Utils.h"
#include "Texture.h"
#include <iostream>

Shader::Shader() = default;

Shader::~Shader()
{
    if (program != 0)
    {
        glDeleteProgram(program);
        this->program = 0;
    }
}

ShaderRef Shader::create(const ShaderInfo* si)
{
    auto pro = loadShade(si);
    if (pro == 0)
    {
        return {};
    }

    auto shader = new Shader;
    shader->program = pro;
    return ShaderRef(shader);
}

ShaderRef Shader::create(const std::string& vertSrc, const std::string& fragSrc)
{
    ShaderInfo si[] = {{GL_VERTEX_SHADER, vertSrc, false}, {GL_FRAGMENT_SHADER, fragSrc, false}, {}};
    return create(si);
}

ShaderRef Shader::createByPath(const std::string& vertPath, const std::string& fragPath)
{
    ShaderInfo si[] = {{GL_VERTEX_SHADER, vertPath, true}, {GL_FRAGMENT_SHADER, fragPath, true}, {}};
    return create(si);
}

GLuint Shader::loadShade(const ShaderInfo* si)
{
    GLuint program = glCreateProgram();

    int i = 0;

    GLuint shader = 0;
    while (GL_NONE != si[i].type)
    {
        if (si[i].isPath)
        {
            auto source = readFileToString(si[i].source);
            shader = loadShade(si[i].type, source);
        }
        else if (!si[i].source.empty())
        {
            shader = loadShade(si[i].type, si[i].source);
        }
        else
        {
            std::cerr << si[i].type << " Shader source is Empty Or File is not exist. (" << si[i].source << ")\n";
            glDeleteProgram(program);
        }
        if (shader == 0)
        {
            std::cerr << " (" << si[i].source << ")\n";
            glDeleteProgram(program);
            return 0;
        }
        glAttachShader(program, shader);
        glDeleteShader(shader);
        i++;
    }

    glLinkProgram(program);

    GLint linked = 1;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (!linked)
    {
        GLsizei len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        char* log = new char[len + 1];
        glGetProgramInfoLog(program, len, &len, log);
        std::cerr << "Shader Linking error: " << log << std::endl;
        delete[] log;
        glDeleteProgram(program);
        return 0;
    }
    CHECK_GL_ERROR();

    return program;
}

GLuint Shader::loadShade(GLenum type, const std::string& source)
{
    GLuint shader = glCreateShader(type);
    const char* src = source.data();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    GLint compiled = 1;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLsizei len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        char* log = new char[len + 1];
        glGetShaderInfoLog(shader, len, &len, log);
        std::cerr << type << "  "
                  << "Shader compilation failed: " << log;
        delete[] log;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLint Shader::location(std::string_view uniformName) const
{
    return glGetUniformLocation(this->program, uniformName.data());
}

void Shader::use()
{
    glUseProgram(this->program);
    this->texUnit = 0;
}

void Shader::bindTexture(const std::string& uniform, const Texture* tex)
{
    tex->bind(this->texUnit);
    GLint location = glGetUniformLocation(this->program, uniform.c_str());
    glUniform1i(location, this->texUnit);
    ++this->texUnit;
}

void Shader::bindTexture(GLint uniformLocation, const Texture* tex)
{
    tex->bind(this->texUnit);
    glUniform1i(uniformLocation, this->texUnit);
    ++this->texUnit;
}

void Shader::setUniform(std::string_view name, bool value) const
{
    auto location = glGetUniformLocation(this->program, name.data());
    if (location == -1)
    {
        return;
    }
    glUniform1i(location, (GLint)value);
}

void Shader::setUniform(std::string_view name, int value) const
{
    auto location = glGetUniformLocation(this->program, name.data());
    if (location == -1)
    {
        return;
    }
    glUniform1i(location, (GLint)value);
}

void Shader::setUniform(std::string_view name, bool* boolArray, int count) const
{
    for (int i = 0; i < count; ++i)
    {
        std::string nameIndex{name};
        nameIndex += '[';
        nameIndex += std::to_string(i);
        nameIndex += ']';
        auto location = glGetUniformLocation(this->program, nameIndex.data());
        if (location == -1)
        {
            return;
        }
        glUniform1i(location, (GLint)(boolArray[i]));
    }
}

void Shader::setUniform(std::string_view name, int* intArray, int count) const
{
    auto location = glGetUniformLocation(this->program, name.data());
    if (location == -1)
    {
        return;
    }
    glUniform1iv(location, count, intArray);
}

void Shader::setUniform(std::string_view name, float f) const
{
    auto location = glGetUniformLocation(this->program, name.data());
    if (location == -1)
    {
        return;
    }
    glUniform1f(location, f);
}

void Shader::setUniform(std::string_view name, float f1, float f2) const
{
    auto location = glGetUniformLocation(this->program, name.data());
    if (location == -1)
    {
        return;
    }
    glUniform2f(location, f1, f2);
}

void Shader::setUniform(std::string_view name, float f1, float f2, float f3) const
{
    auto location = glGetUniformLocation(this->program, name.data());
    if (location == -1)
    {
        return;
    }
    glUniform3f(location, f1, f2, f3);
}

void Shader::setUniform(std::string_view name, float f1, float f2, float f3, float f4) const
{
    auto location = glGetUniformLocation(this->program, name.data());
    if (location == -1)
    {
        return;
    }
    glUniform4f(location, f1, f2, f3, f4);
}

void Shader::setUniform(std::string_view name, const math::Vec2& value) const
{
    auto location = glGetUniformLocation(this->program, name.data());
    if (location == -1)
    {
        return;
    }
    glUniform2f(location, value.x, value.y);
}

void Shader::setUniform(std::string_view name, const math::Vec3& value) const
{
    auto location = glGetUniformLocation(this->program, name.data());
    if (location == -1)
    {
        return;
    }
    glUniform3f(location, value.x, value.y, value.z);
}

void Shader::setUniform(const std::string_view name, const math::Vec4& value) const
{
    auto location = glGetUniformLocation(this->program, name.data());
    if (location == -1)
    {
        return;
    }
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::setUniform(std::string_view name, const math::Vec2* value, int count) const
{
    auto location = glGetUniformLocation(this->program, name.data());
    if (location == -1)
    {
        return;
    }
    glUniform2fv(location, count, reinterpret_cast<const GLfloat*>(value));
}

void Shader::setUniform(std::string_view name, const math::Vec3* value, int count) const
{
    auto location = glGetUniformLocation(this->program, name.data());
    if (location == -1)
    {
        return;
    }
    glUniform3fv(location, count, reinterpret_cast<const GLfloat*>(value));
}

void Shader::setUniform(std::string_view name, const math::Vec4* value, int count) const
{
    auto location = glGetUniformLocation(this->program, name.data());
    if (location == -1)
    {
        return;
    }
    glUniform4fv(location, count, reinterpret_cast<const GLfloat*>(value));
}

void Shader::setUniform(std::string_view name, const math::Mat2& value) const
{
    auto location = glGetUniformLocation(this->program, name.data());
    if (location == -1)
    {
        return;
    }
    glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
}

void Shader::setUniform(std::string_view name, const math::Mat3& value) const
{
    auto location = glGetUniformLocation(this->program, name.data());
    if (location == -1)
    {
        return;
    }
    glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}

void Shader::setUniform(const std::string_view name, const math::Mat4& value) const
{
    auto location = glGetUniformLocation(this->program, name.data());
    if (location == -1)
    {
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

