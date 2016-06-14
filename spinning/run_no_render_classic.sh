#!/bin/bash
$1/moby-regress classic-01.setup classic.xml classic-01.output
$1/moby-regress classic-001.setup classic.xml classic-001.output
$1/moby-regress classic-0001.setup classic.xml classic-0001.output
$1/moby-regress classic-0005.setup classic.xml classic-0005.output
$1/moby-regress classic-00001.setup classic.xml classic-00001.output

