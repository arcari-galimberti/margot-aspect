# README

This is a tutorial of the margot-aspect code generator.  
For demonstration purposes, the files contained in the _test/_ folder can be used; these are:
- a C++ application exploiting some of the possible use cases,
- the related configuration files for the mARGOt framework, and
- the XML configuration file for the code insertion.

For information about the mARGOt framework, refer to: https://gitlab.com/margot_project

## Compiling instructions

1) Clone the margot-aspect project from the repository:
~~~
:::bash
  $ git clone https://github.com/arcari-galimberti/margot-aspect.git
~~~
		
2) Create a _build/_ folder inside the root of the project, and run cmake and make:
~~~
:::bash
  $ mkdir build
  $ cd build
  $ cmake -DCMAKE_BUILD_TYPE=Release ..
  $ make
~~~

## Execution instructions

~~~
Usage: ./aspect_project [options]
Generate AspectC++ aspects for Margot Framework:
  -g [ --generate ] arg Aspect description XML file
  -o [ --output ] arg   Output (.ah) file
  -h [ --help ]         Print the help message
~~~

Launch the executable by passing as its parameters the pathname for the XML configuration file
and the pathname for the generated aspect file:
~~~
:::bash
	$ ./margotaspect -g <configuration_file-pathname>.xml -o <output_aspect_file_pathname>.ah
~~~
