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

    bool board = model.faces.size() == 26;
    int shadePoints = board ? 0 : 8;
    QPolygon shadow;

    // PROJECT 2D FIGURE
    for (int i = 0; i < model.cache.size(); i++) {
        points2d.append(matrix->ProjectPoint(matrix->MultiplyPointAndMatrix(model.cache[i], translation)));
        if (shadePoints > 0 && model.isSelected() && model.cache[0][2] > 0) {
            QVector<double> point ({model.cache[i][0], model.cache[i][1], model.cache[i][2], 1});

            double rotationMatrix [4][4];
            matrix->getRotationMatrix(rotationMatrix);

            double transposedMatrix [4][4];
            matrix->getTransposedMatrix(rotationMatrix, transposedMatrix);

            QVector<double> basePoint = matrix->MultiplyPointAndMatrix(point, transposedMatrix);
            QVector<double> newPoint ({basePoint[0], 0, basePoint[2], 1});

            QVector<double> drawPoint = matrix->MultiplyPointAndMatrix(newPoint, rotationMatrix);

            //QVector<double> newPoint ({model.cache[i][0], model.cache[i][1], 0, 1});
            QVector<double> new2dPoint = matrix->ProjectPoint(matrix->MultiplyPointAndMatrix(drawPoint, translation));
            QPoint point1(new2dPoint[0], new2dPoint[1]);
            shadow << point1;

            shadePoints--;
        }
    }

    // PAINTER
    QColor original = painter->brush().color();
    painter->setBrush(QColor(0, 0, 0, 128));
    painter->drawPolygon(shadow);
    painter->setBrush(original);

    for (int i = 0; i < model.faces.size(); i++) {
        if((board && i > 7) || dotProduct(points2d, model.faces[i])) {
            if (board) {
                QColor color;
                if (i < 8) {
                    color = Qt::black;
                } else if (i < 10) {
                    color = QColorConstants::Svg::sienna.darker(190);
                } else {
                    color = QColorConstants::Svg::oldlace.darker(120);
                }
                painter->setBrush(color);
            }

            paintPolygon(painter,
                    points2d[model.faces[i][0]][0],
                    points2d[model.faces[i][0]][1],

                    points2d[model.faces[i][1]][0],
                    points2d[model.faces[i][1]][1],

                    points2d[model.faces[i][2]][0],
                    points2d[model.faces[i][2]][1]);
        }
    }

    if (model.faces.size() == 16) {
        QPoint point1(points2d[8][0], points2d[8][1]);
        QPoint point2(points2d[9][0], points2d[9][1]);
        QPoint point3(points2d[10][0], points2d[10][1]);
        QPoint point4(points2d[11][0], points2d[11][1]);
        QPoint point5(points2d[12][0], points2d[12][1]);
        QPoint point6(points2d[13][0], points2d[13][1]);
        QPoint point7(points2d[14][0], points2d[14][1]);
        QPoint point8(points2d[15][0], points2d[15][1]);

        QPolygon top;
        top << point1 << point2 << point3 << point4 << point5 << point6 << point7 << point8;
        painter->drawPolygon(top);

        if (model.isSelected()) {
            QPen pen(Qt::blue);
            pen.setWidth(2);
            painter->setPen(pen);
        } else {
            QPen pen(painter->brush().color().darker(150));
            pen.setWidth(2);
            painter->setPen(pen);
        }

        painter->drawLine(point1, point2); // Draw border 1
        painter->drawLine(point2, point3);
        painter->drawLine(point3, point4);
        painter->drawLine(point4, point5);
        painter->drawLine(point5, point6);
        painter->drawLine(point6, point7);
        painter->drawLine(point7, point8);
        painter->drawLine(point8, point1);
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

    QColor brushColor = painter->brush().color();
    // Draw the two borders
    painter->setPen(brushColor.darker(150)); // Set the border color

    painter->drawLine(point1, point2); // Draw border 1
    painter->drawLine(point2, point3); // Draw border 2

    if (brushColor != Qt::black) {
        painter->setPen(brushColor);
        painter->drawLine(point3, point1);
    }
}
