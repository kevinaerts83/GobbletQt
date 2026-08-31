#ifndef SHAPE_ITEM_H
#define SHAPE_ITEM_H

#include <QtQuick/QQuickItem>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QVector>
#include <QColor>

/// Base class for QQuickItem-based shapes that render 3D models
/// via the Qt Scene Graph. Subclasses implement computeGeometry()
/// to produce a list of triangles; updatePaintNode() handles the
/// actual scene graph node construction.
class ShapeItem : public QQuickItem
{
    Q_OBJECT

public:
    explicit ShapeItem(QQuickItem *parent = nullptr);

protected:
    struct TriangleVertex {
        float x, y, x1, y1, x2, y2;
        QColor color0, color1, color2;
        bool top;
    };

    /// Helper: create a TriangleVertex with a single flat color for all 3 vertices
    static TriangleVertex flatTriangle(
        float x, float y,
        float x1, float y1,
        float x2, float y2,
        QColor color,
        bool top);

    /// Helper: vary a base color by a small random-ish amount per vertex for a marble look
    static TriangleVertex marbleTriangle(
        float x, float y,
        float x1, float y1,
        float x2, float y2,
        QColor base,
        bool top,
        int seed);

    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

    /// Subclasses fill the vertex list with projected 2D triangles
    /// from their 3D model.
    virtual void computeGeometry(QVector<TriangleVertex> &vertices) = 0;
};

#endif // SHAPE_ITEM_H
