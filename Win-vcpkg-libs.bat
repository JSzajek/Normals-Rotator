cd Z:\Documents\Cpp Projects\vcpkg

call vcpkg install spdlog:x64-windows-static
call vcpkg install stduuid[gsl-span,system-gen]:x64-windows-static --recurse

PAUSE