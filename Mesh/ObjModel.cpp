
#include <fstream>
#include <sstream>
#include <set>
#include "ObjModel.h"


void ObjModel::update_transform(float deltaTime) {
    model.set_rotationY(160);
    shader->set_model_matrix(model);
}

ObjModel::ObjModel(const std::string& file, std::shared_ptr<IShader>& iShader) : Mesh(iShader) {
    std::ifstream in;
    in.open(file, std::ifstream::in);
    if (in.fail()) std::cout << "Fail to load obj->" << file << std::endl;

    std::string line;

    std::vector<Vec3f> vertices_tmp;
    std::vector<Vec3f> normals_tmp;
    std::vector<Vec2f> texcoords_tmp;

    Vec3i ind_tmp;
    int pos = 0; //for ind
    std::set<int> cached_vertex_ind;

    while (!in.eof()){
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char prefix;
        if(!line.compare(0, 2, "v "))
        {
            iss >> prefix;
            Vec3f vertex;
            iss >> vertex.x;
            iss >> vertex.y;
            iss >> vertex.z;
            vertices_tmp.push_back(vertex);
        }

        vertices.resize(vertices_tmp.size());

        if(!line.compare(0, 3, "vn "))
        {
            iss >> prefix >> prefix;
            Vec3f normal;
            iss >> normal.x;
            iss >> normal.y;
            iss >> normal.z;
            normal.normalize();
            normals_tmp.push_back(normal);
        }
        else if(!line.compare(0, 3, "vt "))
        {
            iss >> prefix >> prefix;
            Vec2f texcoord;
            iss >> texcoord.x;
            iss >> texcoord.y;
            texcoords_tmp.push_back(texcoord);
        }
        else if(!line.compare(0, 2, "f ")){

            int vx, vy, vz;
            int tx, ty, tz;
            int nx, ny, nz;

            iss >> prefix;
            iss >> vx >> prefix >> tx >> prefix >> nx >>
                vy >> prefix >> ty >> prefix >> ny >>
                vz >> prefix >> tz >> prefix >> nz;

            // in wavefront obj all indices start at 1, not zero
            vx--; tx--; nx--;
            vy--; ty--; ny--;
            vz--; tz--; nz--;

            Vertex vert0(vertices_tmp[vx], White, normals_tmp[nx], texcoords_tmp[tx]);
            Vertex vert1(vertices_tmp[vy], White, normals_tmp[ny], texcoords_tmp[ty]);
            Vertex vert2(vertices_tmp[vz], White, normals_tmp[nz], texcoords_tmp[tz]);

            if(cached_vertex_ind.find(vx) == cached_vertex_ind.end()){
                vertices[vx] = vert0;
                cached_vertex_ind.insert(vx);
            }

            if(cached_vertex_ind.find(vy) == cached_vertex_ind.end()){
                vertices[vy] = vert1;
                cached_vertex_ind.insert(vy);
            }

            if(cached_vertex_ind.find(vz) == cached_vertex_ind.end()){
                vertices[vz] = vert2;
                cached_vertex_ind.insert(vz);
            }

            ind.emplace_back(Vec3i(vx, vy, vz));
        }
    }

    cached_vertex_ind.clear();
    in.close();

    std::cerr << "v# " << vertices_tmp.size() <<
              "		vn# " << normals_tmp.size() <<
              "		vt# " << texcoords_tmp.size() <<
              "	f# " << ind.size() << std::endl;
}


