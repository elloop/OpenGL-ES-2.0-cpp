# OpenGL-ES-2.0-cpp
---------------------

# Introduction

This is a graphics application based on OpenGL ES 2.0 API, using C++ and the OpenGL ES Shading Language. The logic structure of this project is like the cocos2d-x game engine, it is started by main.cpp which contains main() function and calls Appdelegate's run() function.

Main contents in this project are:

## Draw Rectangle， Circle， etc , simple graphics.

these simple graphics drawwing programs help understanding the process of a OpenGL ES program running.

## Texture 

- basic texture render

- multiple texture render


![dog_texture.gif](http://elloop.oss-cn-hangzhou.aliyuncs.com/blog_pic/dog_texture.gif?OSSAccessKeyId=LTAI4FtWWR7eyV7ELYCixBe2&Expires=5183646452&Signature=oB1Q9g7MskNK%2F%2BMbG6JAZKvFrxY%3D)

## Mipmapping

![mipmap_effect.gif](http://elloop.oss-cn-hangzhou.aliyuncs.com/blog_pic/mipmap_effect.gif?OSSAccessKeyId=LTAI4FtWWR7eyV7ELYCixBe2&Expires=1587205527&Signature=hOunhdhF1K%2FsHED6TjrUuxog0Fc%3D)

## 3D projection

![comprehensive_test.gif](http://elloop.oss-cn-hangzhou.aliyuncs.com/blog_pic/comprehensive_test.gif?OSSAccessKeyId=LTAI4FtWWR7eyV7ELYCixBe2&Expires=1587205544&Signature=%2FQh89gTHPNGHYj4TIvap2s0DwYc%3D)

## Using of buffer objects: VAO, VBO

![vbo_cube.gif](http://elloop.oss-cn-hangzhou.aliyuncs.com/blog_pic/vbo_cube.gif?OSSAccessKeyId=LTAI4FtWWR7eyV7ELYCixBe2&Expires=1587205564&Signature=oaxEKsbSGnBNRcu36CMiIGKNt14%3D)

## Alpha test, depth test

# [ ] Critical Codes

```c++
namespace elloop { 

class Application;      // application abstraction.

class AppDelegate;      // derived from Application.

class OpenGLView;       // Windows control and OpenGL ES Drawing Context.

class Director;         // control the process of rendering.

class Drawable;         // things to be rendered.

class Ref;              // Object, for reference counting.

class AutoReleasePool;  // manage the reference count of pointer to object of type Ref.

class PoolManager;      // manage AutoReleasePool stack.

// ShaderHelper: help class for compiling and linking shader programs
class ShaderHelper {
public:
    static const ShaderId compileVertexShader(const std::string &shaderSrc);
    static const ShaderId compileFragmentShader(const std::string &shaderSrc);
    static ProgramId linkProgram(ShaderId vertexShaderId, ShaderId fragShaderId);
private:
    static ShaderId compileShader(const std::string &shaderSrc, GLenum shaderType);
};

}
```

# License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).

# Blogs about this project

- [【C++ OpenGL ES 2.0编程笔记】4: 纹理贴图-图片叠加效果实现](http://blog.csdn.net/elloop/article/details/50458613)

- [【C++ OpenGL ES 2.0编程笔记】5: mipmap](http://blog.csdn.net/elloop/article/details/50466163)

- [【C++ OpenGL ES 2.0编程笔记】8: 使用VBO和IBO绘制立方体](http://blog.csdn.net/elloop/article/details/50472699)

- [【C++ OpenGL ES 2.0编程笔记】1: OpenGL ES 2.0 渲染管线和EGL](http://blog.csdn.net/elloop/article/details/50480040)

=============================

# 中文文档

这个项目是一个基于OpenGL ES 2.0 API的图形演示程序，编程语言是C++和The OpenGL ES Shading Language. 工程的逻辑架构是模仿cocos2d-x来组织的，main函数在main.cpp里面，其中启动Appdelegate来完成程序的启动。

主要包含的内容有：

## 绘制矩形，圆等简单图形

## 绘制纹理

## Mipmapping

## 3D投影

## 使用缓冲对象：VAO，VBO

## Alpha测试 深度测试

# 关于此项目的一些博客

- [【C++ OpenGL ES 2.0编程笔记】4: 纹理贴图-图片叠加效果实现](http://blog.csdn.net/elloop/article/details/50458613)

- [【C++ OpenGL ES 2.0编程笔记】5: mipmap](http://blog.csdn.net/elloop/article/details/50466163)

- [【C++ OpenGL ES 2.0编程笔记】8: 使用VBO和IBO绘制立方体](http://blog.csdn.net/elloop/article/details/50472699)

- [【C++ OpenGL ES 2.0编程笔记】1: OpenGL ES 2.0 渲染管线和EGL](http://blog.csdn.net/elloop/article/details/50480040)

# [ ] 关键代码解释

```c++
namespace elloop { 

class Application;      // application abstraction.

class AppDelegate;      // derived from Application.

class OpenGLView;       // Windows control and OpenGL ES Drawing Context.

class Director;         // control the process of rendering.

class Drawable;         // things to be rendered.

class Ref;              // Object, for reference counting.

class AutoReleasePool;  // manage the reference count of pointer to object of type Ref.

class PoolManager;      // manage AutoReleasePool stack.

// ShaderHelper: help class for compiling and linking shader programs
class ShaderHelper {
public:
    static const ShaderId compileVertexShader(const std::string &shaderSrc);
    static const ShaderId compileFragmentShader(const std::string &shaderSrc);
    static ProgramId linkProgram(ShaderId vertexShaderId, ShaderId fragShaderId);
private:
    static ShaderId compileShader(const std::string &shaderSrc, GLenum shaderType);
};

}
```

# 许可协议

[MIT License](https://opensource.org/licenses/MIT) 
