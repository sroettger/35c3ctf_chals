# CTF Infrastructure Walkthrough

The purpose of this walkthrough is to guide you along the way to configure the infrastructure.

## First, enable the necessary GCP features
Before we start, you need to have billing set up, and the compute API enabled. Let's do that now.
1. <walkthrough-project-billing-setup>Select a project and enable billing.</walkthrough-project-billing-setup>
1. <walkthrough-enable-apis apis="compute.googleapis.com">Enable the compute API.</walkthrough-enable-apis>

## Then, configure the project
In order to continue, you need to modify the `cluster_vars` file and edit the `PROJECT` variable to point to the right GCP project name.

So, <walkthrough-editor-select-regex filePath="35c3ctf_chals/kubernetes/cluster_vars" regex="espr-k8s">open the file</walkthrough-editor-select-regex> and replace "espr-k8s" with
```
{{project-id}}
```

## And finally, create the cluster
Run the following command:
```
./create_cluster.sh
```
This only needs to be done once per CTF. A "cluster" is essentially a group of VMs with a "master" that defines what to run there.

# Step 2 - Create a challenge
Now that we have a cluster setup, we need to create a challenge.

In this walkthrough, you'll learn how to create a challenge called "demo-challenge", build the docker image, deploy it to the cluster, and expose it to the internet.

## First, call create_challenge.sh to copy the skeleton
Run the following command, after this walkthrough, you should do this for every challenge.
```
. create_challenge.sh demo-challenge
```

This will create a directory called demo-challenge, and if you look inside it, you'll find out the challenge details and configuration.

In the next step you'll find out how to create a docker image with the newly created challenge.

## Then, create the image
Run the following command this only needs to be done if you want to build the challenge, **but not to deploy it.**
```
./create_image.sh demo-challenge
```

This creates the "image", which means the docker image. Depending on how complex your challenge will be, this can be very fast or take forever.

After you have created the image, you can run it locally, or just deploy it to your cluster.

## Then, deploy it

Run the following command, this builds and deploys the challenge, **but doesn't expose it to the internet.**

```
./deploy.sh demo-challenge
```

This will deploy the image to your cluster, and will soon be consuming CPU resources. The challenge will automatically scale based on traffic.

## And finally, expose it to the internet
Run the following command, this exposes the challenge to the internet **(must be deployed first)**.

```
./expose.sh demo-challenge
```

This should only be done once the challenge is ready to be released to the public.

# Step 3 - Test the challenge

Now we have a challenge up and running, and we need to test it to make sure it works. In this section of the walkthrough you'll connect to the challenge, add and configure a proof of work, and update the running task.

## You can just connect to the challenge now

Run the following command, this connects you to the challenge.

```
nc $(cat demo-challenge/ip) 1
```

If all went well, you should see a shell. Now, let's add a proof of work to the task to avoid people abusing it too much.

## To add a proof of work, just edit pow.yaml
To add a proof of work, you just need to edit the configuration of the challenge.

Edit <walkthrough-editor-select-regex filePath="35c3ctf_chals/kubernetes/demo-challenge/pow.yaml" regex="0">pow.yaml</walkthrough-editor-select-regex> and change the 0 to 1.

Once that's done,  run
```
./update.sh demo-challenge
```
this enables the proof of work.

Note that this is a very weak proof of work (strength of 1), you probably want to set it to 15, 20, or more for actually asking people to do some work.

Once the challenge is updated, just run:
```
nc $(cat demo-challenge/ip) 1
```

This connects you to the challenge with a proof of work in front.

And that's it, now that you saw how to push a challenge and update it, let's clean up to save some resources.

## Let's just clean the challenge
This is the last step of the walkthrough, and this step you probably want to do at the end of the CTF to save resources.

Run:
```
./kill_challenge.sh demo-challenge
```

You can test this worked by running:
```
nc $(cat demo-challenge/ip) 1
```
