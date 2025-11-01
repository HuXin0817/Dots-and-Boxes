#pragma once

#include <string>

#include "BasicSearchModel.h"
#include "ImprovedSearchModel.h"
#include "MonteCarloSearchModel.h"
#include "ParallelSearchModel.h"
#include "SearchModel.h"
#include "SimpleStrategyModel.h"

enum class AIModelType { L0, L1, L2, L3, L4 };

class AIConfig {
  public:
  static SearchModel*
  createModel(AIModelType type) {
    switch (type) {
      case AIModelType::L0:
        return new SimpleStrategyModel();
      case AIModelType::L1:
        return new BasicSearchModel();
      case AIModelType::L2:
        return new ImprovedSearchModel();
      case AIModelType::L3:
        return new MonteCarloSearchModel();
      case AIModelType::L4:
        return new ParallelSearchModel();
      default:
        return new ParallelSearchModel();
    }
  }

  static std::string
  getModelName(AIModelType type) {
    switch (type) {
      case AIModelType::L0:
        return "L0";
      case AIModelType::L1:
        return "L1";
      case AIModelType::L2:
        return "L2";
      case AIModelType::L3:
        return "L3";
      case AIModelType::L4:
        return "L4";
      default:
        return "L4";
    }
  }

  static std::string
  getModelDescription(AIModelType type) {
    switch (type) {
      case AIModelType::L0:
        return "Basic Model - Simple Strategy";
      case AIModelType::L1:
        return "Simple Search Model - Basic Search";
      case AIModelType::L2:
        return "Intermediate Search Model - Improved Search";
      case AIModelType::L3:
        return "Advanced Search Model - Monte Carlo Search";
      case AIModelType::L4:
        return "Highest Level Model - Parallel Monte Carlo Search";
      default:
        return "Highest Level Model - Parallel Monte Carlo Search";
    }
  }

  static AIModelType
  parseModelType(const std::string& name) {
    if (name == "L0" || name == "l0") {
      return AIModelType::L0;
    } else if (name == "L1" || name == "l1") {
      return AIModelType::L1;
    } else if (name == "L2" || name == "l2") {
      return AIModelType::L2;
    } else if (name == "L3" || name == "l3") {
      return AIModelType::L3;
    } else if (name == "L4" || name == "l4") {
      return AIModelType::L4;
    } else {
      return AIModelType::L4;
    }
  }
};
