#include "SchematicHelper.hpp"

#include "libavoid/router.h"
#include "libavoid/shape.h"

namespace schematichelper {

SchematicHelper::SchematicHelper() {
    router_ = std::make_shared<Avoid::Router>(Avoid::OrthogonalRouting);
    router_->setRoutingParameter(Avoid::segmentPenalty, 50);
    router_->setRoutingParameter(Avoid::shapeBufferDistance, 15);
    router_->setRoutingParameter(Avoid::idealNudgingDistance, 25);
    router_->setRoutingOption(Avoid::nudgeOrthogonalSegmentsConnectedToShapes, true);
    router_->setRoutingOption(Avoid::nudgeOrthogonalTouchingColinearSegments, true);
    router_->setRoutingOption(Avoid::nudgeSharedPathsWithCommonEndPoint, false);
}

void SchematicHelper::addLines(const std::vector<Line>& lines) {
    for (auto&& line : lines) {
        assert(!lineMap_.contains(line.id));

        auto srcPt = Avoid::Point(line.src.x, line.src.y);
        auto dstPt = Avoid::Point(line.dst.x, line.dst.y);
        auto connRef = new Avoid::ConnRef(router_.get(), srcPt, dstPt);
        lineMap_[line.id] = connRef;
    }
}

void SchematicHelper::removeLines(const std::vector<int>& lineIds) {
    for (auto&& lineId : lineIds) {
        auto connRef = lineMap_.at(lineId);
        router_->deleteConnector(connRef);
        lineMap_.erase(lineId);
    }
}

void SchematicHelper::moveLines(const std::vector<Line>& lines) {
    for (auto&& line : lines) {
        auto srcPt = Avoid::Point(line.src.x, line.src.y);
        auto dstPt = Avoid::Point(line.dst.x, line.dst.y);

        auto connRef = lineMap_.at(line.id);
        connRef->setSourceEndpoint(srcPt);
        connRef->setDestEndpoint(dstPt);
    }
}

void SchematicHelper::addShapes(const std::vector<Shape>& shapes) {
    for (auto&& shape : shapes) {
        assert(!shapeMap_.contains(shape.id));

        auto polygon = Avoid::Polygon(shape.polygon.size());
        for (auto i = 0; i < shape.polygon.size(); ++i) {
            polygon.ps[i] = Avoid::Point(shape.polygon[i].x, shape.polygon[i].y);
        }
        auto shapeRef = new Avoid::ShapeRef(router_.get(), polygon);
        shapeMap_[shape.id] = shapeRef;
    }
}

void SchematicHelper::removeShapes(const std::vector<int>& shapeIds) {
    for (auto&& shapeId : shapeIds) {
        auto shapeRef = shapeMap_.at(shapeId);
        router_->deleteShape(shapeRef);
        shapeMap_.erase(shapeId);
    }
}

void SchematicHelper::moveShapes(const std::vector<Shape>& shapes) {
    for (auto&& [id, stdNewPolygon] : shapes) {
        auto avoidNewPolygon = Avoid::Polygon(stdNewPolygon.size());
        for (auto i = 0; i < stdNewPolygon.size(); ++i) {
            avoidNewPolygon.ps[i] = Avoid::Point(stdNewPolygon[i].x, stdNewPolygon[i].y);
        }
        shapeMap_.at(id)->setNewPoly(avoidNewPolygon);
    }
}

std::unordered_map<int, std::vector<Point>> SchematicHelper::updateAndGetPaths() {
    router_->processTransaction();

    pathCache_.clear();
    for (auto&& [id, connRef] : lineMap_) {
        auto route = connRef->displayRoute().ps;
        std::vector<Point> points(route.size());
        for (auto i = 0; i < route.size(); ++i) {
            points[i] = Point{route[i].x, route[i].y};
        }
        pathCache_[id] = points;
    }

    return pathCache_;
}

}  // namespace schematichelper
