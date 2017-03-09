// Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
//
// Licensed under the Amazon Software License (the "License").
// You may not use this file except in compliance with the License.
// A copy of the License is located at
//
//  http://aws.amazon.com/asl
//
// or in the "license" file accompanying this file. This file is distributed
// on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
// express or implied. See the License for the specific language governing
// permissions and limitations under the License.

#include "terminate_level_2.h"

#include "terminate_level.h"

namespace aws {
  namespace utils {
    void terminate_level_2::local_throw(int how) {
      LOCAL_THROW("level_2");
    }

    void terminate_level_2::level_3_throw(int how, bool in_catch, bool is_rethrow) {
      NEXT_THROW(level3, "level_3", local_throw(how), "level_2");
    }
  }
}