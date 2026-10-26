# Todo Manager

A simple terminal-based Todo Manager written in C++.

## Features

- Create tasks with a name, description, and priority
- Display all tasks with numbered entries
- Update existing tasks
- Delete tasks
- Save and load tasks from a local file

## Requirements

- A C++ compiler supporting C++17

## Build

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp -o todo
```

## Run

```bash
./todo
```

## Storage

Tasks are stored locally in `tasks.txt`. This file is excluded from Git so personal task data is not committed.
