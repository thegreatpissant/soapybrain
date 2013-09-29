#ifndef SHADER_H_
#define SHADER_H_

using ShaderID = GLint;

class Shader {};
//  We need to understand this more before moving forward
//  Different levels of shaders
//  Guarantee that some will exist - ex: Camera and Viewport transforms
//  Some are more model specific - ex: model space transforms
//
//  Vertex -> Model Transform -> Camera Transform -> ViewportTransform
//
//  How do we chain these properly?

#endif
