#include "stack.h"

Stack::Stack(QQuickItem *parent) : ShapeItem(parent) {
    model = Stack3d();
}

void Stack::computeGeometry(QVector<TriangleVertex> &vertices) {
    model.Rotate(m_matrix, isWhite() ? 0 : 1050, 0, 0);

    // Project 3D points to 2D
    QVector<QVector<double>> points2d;
    double translation[4][4];
    m_matrix->getInverseTranslationMatrix(translation);
    for (int i = 0; i < model.cache.size(); i++) {
        points2d.append(m_matrix->ProjectPoint(m_matrix->MultiplyPointAndMatrix(model.cache[i], translation)));
    }

    const auto& faces = model.faces;

    for (int i = 0; i < faces.size(); i++) {
        float x1 = points2d[faces[i][0]][0];
        float y1 = points2d[faces[i][0]][1];
        float x2 = points2d[faces[i][1]][0];
        float y2 = points2d[faces[i][1]][1];
        float x3 = points2d[faces[i][2]][0];
        float y3 = points2d[faces[i][2]][1];

        vertices.append(flatTriangle(x1, y1, x2, y2, x3, y3, COLOR, true));
    }
}

bool Stack::isWhite() const
{
    return m_isWhite;
}

void Stack::setWhite(const bool &isWhite)
{
    m_isWhite = isWhite;
}