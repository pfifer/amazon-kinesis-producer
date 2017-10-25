#include <aws/utils/test/temp_ca_dir.h>

#include <aws/mutex.h>
#include <aws/utils/logging.h>

#include <boost/filesystem.hpp>

#include <cstdio>
#include <sys/stat.h>
#include <system_error>
#include <cerrno>
#include <memory>

namespace {
    aws::mutex creation_mutex;
    std::shared_ptr<std::string> ca_temp_directory;
};

std::string& aws::utils::test::get_temp_ca_dir() {
    if (!ca_temp_directory) {
        aws::lock_guard<aws::mutex> guard(creation_mutex);
        if (!ca_temp_directory) {
            boost::filesystem::path temp_directory = boost::filesystem::unique_path();
            ca_temp_directory = std::make_shared<std::string>(temp_directory.string());
        }
    }
    return *ca_temp_directory;
}
