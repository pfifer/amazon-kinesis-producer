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

#include "error_translater.h"

#include <sstream>
#ifdef WIN32
#include <Windows.h>
#else
#include <cerrno>
#endif

namespace aws {
  namespace utils {
    int get_last_error() {
#ifdef WIN32
      return GetLastError();
#else
      return errno;
#endif
    }

    std::string translate_error(int code) {
      std::stringstream ss;
#ifdef WIN32
      LPSTR buffer = NULL;
      if (FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, code, 0, (LPSTR)&buffer, 0, NULL)) {
        ss << buffer;
        LocalFree(buffer);
      }
      else {
        ss << "Failed to translate error (" << code << ") due to " << GetLastError() << " while attempting to translate the error code";
      }
#else
      ss << strerror(code);
#endif
      return ss.str();
    }

    std::string translate_last_error() {
      return translate_error(get_last_error());
    }
  }
}
