import qbs 1.0

Product {
    name: "axrcocoa"
    type: "staticlibrary"
    condition: qbs.targetOS === "osx"

    Depends { name: "cpp" }
    Depends { name: "axrcore" }
    Depends { name: "config_header" }
    Depends { name: "Qt"; submodules: [ "core", "gui" ] }

    files: [
        "NSAXRView.h",
        "NSAXRView.mm"
    ]

    Properties {
        condition: qbs.targetOS === "osx"
        cpp.frameworks: [ "Cocoa" ]
    }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: "."
    }
}
