//
// Created by ilya on 06.01.2020.
//

#ifndef YANDEXCPLUSPLUS_5_BLACK_1_WEEK_7_TRANSPORT_BOOK_PART_G_MAP_MAP_H
#define YANDEXCPLUSPLUS_5_BLACK_1_WEEK_7_TRANSPORT_BOOK_PART_G_MAP_MAP_H
#include <memory>
#include <string>

#include "connector.h"
#include "database.h"
#include "svg.h"

namespace TransportDatabase {
enum class LayersType {BUS, STOP, STOP_NAME, BUS_NAME};
class RenderParams {
public:
  double min_lat;
  double max_lat;
  double min_long;
  double max_long;

  double width;
  double height;
  double padding;
  double stop_radius;
  double line_width;
  int32_t stop_label_font_size;
  Svg::Point stop_label_offset;
  Svg::Color underlayer_color;
  double underlayer_width;
  std::vector<Svg::Color> color_palette;
  std::vector<LayersType> layers_order;

  double bus_label_font_size;
  Svg::Point bus_label_offset;
};

class RenderConfig {
public:
  RenderConfig() = default;
  RenderConfig(const RenderParams& rp);
  void ChangeRoutingParams(const RenderParams& rp);
  Svg::Point ZoomIn(const Coordinates& coord);
  Svg::Color GetNewColor();
  RenderParams render_params;
private:
  size_t curr_color = 0;
  double coef = 0;
};

class Layer {
public:
  Layer(RenderConfig &rp);
  virtual ~Layer() = default;
  virtual std::vector<Svg::SvgObjectHolder> CreateLayer(const Database& db) = 0;
  void Clear();
protected:
  RenderConfig params;
};

class BusLayer : public Layer {
public:
  using Layer::Layer;
  std::vector<Svg::SvgObjectHolder> CreateLayer(const Database& db) override;
};

class StopLayer : public Layer {
public:
  using Layer::Layer;
  std::vector<Svg::SvgObjectHolder> CreateLayer(const Database& db) override;
};

class StopNameLayer : public Layer {
public:
  using Layer::Layer;
  std::vector<Svg::SvgObjectHolder> CreateLayer(const Database& db) override;
};

class BusNameLayer : public Layer {
public:
  using Layer::Layer;
  std::vector<Svg::SvgObjectHolder> CreateLayer(const Database& db) override;
};

class Map : public Connector {
  using LayerHolder = std::unique_ptr<Layer>;
  template <typename LayerType>
  LayerHolder CreateLayer();
public:
  using Connector::Connector;
  Map();
  Map(std::shared_ptr<Database> db, const RenderParams& params);

  void SetLayerOrder(const std::vector<LayersType>& layers);
  void CreateMap();
  std::string RenderMap();
  void ClearMap();
private:
  void CreateLayerOrder();
  Svg::Document doc;
  std::vector<LayerHolder> layers;
  RenderConfig render_config;
  std::vector<LayersType> current_order = {LayersType::BUS, LayersType::STOP, LayersType::STOP_NAME};
};
}
#endif //YANDEXCPLUSPLUS_5_BLACK_1_WEEK_7_TRANSPORT_BOOK_PART_G_MAP_MAP_H
