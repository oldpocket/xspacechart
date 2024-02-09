#!/usr/bin/perl -w

########################################################################
# combine_star_data.pl
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
########################################################################

########################################################################
# Take the data about multiple systems from read_ccdm.pl and use the
# Hipparcos spectral class and magnitude whenever possible.
########################################################################

use strict;

# Read the hipparcos data
my %magnitude;
my %spectrum;
read_hipparcos();

open (MULTIPLES, "multiples") || die;
my ($hip, $last_hip);
while (<MULTIPLES>) {
    chomp;
    if (/^\%/) {
	($hip) = /^%(\d+)$/;
	print "$_\n";
    } else {
	my @fields = split /\|/;
	# Skip those stars we consider anomalous
	if (anomalous ($hip, @fields)) {
	    next;
	}
	# Use the Hipparcos data for the primary star of each system, and
	# for secondaries which have entries in the catalogue.
	if (defined($fields[4]) && $fields[4] !~ /^\s*$/ && 
	    ($fields[0] eq "A" || ($fields[4] ne $hip && 
				   $fields[4] ne $last_hip)) &&
	    defined ($spectrum{$fields[4]}) && $spectrum{$fields[4]} ne "" && 
	    defined ($spectrum{$fields[4]}) && $magnitude{$fields[4]} ne "") {
	    print "$fields[0]|$fields[1]|$spectrum{$fields[4]}|".
		"$magnitude{$fields[4]}|$fields[4]\n";
	} else {
	    # Just drop the components whose spectrum or magnitude is not known
	    if ($fields[2] ne "" && $fields[3] ne "") {
		print "$_\n";
	    }
	}
	if ($fields[4]) {
	    $last_hip = $fields[4];
	}
    }
}
close MULTIPLES;

sub read_hipparcos
{
    while (<>) {
	chomp;
	my @fields = split /\|/;
	$fields[1] =~ s/^\s+//;
	$fields[5] =~ s/\s//g;
	$fields[76] =~ s/\s//g;

	$magnitude{$fields[1]} = $fields[5];
	$spectrum{$fields[1]} = $fields[76];
    }
}

# Filter unwanted stars.
sub anomalous
{
    my $hip = shift;
    my @fields = @_;
    # Sirius C and D????
    return 1 if ($hip eq "32349" && $fields[0] =~ /^[CD]$/);
}
