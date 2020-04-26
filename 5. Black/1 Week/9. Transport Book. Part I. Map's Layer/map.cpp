//
// Created by ilya on 06.01.2020.
//

#include <sstream>
#include <utility>
#include <map>

#include "map.h"

namespace TransportDatabase {

RenderConfig::RenderConfig(const RenderParams& rp) {
  ChangeRoutingParams(rp);
}

void RenderConfig::ChangeRoutingParams(const TransportDatabase::RenderParams& rp) {
  render_params = rp;
  double real_width = render_params.width - 2*render_params.padding;
  double real_height = render_params.height - 2*render_params.padding;
  double min_x = render_params.min_long;
  double max_x = render_params.max_long;
  double min_y = render_params.max_lat;
  double max_y = render_params.min_lat;
  double coef_x = (max_x - min_x) ? real_width/(max_x - min_x) : 0;
  double coef_y = (max_y - min_y) ? real_height/(min_y - max_y) : 0;
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
  curr_color = 0;
}

Svg::Point RenderConfig::ZoomIn(const Coordinates& coord) {
  return {.x = (coord.GetLongitude() - render_params.min_long) * coef + render_params.padding,
          .y = (render_params.max_lat - coord.GetLatitude()) * coef + render_params.padding};
}

Svg::Color RenderConfig::GetNewColor() {
  return render_params.color_palette[curr_color++ % render_params.color_palette.size()];
}

Layer::Layer(RenderConfig &rp) : params(rp) {}

void Layer::Clear() {
  params.ChangeRoutingParams(params.render_params);
}

std::vector<Svg::SvgObjectHolder> BusLayer::CreateLayer(const TransportDatabase::Database &db) {
  std::vector<Svg::SvgObjectHolder> result;
  std::map<std::string, std::shared_ptr<Route>> sorted_routes;
  for (const auto& [route_name, route_ptr] : db.TakeRoutes()) {
    sorted_routes[route_name] = route_ptr;
  }
  Svg::Polyline basic_line;
  basic_line.SetStrokeWidth(params.render_params.line_width);
  basic_line.SetStrokeLineCap("round");
  basic_line.SetStrokeLineJoin("round");
  for (const auto& [route_name, route_ptr] : sorted_routes) {
    Svg::Polyline bus = basic_line;
    bus.SetStrokeColor(params.GetNewColor());
    auto stops_name = route_ptr->GetStopsName();
    if (route_ptr->route_type == Route::RouteTypes::LINEAR) {
      for (auto i = stops_name.begin(); i != stops_name.end(); ++i) {
        auto stop = db.TakeStop(*i);
        bus.AddPoint(params.ZoomIn(stop->GetCoord()));
      }
      for (auto i = std::next(stops_name.rbegin()); i != stops_name.rend(); ++i) {
        auto stop = db.TakeStop(*i);
        bus.AddPoint(params.ZoomIn(stop->GetCoord()));
      }
    }
    if (route_ptr->route_type == Route::RouteTypes::CYCLE) {
      for (auto i = stops_name.begin(); i != stops_name.end(); ++i) {
        auto stop = db.TakeStop(*i);
        bus.AddPoint(params.ZoomIn(stop->GetCoord()));
      }
    }
    result.push_back(Svg::MakeSvgObject(bus));
  }
  return result;
}

std::vector<Svg::SvgObjectHolder> StopLayer::CreateLayer(const TransportDatabase::Database &db) {
  std::vector<Svg::SvgObjectHolder> result;
  std::map<std::string, std::shared_ptr<Stop>> sorted_stops;
  for (const auto& [route_name, route_ptr] : db.TakeStops()) {
    sorted_stops[route_name] = route_ptr;
  }
  Svg::Circle base_circle;
  base_circle.SetRadius(params.render_params.stop_radius);
  base_circle.SetFillColor("white");
  for (const auto& [stop_name, stop_ptr] : sorted_stops) {
    Svg::Circle stop = base_circle;
    stop.SetCenter(params.ZoomIn(stop_ptr->GetCoord()));
    result.push_back(Svg::MakeSvgObject(stop));
  }
  return result;
}

std::vector<Svg::SvgObjectHolder> StopNameLayer::CreateLayer(const TransportDatabase::Database &db) {
  std::vector<Svg::SvgObjectHolder> result;
  std::map<std::string, std::shared_ptr<Stop>> sorted_stops;
  for (const auto& [stop_name, stop_ptr] : db.TakeStops()) {
    sorted_stops[stop_name] = stop_ptr;
  }
  Svg::Text base_text;
  base_text.SetOffset(params.render_params.stop_label_offset);
  base_text.SetFontSize(params.render_params.stop_label_font_size);
  base_text.SetFontFamily("Verdana");
  Svg::Text podl = base_text;
  podl.SetFillColor(params.render_params.underlayer_color);
  podl.SetStrokeColor(params.render_params.underlayer_color);
  podl.SetStrokeWidth(params.render_params.underlayer_width);
  podl.SetStrokeLineCap("round");
  podl.SetStrokeLineJoin("round");
  Svg::Text text = base_text;
  text.SetFillColor("black");
  for (const auto& [stop_name, stop_ptr] : sorted_stops) {
    Svg::Text name = text;
    Svg::Text podl_name = podl;
    name.SetData(stop_name);
    name.SetPoint(params.ZoomIn(stop_ptr->GetCoord()));
    podl_name.SetData(stop_name);
    podl_name.SetPoint(params.ZoomIn(stop_ptr->GetCoord()));
    result.push_back(Svg::MakeSvgObject(podl_name));
    result.push_back(Svg::MakeSvgObject(name));
  }
  return result;
}

std::vector<Svg::SvgObjectHolder> BusNameLayer::CreateLayer(const Database &db) {
  std::vector<Svg::SvgObjectHolder> result;
  std::map<std::string, std::shared_ptr<Route>> sorted_route;
  for (const auto& [route_name, route_ptr] : db.TakeRoutes()) {
    sorted_route[route_name] = route_ptr;
  }
  Svg::ExtendedText base_text;
  base_text.SetOffset(params.render_params.bus_label_offset);
  base_text.SetFontSize(params.render_params.bus_label_font_size);
  base_text.SetFontFamily("Verdana");
  base_text.SetFontWeight("bold");
  Svg::ExtendedText podl = base_text;
  podl.SetFillColor(params.render_params.underlayer_color);
  podl.SetStrokeColor(params.render_params.underlayer_color);
  podl.SetStrokeWidth(params.render_params.underlayer_width);
  podl.SetStrokeLineCap("round");
  podl.SetStrokeLineJoin("round");
  Svg::ExtendedText text = base_text;
  for (const auto& [route_name, route_ptr] : sorted_route) {
    if (route_ptr->route_type == Route::RouteTypes::CYCLE) {
      Svg::ExtendedText name = text;
      Svg::ExtendedText podl_name = podl;
      name.SetFillColor(params.GetNewColor());
      auto stop = db.TakeStop(route_ptr->GetStopsName().front());
      name.SetData(route_name);
      name.SetPoint(params.ZoomIn(stop->GetCoord()));
      podl_name.SetData(route_name);
      podl_name.SetPoint(params.ZoomIn(stop->GetCoord()));
      result.push_back(Svg::MakeSvgObject(podl_name));
      result.push_back(Svg::MakeSvgObject(name));
    }
    else if (route_ptr->route_type == Route::RouteTypes::LINEAR) {
      Svg::ExtendedText name = text;
      Svg::ExtendedText podl_name = podl;
      name.SetFillColor(params.GetNewColor());
      auto first_stop = db.TakeStop(route_ptr->GetStopsName().front());
      auto last_stop = db.TakeStop(route_ptr->GetStopsName().back());
      {
        name.SetData(route_name);
        name.SetPoint(params.ZoomIn(first_stop->GetCoord()));
        podl_name.SetData(route_name);
        podl_name.SetPoint(params.ZoomIn(first_stop->GetCoord()));
        result.push_back(Svg::MakeSvgObject(podl_name));
        result.push_back(Svg::MakeSvgObject(name));
      }
      if (first_stop != last_stop){
        name.SetData(route_name);
        name.SetPoint(params.ZoomIn(last_stop->GetCoord()));
        podl_name.SetData(route_name);
        podl_name.SetPoint(params.ZoomIn(last_stop->GetCoord()));
        result.push_back(Svg::MakeSvgObject(podl_name));
        result.push_back(Svg::MakeSvgObject(name));
      }
    }
  }
  return result;
}

Map::Map() {
  SetLayerOrder({LayersType::BUS, LayersType::BUS_NAME, LayersType::STOP, LayersType::STOP_NAME});
}

Map::Map(std::shared_ptr<Database> db, const RenderParams& params) : Connector(std::move(db)) {
  render_config = RenderConfig(params);
  SetLayerOrder(render_config.render_params.layers_order);
}

template <typename LayerType>
Map::LayerHolder Map::CreateLayer()  {
  return std::make_unique<LayerType>(render_config);
}

void Map::SetLayerOrder(const std::vector<LayersType> &layers_order) {
  current_order = layers_order;
  CreateLayerOrder();
}

void Map::CreateMap() {
  for (const auto& layer : layers) {
    auto objects = layer->CreateLayer(*db_);
    for (auto& object : objects) {
      doc.AddDocPart(std::move(object));
    }
  }
  doc.Render(std::cerr);
}

std::string Map::RenderMap() {
  std::stringstream out;
  doc.Render(out);
  ClearMap();
  return out.str();
}

void Map::CreateLayerOrder() {
  for (const auto layer_type : current_order) {
    if (layer_type == LayersType::BUS) {
      layers.push_back(CreateLayer<BusLayer>());
    }
    else if (layer_type == LayersType::STOP) {
      layers.push_back(CreateLayer<StopLayer>());
    }
    else if (layer_type == LayersType::STOP_NAME) {
      layers.push_back(CreateLayer<StopNameLayer>());
    }
    else if (layer_type == LayersType::BUS_NAME) {
      layers.push_back(CreateLayer<BusNameLayer>());
    }
    else {
      throw std::runtime_error("unknown layer type");
    }
  }
}

void Map::ClearMap() {
  doc.Clear();
  layers.clear();
  CreateLayerOrder();
}
}