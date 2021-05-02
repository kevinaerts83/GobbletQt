#include "shape.h"
#include <QPainter>

//![0]
Shape::Shape(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
}
//![0]

QString Shape::name() const
{
    return m_name;
}

void Shape::setName(const QString &name)
{
    m_name = name;
}

QColor Shape::color() const
{
    return m_color;
}

void Shape::setColor(const QColor &color)
{
    m_color = color;
}

//![1]
void Shape::paint(QPainter *painter)
{
    QPen pen(m_color, 2);
    painter->setPen(pen);
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->drawPie(boundingRect().adjusted(1, 1, -1, -1), 90 * 16, 290 * 16);

    /*
     * new gobbler3d and draw gobbler (move call to gobbler class)
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
    }*/
}
//![1]

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

Shape.prototype.dotProduct = function(face, p) {
    var a1 = p[face[1]][0] - p[face[0]][0],
        a2 = p[face[1]][1] - p[face[0]][1],
        b1 = p[face[2]][0] - p[face[0]][0],
        b2 = p[face[2]][1] - p[face[0]][1];

    return (a1*b2 - a2*b1) < 0;
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
