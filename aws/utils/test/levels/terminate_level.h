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

#include <exception>
#include <system_error>
#include <sstream>
#include <iostream>

#define LOCAL_THROW(who) { \
  if (how > 100 && how <= 1000) { \
    throw 10; \
  } \
  if (how > 1000 && how <= 9000) { \
    std::stringstream ss; \
    ss << who << ": How is greater than 1000: " << how; \
    throw std::runtime_error(ss.str()); \
  } \
  if (how > 9000) { \
    std::stringstream ss; \
    ss << who << ": What does the scouter say? It's over 9000!!: " << how; \
    throw std::system_error(std::make_error_code(std::errc::no_buffer_space)); \
  } \
  std::cout << who << ": How is below 100 no exception" << std::endl << std::flush; \
}

#define RETHROW(ex) { \
  if (is_rethrow) { \
    throw ex; \
  } \
}

#define NEXT_THROW(next, next_name, next_method, who) { \
  if (in_catch) { \
    try { \
      next.next_method; \
      std::cout << who << ": No error from " << next_name << std::endl << std::flush; \
    } catch (int code) { \
      std::cout << who << ": Caught code from " << next_name << ": " << code << std::endl << std::flush; \
      RETHROW(code); \
    } catch (std::runtime_error& err) { \
      std::cout << who << ": Caught error from " << next_name << ": " << err.what() << std::endl << std::flush; \
      RETHROW(err); \
    } \
  } else { \
    next.next_method; \
  } \
}
