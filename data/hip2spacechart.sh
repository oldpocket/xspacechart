#!/bin/sh

########################################################################
# hip2spacechart.sh
# Copyright (C) 2002 Free Software Foundation
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
# USAGE:
#   hip2spacechart <map size radius>
#
########################################################################

MAX=$1

echo "Generating data file"

echo "Creating list of multiple stars from CCDM..."
perl read_ccdm.pl <ccdm.dat >multiples
echo "Combining Spectrum and Magnitude data from Hipparcos and CCDM..."
perl combine_star_data.pl <hip_main.dat >systems
echo "Calculating star coordinates..."
perl gen_spacechart.pl $MAX <hip_main.dat >hipparcos.dat
echo "Done"
