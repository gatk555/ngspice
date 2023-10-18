*ng_script_with_params
// This Ngspice interpreter script accepts arbitrary argiments to
// the Verilator compiler (Verilog to C++) and builds a shared library
// or DLL that can be loaded by the d_cosim XSPICE code model.
// Instances of the model are then digital circuit elements whose
// behaviour is controlled by the Verilog source.

// Try and pick a root name for the output file and build directory.
// At least one argument is required: $argc may be non-existent or zero.

set bad=0
if $?argc = 0
   set bad=1
end

if $argc <= 0
   set bad=1
end

if $bad
   echo Arguments acceptable to Verilator are required.
   quit
end

// Disable special processing of '{'.

set noglob

// Set parameters for Windows or Unix-like OS.

if $oscompiled = 2 | $oscompiled = 3 | $oscompiled = 8 // Windows
   set windows=1
   set dirsep1="\\"
   set dirsep2="/"
else
   set windows=0
   set dirsep1="/"
end

// Loop through the arguments to find Verilog source: some_path/xxxx.v
// The output file will have the same base name.

let index=1
set off=1                        // Avoid error in dowhile
repeat $argc
    set base="$argv[$&index]"
    let index = index + 1
    strstr l "$base" ""
    if $l > 2                    // Look for xxxx.v
        strslice tail "$base" -2 2
	strcmp bad "$tail" ".v"
	if $bad <> 0
	    set base=""
	    continue
	end
	let l = $l - 2
	strslice base "$base" 0 $&l
    else
        set base=""
        continue
    end

    dowhile $off >= 0            // Strip leading directories
        strstr off "$base" "$dirsep1"
        if $off >= 0
      	    let off=$off+1
            strslice base "$base" $&off $l
        end
    end

    strstr l "$base" ""          // Check for zero-length string
    if $l > 0
        break
    end
end

if index - 1 > $argc
    set base=verilated           // Default name
end

// Define working directory for Verilator

set tail="_obj_dir"
setcs objdir="$base$tail"

// Default base name of output file.

if $windows
   setcs tail=".DLL"
else
   setcs tail=".so"
end
setcs soname="$base$tail"

// First convert to C++, PREFIX determines the file names.

setcs prefix="Vlng"

// Compilation option for C/C++: -fpic is required by GCC for a shared library
// and path to src/include/ngspice.

setcs cflags="--CFLAGS -fpic"
setcs include="--CFLAGS -I../../../src/include"

// Run Verilator on the given input files.

shell verilator --Mdir $objdir --prefix $prefix $cflags --cc $argv

// Parse the primary interface Class definition for members representing
// the ports of the top-level Verilog module.
// Example conversion:  VL_IN8(&Clk,0,0); ==> VL_DATA(8,Clk,0,0)

echo "/* Generated code: do not edit. */" > inouts.h
echo "/* Generated code: do not edit. */" > inputs.h
echo "/* Generated code: do not edit. */" > outputs.h

// This loop is intended to have the same effect as:
// sed --quiet -e 's/VL_IN\([0-9]*\)(&\(.*\);/VL_DATA(\1,\2/p' \
//    obj_dir/${PREFIX}.h >> inputs.h

set htail=".h"
setcs inout="VL_INOUT"
setcs in="VL_IN"
setcs out="VL_OUT"

set fn="$objdir/$prefix$htail" // Like foo-obj_dir/Vlng.h
fopen fh $fn
if $fh < 0
   quit
end

while 1
    fread line $fh l
    if $l < 0
       break
    end

    // Does it contain a closing parenthesis?

    strstr off "$line" ")"
    if $off < 0
       continue                               // No ")", ignore.
    end
    let off = $off + 1
    strslice line "$line" 0 $&off             // Slice off tail.

    // Is it an inout port definition?

    strstr off "$line" $inout
    if $off >= 0                              // Match found
       let off = $off + 8                     // strlen("VL_INOUT") == 8
       strslice line "$line" $&off $l
       strstr off "$line" "("
       strslice size "$line" 0 $off 
       let off = $off + 2                     // strlen("(&") == 2
       strslice line "$line" $&off $l
       echo VL_DATA($size,$line >> inouts.h   // New macro invocation
       continue
    end

    // Input port?

    strstr off "$line" $in
    if $off >= 0                              // Match found
       let off = $off + 5                     // strlen("VL_IN") == 5
       strslice line "$line" $&off $l
       strstr off "$line" "("
       strslice size "$line" 0 $off 
       let off = $off + 2                     // strlen("(&") == 2
       strslice line "$line" $&off $l
       echo VL_DATA($size,$line >> inputs.h   // New macro invocation
       continue
    end

    // Output port?

    strstr off "$line" $out
    if $off >= 0                              // Match found
       let off = $off + 6                     // strlen("VL_OUT") == 6
       strslice line "$line" $&off $l
       strstr off "$line" "("
       strslice size "$line" 0 $off 
       let off = $off + 2                     // strlen("(&") == 2
       strslice line "$line" $&off $l
       echo VL_DATA($size,$line >> outputs.h  // New macro invocation
       continue
    end
end
fclose $fh

// Compile the generated C++ code along with shim.cpp.  Verilator only
// does this when building an executable binary, so include main.cpp.

shell verilator --Mdir $objdir --prefix $prefix $include $cflags
+ --cc --build --exe
+ main.cpp shim.cpp $argv

// Make a shared library/DLL.

set   v_objs="$objdir/shim.o $objdir/verilated.o $objdir/verilated_threads.o"
setcs tail="__ALL.a"
setcs v_lib="$objdir/$prefix$tail"          // Like Vlng___ALL.a

shell g++ --shared $v_objs $v_lib -pthread -lpthread -latomic -o $soname
