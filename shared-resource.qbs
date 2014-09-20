import qbs

CppApplication {
    type: "application"
    name : "shared-resource-tests"

    cpp.includePaths: [
        "include"
    ]

    files : [
        "tests/main.cpp",
        "include/SharedResource.h"
    ]
}
