#include<iostream>
#include "Math.h"
#include "Camera.h"
#include "Rasterizer.h"
#include "Window.h"
#include "Timer.h"
#include "Mesh/Triangle.h"
#include "Mesh/Box.h"
#include "Shader/PhongShader.h"

int main(){
    int width = 800;
    int height = 600;
    double delta_time;

    Vec3f eye(0.0f, 1.0f, 6.0f);
    Vec3f light_pos(-1.f, 0.f, 5.f);
    Camera camera(eye, Vec3f(0, 0, -5.f),Vec3f (0, 1, 0));
    camera.set_projection(45, (float)width/height, 0.1, 50);

    Rasterizer rasterizer(width, height);
    rasterizer.set_view(camera.get_view_matrix());
    rasterizer.set_projection(camera.get_projection_matrix());
    rasterizer.set_viewport(width, height);

    std::shared_ptr<IShader> shader = std::make_shared<PhongShader>();
    Mesh* box = new Box(shader);
    rasterizer.add_mesh(box);

    PhongShader* phongShader = dynamic_cast<PhongShader*>(shader.get());
    Ambient ambient(Color(255,255,255), 0.2f);
    Material material(0.5f, 0.4f, 500);

    Light* light = new DirectionalLight(light_pos, Green, 10.f);
    phongShader->set_material(&material);
    phongShader->set_ambient(&ambient);
    phongShader->set_light(light);
    phongShader->set_eye_pos(&eye);

    Window window;
    window.init(width, height);
    Timer timer;

    while (window.is_running){
        timer.start();
        rasterizer.clear(Buffers::Color | Buffers::Depth);

        const std::vector<Mesh*>& meshes = rasterizer.get_meshes();
        for (auto mesh : meshes) {
            rasterizer.set_shader(mesh->shader);
            mesh->update_transform(delta_time);
            rasterizer.draw(mesh->vertex_buf_id, mesh->ind_buf_id, RenderMode::Fill);
        }

        window.update(rasterizer.output());
        delta_time = timer.elapsed();
        window.set_title(std::to_string((int)(1/delta_time)).c_str());
    }
    window.stop();

    delete box;
    delete light;
    return 0;
}
