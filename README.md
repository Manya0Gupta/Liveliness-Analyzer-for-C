# Live Variable Analyzer

This project is a simple **Live Variable Analyzer** for a subset of the C programming language, built using **Flex**, **Bison**, and **C**. It performs lexical and syntax analysis, generates an intermediate representation (IR) of the code, performs live variable analysis to detect redundant assignments, and generates a control flow graph (CFG) in DOT format for visualization.

## Features

- Lexical analysis using Flex
- Syntax analysis using Bison
- Symbol table with scoping support
- Intermediate Representation (IR) generation
- Detection of unused (dead) variables and redundant assignments
- Control flow constructs: `if`, `else`, `while`, and compound blocks
- Export of CFG as a DOT file for visualization using Graphviz

## Example Input

```c
int main() {
    int x = 0;
    if (x > 0) {
        int y = 5;
        x = y + 1;
    }
    return x;
}

Example Output
Intermediate Representation:
pgsql
Copy
Edit
Line 2: x = 0
Line 4: y = 5
Line 5: x = y + 1
Line 7: RETURN x
Live Variable Analysis:
pgsql
Copy
Edit
Redundant assignment at line 4: 'y = 5' is never used.
Report written to report.txt
CFG written to cfg.dot
CFG Visualization
You can render the generated cfg.dot file using Graphviz:

bash
Copy
Edit
dot -Tpng cfg.dot -o cfg.png
How to Build
Dependencies
Make sure the following packages are installed:

bash
Copy
Edit
sudo apt update
sudo apt install flex bison build-essential graphviz
Build and Run
Clone the repository and run:

bash
Copy
Edit
make
./analyzer < test.c
Team Members
[Your Name] (Team Lead)

Tanu Gupta

Shreya Yadav

License
This project is licensed under the MIT License.
