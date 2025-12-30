import json
import os
import sys
import subprocess
from pathlib import Path

from PySide6.QtCore import QThread, Signal
from PySide6.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QHBoxLayout,
    QPushButton, QTextEdit, QFileDialog, QLabel, QProgressBar
)

class ScanWorker(QThread):
    log_line = Signal(str)
    progress = Signal(int, int)   
    hit = Signal(str, str)       
    done = Signal(int)            
    error = Signal(str)

    def __init__(self, exe_path: Path, target_dir: Path):
        super().__init__()
        self.exe_path = exe_path
        self.target_dir = target_dir
        self.proc: subprocess.Popen | None = None

    def run(self):
        if not self.exe_path.exists():
            self.error.emit(f"Scanner binary not found: {self.exe_path}")
            self.done.emit(2)
            return

        cmd = [str(self.exe_path), str(self.target_dir), "--json"]

        try:
            # text=True gives str lines; bufsize=1 enables line-buffered reading.
            self.proc = subprocess.Popen(
                cmd,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                bufsize=1,
            )
        except Exception as e:
            self.error.emit(f"Failed to start scanner: {e}")
            self.done.emit(2)
            return

        assert self.proc.stdout is not None
        for raw in self.proc.stdout:
            line = raw.strip()
            if not line:
                continue

            # Try JSONL first; fall back to plain log lines.
            try:
                obj = json.loads(line)
                event = obj.get("event")
                if event == "progress":
                    self.progress.emit(int(obj.get("scanned", 0)), int(obj.get("total", 0)))
                elif event == "hit":
                    self.hit.emit(str(obj.get("path", "")), str(obj.get("signature", "")))
                else:
                    self.log_line.emit(line)
            except json.JSONDecodeError:
                self.log_line.emit(line)

        rc = self.proc.wait()
        self.done.emit(rc)

    def cancel(self):
        if self.proc and self.proc.poll() is None:
            # Graceful stop first; your C++ should handle SIGTERM/CTRL signal if possible.
            try:
                self.proc.terminate()
            except Exception:
                pass


class MainWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Antivirus Scanner")
        self.resize(900, 600)

        self.worker: ScanWorker | None = None
        self.target_dir: Path | None = None

        root = QVBoxLayout(self)

        # Controls
        controls = QHBoxLayout()
        self.pick_btn = QPushButton("Select folder")
        self.start_btn = QPushButton("Start scan")
        self.cancel_btn = QPushButton("Cancel")
        self.cancel_btn.setEnabled(False)

        controls.addWidget(self.pick_btn)
        controls.addWidget(self.start_btn)
        controls.addWidget(self.cancel_btn)
        root.addLayout(controls)

        self.status = QLabel("No folder selected.")
        root.addWidget(self.status)

        self.pbar = QProgressBar()
        self.pbar.setRange(0, 100)
        self.pbar.setValue(0)
        root.addWidget(self.pbar)

        self.log = QTextEdit()
        self.log.setReadOnly(True)
        root.addWidget(self.log)

        # Wire buttons
        self.pick_btn.clicked.connect(self.pick_folder)
        self.start_btn.clicked.connect(self.start_scan)
        self.cancel_btn.clicked.connect(self.cancel_scan)

        # Find scanner binary relative to repo structure:
        # av_project/gui/app.py -> av_project/build/run_scan(.exe)
        self.scanner_exe = self._resolve_scanner_path()

    def _resolve_scanner_path(self) -> Path:
        project_root = Path(__file__).resolve().parents[1]
        build_dir = project_root / "build"
        exe = "run_scan.exe" if os.name == "nt" else "run_scan"
        return build_dir / exe

    def pick_folder(self):
        folder = QFileDialog.getExistingDirectory(self, "Select folder to scan")
        if folder:
            self.target_dir = Path(folder)
            self.status.setText(f"Selected: {self.target_dir}")
            self.log.append(f"Selected folder: {self.target_dir}")

    def start_scan(self):
        if self.worker is not None:
            self.log.append("Scan already running.")
            return
        if self.target_dir is None:
            self.log.append("Select a folder first.")
            return

        self.pbar.setValue(0)
        self.start_btn.setEnabled(False)
        self.cancel_btn.setEnabled(True)

        self.log.append(f"Starting scanner: {self.scanner_exe}")
        self.worker = ScanWorker(self.scanner_exe, self.target_dir)

        self.worker.log_line.connect(self.log.append)
        self.worker.error.connect(lambda msg: self.log.append(f"[ERROR] {msg}"))
        self.worker.progress.connect(self.on_progress)
        self.worker.hit.connect(self.on_hit)
        self.worker.done.connect(self.on_done)

        self.worker.start()

    def cancel_scan(self):
        if self.worker:
            self.log.append("Cancel requested...")
            self.worker.cancel()

    def on_progress(self, scanned: int, total: int):
        if total > 0:
            pct = int((scanned * 100) / total)
            self.pbar.setValue(max(0, min(100, pct)))
            self.status.setText(f"Scanning... {scanned}/{total}")
        else:
            self.status.setText(f"Scanning... {scanned}")

    def on_hit(self, path: str, signature: str):
        self.log.append(f"[HIT] {path} ({signature})")

    def on_done(self, rc: int):
        self.log.append(f"Scan finished. Exit code: {rc}")
        if rc == 0:
            self.status.setText("Completed: no threats found.")
        elif rc == 1:
            self.status.setText("Completed: threats found.")
        elif rc == 130:
            self.status.setText("Cancelled.")
        else:
            self.status.setText("Completed with errors.")

        self.start_btn.setEnabled(True)
        self.cancel_btn.setEnabled(False)
        self.worker = None


if __name__ == "__main__":
    app = QApplication(sys.argv)
    w = MainWindow()
    w.show()
    sys.exit(app.exec())
