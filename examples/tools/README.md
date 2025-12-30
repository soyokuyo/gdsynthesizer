# Tools

This directory contains utility tools for working with GDSYNTHESIZER examples.

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

