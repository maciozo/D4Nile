#!/usr/bin/env bash

until ./index.js; do
	echo "JS server crashed with exit code $?. Respawning :)">&2
	sleep 1
done