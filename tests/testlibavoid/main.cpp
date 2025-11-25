#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QGraphicsLineItem>
#include <QMainWindow>
#include <QDebug>
#include <QPainterPath>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>
#include <QColorDialog>
#include <QColor>
#include <QTimer>

#include "SchematicHelper.hpp"

// Color helper for different lines
class ColorHelper {
public:
    static QColor getLineColor(int lineId) {
        static std::vector<QColor> colors = {
            Qt::red, Qt::blue, Qt::green, Qt::magenta,
            Qt::cyan, Qt::yellow, Qt::darkRed, Qt::darkBlue,
            Qt::darkGreen, Qt::darkMagenta, Qt::darkCyan
        };
        return colors[lineId % colors.size()];
    }
};

schematichelper::SchematicHelper* schematicHelper = new schematichelper::SchematicHelper{};

// Global variables for path management
QGraphicsScene* g_scene = nullptr;
std::unordered_map<int, QGraphicsPathItem*> g_pathItems; // Store multiple paths by line ID

// Global functions for path management
void drawPaths() {
    if (!g_scene) return;

    auto pathsMap = schematicHelper->updateAndGetPaths();

    for (const auto& [lineId, path] : pathsMap) {
        if (path.empty()) {
            qDebug() << "Path for line" << lineId << "is empty";
            continue;
        }

        // Convert schematic path to QPainterPath
        QPainterPath painterPath;
        painterPath.moveTo(path[0].x, path[0].y);
        for (size_t i = 1; i < path.size(); ++i) {
            painterPath.lineTo(path[i].x, path[i].y);
        }

        // Create graphics path item
        QGraphicsPathItem* pathItem = new QGraphicsPathItem(painterPath);
        pathItem->setPen(QPen(ColorHelper::getLineColor(lineId), 3));
        pathItem->setZValue(-1); // Draw behind squares
        g_scene->addItem(pathItem);

        // Store the path item
        g_pathItems[lineId] = pathItem;

        qDebug() << "Drew path for line" << lineId << "with" << path.size() << "points";
    }
}

void clearAllPaths() {
    for (auto& [lineId, pathItem] : g_pathItems) {
        if (pathItem && g_scene) {
            g_scene->removeItem(pathItem);
            delete pathItem;
        }
    }
    g_pathItems.clear();
    qDebug() << "Cleared all paths";
}

void clearAndRedrawPaths() {
    clearAllPaths();
    drawPaths();
}

// Custom square class that prints position when moved
class DraggableSquare : public QGraphicsRectItem {
public:
    DraggableSquare(qreal x, qreal y, qreal width, qreal height, int shapeId, QGraphicsItem *parent = nullptr)
        : QGraphicsRectItem(x, y, width, height, parent), m_shapeId(shapeId) {
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

        // Set appearance
        setBrush(QBrush(Qt::lightGray));
        setPen(QPen(Qt::black, 2));

        // Add shape ID text
        m_textItem = new QGraphicsSimpleTextItem(QString::number(shapeId), this);
        m_textItem->setPos(width/4, height/4);
    }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override {
        if (change == ItemPositionChange) {
            QPointF newPos = value.toPointF();

            // Get the polygon representing the shape
            QPolygonF polygon = mapToScene(rect());
            polygon.translate(newPos - pos()); // Adjust for new position

            // Remove the duplicate last point if it exists
            if (polygon.size() > 1 && polygon.front() == polygon.back()) {
                polygon.pop_back();
            }

            qDebug() << "Shape" << m_shapeId << "moved to:";
            for (int i = 0; i < polygon.size(); ++i) {
                qDebug() << "  Vertex" << i << ": (" << polygon[i].x() << "," << polygon[i].y() << ")";
            }

            schematichelper::Shape shape{};
            shape.id = m_shapeId;
            for (const auto& p : polygon) {
                shape.polygon.push_back({p.x(), p.y()});
            }
            schematicHelper->moveShapes({shape});

            // Update paths after a small delay to avoid too frequent updates
            QTimer::singleShot(50, []() {
                clearAndRedrawPaths();
            });
        }
        return QGraphicsRectItem::itemChange(change, value);
    }

private:
    int m_shapeId;
    QGraphicsSimpleTextItem* m_textItem;
};

class ControlPanel : public QWidget {
    Q_OBJECT

public:
    ControlPanel(QWidget* parent = nullptr) : QWidget(parent) {
        setupUI();
        connectSignals();
    }

signals:
    void addLineRequested(int lineId, double srcX, double srcY, double dstX, double dstY);
    void removeLineRequested(int lineId);
    void addShapeRequested(double x, double y, double width, double height);
    void removeShapeRequested(int shapeId);

private slots:
    void onAddLine() {
        emit addLineRequested(m_lineIdSpin->value(),
                             m_srcXSpin->value(), m_srcYSpin->value(),
                             m_dstXSpin->value(), m_dstYSpin->value());
    }

    void onRemoveLine() {
        emit removeLineRequested(m_removeLineIdSpin->value());
    }

    void onAddShape() {
        emit addShapeRequested(m_shapeXSpin->value(), m_shapeYSpin->value(),
                              m_shapeWidthSpin->value(), m_shapeHeightSpin->value());
    }

    void onRemoveShape() {
        emit removeShapeRequested(m_removeShapeIdSpin->value());
    }

private:
    void setupUI() {
        setFixedWidth(250);
        QVBoxLayout* mainLayout = new QVBoxLayout(this);

        // Line controls
        QGroupBox* lineGroup = new QGroupBox("Line Controls");
        QVBoxLayout* lineLayout = new QVBoxLayout(lineGroup);

        // Add line
        QLabel* addLineLabel = new QLabel("Add Line:");
        m_lineIdSpin = new QSpinBox();
        m_lineIdSpin->setRange(0, 999);
        m_lineIdSpin->setValue(1);

        m_srcXSpin = new QSpinBox();
        m_srcXSpin->setRange(0, 500);
        m_srcYSpin = new QSpinBox();
        m_srcYSpin->setRange(0, 500);
        m_dstXSpin = new QSpinBox();
        m_dstXSpin->setRange(0, 500);
        m_dstXSpin->setValue(200);
        m_dstYSpin = new QSpinBox();
        m_dstYSpin->setRange(0, 500);
        m_dstYSpin->setValue(200);

        m_addLineBtn = new QPushButton("Add Line");

        lineLayout->addWidget(addLineLabel);
        lineLayout->addWidget(new QLabel("Line ID:"));
        lineLayout->addWidget(m_lineIdSpin);
        lineLayout->addWidget(new QLabel("Source (X, Y):"));
        lineLayout->addWidget(m_srcXSpin);
        lineLayout->addWidget(m_srcYSpin);
        lineLayout->addWidget(new QLabel("Destination (X, Y):"));
        lineLayout->addWidget(m_dstXSpin);
        lineLayout->addWidget(m_dstYSpin);
        lineLayout->addWidget(m_addLineBtn);

        // Remove line
        QLabel* removeLineLabel = new QLabel("Remove Line:");
        m_removeLineIdSpin = new QSpinBox();
        m_removeLineIdSpin->setRange(0, 999);
        m_removeLineBtn = new QPushButton("Remove Line");

        lineLayout->addWidget(removeLineLabel);
        lineLayout->addWidget(m_removeLineIdSpin);
        lineLayout->addWidget(m_removeLineBtn);

        // Shape controls
        QGroupBox* shapeGroup = new QGroupBox("Shape Controls");
        QVBoxLayout* shapeLayout = new QVBoxLayout(shapeGroup);

        // Add shape
        QLabel* addShapeLabel = new QLabel("Add Shape:");
        m_shapeXSpin = new QSpinBox();
        m_shapeXSpin->setRange(0, 500);
        m_shapeXSpin->setValue(100);
        m_shapeYSpin = new QSpinBox();
        m_shapeYSpin->setRange(0, 500);
        m_shapeYSpin->setValue(100);
        m_shapeWidthSpin = new QSpinBox();
        m_shapeWidthSpin->setRange(10, 100);
        m_shapeWidthSpin->setValue(30);
        m_shapeHeightSpin = new QSpinBox();
        m_shapeHeightSpin->setRange(10, 100);
        m_shapeHeightSpin->setValue(30);
        m_addShapeBtn = new QPushButton("Add Shape");

        shapeLayout->addWidget(addShapeLabel);
        shapeLayout->addWidget(new QLabel("Position (X, Y):"));
        shapeLayout->addWidget(m_shapeXSpin);
        shapeLayout->addWidget(m_shapeYSpin);
        shapeLayout->addWidget(new QLabel("Size (W, H):"));
        shapeLayout->addWidget(m_shapeWidthSpin);
        shapeLayout->addWidget(m_shapeHeightSpin);
        shapeLayout->addWidget(m_addShapeBtn);

        // Remove shape
        QLabel* removeShapeLabel = new QLabel("Remove Shape:");
        m_removeShapeIdSpin = new QSpinBox();
        m_removeShapeIdSpin->setRange(0, 999);
        m_removeShapeBtn = new QPushButton("Remove Shape");

        shapeLayout->addWidget(removeShapeLabel);
        shapeLayout->addWidget(m_removeShapeIdSpin);
        shapeLayout->addWidget(m_removeShapeBtn);

        mainLayout->addWidget(lineGroup);
        mainLayout->addWidget(shapeGroup);
        mainLayout->addStretch();
    }

    void connectSignals() {
        connect(m_addLineBtn, &QPushButton::clicked, this, &ControlPanel::onAddLine);
        connect(m_removeLineBtn, &QPushButton::clicked, this, &ControlPanel::onRemoveLine);
        connect(m_addShapeBtn, &QPushButton::clicked, this, &ControlPanel::onAddShape);
        connect(m_removeShapeBtn, &QPushButton::clicked, this, &ControlPanel::onRemoveShape);
    }

    QSpinBox* m_lineIdSpin;
    QSpinBox* m_srcXSpin;
    QSpinBox* m_srcYSpin;
    QSpinBox* m_dstXSpin;
    QSpinBox* m_dstYSpin;
    QPushButton* m_addLineBtn;

    QSpinBox* m_removeLineIdSpin;
    QPushButton* m_removeLineBtn;

    QSpinBox* m_shapeXSpin;
    QSpinBox* m_shapeYSpin;
    QSpinBox* m_shapeWidthSpin;
    QSpinBox* m_shapeHeightSpin;
    QPushButton* m_addShapeBtn;

    QSpinBox* m_removeShapeIdSpin;
    QPushButton* m_removeShapeBtn;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr) : QMainWindow(parent), m_nextShapeId(0) {
        setupUI();
        setupInitialScene();
        connectSignals();
    }

    ~MainWindow() {
        g_scene = nullptr;
    }

private slots:
    void onAddLine(int lineId, double srcX, double srcY, double dstX, double dstY) {
        schematichelper::Line line{};
        line.id = lineId;
        line.src = {srcX, srcY};
        line.dst = {dstX, dstY};
        schematicHelper->addLines({line});

        // Add visual indicators for line endpoints
        addLineEndpoints(lineId, srcX, srcY, dstX, dstY);

        clearAndRedrawPaths();
        qDebug() << "Added line" << lineId << "from (" << srcX << "," << srcY
                 << ") to (" << dstX << "," << dstY << ")";
    }

    void onRemoveLine(int lineId) {
        schematicHelper->removeLines({lineId});
        removeLineEndpoints(lineId);
        clearAndRedrawPaths();
        qDebug() << "Removed line" << lineId;
    }

    void onAddShape(double x, double y, double width, double height) {
        int shapeId = m_nextShapeId++;

        auto square = new DraggableSquare(x, y, width, height, shapeId);
        m_scene->addItem(square);

        schematichelper::Shape shape{};
        shape.id = shapeId;
        shape.polygon = {
            {x, y},
            {x + width, y},
            {x + width, y + height},
            {x, y + height},
        };
        schematicHelper->addShapes({shape});

        m_shapes[shapeId] = square;

        clearAndRedrawPaths();
        qDebug() << "Added shape" << shapeId << "at (" << x << "," << y
                 << ") with size (" << width << "," << height << ")";
    }

    void onRemoveShape(int shapeId) {
        auto it = m_shapes.find(shapeId);
        if (it != m_shapes.end()) {
            m_scene->removeItem(it->second);
            delete it->second;
            m_shapes.erase(it);
        }

        schematicHelper->removeShapes({shapeId});
        clearAndRedrawPaths();
        qDebug() << "Removed shape" << shapeId;
    }

private:
    void setupUI() {
        QWidget* centralWidget = new QWidget();
        setCentralWidget(centralWidget);

        QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

        // Graphics view
        m_scene = new QGraphicsScene(this);
        m_scene->setSceneRect(0, 0, 500, 400);
        g_scene = m_scene; // Set global scene reference

        QGraphicsView* view = new QGraphicsView(m_scene);
        view->setMinimumSize(600, 500);

        // Control panel
        m_controlPanel = new ControlPanel();

        mainLayout->addWidget(view);
        mainLayout->addWidget(m_controlPanel);

        setWindowTitle("SchematicHelper Visual Test");
        resize(900, 600);
    }

    void setupInitialScene() {
        // Add some initial shapes
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 2; ++j) {
                onAddShape(50 + i * 80, 50 + j * 80, 30, 30);
            }
        }

        // Add some initial lines
        onAddLine(0, 10, 10, 450, 350);
        onAddLine(1, 10, 350, 450, 10);
    }

    void connectSignals() {
        connect(m_controlPanel, &ControlPanel::addLineRequested,
                this, &MainWindow::onAddLine);
        connect(m_controlPanel, &ControlPanel::removeLineRequested,
                this, &MainWindow::onRemoveLine);
        connect(m_controlPanel, &ControlPanel::addShapeRequested,
                this, &MainWindow::onAddShape);
        connect(m_controlPanel, &ControlPanel::removeShapeRequested,
                this, &MainWindow::onRemoveShape);
    }

    void addLineEndpoints(int lineId, double srcX, double srcY, double dstX, double dstY) {
        // Add visual indicators for line start and end points
        QGraphicsEllipseItem* srcPoint = new QGraphicsEllipseItem(srcX - 3, srcY - 3, 6, 6);
        srcPoint->setBrush(QBrush(ColorHelper::getLineColor(lineId)));
        srcPoint->setPen(QPen(Qt::black, 1));
        m_scene->addItem(srcPoint);

        QGraphicsEllipseItem* dstPoint = new QGraphicsEllipseItem(dstX - 3, dstY - 3, 6, 6);
        dstPoint->setBrush(QBrush(ColorHelper::getLineColor(lineId)));
        dstPoint->setPen(QPen(Qt::black, 1));
        m_scene->addItem(dstPoint);

        m_lineEndpoints[lineId] = {srcPoint, dstPoint};
    }

    void removeLineEndpoints(int lineId) {
        auto it = m_lineEndpoints.find(lineId);
        if (it != m_lineEndpoints.end()) {
            m_scene->removeItem(it->second.first);
            m_scene->removeItem(it->second.second);
            delete it->second.first;
            delete it->second.second;
            m_lineEndpoints.erase(it);
        }
    }

    QGraphicsScene* m_scene;
    ControlPanel* m_controlPanel;
    int m_nextShapeId;
    std::unordered_map<int, DraggableSquare*> m_shapes;
    std::unordered_map<int, std::pair<QGraphicsEllipseItem*, QGraphicsEllipseItem*>> m_lineEndpoints;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    qDebug() << "SchematicHelper Visual Test Started";
    qDebug() << "Instructions:";
    qDebug() << "1. Drag shapes to see path routing updates";
    qDebug() << "2. Use control panel to add/remove lines and shapes";
    qDebug() << "3. Different lines are shown in different colors";

    int result = app.exec();

    // Clean up
    delete schematicHelper;
    return result;
}

#include "main.moc" // Required for Q_OBJECT
