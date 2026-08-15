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
        QColor faceColor;
        bool top, selected;
    };

    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

    /// Subclasses fill the vertex list with projected 2D triangles
    /// from their 3D model.
    virtual void computeGeometry(QVector<TriangleVertex> &vertices) = 0;
};

#endif // SHAPE_ITEM_H
