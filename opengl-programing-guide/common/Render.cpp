#include "Render.h"

void Renderer::render( std::vector<shared_ptr<Actor>> &actors ) {

    //  Select appropriate shaders for this model
    //  Will Chain standard MVW transforms as well as effects

    //  Unload the shader and continue
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glViewport( 0, 0, display.screen_width, display.screen_height );
    for ( auto a : actors ) {
        get_model (a->model_id)->render( a->state );
    }
    glBindVertexArray( 0 );
}

void Renderer::add_model( shared_ptr<Model> model ) {
    std::cout << "Renderer added Model id " << GID << " Name " << model->name << std::endl;
    models[GID] = model;
    ++GID;
}
shared_ptr<Model> Renderer::get_model (ModelID mid) {
    if (models.empty())
    {
        std::cerr << "No models available" << std::endl;
        std::exit (EXIT_FAILURE);
    }
    if (models.find (mid) == models.end())
        return models[0];
    return models[mid];
}

void Renderer::init( ) {
    glEnable(GL_DEPTH_TEST);
    glClearColor( 0.2, 0.2, 0.2, 1.0 );
}

