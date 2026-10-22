
from pathlib import Path
import subprocess
import threading

from flask import Flask, jsonify, request, send_from_directory

ROOT = Path(__file__).resolve().parent.parent
FRONTEND_DIR = ROOT / "frontend"
DATA_DIR = ROOT / "data"
ANALYZER = ROOT / "analyzer.exe"

DEPENDENCIES_FILE = DATA_DIR / "dependencies.txt"
PACKAGES_FILE = DATA_DIR / "osv_packages.txt"

app = Flask(__name__)
app.config["MAX_CONTENT_LENGTH"] = 2 * 1024 * 1024

# Prevent two requests from changing the input files simultaneously.
analysis_lock = threading.Lock()


@app.get("/")
def home():
    return send_from_directory(FRONTEND_DIR, "index.html")


@app.get("/frontend/<path:filename>")
def frontend_file(filename):
    return send_from_directory(FRONTEND_DIR, filename)


def read_uploaded_text(field_name):
    uploaded = request.files.get(field_name)

    if uploaded is None or not uploaded.filename:
        return None

    if not uploaded.filename.lower().endswith(".txt"):
        raise ValueError(f"{field_name}: upload a .txt file.")

    content = uploaded.read()

    if not content:
        raise ValueError(f"{field_name}: the file is empty.")

    try:
        return content.decode("utf-8")
    except UnicodeDecodeError:
        raise ValueError(f"{field_name}: the file must use UTF-8 text.")


@app.get("/api/health")
def health():
    return jsonify({
        "status": "ok",
        "analyzer_exists": ANALYZER.is_file(),
        "dependencies_file_exists": DEPENDENCIES_FILE.is_file(),
        "packages_file_exists": PACKAGES_FILE.is_file()
    })


@app.post("/api/analyze")
def analyze():
    if not ANALYZER.is_file():
        return jsonify({
            "error": "analyzer.exe was not found. Compile the C++ project first."
        }), 500

    try:
        dependencies_text = read_uploaded_text("dependencies")
        packages_text = read_uploaded_text("packages")
    except ValueError as error:
        return jsonify({"error": str(error)}), 400

    with analysis_lock:
        originals = {}

        try:
            # Back up original files before using optional uploads.
            for path, content in (
                (DEPENDENCIES_FILE, dependencies_text),
                (PACKAGES_FILE, packages_text),
            ):
                if content is not None:
                    if not path.is_file():
                        return jsonify({
                            "error": f"Required input file not found: {path.name}"
                        }), 500

                    originals[path] = path.read_bytes()
                    path.write_text(content, encoding="utf-8")

            result = subprocess.run(
                [str(ANALYZER)],
                cwd=str(ROOT),
                capture_output=True,
                text=True,
                encoding="utf-8",
                errors="replace",
                timeout=180,
                check=False,
                shell=False
            )

            report = result.stdout
            if result.stderr:
                report += "\n\n--- STDERR ---\n" + result.stderr

            if result.returncode != 0:
                return jsonify({
                    "error": "The C++ analyzer exited with an error.",
                    "return_code": result.returncode,
                    "report": report
                }), 500

            return jsonify({
                "success": True,
                "report": report,
                "message": "C++ analysis completed."
            })

        except subprocess.TimeoutExpired:
            return jsonify({
                "error": "Analysis timed out after 180 seconds. Check your network and OSV API connection."
            }), 504

        except OSError as error:
            return jsonify({
                "error": f"Could not run the analyzer: {error}"
            }), 500

        finally:
            # Restore every input file changed for this request.
            for path, content in originals.items():
                path.write_bytes(content)


@app.errorhandler(413)
def file_too_large(_error):
    return jsonify({
        "error": "The combined upload size exceeds the 2 MB limit."
    }), 413


if __name__ == "__main__":
    print("Dependency Risk Analyzer web backend")
    print("Open http://127.0.0.1:5000 in your browser")
    app.run(host="127.0.0.1", port=5000, debug=False)
