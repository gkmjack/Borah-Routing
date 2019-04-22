# borah-routing
ECE-6133 final project


Authors: Kaiming Guan, Ethan Lyons

This part of the program implements Borah routing and generates the final MST tree.
The intermediate and final state of the tree is dumped into output files, and can be displayed using another MATLAB script provided in the display_script folder.

Usage:
1. cd into the 'code' directory
2. type 'make'
3. Drag ./borah.out along with input files into a same folder.
4. type './borah.out [INPUT_FILE]'
5. The intermediate stage after insertion of each Steiner point will be stored in ./outputs/ directory.
