#include "gobbler.h"
#include <QSGGeometry>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>

Gobbler::Gobbler(QQuickItem *parent, std::shared_ptr<Gobbler3d> sharedModel) : ShapeItem(parent) {
    if (sharedModel) {
        model.setModel(sharedModel);
    }
}

QString Gobbler::name() const
{
    return m_name;
}

void Gobbler::setName(const QString &name)
{
    m_name = name;
}

bool Gobbler::isWhite() const
{
    return m_isWhite;
}

void Gobbler::setWhite(const bool &isWhite)
{
    m_isWhite = isWhite;
}

int Gobbler::size() const
{
    return m_size;
}

void Gobbler::setSize(const int &size)
{
    m_size = size;
}

int Gobbler::depth() const
{
    return m_depth;
}

void Gobbler::setDepth(const int &depth)
{
    m_depth = depth;
}

double Gobbler::x3d() const
{
    return m_x3d;
}

void Gobbler::setX3d(const double &x3d)
{
    m_x3d = x3d;
}

double Gobbler::y3d() const
{
    return m_y3d;
}

void Gobbler::setY3d(const double &y3d)
{
    m_y3d = y3d;
}

double Gobbler::z3d() const
{
    return m_z3d;
}

void Gobbler::setZ3d(const double &z3d)
{
    m_z3d = z3d;
}

void Gobbler::calculateZIndex() {
    if (depth() > 0) {
        m_zIndex = 0;
        return;
    }
    if (y3d() > 100) {
        m_zIndex = 10000;
        return;
    }

    if (!model.isOnBoard() && y3d() == 0) {
        m_zIndex = m_matrix->isVertical() ? x3d() * -3 : z3d();
    } else {
        m_zIndex = x3d() * m_matrix->yangleSin() + z3d() * m_matrix->yangleCos();
    }
}

double Gobbler::getZIndex() const
{
    return m_zIndex;
}

// Static comparator function to sort by z-index (ascending)
bool Gobbler::compareByZindex(const Gobbler* a, const Gobbler* b) {
    return a->m_zIndex < b->m_zIndex;
}

static bool dotProduct(const QVector<QVector<double>> &points, const QVector<int> &face) {
    double a1 = points[face[1]][0] - points[face[0]][0];
    double a2 = points[face[1]][1] - points[face[0]][1];
    double b1 = points[face[2]][0] - points[face[0]][0];
    double b2 = points[face[2]][1] - points[face[0]][1];
    return (a1*b2 - a2*b1) < 0;
}

void Gobbler::computeGeometry(QVector<TriangleVertex> &vertices) {
    model.Rotate(m_matrix, m_x3d, m_y3d, m_z3d);

    QColor baseColor = m_isWhite
        ? QColor(QColorConstants::Svg::linen).darker(120)
        : QColor(QColorConstants::Svg::peru).darker(140);

    const auto& cache = model.getCache();
    const auto& faces = model.getFaces();
    const auto& points = model.getPoints();

    // Project 3D cache to 2D
    QVector<QVector<double>> points2d;
    double translation[4][4];
    m_matrix->getInverseTranslationMatrix(translation);

    for (int i = 0; i < cache.size(); i++) {
        points2d.append(m_matrix->ProjectPoint(m_matrix->MultiplyPointAndMatrix(cache[i], translation)));
    }

    // Draw shadow when selected
    if (model.isSelected() && cache.size() > 0 && cache[0][2] > 0) {
        double rotationMatrix[4][4];
        m_matrix->getRotationMatrix(rotationMatrix);
        double transposedMatrix[4][4];
        m_matrix->getTransposedMatrix(rotationMatrix, transposedMatrix);

        QVector<QVector<double>> shadowPoints;
        int shadeCount = qMin(8, static_cast<int>(cache.size()));
        for (int i = 0; i < shadeCount; i++) {
            QVector<double> point({cache[i][0], cache[i][1], cache[i][2], 1});
            QVector<double> basePoint = m_matrix->MultiplyPointAndMatrix(point, transposedMatrix);
            QVector<double> newPoint({basePoint[0], 0, basePoint[2], 1});
            QVector<double> drawPoint = m_matrix->MultiplyPointAndMatrix(newPoint, rotationMatrix);
            QVector<double> new2dPoint = m_matrix->ProjectPoint(m_matrix->MultiplyPointAndMatrix(drawPoint, translation));
            shadowPoints.append(new2dPoint);
        }

        // Triangulate shadow polygon (fan from first point)
        QColor shadowColor(0, 0, 0, 128);
        for (int i = 1; i < shadowPoints.size() - 1; i++) {
            vertices.append({
                static_cast<float>(shadowPoints[0][0]),
                static_cast<float>(shadowPoints[0][1]),
                static_cast<float>(shadowPoints[i][0]),
                static_cast<float>(shadowPoints[i][1]),
                static_cast<float>(shadowPoints[i+1][0]),
                static_cast<float>(shadowPoints[i+1][1]),
                shadowColor, true, false});
        }
    }

    // Render visible faces
    for (int i = 0; i < faces.size(); i++) {
        if (dotProduct(points2d, faces[i])) {
            float x1 = points2d[faces[i][0]][0];
            float y1 = points2d[faces[i][0]][1];
            float x2 = points2d[faces[i][1]][0];
            float y2 = points2d[faces[i][1]][1];
            float x3 = points2d[faces[i][2]][0];
            float y3 = points2d[faces[i][2]][1];

            vertices.append({x1, y1, x2, y2, x3, y3, baseColor, false, false});
        }
    }

    // Draw top polygon (faces == 16 means it's a gobbler)
    if (faces.size() == 16 && points2d.size() > 15) {
        // Triangulate octagonal top as a fan
        for (int i = 9; i < 16; i++) {
            int next = (i < 15) ? i + 1 : 8;
            vertices.append({
                static_cast<float>(points2d[8][0]),
                static_cast<float>(points2d[8][1]),
                static_cast<float>(points2d[i][0]),
                static_cast<float>(points2d[i][1]),
                static_cast<float>(points2d[next][0]),
                static_cast<float>(points2d[next][1]),
                baseColor,
                true, model.isSelected()});
        }
    }
}

QSGNode *Gobbler::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) {
    if (this->depth() != 0) {
        delete oldNode;
        return nullptr;
    }
    return ShapeItem::updatePaintNode(oldNode, data);
}
