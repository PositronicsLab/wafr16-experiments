#!/bin/bash
$1/moby-regress adaptive-01.setup adaptive.xml adaptive-01.output
$1/moby-regress adaptive-001.setup adaptive.xml adaptive-001.output
$1/moby-regress adaptive-0005.setup adaptive.xml adaptive-0005.output
$1/moby-regress adaptive-00025.setup adaptive.xml adaptive-00025.output
$1/moby-regress adaptive-0001.setup adaptive.xml adaptive-0001.output
$1/moby-regress adaptive-00001.setup adaptive.xml adaptive-00001.output
#$1/moby-regress adaptive-000001.setup adaptive.xml adaptive-000001.output

