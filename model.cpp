#include <iostream>
#include <sstream>
#include "model.h"

Model::Model(const std::string filename) {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            QVector3D v;
            for (int i=0;i<3;i++) iss >> v[i];
            verts.push_back(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            QVector3D n;
            for (int i=0;i<3;i++) iss >> n[i];
            norms.push_back(n.normalized());
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            QVector2D uv;
            for (int i=0;i<2;i++) iss >> uv[i];
            tex_coord.push_back(QVector2D(uv[0], 1. - uv[1]));
        }  else if (!line.compare(0, 2, "f ")) {
            int f,t,n;
            iss >> trash;
            int cnt = 0;
            while (iss >> f >> trash >> t >> trash >> n) {
                facet_vrt.push_back(--f);
                facet_tex.push_back(--t);
                facet_nrm.push_back(--n);
                cnt++;
            }
            if (3!=cnt) {
                std::cerr << "Error: the obj file is supposed to be triangulated" << std::endl;
                return;
            }
        }
    }
    std::cerr << "# v# " << nverts() << " f# "  << nfaces() << " vt# " << tex_coord.size() << " vn# " << norms.size() << std::endl;
    load_texture(filename, "_diffuse.png",    m_diffusemap );
    load_texture(filename, "_nm_tangent.png", m_normalmap  );
    load_texture(filename, "_spec.png",       m_specularmap);
}

int Model::nverts() const {
    return verts.size();
}

int Model::nfaces() const {
    return facet_vrt.size()/3;
}

QVector3D Model::vert(const int i) const {
    return verts[i];
}

QVector3D Model::vert(const int iface, const int nthvert) const {
    return verts[facet_vrt[iface*3+nthvert]];
}

void Model::load_texture(std::string filename, const std::string suffix, QImage &img) {
    size_t dot = filename.find_last_of(".");
    if (dot==std::string::npos) return;
    std::string texfile = filename.substr(0,dot) + suffix;
    std::cerr << "texture file " << texfile << " loading " << (img.load(texfile.c_str()) ? "ok" : "failed") << std::endl;
}

QVector3D Model::normal(const QVector2D &uvf) const {
    QRgb c = m_normalmap.pixel(uvf[0]*m_normalmap.width(), uvf[1]*m_normalmap.height());
    return QVector3D{(float)qRed(c),(float)qGreen(c),(float)qBlue(c)}*2. / 255. - QVector3D{1,1,1};
}

QVector2D Model::uv(const int iface, const int nthvert) const {
    return tex_coord[facet_tex[iface*3+nthvert]];
}

QVector3D Model::normal(const int iface, const int nthvert) const {
    return norms[facet_nrm[iface*3+nthvert]];
}

