import os
import subprocess


def test_readme_exists():
    assert os.path.isfile("README.md")


def test_gitignore_exists():
    assert os.path.isfile(".gitignore")


def test_binaries_not_tracked():
    result = subprocess.run(
        ["git", "ls-files", "celerybeat-schedule.db"],
        capture_output=True, text=True
    )
    assert result.stdout.strip() == "", f"Binary still tracked: {result.stdout.strip()}"


def test_db_files_not_tracked():
    result = subprocess.run(
        ["git", "ls-files", "*.db"],
        capture_output=True, text=True
    )
    assert result.stdout.strip() == "", f"DB files still tracked: {result.stdout.strip()}"
