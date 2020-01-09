Codelab

1. Go to https://console.developers.google.com/apis/api/compute.googleapis.com/overview and enable compute + billing.
1. Modify `cluster_vars` to point to the right version of Kubernetes, and the right GCP project name.
1. Run `./create_cluster.sh` - this only needs to be done once per CTF.
1. Modify `challenge-skeleton/challenge.yaml` and modify the image path to point to the right project name.
1. Run `./create_image.sh challenge-skeleton` - this only needs to be done if you want to build the challenge, but not to deploy it.
1. Run `./deploy.sh challenge-skeleton` - this builds and deploys the challenge, but doesn't expose it to the internet.
1. Run `./expose.sh challenge-skeleton` - this exposes the challenge to the internet (does not build it).
1. Run `nc $theip 1` - this connects you to the challenge.
1. Edit `pow.yaml` and change the 0 to 1, then run `./update.sh challenge-skeleton` - this enables the proof of work.
1. Run `nc $theip 1` - this connects you to the challenge with a proof of work in front.

