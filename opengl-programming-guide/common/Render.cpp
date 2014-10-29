#include "Render.h"

void Renderer::Render( std::vector<shared_ptr<Actor>> &actors ) {



    static float rot = 1.0f;
    for ( shared_ptr<Actor> a : actors ) {
        glm::mat4 model = glm::mat4(1.0f);
        model *= glm::translate(glm::mat4(1.0f), a->getPosition());
        model *= glm::rotate( glm::mat4(1.0f), a->getOrientation().x + rot, glm::vec3(0.0f, 1.0f, 0.0));
        a->getShader()->setUniform("M", model);
        get_model (a->model_id)->render( );
    }
    rot += 0.2f;

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
}
