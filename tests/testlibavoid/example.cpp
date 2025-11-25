// #include <QApplication>
// #include <QGraphicsView>
// #include <QGraphicsScene>
// #include <QGraphicsRectItem>
// #include <QGraphicsEllipseItem>
// #include <QGraphicsPathItem>
// #include <QMainWindow>
// #include <QDebug>
// #include <QPainterPath>

// #include <iostream>

// #include "SchematicHelper.hpp"

// schematichelper::SchematicHelper* schematicHelper = new schematichelper::SchematicHelper{};

// // Custom square class that prints position when moved
// class DraggableSquare : public QGraphicsRectItem {
// public:
//     DraggableSquare(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr)
//         : QGraphicsRectItem(x, y, width, height, parent) {
//         setFlag(QGraphicsItem::ItemIsMovable, true);
//         setFlag(QGraphicsItem::ItemSendsGeometryChanges, true); // Enable itemChange notifications

//         // Set appearance
//         setBrush(QBrush(Qt::lightGray));
//         setPen(QPen(Qt::black, 1));
//     }

// protected:
//     // Override itemChange to detect position changes
//     QVariant itemChange(GraphicsItemChange change, const QVariant &value) override {
//         if (change == ItemPositionChange) {
//             QPointF newPos = value.toPointF();

//             // Get the polygon representing the shape
//             QPolygonF polygon = mapToScene(rect());
//             polygon.translate(newPos - pos()); // Adjust for new position

//             assert(polygon.front() == polygon.back());
//             polygon.pop_back();

//             qDebug() << "Polygon vertices:";
//             for (int i = 0; i < polygon.size(); ++i) {
//                 qDebug() << "  Vertex" << i << ": (" << polygon[i].x() << "," << polygon[i].y() << ")";
//             }
//             qDebug() << "---";

//             schematichelper::Shape shape{};
//             shape.id = this->data(114514).toInt();
//             for (auto p : polygon) {
//                 shape.polygon.push_back({p.x(), p.y()});
//             }
//             schematicHelper->moveShapes({shape});
//             schematicHelper->update();
//             auto path = schematicHelper->getPaths()[0];
//             // clear last path and redraw
//             for (auto p : path) {
//                 std::cout << "(" << p.x << "," << p.y << ") ";
//             }
//             std::cout << std::endl;
//         }
//         return QGraphicsRectItem::itemChange(change, value);
//     }
// };

// class MainWindow : public QMainWindow {
// public:
//     explicit MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
//         QGraphicsScene *scene = new QGraphicsScene(this);
//         QGraphicsView *view = new QGraphicsView(scene, this);
//         setCentralWidget(view);

//         // Create a 5x5 grid of squares
//         for (auto x = 0; x <= 200; x += 50) {
//             for (auto y = 0; y <= 200; y += 50) {
//                 // Fixed: now passing 4 parameters (x, y, width, height)
//                 auto square = new DraggableSquare(x, y, 30, 30);
//                 scene->addItem(square);

//                 schematichelper::Shape shape{};
//                 shape.id = (x / 50) + (y / 50) * 5;
//                 shape.polygon = {
//                     {1.0 * x, 1.0 * y},
//                     {1.0 * x + 30, 1.0 * y},
//                     {1.0 * x + 30, 1.0 * y + 30},
//                     {1.0 * x, 1.0 * y + 30},
//                 };
//                 schematicHelper->addShapes({shape});

//                 square->setData(114514, shape.id); // Store shape ID for reference
//             }
//         }

//         schematichelper::Line line{};
//         line.id = 0;
//         line.src = {0, 0};
//         line.dst = {250, 250};
//         schematicHelper->addLines({line});

//         schematicHelper->update();
//         auto path = schematicHelper->getPaths()[0];
//         for (auto p : path) {
//             std::cout << "(" << p.x << "," << p.y << ") ";
//         }
//         std::cout << std::endl;
//         // draw path

//         resize(500, 400);

//         // Print initial instruction
//         qDebug() << "Created 5x5 grid of draggable squares with a zigzag line. Drag them to see position changes.";
//     }
// };

// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);
//     MainWindow window;
//     window.show();
//     return app.exec();
// }
