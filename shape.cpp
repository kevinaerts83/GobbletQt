#include "shape.h"
#include "shape3d.h"
#include <QPainter>

Shape::Shape()
{
}

void Shape::paint(Matrix *matrix, Shape3d model, QPainter *painter)
{
    painter->setRenderHints(QPainter::Antialiasing, true);

    QVector<QVector<double>> points2d;

    // TRANSLATE THE 2D PICTURE
    double translation [4][4];
    matrix->getInverseTranslationMatrix(translation);

    // PROJECT 2D FIGURE
    for (int i = 0; i < model.cache.size(); i++) {
        points2d.append(matrix->ProjectPoint(matrix->MultiplyPointAndMatrix(model.cache[i], translation)));
    }

    // PAINTER
    for (int i = 0; i < model.faces.size(); i++) {
        if(dotProduct(points2d, model.faces[i])) {
            paintPolygon(painter,
                    points2d[model.faces[i][0]][0],
                    points2d[model.faces[i][0]][1],

                    points2d[model.faces[i][1]][0],
                    points2d[model.faces[i][1]][1],

                    points2d[model.faces[i][2]][0],
                    points2d[model.faces[i][2]][1]);
        }
    }
}
//![1]

bool Shape::dotProduct(QVector<QVector<double>> points, QVector<int> face) {
    double a1 = points[face[1]][0] - points[face[0]][0];
    double a2 = points[face[1]][1] - points[face[0]][1];
    double b1 = points[face[2]][0] - points[face[0]][0];
    double b2 = points[face[2]][1] - points[face[0]][1];

    face[3] = (a1*b2 - a2*b1) < 0 ? 1 : 0;
    return face[3] == 1;
};

void Shape::paintPolygon(QPainter *painter, qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3) {
    painter->setPen(Qt::NoPen);    // Set no border pen
    // Define the triangle points
    QPoint point1(x1, y1);
    QPoint point2(x2, y2);
    QPoint point3(x3, y3);

    QPolygon triangle;
    triangle << point1 << point2 << point3;

    painter->drawPolygon(triangle);  // Draw the filled triangle

    // Draw the two borders
    painter->setPen(Qt::black);      // Set the border color

    painter->drawLine(point1, point2);  // Draw border 1
    painter->drawLine(point2, point3);  // Draw border 2

    QColor brushColor = painter->brush().color();
    if (brushColor != Qt::black) {
        painter->setPen(brushColor);
        painter->drawLine(point3, point1);
    }
}
