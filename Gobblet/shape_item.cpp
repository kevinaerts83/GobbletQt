#include "shape_item.h"
#include <QSGGeometry>

ShapeItem::ShapeItem(QQuickItem *parent) : QQuickItem(parent) {
    setFlag(ItemHasContents, true);
}

QSGNode *ShapeItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) {
    QVector<TriangleVertex> vertices;
    computeGeometry(vertices);

    if (vertices.isEmpty()) {
        delete oldNode;
        return nullptr;
    }

    // Build a parent node with one child per triangle (each with its own color)
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
        QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 3);
        geometry->setDrawingMode(QSGGeometry::DrawTriangles);

        QSGGeometry::Point2D *v = geometry->vertexDataAsPoint2D();
        v[0].set(vertices[i].x, vertices[i].y);
        v[1].set(vertices[i].x1, vertices[i].y1);
        v[2].set(vertices[i].x2, vertices[i].y2);

        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        QColor color = vertices[i].faceColor;
        material->setColor(color);
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
