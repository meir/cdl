# cdl

This project is a small remake in C of a bash script that i very frequently use.
I just wanted to make this a bit more stable and easier to install and thus converted it into a C program with makefile and an install script.

## What is this?

It is a very simple command to just save directory paths and quickly switch between them.
I use this very often since i got bothered by constantly typing `cd ../../../../folder` and such.
For example, you have the following folder structure:
```
~/Documents
│
├── work
│   │
│   ├── projectA
│   │   ├── src
│   │   └── docs
│   │
│   └── projectB
│       └── src
│
└── personal
    └── cool_project
        ├── src
        └── docs
```

in this situation if you want to navigate from `~/Documents/work/projectA/src` to your personal project, you would either have to do `cd ~/Documents/personal/cool_project` or `cd ../../../personal/cool_project`
what this tool allows is for you to be in `~/Documents/personal/cool_project` and do `cds cool_project` and later once you want to navigate to it from anywhere, you can do `cdl cool_project` instead.

## Requirements

 - gcc compiler

## How to install

To start the installation script just run the following command as root (for bin access)
```
bash <(curl -s https://raw.githubusercontent.com/meir/cdl/main/install.sh)
```

To uninstall you can use the same script and add `uninstall` after it.

## How to use

This project has a few commands, they're simple CRUD commands to change the cache file.
command | usage       | description
--------|-------------|-------------
cdl     | cdl (name)  | change directory to `last` or (name)
cds     | cds (name)  | save directory path on `last` or (name)
cdls    | cdls        | list all saved name + directory paths
cdr     | cdr (name)  | remove directory path from cache by `last` or (name)
cdp     | cdp (name)  | print directory path saved in `last` or (name)

