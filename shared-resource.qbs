import qbs

CppApplication {
    type: "application"
    name : "shared-resource-tests"

    cpp.cxxFlags : "-std=c++11";

    cpp.includePaths: [
        "include"
    ]

    cpp.staticLibraries: [
        "boost_unit_test_framework"
    ]

    files : [
        "tests/main.cpp",
        "include/SharedResource.h"
    ]
}
