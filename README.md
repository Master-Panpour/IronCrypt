# IronCrypt
Hackathon Preparation!!
# Cybersecurity Employee Activity Monitor

A lightweight C-based tool that analyzes login activity for suspicious behavior on Linux or Windows systems.

## Features
- Detects abnormal login hours (1 AM - 4 AM)
- Flags users accessing too many files
- Parses `/var/log/auth.log` (Linux) or Windows Event Log (ID 4624)

## Compile & Run

```bash
make
sudo ./monitor