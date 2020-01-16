# CTF Configuration Walkthrough

[![Open in Cloud Shell](https://gstatic.com/cloudssh/images/open-btn.png)](https://console.cloud.google.com/cloudshell/open?git_repo=https://github.com/sirdarckcat/35c3ctf_chals&tutorial=kubernetes/walkthrough.md)

# Step 1 - Configure the CTF
## First, enable the necessary GCP features
1. <walkthrough-project-billing-setup>Select a project and enable billing.</walkthrough-project-billing-setup>
1. <walkthrough-enable-apis apis="compute.googleapis.com">Enable the compute API.</walkthrough-enable-apis>

## Then, configure the project
Modify the `cluster_vars` file and edit the `PROJECT` variable to point to the right GCP project name.
<walkthrough-editor-select-regex filePath="35c3ctf_chals/kubernetes/cluster_vars" regex="espr-k8s">open file</walkthrough-editor-select-regex>

## And finally, create the cluster
Run ```./create_cluster.sh``` - this only needs to be done once per CTF.

# Step 2 - Create a challenge
## First, call create_challenge.sh to copy the skeleton
Run ```. create_challenge.sh demo-challenge``` - this should be done for every challenge once.
## Then, create the image
Run ```./create_image.sh demo-challenge``` - this only needs to be done if you want to build the challenge, **but not to deploy it.**
## Then, deploy it
Run ```./deploy.sh demo-challenge``` - this builds and deploys the challenge, **but doesn't expose it to the internet.**
## And finally, expose it to the internet
Run ```./expose.sh demo-challenge``` - this exposes the challenge to the internet **(must be deployed first)**.

# Step 3 - Test the challenge
## You can just connect to the challenge now
1. Run ```nc $(cat demo-challenge/ip) 1``` - this connects you to the challenge.
## To add a proof of work, just edit pow.yaml
1. Edit <walkthrough-editor-select-regex filePath="35c3ctf_chals/kubernetes/demo-challenge/pow.yaml" regex="0">pow.yaml</walkthrough-editor-select-regex> and change the 0 to 1, then run ```./update.sh demo-challenge``` - this enables the proof of work.
## Test the pow
1. Run ```nc $(cat demo-challenge/ip) 1``` - this connects you to the challenge with a proof of work in front.

# Step 4 - Clean up
## Let's just clean the challenge
1. Run ```./kill_challenge.sh demo-challenge``` - this kills the challenge.
## And check it's actually down
1. Run ```nc $(cat demo-challenge/ip) 1``` - this will not work anymore.
