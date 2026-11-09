[![](https://github.com/zeebyezon/zeeedit/actions/workflows/build_and_test.yml/badge.svg)](https://github.com/zeebyezon/zeeedit/actions)

# ZeeEdit

ZeeEdit is a plugin that controls hardware MIDI gear, such as a synthesizer, from inside your DAW. It is a MIDI effect: it exposes the hardware's parameters as plugin parameters and sends the corresponding MIDI messages to the device, so you can edit and automate the hardware from your DAW's interface.

Currently the plugin is hardcoded to control the **Sequential Prophet-5**. In the future it may read the description of the target device (parameters, MIDI mapping, programs) from a configuration file, so it can control other hardware.

Features include a global MIDI channel setting and program change support.

Available formats:

| Format     | Platforms             |
|------------|-----------------------|
| VST3       | Windows, macOS, Linux |
| CLAP       | Windows, macOS, Linux |
| Standalone | Windows, macOS, Linux |
| AU, AUv3   | macOS only            |

Built with [JUCE](https://juce.com/) and based on the [Pamplejuce](https://github.com/sudara/pamplejuce) template.

## Prerequisites

- [Git](https://git-scm.com/)
- [CMake](https://cmake.org/download/) 3.25 or newer
- A C++20-capable compiler (JUCE requirements apply):
  - **Windows:** Visual Studio 2022 (or Build Tools) with the "Desktop development with C++" workload
  - **macOS:** Xcode and its command line tools
  - **Linux:** GCC or Clang, plus the JUCE Linux dependencies (see the [JUCE docs](https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md))
- Network access on the first configure, since CPM downloads some dependencies

### Installing the compiler on Windows

Installing "Build Tools" alone is not enough: it installs no compiler until the C++ workload is added.

1. Install the Visual Studio 2022 Build Tools, for example with winget:

   ```
   winget install Microsoft.VisualStudio.2022.BuildTools
   ```

   (or download them from [visualstudio.microsoft.com](https://visualstudio.microsoft.com/downloads/), under "Tools for Visual Studio").

2. Add the C++ workload. Either:
   - open **Visual Studio Installer**, click **Modify** on "Build Tools 2022", tick **Desktop development with C++** and install; or
   - from an elevated PowerShell:

     ```
     & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\setup.exe" modify `
       --installPath "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools" `
       --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended --passive
     ```

   This installs MSVC, the Windows SDK and the CMake integration.

3. Verify: open **Developer PowerShell for VS 2022** from the Start menu and run `cl`. It should print the compiler version banner.

When typing the CMake commands below yourself, always do it from that Developer PowerShell (or the "x64 Native Tools Command Prompt"), so the compiler is on the PATH. If you build from VS Code instead (see [Building from VS Code](#building-from-vs-code)), you don't need it.

## Building

1. Clone the repository with its submodules (JUCE, the CMake includes, melatonin_inspector and clap-juce-extensions):

   ```
   git clone --recurse-submodules https://github.com/zeebyezon/zeeedit.git
   ```

   If you already cloned without `--recurse-submodules`:

   ```
   git submodule update --init --recursive
   ```

2. Configure the project, from the repository root. On Windows, use a Developer PowerShell / x64 Native Tools prompt for VS 2022, and pass the generator and platform as below (a bare `cmake ..` may pick NMake or a 32-bit compiler).

   ```
   cmake -B cmake-build -G "Visual Studio 17 2022" -A x64
   ```

   On macOS or Linux, you can use Ninja or the default generator:

   ```
   cmake -B cmake-build -G Ninja -DCMAKE_BUILD_TYPE=Debug
   ```

3. Build:

   ```
   cmake --build cmake-build --config Debug
   ```

   Use `--config Release` for an optimized build.

The first configure downloads dependencies and the first build compiles JUCE, so both can take a while. The plugins are produced in `cmake-build/ZeeEditProphet5_artefacts/`, in one folder per configuration and format (for example `Debug/VST3/`).

### Building from VS Code

Instead of typing the commands in a Developer PowerShell, you can build from VS Code with the CMake Tools extension, which sets up the compiler environment itself.

1. Install the **CMake Tools** extension (publisher: Microsoft): press `Ctrl+Shift+X`, search for "CMake Tools" and click Install. The Microsoft **C/C++** extension is also recommended.
2. Select the compiler kit: press `Ctrl+Shift+P`, run **CMake: Select a Kit** and choose **Visual Studio Build Tools 2022 Release - amd64**. Choose the amd64 kit so the build is 64-bit. If the kit is not listed, run **CMake: Scan for Kits** first. You can also click the kit name in the status bar.
3. The project settings in `.vscode/settings.json` are picked up automatically. They build into `cmake-build/` (one folder for all configurations), use the Visual Studio 2022 generator, and don't copy the plugin after build:

   ```json
   {
     "cmake.buildDirectory": "${workspaceFolder}/cmake-build",
     "cmake.generator": "Visual Studio 17 2022",
     "cmake.configureArgs": ["-DZEEEDIT_COPY_PLUGIN_AFTER_BUILD=OFF"]
   }
   ```

4. Run **CMake: Configure**, then **CMake: Build** (`F7`). The plugins end up in `cmake-build/ZeeEditProphet5_artefacts/<config>/`.

To switch between Debug and Release, press `Ctrl+Shift+P` and run **CMake: Select Variant**. (The current variant is normally shown in the status bar, but the entry may be hidden; the command always works.) Both configurations share the same `cmake-build/` folder.

### Plugin copy directory

By default the built plugin is not copied anywhere. To copy it after each build, configure with `-DZEEEDIT_COPY_PLUGIN_AFTER_BUILD=ON`. It then goes to JUCE's standard plugin folders (on Windows, the VST3 one is `C:\Program Files\Common Files\VST3`, which may need administrator rights). To copy the VST/VST3 plugin to another folder, also set `ZEEEDIT_VST_COPY_DIR`:

```
cmake -B cmake-build -G "Visual Studio 17 2022" -A x64 -DZEEEDIT_COPY_PLUGIN_AFTER_BUILD=ON -DZEEEDIT_VST_COPY_DIR="D:/Studio/VST-mine"
```

### CLAP

The CLAP target is excluded from the default build. Build it explicitly with the `ZeeEditProphet5_CLAP` target.

## Tests

> **Note that there is no test for the moment**

Unit tests use Catch2 and are in the `tests/` folder. Build and run the `Tests` target:

```
cmake --build cmake-build --config Debug --target Tests
```

Then run the tests:

```
ctest --test-dir cmake-build -C Debug
```

## Usage

Load the plugin as a MIDI effect in your DAW, route its MIDI output to the Prophet-5 (via your MIDI interface), and set the MIDI channel to match the synth. Moving the plugin's parameters then sends the matching MIDI messages to the hardware.

The plugin also receives messages from the hardware, so turning a knob on the synth updates the plugin's parameters. For this to work, the MIDI input corresponding to which the synth is connected must be routed to the plugin's track, and in most DAWs that track must be armed for recording (the exact behavior depends on the DAW).
