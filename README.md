# header_docu
create HTML- or Markdown- based documentation from file header


With great respect to Doxygen, JSDoc or DoxDox, but they all reading / parsing the complete textfile.

__For me I just need parsing the first comment block of a single textfile or every textfile with a defined file-extension in a folder, doesn't matter if *.js, *.h, *.hpp or whatever__

This is the first quick & dirty version of the commandline tool, written with QT5 and C++17


__See example__ markdown output here:

<a href="https://github.com/Zheng-Bote/ESP32_libs/wiki" alt="Github ESP32" title="https://github.com/Zheng-Bote/ESP32_libs/wiki">https://github.com/Zheng-Bote/ESP32_libs/wiki</a>




## Roadmap:
- [x] quick and dirty commandline parser

- [ ] migrate QT commandline to vanilla cxx

- [ ] build Linux and Windows desktop app with QT

- [ ] build Windows desktop app with C#

- [ ] CMake / make support for compilation and installation



## Compile
```
qmake -r "CONFIG+=release"
make
```

* depending on your system environment you maybe want to modify the MAKEFILE g++ compiler option to
```g++ -std=c++2a``` || ```-std=gnu++1z```


# header of main.cpp

- - -
## TITLE:
>     main.cpp

### BRIEF:
>     header_docu (main.cpp)

### DESC:
>     header_docu is a small nafty C++ commandline tool to read the first comment block of a textfile and outputs it as HTML5- or markdown- snippet    

>     DEPENDENCIES:
>         cxxopts
>         a lightweight C++ option parser library, supporting the standard GNU style syntax for options.
>         https://github.com/jarro2783/cxxopts

### SOURCE:
>     https://github.com/Zheng-Bote/header_docu

### SYNTAX:
>     header_docu -h | header_docu --help
>     
>     -c, --css arg   include external CSS file for html output (default: false)
>     -d, --dir arg   parse directory (default: .)
>     -e, --ext arg   file extension (default: h)
>     -f, --file arg  1 single textfile
>     -o, --out arg   output type md | html (default: md)
>     -h, --help      Print usage
>     -v, --version   Version

>     EXAMPLES:
>     /usr/local/bin/header_docu --dir . --ext h --out md // read/parse all *.h files in the current directory
>     /usr/local/bin/header_docu --file ~/DEV/CPP/header_docu/main.cpp --out html // read/parse single file and output as HTML5

### RETURN:
>     output.md | output.html
>     void

### HISTORY:
> Version | Date       | Developer        | Comments
> ------- | ---------- | ---------------- | ---------------------------------------------------------------
> 0.1.0   | 2022-03-05 | RZheng           | created 
> 0.1.1   | 2022-03-06 | RZheng           | fixed markdown title 


