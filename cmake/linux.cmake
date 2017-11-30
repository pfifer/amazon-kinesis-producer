
# -stdlib=libc++
set(THIRD_PARTY_CFLAGS_VALUE "CFLAGS='-mmacosx-version-min=10.9'")
set(THIRD_PARTY_CXXFLAGS_VALUE "CXXFLAGS='-stdlib=libc++'")
set(THIRD_PARTY_LDFLAGS_VALUE "LDFLAGS='-nodefaultlibs -lpthread -ldl -lc++ -lc++abi -lm -lc -lgcc_s'")

include("cmake/nix-common.cmake")
