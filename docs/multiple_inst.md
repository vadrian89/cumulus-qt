---
layout: default
title: Cumulus Multiple Instances
---

# Cumulus QT
# For multiple instances:
1. make a new shell script with the following content updated for Cumulus installation path  
```
#!/bin/bash
<path-to-cumulus-executable> -i <whatever-you-want>
# e.g. /home/user/Cumulus/Cumulus -i NewInstance
```
2. Make the shell script executable
```
$sudo chmod +x <script_name>
```
