#pragma once

#include <glog/logging.h>

#define AWSCRATCH_CHECK_OUTCOME(outcome) \
  do { \
    if (!outcome.IsSuccess()) { \
      auto err = outcome.GetError(); \
      LOG(INFO) << err.GetExceptionName() << " : '" << err.GetMessage() << "'"; \
      throw err; \
    } \
  } while(0)
