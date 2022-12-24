#include "Render.hpp"
#include <iostream>
using std::cout;
using std::endl;

void Renderer::render( std::vector<std::shared_ptr<Actor>> actors ) {
    //  Set some state
    glClearColor (0.4f, 0.4f, 1.0f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable( GL_CULL_FACE );
    glFrontFace( GL_CCW );
    //  Render Objects
    for( std::shared_ptr<Actor> a : actors ) {
        auto shader = get_shader( a->model_id );
        shader->use();
        shader->setUniform("Model", a->getTransform());
        shader->setUniform("View", glm::inverse(camera->getTransform()));
        shader->setUniform("Projection", display->getProjection());
        get_model( a->model_id )->render();
        shader->unuse();
    }
    //  Cleanup
    glBindVertexArray( 0 );
    glFinish();
}

ModelID Renderer::add_model( std::shared_ptr<Model> model ) {
    std::cout << "Renderer added Model id " << GID << " Name " << model->name << std::endl;
    models[GID] = model;
    ++GID;
    return GID;
}

std::shared_ptr<Model> Renderer::get_model (ModelID mid) {
    if (models.empty())
    {
        std::cerr << "No models available" << std::endl;
        std::exit (EXIT_FAILURE);
    }
    if (models.find (mid) == models.end())
        return models[0];
    return models[mid];
}

ShaderID Renderer::add_shader( std::shared_ptr<ShaderProgram> shader )
{
    std::cout << "Renderer adding Shader id:" << SID << ", Name:\"" << shader->getName() << "\"" << endl;
    shaders[SID] = shader;
    ++SID;
    return SID-1;
}

std::shared_ptr<ShaderProgram> Renderer::get_shader( ModelID mid )
{
    ShaderID sid = get_model(mid)->shader;
    if (shaders.empty())
    {
        std::cerr << "No shaders available" << std::endl;
        std::exit (EXIT_FAILURE);
    }
    if (shaders.find(sid) == shaders.end())
        return shaders[0];
    return shaders[sid];
}

void Renderer::set_display(std::shared_ptr<Display> disp)
{
    this->display = disp;
}

void Renderer::set_camera(std::shared_ptr<Camera> camera)
{
    this->camera = camera;
}

Renderer::~Renderer()
{
}

void Renderer::init( ) {
    glEnable(GL_DEPTH_TEST);
}
