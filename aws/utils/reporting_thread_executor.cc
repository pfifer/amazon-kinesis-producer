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


#include "reporting_thread_executor.h"

using namespace aws::utils;

bool reporting_thread_executor::SubmitToThread(std::function<void()> &&function) {
  std::thread t = make_reporting_thread(std::move(function));
  t.detach();

  return true;
}