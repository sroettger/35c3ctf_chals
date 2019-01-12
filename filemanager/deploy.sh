#!/bin/bash

set -Eeuo pipefail

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

gcloud app deploy --version live --project filemanager

"$DIR/../kubernetes/deploy_bot.sh" "$DIR"

echo 'REMEMBER TO ADD THE FLAG AND UPDATE THE COOKIES!!!!!!!!!!'
