
#include <fstream>
#include <sstream>
#include <set>
#include "ObjModel.h"


void ObjModel::update_transform(float deltaTime) {
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
        else if(!line.compare(0, 3, "vn "))
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
            iss >> prefix;
            int index[3]; // v/vt/vn
            while(iss >> index[0] >> prefix >> index[1] >> prefix >> index[2])
            {
                Vertex data;
                data.position = vertices_tmp[index[0] - 1];
                data.texcoord = texcoords_tmp[index[1] - 1];
                data.normal = normals_tmp[index[2] - 1];
                data.color = White;

                if (pos == 0){
                    ind_tmp.x = index[0] - 1;
                    pos++;
                }else if(pos == 1){
                    ind_tmp.y = index[0] - 1;
                    pos++;
                }else if(pos == 2){
                    ind_tmp.z = index[0] - 1;
                    ind.emplace_back(ind_tmp);
                    pos = 0;
                }
                //Add if vertex not included
                if(cached_vertex_ind.find(index[0] - 1) == cached_vertex_ind.end()){
                    vertices.emplace_back(data);
                    cached_vertex_ind.insert(index[0] - 1);
                }
            }
        }
    }
    cached_vertex_ind.clear();
    in.close();

    std::cerr << "v# " << vertices.size() <<
              "		vn# " << normals_tmp.size() <<
              "		vt# " << texcoords_tmp.size() <<
              "	f# " << ind.size() << std::endl;
}


