//
// Created by Pfifer, Justin on 3/3/17.
//

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
