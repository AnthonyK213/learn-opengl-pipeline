#pragma once
#include <vector>
#include <string>
#include <QImage>
#include <QVector2d>
#include <QVector3d>
#include <fstream>

class Model {
    std::vector<QVector3D> verts{};     // array of vertices
    std::vector<QVector2D> tex_coord{}; // per-vertex array of tex coords
    std::vector<QVector3D> norms{};     // per-vertex array of normal vectors
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
    QVector3D normal(const int iface, const int nthvert) const; // per triangle corner normal vertex
    QVector3D normal(const QVector2D &uv) const;                     // fetch the normal vector from the normal map texture
    QVector3D vert(const int i) const;
    QVector3D vert(const int iface, const int nthvert) const;
    QVector2D uv(const int iface, const int nthvert) const;
    const QImage& diffuse()  const { return m_diffusemap;  }
    const QImage& specular() const { return m_specularmap; }
};

