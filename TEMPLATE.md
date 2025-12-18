# Android Native Template – Usage Guide

This repository is a reusable template for Android NativeActivity projects
written entirely in C/C++ (no Java/Kotlin application code).

---

## Initialization script

The file `init-template.sh` converts this template into a new project by
renaming all required identifiers consistently.

### Usage

```bash
./init-template.sh \
  <applicationId> \
  <namespace> \
  <libName> \
  <appLabel> \
  <rootProjectName>
