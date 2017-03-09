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

#include "terminate_level_3.h"

#ifndef AMAZON_KINESIS_PRODUCER_INTERNAL_TERMINATE_LEVEL_2_H
#define AMAZON_KINESIS_PRODUCER_INTERNAL_TERMINATE_LEVEL_2_H



namespace aws {
  namespace utils {
    class terminate_level_2 {
    private:
      terminate_level_3 level3;
    public:
      void local_throw(int how);

      void level_3_throw(int how, bool in_catch, bool is_rethrow);
    };
  }
}


#endif //AMAZON_KINESIS_PRODUCER_INTERNAL_TERMINATE_LEVEL_2_H
