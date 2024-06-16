# Dll-Injection-Skeleton

The DLL Injector library provides a simple and efficient way to inject and eject DLLs into and from running processes on Windows. With support for both command-line interface and direct integration through CMake, this tool is ideal for developers needing to modify or debug application behavior dynamically. The library leverages Windows API functions to perform DLL injection and ejection, making it a versatile addition to any developer's toolkit. The CMake integration ensures seamless inclusion in larger projects, allowing immediate access to its functionality through easy linking.

## Build

### Prerequisites

Before building the project, ensure you have the following installed on your system:

-   **CMake**: Version 3.0 or higher.
-   **Visual Studio**: Recommended version for Windows development.

### Steps to Build

#### 1. Clone the Repository

Clone the "Dll-Injection-Skeleton" repository from GitHub:
```bash
cd path/to/dll-injection-skeleton
```

#### 2. Configure the Build with CMake

Run CMake to configure the build. Specify the generator appropriate for your installed Visual Studio version (e.g., "Visual Studio 2019"):

```bash
mkdir build
cd build
cmake -G "Visual Studio 16 2019" ..  # Replace with your Visual Studio version
```

#### 3. Build the Project

Open the generated solution file (`dll-injection-skeleton.sln`) in Visual Studio and build the project:

-   Select `Build` > `Build Solution` from the Visual Studio menu.
-   Alternatively, build from the command line:
```bash
cmake --build . --config Release
```
Replace `Release` with `Debug` if you need a debug build.

#### 4. Running Examples (Optional)

If you enabled building examples (`DIS_BUILD_EXAMPLES` option), you can run them after building:

-   Navigate to the `build` directory (if not already there).
-   Run the built examples by injecting them into any process of your choice using **CLI** (See CLI).


## Integration

To integrate the **Dll-Injection-Skeleton** library into your project, follow these steps:

### Step 1: Clone the Repository

Clone the **Dll-Injection-Skeleton** repository from GitHub into your project's directory:
```bash
git clone https://github.com/gurka/DllInjectionSkeleton.git
```
### Step 2: Include as Subdirectory

Integrate the library into your CMake project by adding it as a subdirectory in your `CMakeLists.txt` file. Assuming your project's root directory is where you want to include **Dll-Injection-Skeleton**:
```cmake
add_subdirectory(path/to/DllInjectionSkeleton)
```

Replace `path/to/DllInjectionSkeleton` with the actual path relative to your project.

### Step 3: Link to the Library

Link your project to the **Dll-Injection-Skeleton** library target. For example, if your project is named `MyProject` and you want to link against `dll-injector` target from **Dll-Injection-Skeleton**, modify your `CMakeLists.txt`:
```cmake
target_link_libraries(MyProject PRIVATE dll-injector)
```

Make sure to replace `MyProject` with the actual name of your project.

### Step 4: Build Your Project

Build your project using CMake and your preferred build tool (e.g., Visual Studio, Ninja, etc.). Ensure that CMake correctly identifies and integrates **Dll-Injection-Skeleton** into your build system.

### Example CMakeLists.txt

Here is a simplified example of how your `CMakeLists.txt` might look after integrating **Dll-Injection-Skeleton**:
```cmake
cmake_minimum_required(VERSION 3.0)
project(MyProject)

# Include Dll-Injection-Skeleton as a subdirectory
add_subdirectory(path/to/DllInjectionSkeleton)

# Define your project's executable or library
add_executable(MyProject main.cpp)

# Link your project to Dll-Injection-Skeleton
target_link_libraries(MyProject PRIVATE dll-injector)
```

Adjust paths and configurations according to your project's structure and requirements.


## Usage & CLI

### Installation:
Follow instructions in the Build section to build using CMake or download the tool.

### CLI Usage

The `dll-injector-cli` tool supports two commands:

-   **Inject Command**: Injects a DLL into a specified process.
```bash
dll-injector-cli inject [process name.exe] [dll path]
```
- **Eject Command**: Ejects a DLL from a specified process.
```bash
dll-injector-cli eject [process name.exe] [dll name]
```
### Example Usage

Examples of using `dll-injector-cli`:
```bash
# Inject DLL into a process
dll-injector-cli inject notepad.exe path/to/mydll.dll

# Eject DLL from a process
dll-injector-cli eject notepad.exe mydll.dll
```
Ensure the process is running and accessible to the user. Successful DLL operations depend on correct process and DLL path/name specifications.

### Notes

-   Ensure appropriate permissions to perform DLL injection and ejection.
-   Integrate the `dll-injector` target into your CMake projects for programmatic access to DLL injection capabilities, for more information check **Integration** Section.

