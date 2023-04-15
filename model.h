#pragma once
#include <vector>
#include <string>
#include "geometry.h"
#include <QImage>
#include <fstream>

class Model {
    std::vector<vec3> verts{};     // array of vertices
    std::vector<vec2> tex_coord{}; // per-vertex array of tex coords
    std::vector<vec3> norms{};     // per-vertex array of normal vectors
    std::vector<int> facet_vrt{};
    std::vector<int> facet_tex{};  // per-triangle indices in the above arrays
    std::vector<int> facet_nrm{};
    QImage m_diffusemap{};
    QImage m_normalmap{};          // normal map texture
    QImage m_specularmap{};        // specular map texture
    void load_texture(std::string filename, const std::string suffix, QImage& img);
public:
    Model(const std::string filename);
    int nverts() const;
    int nfaces() const;
    vec3 normal(const int iface, const int nthvert) const; // per triangle corner normal vertex
    vec3 normal(const vec2 &uv) const;                     // fetch the normal vector from the normal map texture
    vec3 vert(const int i) const;
    vec3 vert(const int iface, const int nthvert) const;
    vec2 uv(const int iface, const int nthvert) const;
    const QImage& diffuse()  const { return m_diffusemap;  }
    const QImage& specular() const { return m_specularmap; }
};

