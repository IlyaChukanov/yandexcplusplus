//
// Created by ilya on 06.01.2020.
//

#ifndef YANDEXCPLUSPLUS_5_BLACK_1_WEEK_7_TRANSPORT_BOOK_PART_G_MAP_CONNECTOR_H
#define YANDEXCPLUSPLUS_5_BLACK_1_WEEK_7_TRANSPORT_BOOK_PART_G_MAP_CONNECTOR_H

#include <memory>
#include "database.h"

// Interface for connecting to database
namespace TransportDatabase {
class Connector {
public:
  Connector() {
    db_ = std::make_shared<Database>();
  }
  explicit Connector(std::shared_ptr<Database> db) {
    ChangeDatabase(db);
  }
  virtual void ChangeDatabase(std::shared_ptr<Database> db) {
    db_ = std::move(db);
  }
  virtual ~Connector() = default;
protected:
  std::shared_ptr<Database> db_;
};
}
#endif //YANDEXCPLUSPLUS_5_BLACK_1_WEEK_7_TRANSPORT_BOOK_PART_G_MAP_CONNECTOR_H
