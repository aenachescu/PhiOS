Modules Development Guide
=========

PhiOS is divided in components, each component being composed of more modules.
Components:
* Drivers
* Utils
* Kernel

How to add a new module?
-----

Following steps must be followed to add a new module to one of the components:
1. Add a new directory to component's directory (`PhiOS/component/module`)
2. Create `src` and `include` directories in module directory
3. Create a CMakeLists.txt file
4. Configure cmake file for your module
5. At the end of the file, add module's sources, include paths and libraries
to component's global variables

Component's global variables
-----

Each component has global variables for sources, include paths and libraries.
Those variables has the following names structure:
* `<component>_C_SOURCES` - C language sources
* `<component>_ASM_SOURCES` - ASM language sources
* `<component>_STATIC_LIBS` - static libs created by the component
* `<component>_INCLUDE_PATHS` - include paths of the component

If you need to add your module's sources, paths and libs to the component, you
need to append them to the global variables. Example:
```
# Set current path
# All sources must have absolute path!
# You could use ${CMAKE_CURRENT_SOURCE_DIR} that represents current directory
# Or ${PRJ_PATH} that represents PhiOS root directory
set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Set temporary variables
list(APPEND <module>_C_SOURCES "${MODULE_PATH}/source_file.c")
list(APPEND <module>_ASM_SOURCES "${MODULE_PATH}/source_file.s")
# Libs doesn't need absolute path
list(APPEND <module>_STATIC_LIBS "someNameOfTheLib")
list(APPEND <module>_INCLUDE_PATH "${PRJ_PATH}/some/directory/to/include")

# Update parent scope
set(<component>_C_SOURCES "${C_SOURCES};${<module>_C_SOURCES}" PARENT_SCOPE)
set(<component>_ASM_SOURCES "${ASM_SOURCES};${<module>_ASM_SOURCES}" PARENT_SCOPE)
set(<component>_STATIC_LIBS "${STATIC_LIBS};${<module>_STATIC_LIBS}" PARENT_SCOPE)
set(<component>_INCLUDE_PATH "${INCLUDE_PATH};${<module>_INCLUDE_PATH}" PARENT_SCOPE)

# More examples could be found in drivers/test and util/test modules
```
