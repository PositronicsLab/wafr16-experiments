#!/bin/bash
$1/moby-regress classic-01.setup classic-01.xml classic-01.output
$1/moby-regress classic-001.setup classic-001.xml classic-001.output
$1/moby-regress classic-0001.setup classic-0001.xml classic-0001.output
$1/moby-regress classic-0005.setup classic-0005.xml classic-0005.output
$1/moby-regress classic-00001.setup classic-00001.xml classic-00001.output

