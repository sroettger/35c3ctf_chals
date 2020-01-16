# Walkthrough
[![Open in Cloud Shell](https://gstatic.com/cloudssh/images/open-btn.png)](https://console.cloud.google.com/cloudshell/open?git_repo=https://github.com/sirdarckcat/35c3ctf_chals&tutorial=kubernetes/walkthrough.md)

# Future Work
 - Automate the setup of `cluster_vars`, and have a command that generates `challenge.yaml`.
 - Cleanup the proof of work to point somewhere that isn't the ccc website.

# Budgetting
 - Calculate the resources and the amount of budget assuming 100% consumption.
 - Add a budget alert at 25% budget, 50% budget, 75% budget and 100% of budget.
 - Plan for the CTF to last 50% longer than it will, increase proof of work on challenges using too much CPU.

# Errors
 - If you forget to update the kubernetes version in `cluster_vars`, you get an error saying that it's not supported.
 - If you forget to update the project name in `challenge-skeleton/challenge.yaml`, you get an error referencing `espr-k8s`
 - If you can't connect to port 1 of the IP address, that's probably because the challenge didnt deploy.

# Debugging
 - Go to https://console.cloud.google.com/kubernetes/list and click on services.
 - Look for the challenge, and click on it to reach the kubernetes service page for the challenge.

## Debugging deployment
 - From the kubernetes service page for the challenge.
 - Under overview, look for the "Stackdriver logs" row and click on the link.

## Debugging challenge
 - From the kubernetes service page for the challenge.
 - Under overview, look for "Service Pods", click on one of them.
 - On the top right corner there's a "KUBECTL" button, click on it, and then on "Exec" and then on the task name.
 - On the Cloud Shell, modify the end of command from `-- ls` to `-it -- /bin/bash` and run `cd home/user`.
