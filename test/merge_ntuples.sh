#!/bin/sh
hadd $3 $2 `xrdfs root://eoscms.cern.ch ls -u $1 | grep '\.root'`
