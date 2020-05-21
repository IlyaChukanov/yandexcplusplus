#include "yellow_pages.h"

#include <algorithm>
#include <string>
#include <set>

namespace YellowPages {

void MergeSingle(Company& company,
                 const google::protobuf::FieldDescriptor* current_field,
                 const Signals& signals,
                 const Providers& providers) {
  auto field = company.GetDescriptor()->FindFieldByName(current_field->name());
  std::string serialized_obj;
  for (const auto& signal : signals) {
    const auto& signal_company = signal.company();
    auto ref = company.GetReflection();
    if (ref->HasField(signal_company, field)) {
      serialized_obj = ref->GetMessage(signal_company, field).SerializeAsString();
      break;
    }
  }
  if (!serialized_obj.empty()) {
    auto item = company.GetReflection()->MutableMessage(&company, field);
    item->ParseFromString(serialized_obj);
  }
}

void MergeRepeated(Company& company,
                 const google::protobuf::FieldDescriptor* current_field,
                 const Signals& signals,
                 const Providers& providers) {
  auto field = company.GetDescriptor()->FindFieldByName(current_field->name());
  std::set<std::string> serialized_objects;
  int priority = 0;
  for (const auto& signal : signals) {
    const auto& provider = providers.at(signal.provider_id());
    const auto& signal_company = signal.company();
    auto ref = company.GetReflection();
    auto size = ref->FieldSize(signal_company, field);
    if (provider.priority() < priority || size == 0) {
      continue;
    }
    if (provider.priority() > priority) {
      priority = provider.priority();
      serialized_objects.clear();
    }
    for (int i = 0; i < size; ++i) {
      serialized_objects.insert(ref->GetRepeatedMessage(signal_company, field, i).SerializeAsString());
    }
  }
  for (const auto& object : serialized_objects) {
    auto item = company.GetReflection()->AddMessage(&company, field);
    item->ParseFromString(object);
  }
}

Company Merge(const Signals& signals, const Providers& providers) {
  // Наивная неправильная реализация: берём все данные из самого первого сигнала, никак не учитываем приоритеты
  auto sorted_signals = signals;
  std::sort(std::begin(sorted_signals), std::end(sorted_signals),
            [&providers](const Signal& a, const Signal& b){
              return providers.at(a.provider_id()).priority() > providers.at(b.provider_id()).priority();
            });
  Company company = Company();
  const auto* message_desc = company.GetDescriptor();
  for (int i = 0; i < message_desc->field_count(); ++i) {
    const auto* current_field = message_desc->field(i);
    if (current_field->is_repeated()) {
      MergeRepeated(company, current_field, sorted_signals, providers);
    }
    else {
      MergeSingle(company, current_field, sorted_signals, providers);
    }
  }
  return company;
}

}
