# Personal Hyperscan Utils

## Platform

x86_64 RHEL7.3 GCC4.8.5 CMake2.8 [Hyperscan5.0.0](https://github.com/intel/hyperscan)

**Editor** : `VScode` - newest version,
**Extensions** : `ms-vscode.cpptools` , `twxs.cmake`

## Summary

Personal Utils, encapsulation of hyperscan's commonly-used interfaces.

Include:
* hs_scan, OnMatch
* hs_compile_multi
* hs_serialize_database, hs_deserialize_database
* User Defined:
    + FileParse
    + StringSplit
    + dbFromFile

## Workflow

Correct workflow using this repo:
1. `git clone ...`, checkout to branch `main`(default)
2. create a new branch for your every individual project based on branch main
3. Starting coding in `app/`, and, modify the `CMakeLists.txt` and `src/config.h.in` to define your MACROS
