
# -stdlib=libc++
set(THIRD_PARTY_CXXFLAGS_VALUE "-stdlib=libc++")
set(THIRD_PARTY_LDFLAGS_VALUE "-nodefaultlibs -lpthread -ldl -lc++ -lc++abi -lm -lc -lgcc_s")

include("cmake/nix-common.cmake")
