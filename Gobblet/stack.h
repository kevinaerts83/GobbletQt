#ifndef STACK_H
#define STACK_H

#include "shape_item.h"
#include "stack3d.h"

class Stack : public ShapeItem
{
    Q_OBJECT

    Q_PROPERTY(Matrix* matrix MEMBER m_matrix)
    Q_PROPERTY(bool isWhite READ isWhite WRITE setWhite)

    QML_ELEMENT

public:
    Stack(QQuickItem *parent = nullptr);
    Stack3d model;
    Matrix* m_matrix = nullptr;

    bool isWhite() const;
    void setWhite(const bool &isWhite);

protected:
    void computeGeometry(QVector<TriangleVertex> &vertices) override;

private:
    bool m_isWhite = true;

    constexpr static const QColor COLOR = QColor(55, 55, 55, 255);
};

#endif // STACK_H
