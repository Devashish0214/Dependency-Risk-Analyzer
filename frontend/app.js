
const form = document.getElementById("analysis-form");
const analyzeButton = document.getElementById("analyze-button");
const messageBox = document.getElementById("message");
const connectionStatus = document.getElementById("connection-status");
const resultStatus = document.getElementById("result-status");
const reportOutput = document.getElementById("full-report");
const copyButton = document.getElementById("copy-report");

const dependenciesInput = document.getElementById("dependencies-file");
const packagesInput = document.getElementById("packages-file");

let latestReport = "";

function showMessage(text, type = "info") {
    messageBox.textContent = text;
    messageBox.className = `message visible ${type}`;
}

function setMetric(id, value) {
    document.getElementById(id).textContent = String(value);
}
function clearResults() {
    // Clear the previous analysis report.
    latestReport = "";
    reportOutput.textContent = "No analysis results available.";

    // Reset metrics so old results are not shown after a failure.
    setMetric("metric-packages", "—");
    setMetric("metric-vulnerabilities", "—");
    setMetric("metric-failed", "—");
    setMetric("metric-successful", "—");

    // Clear the previous package risk table.
    const table = document.getElementById("risk-table");
    table.replaceChildren();

    const row = document.createElement("tr");
    const cell = document.createElement("td");
    cell.colSpan = 5;
    cell.className = "empty-state";
    cell.textContent = "Run a successful analysis to view package risks.";
    row.appendChild(cell);
    table.appendChild(row);

    // Disable copying because there is no valid report.
    copyButton.disabled = true;
}
function parseInteger(text) {
    const match = text.match(/:\s*(\d+)/);
    return match ? Number(match[1]) : 0;
}

function parseReport(report) {
    const lines = report.split(/\r?\n/);

    const result = {
        packages: 0,
        vulnerabilities: 0,
        failed: 0,
        successful: 0,
        rows: []
    };

    // Read package-level OSV results from the actual C++ output.
    for (let i = 0; i < lines.length; i++) {
        const packageMatch = lines[i].match(/^Package:\s*(.+)$/);

        if (packageMatch) {
            const name = packageMatch[1].trim();
            let version = "—";
            let count = 0;
            let queryStatus = "";

            for (let j = i + 1; j < Math.min(i + 12, lines.length); j++) {
                if (/^Package:\s*/.test(lines[j])) break;

                const versionMatch = lines[j].match(/^Version:\s*(.+)$/);
                const countMatch = lines[j].match(
                    /^Matching vulnerability records:\s*(\d+)/
                );
                const statusMatch = lines[j].match(/^Query status:\s*(.+)$/);

                if (versionMatch) version = versionMatch[1].trim();
                if (countMatch) count = Number(countMatch[1]);
                if (statusMatch) queryStatus = statusMatch[1].trim();
            }

            result.packages++;

            if (queryStatus.toUpperCase() === "SUCCESS") {
                result.successful++;
                result.vulnerabilities += count;
            } else if (queryStatus) {
                result.failed++;
            }

            result.rows.push({
                name,
                version,
                vulnerabilities: count,
                queryStatus
            });
        }
    }

    // Extract risk scores from the aligned risk-report section.
    const riskHeading = lines.findIndex(
        line => line.includes("========== RISK REPORT ==========")
    );

    if (riskHeading !== -1) {
        for (let i = riskHeading + 1; i < lines.length; i++) {
            const line = lines[i].trim();

            if (!line || /^-+$/.test(line)) continue;
            if (line.startsWith("Risk formula:")) break;

            if (/^Package\s+Version\s+Vulnerabilities/.test(line)) continue;

            const match = line.match(
                /^(\S+)\s+(\S+)\s+(\d+)\s+(\d+(?:\.\d+)?)\s+(\d+(?:\.\d+)?)$/
            );

            if (match) {
                const [, name, version, count, risk, trust] = match;
                const existing = result.rows.find(
                    row => row.name.toLowerCase() === name.toLowerCase()
                );

                if (existing) {
                    existing.version = version;
                    existing.vulnerabilities = Number(count);
                    existing.risk = Number(risk);
                    existing.trust = Number(trust);
                } else {
                    result.rows.push({
                        name,
                        version,
                        vulnerabilities: Number(count),
                        risk: Number(risk),
                        trust: Number(trust)
                    });
                }
            }
        }
    }

    // Prefer the analyzer's explicit query totals when available.
    const processedLine = lines.find(line =>
        line.startsWith("OSV packages processed:")
    );
    const failedLine = lines.find(line =>
        line.startsWith("Failed queries:")
    );
    const successfulLine = lines.find(line =>
        line.startsWith("Successful queries:")
    );

    if (processedLine) result.packages = parseInteger(processedLine);
    if (failedLine) result.failed = parseInteger(failedLine);
    if (successfulLine) result.successful = parseInteger(successfulLine);

    return result;
}

function renderResults(data) {
    setMetric("metric-packages", data.packages);
    setMetric("metric-vulnerabilities", data.vulnerabilities);
    setMetric("metric-failed", data.failed);
    setMetric("metric-successful", data.successful);

    const table = document.getElementById("risk-table");
    table.replaceChildren();

    if (data.rows.length === 0) {
        const row = document.createElement("tr");
        const cell = document.createElement("td");
        cell.colSpan = 5;
        cell.className = "empty-state";
        cell.textContent =
            "No package rows could be parsed. Review the full report below.";
        row.appendChild(cell);
        table.appendChild(row);
        return;
    }

    for (const item of data.rows) {
        const row = document.createElement("tr");

        const values = [
            item.name,
            item.version,
            item.vulnerabilities,
            item.risk === undefined ? "N/A" : item.risk.toFixed(1),
            item.trust === undefined ? "N/A" : item.trust.toFixed(1)
        ];

        values.forEach((value, index) => {
            const cell = document.createElement("td");
            cell.textContent = String(value);

            if (index === 0) cell.className = "package-name";

            if (index === 3 && item.risk !== undefined) {
                cell.classList.add("score");
                cell.classList.add(
                    item.risk >= 70
                        ? "score-high"
                        : item.risk >= 40
                            ? "score-medium"
                            : "score-low"
                );
            }

            if (index === 4 && item.trust !== undefined) {
                cell.classList.add("score");
                cell.classList.add(
                    item.trust < 40
                        ? "score-high"
                        : item.trust < 70
                            ? "score-medium"
                            : "score-low"
                );
            }

            row.appendChild(cell);
        });

        table.appendChild(row);
    }
}

async function checkBackend() {
    try {
        const response = await fetch("/api/health");
        if (!response.ok) throw new Error("Health check failed");

        const health = await response.json();

        if (
            health.status === "ok" &&
            health.analyzer_exists &&
            health.dependencies_file_exists &&
            health.packages_file_exists
        ) {
            connectionStatus.textContent = "Backend connected";
        } else {
            connectionStatus.textContent = "Check backend files";
        }
    } catch {
        connectionStatus.textContent = "Backend unavailable";
    }
}

dependenciesInput.addEventListener("change", () => {
    document.getElementById("dependencies-name").textContent =
        dependenciesInput.files[0]?.name || "Using existing project file";
});

packagesInput.addEventListener("change", () => {
    document.getElementById("packages-name").textContent =
        packagesInput.files[0]?.name || "Using existing project file";
});

form.addEventListener("submit", async event => {
    event.preventDefault();

    // Remove results from the previous analysis.
    clearResults();

    analyzeButton.disabled = true;
    analyzeButton.textContent = "Analyzing...";
    resultStatus.textContent = "Analysis in progress";
    showMessage(
        "Running the C++ analyzer. OSV requests may take some time.",
        "info"
    );

    reportOutput.textContent = "Analysis in progress...";
    copyButton.disabled = true;

    const formData = new FormData();

    if (dependenciesInput.files.length > 0) {
        formData.append("dependencies", dependenciesInput.files[0]);
    }

    if (packagesInput.files.length > 0) {
        formData.append("packages", packagesInput.files[0]);
    }

    try {
        const response = await fetch("/api/analyze", {
            method: "POST",
            body: formData
        });

        const data = await response.json();

        if (!response.ok || !data.success) {
            throw new Error(data.error || "Analysis failed.");
        }

        latestReport = data.report || "";
        reportOutput.textContent =
            latestReport || "The analyzer returned an empty report.";

        const parsed = parseReport(latestReport);
        renderResults(parsed);

        resultStatus.textContent = "Analysis completed";
        showMessage(
            "Analysis completed. Results below come from the C++ analyzer output.",
            "success"
        );
        copyButton.disabled = !latestReport;
    } catch (error) {
        resultStatus.textContent = "Analysis failed";
        reportOutput.textContent =
            error.message || "Could not complete analysis.";
        showMessage(
            error.message || "Could not connect to the backend.",
            "error"
        );
    } finally {
        analyzeButton.disabled = false;
        analyzeButton.innerHTML = 'Run analysis <span>→</span>';
    }
});

copyButton.addEventListener("click", async () => {
    if (!latestReport) return;

    try {
        await navigator.clipboard.writeText(latestReport);
        showMessage("Full report copied to clipboard.", "success");
    } catch {
        showMessage(
            "Clipboard access was denied. Select and copy the report manually.",
            "error"
        );
    }
});

checkBackend();
