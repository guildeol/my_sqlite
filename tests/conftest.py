import pytest
import subprocess
import os
import time
import select

@pytest.fixture
def shell():
    # Path to the SQLite CLI executable
    sqlite_cli_path = os.path.join(os.path.dirname(__file__), '../build/my_sqlite')
    if not os.path.exists(sqlite_cli_path):
        raise FileNotFoundError(f"SQLite CLI not found at {sqlite_cli_path}")

    process = subprocess.Popen([sqlite_cli_path, "--no-prompt"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    class SQLiteShell:
        def __init__(self, process):
            self.process = process
            self.commands = ""

        def run(self, script):
            # Ensure the program will terminate
            script += ".exit" + "\n"
            output = self.process.communicate(input=script.encode('utf-8'), timeout=1)

            return output[0].decode('utf-8').splitlines()

        def close(self):
            self.process.terminate()

    shell = SQLiteShell(process)
    yield shell
    shell.close()
