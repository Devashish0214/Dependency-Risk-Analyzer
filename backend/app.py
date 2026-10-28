
from pathlib import Path
import subprocess
import threading

from flask import Flask, jsonify, request, send_from_directory


# --------------------------------------------------
# PROJECT PATHS AND CONFIGURATION
# --------------------------------------------------

ROOT = Path(__file__).resolve().parent.parent
FRONTEND_DIR = ROOT / "frontend"
DATA_DIR = ROOT / "data"

ANALYZER = ROOT / "analyzer.exe"
DEPENDENCIES_FILE = DATA_DIR / "dependencies.txt"
PACKAGES_FILE = DATA_DIR / "osv_packages.txt"

app = Flask(__name__)

# Limit the total uploaded request size to 2 MB.
app.config["MAX_CONTENT_LENGTH"] = 2 * 1024 * 1024

# Prevent simultaneous analyses from overwriting input files.
analysis_lock = threading.Lock()


# --------------------------------------------------
# FRONTEND ROUTES
# --------------------------------------------------

@app.get("/")
def home():
    """Serve the main dashboard page."""
    return send_from_directory(FRONTEND_DIR, "index.html")


@app.get("/frontend/<path:filename>")
def frontend_file(filename):
    """Serve frontend CSS, JavaScript, and other assets."""
    return send_from_directory(FRONTEND_DIR, filename)


# --------------------------------------------------
# DEPENDENCY FILE VALIDATION
# --------------------------------------------------

def validate_dependency_text(content):
    """
    Validate dependency relationships.

    Expected format:
        Project React
        Project Express
        React Axios
    """
    valid_lines = 0

    for line_number, line in enumerate(content.splitlines(), start=1):
        parts = line.split()

        # Allow blank lines.
        if not parts:
            continue

        # Each relationship must contain exactly two names.
        if len(parts) != 2:
            raise ValueError(
                f"dependencies: invalid line {line_number}. "
                "Each line must contain exactly two names. "
                "Example: Project React"
            )

        valid_lines += 1

    # Reject empty files and files containing only blank lines.
    if valid_lines == 0:
        raise ValueError(
            "dependencies: the file contains no valid dependency "
            "relationships."
        )


# --------------------------------------------------
# OSV PACKAGE FILE VALIDATION
# --------------------------------------------------

def validate_package_text(content):
    """
    Validate package entries used for OSV vulnerability queries.

    Expected format:
        npm lodash 4.17.20
        npm axios 0.21.0
        npm express 4.17.1
    """
    valid_lines = 0

    for line_number, line in enumerate(content.splitlines(), start=1):
        parts = line.split()

        # Allow blank lines.
        if not parts:
            continue

        # Each package entry must contain three fields.
        if len(parts) != 3:
            raise ValueError(
                f"packages: invalid line {line_number}. "
                "Each line must contain exactly three fields: "
                "ecosystem, package name, and version. "
                "Example: npm lodash 4.17.20"
            )

        ecosystem, package_name, version = parts

        # Reject missing or whitespace-only values.
        if not ecosystem or not package_name or not version:
            raise ValueError(
                f"packages: invalid line {line_number}. "
                "Ecosystem, package name, and version are required."
            )

    # Reject empty files and files containing only blank lines.
        valid_lines += 1

    if valid_lines == 0:
        raise ValueError(
            "packages: the file contains no valid package entries."
        )


# --------------------------------------------------
# UPLOADED FILE READING
# --------------------------------------------------

def read_uploaded_text(field_name):
    """
    Read an optional uploaded .txt file.

    Returns None when the user does not upload a file.
    Raises ValueError for invalid uploads.
    """
    uploaded = request.files.get(field_name)

    # No file selected: use the existing project file.
    if uploaded is None or not uploaded.filename:
        return None

    # Only accept .txt files.
    if not uploaded.filename.lower().endswith(".txt"):
        raise ValueError(
            f"{field_name}: upload a .txt file."
        )

    # Read the uploaded file.
    content = uploaded.read()

    # Reject empty files.
    if not content:
        raise ValueError(
            f"{field_name}: the file is empty."
        )

    # Require UTF-8 text.
    try:
        return content.decode("utf-8")
    except UnicodeDecodeError:
        raise ValueError(
            f"{field_name}: the file must use UTF-8 text."
        )


# --------------------------------------------------
# BACKEND HEALTH CHECK
# --------------------------------------------------

@app.get("/api/health")
def health():
    """Report whether the backend and required files are available."""
    return jsonify({
        "status": "ok",
        "analyzer_exists": ANALYZER.is_file(),
        "dependencies_file_exists": DEPENDENCIES_FILE.is_file(),
        "packages_file_exists": PACKAGES_FILE.is_file()
    })


# --------------------------------------------------
# MAIN ANALYSIS ROUTE
# --------------------------------------------------

@app.post("/api/analyze")
def analyze():
    """Validate inputs, run the C++ analyzer, and return its report."""

    # Ensure the compiled C++ executable exists.
    if not ANALYZER.is_file():
        return jsonify({
            "error": (
                "analyzer.exe was not found. "
                "Compile the C++ project first."
            )
        }), 500

    # Read and validate uploads before changing any original files.
    try:
        dependencies_text = read_uploaded_text("dependencies")
        packages_text = read_uploaded_text("packages")

        if dependencies_text is not None:
            validate_dependency_text(dependencies_text)

        if packages_text is not None:
            validate_package_text(packages_text)

    except ValueError as error:
        return jsonify({
            "error": str(error)
        }), 400

    # Only one request can replace the input files at a time.
    with analysis_lock:
        originals = {}

        try:
            # Map each optional upload to its corresponding input file.
            upload_targets = (
                (DEPENDENCIES_FILE, dependencies_text),
                (PACKAGES_FILE, packages_text),
            )

            # Back up original files before applying uploads.
            for path, content in upload_targets:
                if content is None:
                    continue

                if not path.is_file():
                    return jsonify({
                        "error": (
                            f"Required input file not found: {path.name}"
                        )
                    }), 500

                originals[path] = path.read_bytes()

                path.write_text(
                    content,
                    encoding="utf-8"
                )

            # Execute the C++ analyzer.
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

            # Collect standard output and any error output.
            report = result.stdout

            if result.stderr:
                report += "\n\n--- STDERR ---\n" + result.stderr

            # Report a failed C++ execution.
            if result.returncode != 0:
                return jsonify({
                    "error": "The C++ analyzer exited with an error.",
                    "return_code": result.returncode,
                    "report": report
                }), 500

            # Return the completed analysis report.
            return jsonify({
                "success": True,
                "report": report,
                "message": "C++ analysis completed."
            })

        except subprocess.TimeoutExpired:
            return jsonify({
                "error": (
                    "Analysis timed out after 180 seconds. "
                    "Check your network and OSV API connection."
                )
            }), 504

        except OSError as error:
            return jsonify({
                "error": f"Could not run the analyzer: {error}"
            }), 500

        finally:
            # Restore every original file changed for this request.
            for path, content in originals.items():
                try:
                    path.write_bytes(content)
                except OSError as error:
                    app.logger.exception(
                        "Could not restore input file %s: %s",
                        path.name,
                        error
                    )


# --------------------------------------------------
# UPLOAD SIZE ERROR HANDLER
# --------------------------------------------------

@app.errorhandler(413)
def file_too_large(_error):
    """Return a readable error when the upload limit is exceeded."""
    return jsonify({
        "error": (
            "The combined upload size exceeds the 2 MB limit."
        )
    }), 413


# --------------------------------------------------
# START THE FLASK SERVER
# --------------------------------------------------

if __name__ == "__main__":
    print("Dependency Risk Analyzer web backend")
    print("Open http://127.0.0.1:5000 in your browser")

    app.run(
        host="127.0.0.1",
        port=5000,
        debug=False
    )
