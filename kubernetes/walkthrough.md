# CTF Configuration Walkthrough

[![Open in Cloud Shell](https://gstatic.com/cloudssh/images/open-btn.png)](https://console.cloud.google.com/cloudshell/open?git_repo=https://github.com/sirdarckcat/35c3ctf_chals&tutorial=kubernetes/walkthrough.md)

# Step 1 - Enable everything
1. Enable [compute + billing](https://console.developers.google.com/apis/api/compute.googleapis.com/overview) in GCP.
<walkthrough-project-setup>pjs</walkthrough-project-setup>
<walkthrough-project-billing-setup>set</walkthrough-project-billing-setup>
<walkthrough-devshell-precreate>pre</walkthrough-devshell-precreate>
<walkthrough-editor-open-file filePath="kubernetes/">open</walkthrough-editor-open-file>

# Step 2 - Configure your project name
1. Modify the `cluster_vars` file and edit the `PROJECT` variable to point to the right GCP project name.
<walkthrough-editor-select-regex filePath="kubernetes/cluster_vars" regex="espr-k8s">sel</walkthrough-editor-select-regex>

# Step 3 - Open Cloud Shell
1. Open Cloud Shell <walkthrough-open-cloud-shell-button>ocs</walkthrough-open-cloud-shell-button>.
1. Run `./create_cluster.sh` - this only needs to be done once per CTF.
1. Run `. create_challenge.sh demo-challenge` - this should be done for every challenge once.
1. Run `./create_image.sh demo-challenge` - this only needs to be done if you want to build the challenge, but not to deploy it.
1. Run `./deploy.sh demo-challenge` - this builds and deploys the challenge, but doesn't expose it to the internet.
1. Run `./expose.sh demo-challenge` - this exposes the challenge to the internet (does not build it).
1. Run `nc $theip 1` - this connects you to the challenge.
1. Edit `pow.yaml` and change the 0 to 1, then run `./update.sh demo-challenge` - this enables the proof of work.
1. Run `nc $theip 1` - this connects you to the challenge with a proof of work in front.
1. Run `./kill_challenge.sh demo-challenge` - this kills the challenge.
1. Run `nc $theip 1` - this will not work anymore.
