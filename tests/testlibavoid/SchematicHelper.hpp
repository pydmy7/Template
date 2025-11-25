#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

namespace Avoid {
class Router;
class ConnRef;
class ShapeRef;
}

namespace schematichelper {

struct Point {
    double x;
    double y;
};

struct Line {
    int id;
    Point src;
    Point dst;
};

struct Shape {
    int id;
    std::vector<Point> polygon;
};

class SchematicHelper final {
public:
    explicit SchematicHelper();

    void addLines(const std::vector<Line>& lines);
    void removeLines(const std::vector<int>& lineIds);
    void moveLines(const std::vector<Line>& lines);

    void addShapes(const std::vector<Shape>& shapes);
    void removeShapes(const std::vector<int>& shapeIds);
    void moveShapes(const std::vector<Shape>& shapes);

    std::unordered_map<int, std::vector<Point>> updateAndGetPaths();

private:
    std::shared_ptr<Avoid::Router> router_;
    std::unordered_map<int, Avoid::ConnRef*> lineMap_;
    std::unordered_map<int, Avoid::ShapeRef*> shapeMap_;
    std::unordered_map<int, std::vector<Point>> pathCache_;
};

}  // namespace schematichelper
