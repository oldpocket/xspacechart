#!/usr/bin/perl -w

########################################################################
# read_ccdm.pl
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
# Read the data from the CCDM catalogue and create a list of multiple
# systems in the Hipparcos catalogue, with data from each component, and
# a list of Hipparcos entries which correspond to secondary stars in a
# system.
########################################################################

use strict;

open (SECONDARIES, ">", "secondaries") || die;

my @components;

while (<>) {
    chomp;
    my %fields = parse_record ($_);

    # Is this a new system?
    if ($fields{rComp} eq "" && $fields{Comp} eq "A") {
	print_system ();
	if ($fields{HIC} ne "") {
	    @components = (\%fields);
	} else {
	    @components = ();
	}
    } else {
	# Are we saving this system?
	if (@components > 0) {
	    # If this Hipparcos number is not the main component of a star,
	    # add it to the secondaries list.
	    if ($fields{HIC} ne "" && $fields{HIC} ne $components[0]->{HIC}) {
		print SECONDARIES $fields{HIC} . "\n";
		# Always add these, as the data will be found in Hipparcos
		push @components, \%fields;
	    } else {
		# Ignore components for which there isn't enough data
		if ($fields{Sp} ne "" && $fields{Vmag} ne "") {
		    push @components, \%fields;
		}
	    }
	}
    }	
}

sub print_system
{
    if (@components > 1) {
	print  "%" . $components[0]->{HIC} . "\n";
	foreach (@components) {
	    print_component (%{$_});
	}
    }
}

sub print_component
{
    my %star = @_;

    my $dm = "";
    if ($star{DM} ne "") {
	my ($number, $suffix, $catalogue) = 
	    $star{DM} =~ /([\+\-][0-9 ]{7})(.)(\d)/;
	if ($catalogue eq "0") {
	    $dm = "BD$number";
	} elsif ($catalogue eq "2") {
	    $dm = "CD$number";
	} elsif ($catalogue eq "4") {
	    $dm = "CP$number";
	} elsif ($catalogue eq "6") {
	    $dm = "";
	} elsif ($catalogue eq "8") {
	    $dm = "";
	}
	
	if ($suffix ne ".") {
	    $dm .= $suffix;
	}
    }
    print "$star{Comp}|$dm|$star{Sp}|$star{Vmag}|$star{HIC}\n";
}

sub parse_record
{
    shift;
    my %fields;
    my @names = qw(CCDM rComp Comp Note1 Note2 Disc dRAs dDEs r_dRAs Year
		   theta rho Obs Vmag Sp pmNote pmRA pnDE DM Name2 HD m_HD
		   ADSBDS m_ADSBDS IDS m_IDS HIC);
    my @fields = /
	\s
	(.{10}) # CCDM
	(.)     # rComp
	(.)     # Comp
	(.)     # Note1
	(.)     # Note2
	(.{7})  # Disc
	\s
        (.{7})  # dRAs
	(.{7})  # dDEs
	\s
	(.{2})  # r_dRAs
	\s
	(.{4})  # Year
	\s
	(.{3})  # theta
	(.{6})  # rho
	\s
	(.{2})  # Obs
	\s
	(.{4})  # Vmag
	\s
	(.{2})  # Sp
	(.)     # pmNote
	(.{5})  # pmRA
	(.{5})  # pnDE
	(.{10}) # DM
	(.{10}) # Name2
	\s
	(.{6})  # HD
	(.{2})  # m_HD
	(.{6})  # ADSBDS
	(.)     # m_ADSBDS
	(.)     # m_IDS
	(.{11}) # IDS
	\s
	(.{6})  # HIC
	/x;
    my $i;
    for ($i = 0; $i < @fields; $i++) {
	$fields[$i] =~ s/^\s+//;
	$fields{$names[$i]} = $fields[$i];
    }
    return %fields;
}
