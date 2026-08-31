#include "shape_item.h"
#include <QSGGeometry>
#include <QSGVertexColorMaterial>

ShapeItem::ShapeItem(QQuickItem *parent) : QQuickItem(parent) {
    setFlag(ItemHasContents, true);
}

ShapeItem::TriangleVertex ShapeItem::flatTriangle(
        float x, float y,
        float x1, float y1,
        float x2, float y2,
        QColor color,
        bool top) {

    return {x, y, x1, y1, x2, y2, color, color, color, top};
}

static QColor varyColor(QColor base, int amount) {
    int r = qBound(0, base.red() + amount, 255);
    int g = qBound(0, base.green() + amount, 255);
    int b = qBound(0, base.blue() + amount, 255);
    return QColor(r, g, b, base.alpha());
}

ShapeItem::TriangleVertex ShapeItem::marbleTriangle(
        float x, float y,
        float x1, float y1,
        float x2, float y2,
        QColor base,
        bool top,
        int seed) {

    // Simple deterministic variation based on seed to create a veined/marble look
    int v0 = ((seed * 17) % 31) - 15;       // range -15..+15
    int v1 = ((seed * 37 + 7) % 31) - 15;
    int v2 = ((seed * 53 + 13) % 31) - 15;
    return {x, y, x1, y1, x2, y2, varyColor(base, v0), varyColor(base, v1), varyColor(base, v2), top};
}

QSGNode *ShapeItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) {
    QVector<TriangleVertex> vertices;
    computeGeometry(vertices);

    if (vertices.isEmpty()) {
        delete oldNode;
        return nullptr;
    }

    // Build a parent node with one child per triangle
    QSGNode *rootNode = oldNode;
    if (!rootNode) {
        rootNode = new QSGNode;
    }

    // Remove and delete old children
    while (rootNode->childCount() > 0) {
        QSGNode *child = rootNode->firstChild();
        rootNode->removeChildNode(child);
        delete child;
    }

    for (int i = 0; i < vertices.size(); i++) {
        QSGGeometryNode *node = new QSGGeometryNode;
        QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_ColoredPoint2D(), 3);
        geometry->setDrawingMode(QSGGeometry::DrawTriangles);

        QSGGeometry::ColoredPoint2D *v = geometry->vertexDataAsColoredPoint2D();
        v[0].set(vertices[i].x, vertices[i].y,
                 vertices[i].color0.red(), vertices[i].color0.green(),
                 vertices[i].color0.blue(), vertices[i].color0.alpha());
        v[1].set(vertices[i].x1, vertices[i].y1,
                 vertices[i].color1.red(), vertices[i].color1.green(),
                 vertices[i].color1.blue(), vertices[i].color1.alpha());
        v[2].set(vertices[i].x2, vertices[i].y2,
                 vertices[i].color2.red(), vertices[i].color2.green(),
                 vertices[i].color2.blue(), vertices[i].color2.alpha());

        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGVertexColorMaterial *material = new QSGVertexColorMaterial;
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);

        rootNode->appendChildNode(node);

        QColor lineColorValue = Qt::black;

        if (!vertices[i].top) {
            QSGGeometry *line = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2);
            line->setDrawingMode(QSGGeometry::DrawLines);
            line->vertexDataAsPoint2D()[0].set(vertices[i].x, vertices[i].y);
            line->vertexDataAsPoint2D()[1].set(vertices[i].x1, vertices[i].y1);

            QSGFlatColorMaterial *lineMaterial1 = new QSGFlatColorMaterial;
            lineMaterial1->setColor(lineColorValue);

            QSGGeometryNode *lineNode = new QSGGeometryNode;
            lineNode->setGeometry(line);
            lineNode->setFlag(QSGNode::OwnsGeometry);
            lineNode->setMaterial(lineMaterial1);
            lineNode->setFlag(QSGNode::OwnsMaterial);

            rootNode->appendChildNode(lineNode);
        }
        QSGGeometry *line2 = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2);
        line2->setDrawingMode(QSGGeometry::DrawLines);
        line2->vertexDataAsPoint2D()[0].set(vertices[i].x1, vertices[i].y1);
        line2->vertexDataAsPoint2D()[1].set(vertices[i].x2, vertices[i].y2);

        QSGFlatColorMaterial *lineMaterial2 = new QSGFlatColorMaterial;
        lineMaterial2->setColor(lineColorValue);

        QSGGeometryNode *lineNode2 = new QSGGeometryNode;
        lineNode2->setGeometry(line2);
        lineNode2->setFlag(QSGNode::OwnsGeometry);
        lineNode2->setMaterial(lineMaterial2);
        lineNode2->setFlag(QSGNode::OwnsMaterial);

        rootNode->appendChildNode(lineNode2);
    }

    return rootNode;
}
