#!/usr/bin/env bash
docker rmi hw1_run:temp
docker build -t hw1_run:temp . && docker run --rm hw1_run:temp

