# 35c3ctf_chals

This repo contains both my challenges from the 35C3 CTF as well as my infrastructure scripts to deploy on kubernetes.

== Challenges ==
* filemanager: a web challenge in which you had to abuse Chrome's XSS Auditor as an xs-search side channel
* krautflare: a Chrome exploitation challenge (v8) based on [this bug](https://bugs.chromium.org/p/project-zero/issues/detail?id=1710)
* logrotate: a linux file race in a real tool using an insecure config
* namespaces: a sandbox challenges based on Linux user namespaces

== Infrastructure ==
You can find scripts to create a kubernetes cluster on GCE and deploy/update challenges with an optional proof-of-work in the kubernetes folder. See the [README](kubernetes/README.md) for details on how to use it.

Besides that, there's an XSS Bot based on chrome headless (puppeteer) in [kubernetes/xss-bot](kubernetes/xss-bot). You can check out the [filemanager](filemanager) challenge for an example on how to use it.
