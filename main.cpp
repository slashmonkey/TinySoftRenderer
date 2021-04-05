#include<iostream>
#include "Math.h"
#include "Camera.h"
#include "Rasterizer.h"
#include "Window.h"
#include "Timer.h"
#include "Mesh/Triangle.h"
#include "Mesh/Box.h"

int main(){
    int width = 800;
    int height = 600;
    double delta_time;

    Camera camera(Vec3f(0.0f, 0.0f, 3.0f),Vec3f(0, 0, -5.f),Vec3f (0, 1, 0));
    camera.set_projection(45, (float)width/height, 0.1, 50);

    Rasterizer rasterizer(width, height);
    rasterizer.set_view(camera.get_view_matrix());
    rasterizer.set_projection(camera.get_projection_matrix());
    rasterizer.set_viewport(width, height);

    std::shared_ptr<IShader> simpleShader = std::make_shared<SimpleShader>();
    Mesh* box = new Box(simpleShader);
    rasterizer.add_mesh(box);

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

    return 0;
}
