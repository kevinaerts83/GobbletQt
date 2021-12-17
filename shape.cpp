#include "shape.h"
#include "shape3d.h"
#include <QPainter>

//![0]
Shape::Shape()
{
}
//![0]

//![1]
void Shape::paint(Shape3d model, QPainter *painter)
{
    painter->setRenderHints(QPainter::Antialiasing, true);

    QVector<QVector<double>> points2d;

    double translation [4][4];
    model.matrix.getTranslationMatrix(getX(), getY(), getZ(), translation);

    for (int i = 0; i < model.cache.size(); i++) {
        points2d.append(model.matrix.ProjectPoint(model.matrix.MultiplyPointAndMatrix(model.cache[i], translation)));
    }

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

void Shape::paintPolygon(QPainter *painter, qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3)
{
    QPointF points[3] = {
        QPointF(x1, y1),
        QPointF(x2, y2),
        QPointF(x3, y3)
    };

    painter->drawPolygon(points, 3);
}

/*

Shape.prototype.translateAndProject = function() {
    this.points_2d = [];
    var i, l, translation,
        isPawn = false,
        c = 0;
    if(this.size !== undefined) {
        isPawn = true;
        c = this.size;
    }
    if(!isPawn || this.visible === true) {
        translation = this.matrix.getTranslation(this.centerPoint2D[0], this.centerPoint2D[1], this.centerPoint2D[2]);
        for (i = 0, l = this.shape3D.cache[c].length; i < l; i+=1) {
            this.points_2d[i] = this.matrix.projectPoint(this.matrix.multiplyPointAndMatrix(this.shape3D.cache[c][i], translation));
            if(isPawn) {
                this.setBoundaries(this.points_2d[i], i===0);
            }
        }
    }
};

Shape.prototype.draw = function(ctx, isSelection) {
    if(ctx && this.points_2d.length > 0 && (this.visible === undefined || this.visible === true)) {
        var i, l, face, colourLength = this.facesColour.length-1;
        ctx.lineWidth = 1;
        if(isSelection) {
            ctx.strokeStyle = this.darkerColor(this.facesColour[0], this.percent*2);
        } else {
            ctx.strokeStyle = (this.facesColour.length > 1) ? '#000' : this.darkerColor(this.facesColour[0], this.percent);
        }
        ctx.lineWidth= (this.facesColour.length > 1) ? 1 : 2;
        for (i = 0, l = this.shape3D.faces.length; i < l; i+=1) {
            face = this.shape3D.faces[i];

            face[3] = this.dotProduct(face, this.points_2d) ? 1 : 0;
            if(face[3] === 1) {
                ctx.fillStyle = this.facesColour[i > colourLength ? colourLength : i];
                ctx.beginPath();
                ctx.moveTo(this.points_2d[face[0]][0],this.points_2d[face[0]][1]);
                ctx.lineTo(this.points_2d[face[1]][0],this.points_2d[face[1]][1]);
                ctx.lineTo(this.points_2d[face[2]][0],this.points_2d[face[2]][1]);
                ctx.stroke();

                ctx.fill();
            }
        }
    }
};

Shape.prototype.setBoundaries = function(point2d, first) {
    if(first) {
        this.minX = point2d[0];
        this.minY = point2d[1];
        this.maxX = point2d[0];
        this.maxY = point2d[1];
    }
    else {
        if(point2d[0] < this.minX) {
            this.minX = point2d[0];
        }
        if(point2d[0] > this.maxX) {
            this.maxX = point2d[0];
        }
        if(point2d[1] < this.minY) {
            this.minY = point2d[1];
        }
        if(point2d[1] > this.maxY) {
            this.maxY = point2d[1];
        }
    }
};

Shape.prototype.contains = function(mx, my) {
  // All we have to do is make sure the Mouse X,Y fall in the area between
  // the shape's X and (X + Height) and its Y and (Y + Height)
  if((this.minX <= mx) && (this.maxX >= mx) && (this.minY <= my) && (this.maxY >= my)) {
      this.mouseX = mx;
      this.mouseY = my;
      return true;
  }

  this.mouseX = -1;
  this.mouseY = -1;
  return false;
};

*/
