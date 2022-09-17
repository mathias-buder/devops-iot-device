- [Introduction](#introduction)
  - [Full Development Cycle](#full-development-cycle)
- [System Architecture](#system-architecture)
  - [Development Board](#development-board)
- [Software Architecture](#software-architecture)
  - [Source Code Structure](#source-code-structure)
  - [Data structure Layout](#data-structure-layout)
- [Simulation Environment Architecture](#simulation-environment-architecture)

# Introduction

## Full Development Cycle


```mermaid
flowchart LR
    subgraph Development Environment
    direction LR
        A[Start] --> B((Software\ndevelopment ))
        B --> |SIL Build| B1
        B1((Test/Validate\nin SIL Env))
        B2((Log file\nserver)) --> B1
        B1 --> |Improve| B
        B --> |Push/merge to\nmaster branch| C((CI\nBuild and Test))
        C --> |Fix/Improve| B
        C --> |Deploy| E((Version server))
    end

    subgraph Embedded Target
    direction LR
        D1[Start] --> D2((Init))
        D2 --> |Check for software version| E
        D2 --> |No new software version| D3((Run cycle))
        D2 --> |New software available| D4((Update\nSoftware))
        D4 --> D3
        D3 --> D5((Write log file\n to local storage))
        D5 --> D3
        D5 --> D6((Exec\nfinished))
        D6 --> |Upload log files on\ninternal availability| B2
    end
```


# System Architecture

<p align="center"><img src='images/system_architecture.png' width='80%'></p>

## Development Board
<p align="center"><img src='images/dev_board.png' width='80%'></p>


# Software Architecture
## Source Code Structure
## Data structure Layout

# Simulation Environment Architecture

