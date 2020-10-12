# Demo_graph
This example project demonstrates several features of Transd. The project consists
of C++ and Transd programs working together. The C++ program calls the Transd
program "renderer". The renderer does its job of creating a graph of a function and
stores it in a vector. After the job is done the C++ program obtaines the result vector
and writes it in a PPM image file.     

This project differs from a more simple Demo_tiny project in many aspects and
demonstrates many notable points.

The Transd renderer is called not many times, but only once and does all its job in an
internal loop.

The Transd program could be placed in a single file, but for illustrative purposes is
separated to several semantic parts:

1. The worker loop (in module1), instantiates the renderer object of Quad class and
repeatedly calls its render function.

2. The Quad class definition file which serves as a class library which can be used by
other modules (by module1 in this case). The Quad class implements the renderer of
the quadratic function graph ( f(x) = x^2 ). In the same way also renderers for linear
function, cubic function, etc. could be implemented (or even a ray-tracer renderer of
non-mathematical objects).  

3. The Export module is located in a separate directory and serves for two purposes.
First, it can be considered as a configuration file where users of the program can set
various parameters. Second, this module is the data interface between C++ and
Transd programs: C++ program has the full read/write access to all the data members
of the Export module.

The module import system is observable in the module1 definition. Any module can
request import of other class or module, defined in one of the files listed in the
 assembly description (in the asm.td file). This import request can be denied, however,
 in accordance with access control rules defined in the imported module. But if the
 request is fullfilled, the importing module can instantiate objects of imported classes
 (module1 instantiates an object of the Quad class) and access members of imported
 modules (module1 uses the vector 'v' from the Export module).

Another Transd feature used here is the Access Control System, which is visible in the
declaration of the Export module:

`public module Export: {`

Which declares all the module's contents as accessible from anywhere. For the Export
module this type of access is mandatory, but generally modules can limit access to all
or some of their members and can choose for which other modules they grant or
restrict access.   


### How to build

#### Windows

1. Place the files 'asm.td' and 'getshade.td' from the repository 'src' directory
to some writable directory. (Or leave them in place if their current directory is
writable.) Into this directory the output file of the program will be written.

2. Edit the 'asm.td' file and specify the path to the 'getshade.td' file.

3. Edit the 'main.cpp' file: locate the following line at the top of the file:

`std::string workingDir = "Y:\\your\\working\\directory\\"`

and change it by specifying your output directory (with trailing backslash).

4. Build the solution and run the executable. Check the image in the output "graph.ppm"
file. PPM image files can be opened e.g. with LibreOffice, GIMP, etc. Report of any
issues here: https://github.com/transd-lang/Demo_tiny/issues

#### Linux

To be written...
