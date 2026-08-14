#ifndef SHAPE3D_H
#define SHAPE3D_H
#include "matrix.h"
#include <QVector>

class Shape3d
{
public:
    QVector<QVector<double>> points;
    QVector<QVector<int>> faces;
    QVector<QVector<double>> cache;

    Shape3d() {};

    void Rotate(Matrix* matrix, double x, double y, double z);

    bool isSelected() const;
    void toggleSelection();

    bool isOnBoard() const;
    void setOnBoard();
private:
    void Zoom(Matrix* matrix);
    bool m_selected = false;
    bool m_onBoard = false; // for animation, Don't switch rotation during movement from stack to board
};

/// Per-instance 3D state that references a shared Shape3d model.
/// Holds mutable state (selection, onBoard, transform cache) while
/// the immutable geometry (points, faces) lives in the shared model.
class Shape3dInstance
{
public:
    Shape3dInstance() : m_model(nullptr) {}
    explicit Shape3dInstance(std::shared_ptr<Shape3d> model) : m_model(model) {}

    void setModel(std::shared_ptr<Shape3d> model) { m_model = model; }

    void Rotate(Matrix* matrix, double x, double y, double z);

    bool isSelected() const { return m_selected; }
    void toggleSelection() { m_selected = !m_selected; }

    bool isOnBoard() const { return m_onBoard; }
    void setOnBoard() { m_onBoard = true; }

    const QVector<QVector<double>>& getPoints() const { return m_model->points; }
    const QVector<QVector<int>>& getFaces() const { return m_model->faces; }
    const QVector<QVector<double>>& getCache() const { return m_cache; }

private:
    void Zoom(Matrix* matrix);
    std::shared_ptr<Shape3d> m_model;
    QVector<QVector<double>> m_cache;
    bool m_selected = false;
    bool m_onBoard = false;
};

#endif // SHAPE3D_H
