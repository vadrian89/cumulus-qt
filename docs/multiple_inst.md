---
layout: default
title: Cumulus Multiple Instances
---

# Cumulus QT
# For multiple instances:
1. make a new shell script with the following content updated for Cumulus installation path  
```
#!/bin/bash
\<path-to-cumulus-executable> \<whatever-you-want>
# e.g. /home/user/Cumulus/Cumulus NewInstance
```
2. Make the shell script executable
```
$sudo chmod +x <script_name>
```
3. **Optional** Start on login

Add the following content to your `/home/<user>/.profile` file:  
```
if [ -f \<path-to-shell-script> ]; then  
  \<path-to-shell-script>  
fi
```
Example:
```  
if [ -f /home/script.sh ]; then  
  /home/script.sh  
fi  
```
This will check if the script exists, so in case you accidentally delete you will not be getting errors on login.
