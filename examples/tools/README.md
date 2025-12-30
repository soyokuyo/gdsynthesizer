# Tools

This directory contains utility tools for working with GDSYNTHESIZER examples.

## copy_build_artifacts.bat

A Windows batch script that automatically copies build artifacts from `project/bin` to all example project `bin` directories, updating only when the destination files are older or missing.

### Overview

After building GDSYNTHESIZER with `scons`, the compiled libraries (`.dll`, `.wasm`) and configuration files (`.gdextension`) are generated in `project/bin/`. This script automatically distributes these files to all example projects' `bin` directories, ensuring they have the latest build artifacts.

The script uses Windows `robocopy` command to compare file timestamps and only copies files when:
- The destination file does not exist
- The source file is newer than the destination file

### Requirements

- Windows operating system
- `robocopy` command (included with Windows)

### Usage

Run the script from the repository root directory:

```batch
examples\tools\copy_build_artifacts.bat
```

Or from the `examples/tools/` directory:

```batch
copy_build_artifacts.bat
```

The script will:
1. Automatically detect the repository root directory
2. Find all `examples/*/godot_project/bin` directories
3. Copy files from `project/bin/` to each example's `bin` directory
4. Display progress messages for each copied file

### Features

- **Automatic Path Detection**: Automatically finds the repository root and example directories
- **Selective Copying**: Only copies files when destination is older or missing (saves time on subsequent runs)
- **Multiple File Types**: Copies `.gdextension`, `.dll`, and `.wasm` files
- **Multiple Examples**: Processes all example projects in a single run
- **Error Handling**: Checks for source directory existence and handles missing files gracefully

### Example Workflow

1. Build GDSYNTHESIZER:
   ```batch
   scons platform=windows use_mingw=yes target=template_release
   ```

2. Copy build artifacts to examples:
   ```batch
   examples\tools\copy_build_artifacts.bat
   ```

3. The script will output something like:
   ```
   Copying build artifacts from C:\dev\cursor\20251210\gdsynthesizer\project\bin
   
   Processing: C:\dev\cursor\20251210\gdsynthesizer\examples\kuyosynthe\godot_project\bin
     Copied: gdsynthesizer.gdextension
     Copied: libgdsynthesizer.windows.template_release.x86_64.dll
     Copied: libgdsynthesizer.web.template_release.wasm32.wasm
   
   Done.
   ```

### Notes

- The script uses `robocopy` with `/IS /IT` flags to copy files even if they are identical or newer
- Only files that are actually copied will be displayed in the output
- The script will skip example directories that don't have a `bin` subdirectory
- This script is designed for Windows only. For Linux/macOS, you can create a similar shell script using `rsync` or `cp -u`

---

## godot_python_web_server.py

A simple HTTP server script for testing Godot WEB exports locally.

### Overview

When exporting a Godot project to WEB (HTML5), you need to run it through an HTTP server instead of opening the HTML file directly. This is because:
- Godot WEB exports require proper CORS (Cross-Origin Resource Sharing) headers
- SharedArrayBuffer and other advanced features require specific security headers
- Direct file access (`file://`) is restricted by browser security policies

This Python script provides a simple HTTP server with the necessary CORS headers configured for Godot WEB exports.

### Requirements

- Python 3.x

### Usage

1. Navigate to the directory containing your exported Godot WEB project (the directory containing `index.html` and other exported files).

2. Run the script:
   ```bash
   python3 godot_python_web_server.py
   ```
   
   Or specify a custom port:
   ```bash
   python3 godot_python_web_server.py 8080
   ```

3. The server will start on port 8000 (or the specified port) and display a message like:
   ```
   Serving HTTP on 0.0.0.0 port 8000 (http://0.0.0.0:8000/) ...
   ```

4. Open your web browser and navigate to:
   ```
   http://localhost:8000
   ```

### Features

- **CORS Headers**: Automatically sets `Cross-Origin-Opener-Policy: same-origin` and `Cross-Origin-Embedder-Policy: require-corp` headers required for Godot WEB exports
- **Simple**: No additional dependencies required (uses Python standard library)
- **Configurable Port**: Default port is 8000, but can be customized via command-line argument

### Example Workflow

1. Export your Godot project to WEB platform (e.g., `examples/kuyosynthe/godot_project/export/`)
2. Navigate to the export directory:
   ```bash
   cd examples/kuyosynthe/godot_project/export/
   ```
3. Run the server:
   ```bash
   python3 ../../tools/godot_python_web_server.py
   ```
4. Open `http://localhost:8000` in your web browser

### Notes

- The server will serve files from the current working directory
- Press `Ctrl+C` to stop the server
- This is a simple development server and should not be used for production deployments

