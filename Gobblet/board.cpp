#include "board.h"

Board::Board(QQuickItem *parent) : ShapeItem(parent) {
    model = Board3d();
}

static bool dotProduct(const QVector<QVector<double>> &points, const QVector<int> &face) {
    double a1 = points[face[1]][0] - points[face[0]][0];
    double a2 = points[face[1]][1] - points[face[0]][1];
    double b1 = points[face[2]][0] - points[face[0]][0];
    double b2 = points[face[2]][1] - points[face[0]][1];
    return (a1*b2 - a2*b1) < 0;
}

void Board::computeGeometry(QVector<TriangleVertex> &vertices) {
    model.Rotate(m_matrix, 0, 0, 0);

    // Project 3D points to 2D
    QVector<QVector<double>> points2d;
    double translation[4][4];
    m_matrix->getInverseTranslationMatrix(translation);

    for (int i = 0; i < model.cache.size(); i++) {
        points2d.append(m_matrix->ProjectPoint(m_matrix->MultiplyPointAndMatrix(model.cache[i], translation)));
    }

    const auto& faces = model.faces;

    for (int i = 0; i < faces.size(); i++) {
        // Side faces (0-7): only draw if visible via backface culling
        // Top faces (8+): always draw
        if ((i > 7) || dotProduct(points2d, faces[i])) {
            QColor color;
            if (i < 8) {
                color = QColorConstants::Svg::burlywood;
            } else if (i < 10) {
                color = QColor(QColorConstants::Svg::sienna).darker(190);
            } else {
                color = QColor(QColorConstants::Svg::oldlace).darker(120);
            }

            float x1 = points2d[faces[i][0]][0];
            float y1 = points2d[faces[i][0]][1];
            float x2 = points2d[faces[i][1]][0];
            float y2 = points2d[faces[i][1]][1];
            float x3 = points2d[faces[i][2]][0];
            float y3 = points2d[faces[i][2]][1];

            vertices.append({x1, y1, x2, y2, x3, y3, color, false});
        }
    }
}
