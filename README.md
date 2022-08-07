# Bandwidth minimization

Project for Computational Intelligence course.

Implementation of algorithms for reducing the graph bandwidth.

Comparison of different algorithms and visualization of results.

## Usage

After cloning the project, go to project folder and type:
```
make
```
To run the program use:
```
./simulated_annealing filename
```
**Note:** You can only use files in the internal format as input (these files are located in `data/hb` and `data/generated`).
To use files in Market Matrix format (.mtx), you must first convert them using the `save_to_internal_format` function
from the Jupyter Notebook file.
