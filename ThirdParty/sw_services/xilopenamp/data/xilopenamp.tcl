###############################################################################
#
# Copyright (C) 2015 Xilinx, Inc.  All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# Use of the Software is limited solely to applications:
# (a) running on a Xilinx device, or
# (b) that interact with a Xilinx device through a bus or interconnect.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
# OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Except as contained in this notice, the name of the Xilinx shall not be used
# in advertising or otherwise to promote the sale, use or other dealings in
# this Software without prior written authorization from Xilinx.
#
###############################################################################
#
# Modification History
#
# Ver   Who  Date     Changes
# ----- ---- -------- -----------------------------------------------
# 1.00  pkp  12/03/15 Initial Release
##############################################################################

#---------------------------------------------
# openamp_drc
#---------------------------------------------
proc openamp_drc {libhandle} {
    # check processor type
    set proc_instance [hsi::get_sw_processor];
    set hw_processor [common::get_property HW_INSTANCE $proc_instance]

    set proc_type [common::get_property IP_NAME [hsi::get_cells -hier $hw_processor]];
    if { ( $proc_type != "psu_cortexr5" ) && ( $proc_type != "ps7_cortexa9" ) } {
                error "ERROR: This library is supported only for CortexR5 and CortexA9 processors.";
                return;
    }
}

proc generate {libhandle} {

}

#-------
# post_generate: called after generate called on all libraries
#-------
proc post_generate {libhandle} {

	xgen_opts_file $libhandle
}

#-------
# execs_generate: called after BSP's, libraries and drivers have been compiled
#	This procedure builds the libxilopenamp.a library
#-------
proc execs_generate {libhandle} {

}

proc xgen_opts_file {libhandle} {
	set oslist [hsi::get_os];

	if { [llength $oslist] != 1 } {
	    return 0;
	}
	set os [lindex $oslist 0];
	set amposfn [file join "src" "amp_os.h"]
	file delete $amposfn
	set ampos [open $amposfn w]
	::hsi::utils::write_c_header $ampos "Definition of OS for which the BSP is built"
	puts $ampos "/******************************************************************/"
	puts $ampos ""
	puts $ampos "/* Operating System definition */"
	if { $os == "freertos823_xilinx" } {
		puts $ampos "#define USE_FREERTOS TRUE"
	      } else {
		puts $ampos "#define USE_BAREMETAL TRUE"
	      }
	puts $ampos ""
	puts $ampos "/******************************************************************/"
	close $ampos
	# Copy the include files to the include directory
	set srcdir src
	set dstdir [file join .. .. include]

	# Create dstdir if it does not exist
	if { ! [file exists $dstdir] } {
		file mkdir $dstdir
	}
	# Get list of files in the srcdir
		set sources [glob -join $srcdir *.h]

		# Copy each of the files in the list to dstdir
		foreach source $sources {
			file copy -force $source $dstdir
	}

}
