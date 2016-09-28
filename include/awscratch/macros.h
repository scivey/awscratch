#pragma once

#include <folly/Demangle.h>
#include <glog/logging.h>

#define AWSCRATCH_CHECK_OUTCOME(outcome) \
  do { \
    if (!outcome.IsSuccess()) { \
      auto err = outcome.GetError(); \
      LOG(INFO) << err.GetExceptionName() << " : '" << err.GetMessage() << "'"; \
      throw err; \
    } \
  } while(0)


#define LOG_VAL_TYPE(x) \
  LOG(INFO) << "Type of '" #x "' : '" << folly::demangle(typeid(decltype(x))).toStdString() << "'"

#define LOG_TYPE(x) \
  LOG(INFO) << "Type: '" << folly::demangle(typeid(x)).toStdString() << "'"
