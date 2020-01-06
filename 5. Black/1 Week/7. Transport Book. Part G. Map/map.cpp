//
// Created by ilya on 06.01.2020.
//

#include <sstream>
#include <utility>

#include "map.h"
#include "svg.h"

namespace TransportDatabase {

Layer::Layer(const TransportDatabase::RenderConfig &rp) : params(rp) {}

RenderConfig::RenderConfig(TransportDatabase::RenderParams rp) {
  ChangeRoutingParams(rp);
}

void RenderConfig::ChangeRoutingParams(TransportDatabase::RenderParams rp) {
  render_params = std::move(rp);
  double real_width = render_params.width - 2*render_params.padding;
  double real_height = render_params.height - 2*render_params.padding;
  double min_x = render_params.min_long;
  double max_x = render_params.max_long;
  double min_y = render_params.max_lat;
  double max_y = render_params.min_lat;
  double coef_x = (max_x - min_x) ? real_width/(max_x - min_x) : 0;
  double coef_y = (max_x - min_x) ? real_height/(max_y - min_y) : 0;
  if (!coef_x && !coef_y) {
    coef = 0;
  }
  else if (!coef_x) {
    coef = coef_y;
  }
  else if (!coef_y) {
    coef = coef_x;
  }
  else {
    coef = std::min(coef_x, coef_y);
  }
}

Svg::Point RenderConfig::ZoomIn(double longitude, double latitude) {
  return {.x = (longitude - render_params.min_long) * coef + render_params.padding,
          .y = (render_params.max_lat - latitude) * coef + render_params.padding};
}


std::vector<Svg::ObjectHolder> BusLayer::CreateLayer(const TransportDatabase::Database &db) {
  // TODO Сделать формирование отсортированного списка маршрутов(возможно изменить функцию возвращения из database)
  // TODO Сделать формирование полилинии из этого списка
  // TODO Придумать как получить min/max lat/long для расчетов. Возможно при добавлении сохранять данные, а затем получать
}

//
Map::Map() {
  SetLayerOrder({LayersType::BUS, LayersType::STOP, LayersType::STOP_NAME});
}

template <typename LayerType>
Map::LayerHolder Map::CreateLayer()  {
  return std::make_unique<LayerType>(render_config);
}

Map::Map(std::shared_ptr<Database> db, RenderParams params) : Connector(std::move(db)) {
  render_config = RenderConfig(std::move(params));
  SetLayerOrder(render_config.render_params.layers_order);
}

void Map::SetLayerOrder(const std::vector<LayersType> &layers_order) {
  for (const auto layer_type : layers_order) {
    switch (layer_type) {
    case LayersType::BUS: {
      layers.push_back(CreateLayer<BusLayer>());
    }
    case LayersType::STOP: {
      layers.push_back(CreateLayer<StopLayer>());
    }
    case LayersType::STOP_NAME: {
      layers.push_back(CreateLayer<StopNameLayer>());
    }
    case LayersType::BUS_NAME: {
      layers.push_back(CreateLayer<BusNameLayer>());
    }
    default:
      throw std::runtime_error("unknown layer type");
    }
  }
}

void Map::CreateMap() {
  for (const auto& layer : layers) {
    auto objects = layer->CreateLayer(*db_);
    for (auto& object : objects) {
      doc.Add(std::move(object));
    }
  }
}

std::string Map::RenderMap() const {
  std::stringstream out;
  doc.Render(out);
  return out.str();
}

void Map::ClearMap() {
  doc.Clear();
}
}