/**
 * @file kinematics_plugin_factory.h
 * @brief Kinematics Plugin Factory
 *
 * @author Levi Armstrong
 * @date April 15, 2018
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2013, Southwest Research Institute
 *
 * @par License
 * Software License Agreement (Apache License)
 * @par
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * @par
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TESSERACT_KINEMATICS_KINEMATICS_PLUGIN_FACTORY_H
#define TESSERACT_KINEMATICS_KINEMATICS_PLUGIN_FACTORY_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <string>
#include <memory>
#include <map>
#include <yaml-cpp/yaml.h>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_kinematics/core/inverse_kinematics.h>
#include <tesseract_kinematics/core/forward_kinematics.h>
#include <tesseract_scene_graph/graph.h>
#include <tesseract_scene_graph/scene_state.h>
#include <tesseract_common/plugin_loader.h>

namespace tesseract_common
{
class PluginLoader;
}

namespace tesseract_kinematics
{
/** @brief Forward declare Plugin Factory */
class KinematicsPluginFactory;

/** @brief Define a inverse kinematics plugin which the factory can create an instance */
class InvKinFactory
{
public:
  using Ptr = std::shared_ptr<InvKinFactory>;
  using ConstPtr = std::shared_ptr<const InvKinFactory>;

  virtual ~InvKinFactory() = default;

  /**
   * @brief Create Inverse Kinematics Object
   * @param name The name of the kinematic chain
   * @param scene_graph The Tesseract Scene Graph
   * @param scene_state The state of the scene graph
   * @param plugin_factory Provide access to the plugin factory so plugins and load plugins
   * @return If failed to create, nullptr is returned.
   */
  virtual InverseKinematics::UPtr create(const std::string& name,
                                         const tesseract_scene_graph::SceneGraph& scene_graph,
                                         const tesseract_scene_graph::SceneState& scene_state,
                                         const KinematicsPluginFactory& plugin_factory,
                                         const YAML::Node& config) const = 0;
};

/** @brief Define a forward kinematics plugin which the factory can create an instance */
class FwdKinFactory
{
public:
  using Ptr = std::shared_ptr<FwdKinFactory>;
  using ConstPtr = std::shared_ptr<const FwdKinFactory>;

  virtual ~FwdKinFactory() = default;

  /**
   * @brief Create Inverse Kinematics Object
   * @param name The name of the kinematic chain
   * @param scene_graph The Tesseract Scene Graph
   * @param scene_state The state of the scene graph
   * @param plugin_factory Provide access to the plugin factory so plugins and load plugins
   * @return If failed to create, nullptr is returned.
   */
  virtual ForwardKinematics::UPtr create(const std::string& name,
                                         const tesseract_scene_graph::SceneGraph& scene_graph,
                                         const tesseract_scene_graph::SceneState& scene_state,
                                         const KinematicsPluginFactory& plugin_factory,
                                         const YAML::Node& config) const = 0;
};

/** @brief The KinematicsPluginInfo struct */
struct KinematicsPluginInfo
{
  /** @brief The plugin solver name */
  std::string name;

  /** @brief The plugin class name */
  std::string class_name;

  /** @brief Then kinematic group name the plugin is related to */
  std::string group;

  /** @brief The kinematic plugin config data */
  YAML::Node config;
};

class KinematicsPluginFactory
{
public:
  KinematicsPluginFactory();
  ~KinematicsPluginFactory();

  /**
   * @brief Load plugins from yaml nodde
   * @param config The config node
   */
  KinematicsPluginFactory(YAML::Node config);

  /**
   * @brief Load plugins from file path
   * @param config The config file path
   */
  KinematicsPluginFactory(tesseract_common::fs::path config);

  /**
   * @brief Load plugins from string
   * @param config The config string
   */
  KinematicsPluginFactory(std::string config);

  /**
   * @brief Add location for the plugin loader to search
   * @param path The full path to the directory
   */
  void addSearchPath(const std::string& path);

  /**
   * @brief Get the plugin search paths
   * @return The search paths
   */
  const std::set<std::string>& getSearchPaths() const;

  /**
   * @brief Add a library to search for plugin name
   * @param library_name The library name without the prefix or sufix
   */
  void addSearchLibrary(const std::string& library_name);

  /**
   * @brief Get the plugin search libraries
   * @return The search libraries
   */
  const std::set<std::string>& getSearchLibraries() const;

  /**
   * @brief Add a forward kinematics plugin to the manager
   * @param plugin_info The plugin information
   */
  void addFwdKinPlugin(KinematicsPluginInfo plugin_info);

  /**
   * @brief remove forward kinematics plugin from the manager
   * @param group_name The group name
   * @param solver_name The solver name
   */
  void removeFwdKinPlugin(const std::string& group_name, const std::string& solver_name);

  /**
   * @brief Add a inverse kinematics plugin to the manager
   * @param plugin_info The plugin information
   */
  void addInvKinPlugin(KinematicsPluginInfo plugin_info);

  /**
   * @brief remove inverse kinematics plugin from the manager
   * @param group_name The group name
   * @param solver_name The solver name
   */
  void removeInvKinPlugin(const std::string& group_name, const std::string& solver_name);

  /**
   * @brief Get forward kinematics object given group name and solver name
   * @details This looks for kinematics plugin info added using addFwdKinPlugin. If not found nullptr is returned.
   * @param group_name The group name
   * @param solver_name The solver
   * @param scene_graph The scene graph
   * @param scene_state The scene state
   */
  ForwardKinematics::UPtr getFwdKin(const std::string& group_name,
                                    const std::string& solver_name,
                                    const tesseract_scene_graph::SceneGraph& scene_graph,
                                    const tesseract_scene_graph::SceneState& scene_state) const;

  /**
   * @brief Get inverse kinematics object given group name and solver name
   * @details This looks for kinematics plugin info added using addInvKinPlugin. If not found nullptr is returned.
   * @param group_name The group name
   * @param solver_name The solver
   * @param scene_graph The scene graph
   * @param scene_state The scene state
   */
  InverseKinematics::UPtr getInvKin(const std::string& group_name,
                                    const std::string& solver_name,
                                    const tesseract_scene_graph::SceneGraph& scene_graph,
                                    const tesseract_scene_graph::SceneState& scene_state) const;

  /**
   * @brief Get forward kinematics object given plugin info
   * @param plugin_info The plugin information to create kinematics object
   * @param scene_graph The scene graph
   * @param scene_state The scene state
   */
  ForwardKinematics::UPtr getFwdKin(const KinematicsPluginInfo& plugin_info,
                                    const tesseract_scene_graph::SceneGraph& scene_graph,
                                    const tesseract_scene_graph::SceneState& scene_state) const;

  /**
   * @brief Get inverse kinematics object given plugin info
   * @param plugin_info The plugin information to create kinematics object
   * @param scene_graph The scene graph
   * @param scene_state The scene state
   */
  InverseKinematics::UPtr getInvKin(const KinematicsPluginInfo& plugin_info,
                                    const tesseract_scene_graph::SceneGraph& scene_graph,
                                    const tesseract_scene_graph::SceneState& scene_state) const;

private:
  mutable std::map<std::string, FwdKinFactory::Ptr> fwd_kin_factories_;
  mutable std::map<std::string, InvKinFactory::Ptr> inv_kin_factories_;
  std::map<std::string, std::map<std::string, KinematicsPluginInfo>> fwd_plugin_info_;
  std::map<std::string, std::map<std::string, KinematicsPluginInfo>> inv_plugin_info_;
  std::unique_ptr<tesseract_common::PluginLoader> plugin_loader_;
};

}  // namespace tesseract_kinematics
#endif  // TESSERACT_KINEMATICS_KINEMATICS_PLUGIN_FACTORY_H