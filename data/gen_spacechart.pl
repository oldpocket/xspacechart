#!/usr/bin/perl -w

########################################################################
# gen_spacechart.pl
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
# Take the list of multiple systems, the list of secondaries, the lists
# of names, and the Hipparcos catalogue and combine them into a SpaceChart
# data file.
########################################################################

use strict;

my %bayer_names;
my %bayer_short_names;
my %proper_names;

# Maximum distance, in parsecs
my $MAX_DISTANCE = 30;
# If the first arg is a number, then it's the maximum distance
if ($ARGV[0] && $ARGV[0] =~ /^\d+$|
                             ^\d*\.\d+$
                            /x) {
    $MAX_DISTANCE = shift @ARGV;
}

# Read the name files

read_bayer_flamsteed();

read_proper_names();

# Read the output of group_multiples.pl

my %secondaries;
read_secondaries();

my %multiples;
read_multiples();

# The acknowledgements to the origin of the data
acknowledgements();

# The Sun: a special case
print 
"0001,Sun      ,Sol            ,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000,   0.000
 ,              ,Sol                                    ,                                       , G2 V         ,  1.0\n%\n";

# Now, read the Hipparcos data and format it as required.
my $i = 0;
while (<>) {
    chomp;
    my @fields = split /\|/;
    $fields[1] =~ s/^\s+//;
    $fields[11] =~ s/^\s+//;
    $fields[11] = 0 if ($fields[11] eq "");
    $fields[11] = abs ($fields[11]);
    my ($cat_name, $short_name, $x, $y, $z,
	$ra_hours, $ra_minutes, $ra_seconds, 
	$dec_degrees, $dec_minutes, $dec_seconds);
    my ($proper_name, $bayer_name) = ("", "");
    my $spectrum;
    my @components;

    if ($secondaries{$fields[1]}) {
	# This is not the primary star of a system. Forget about it.
	next;
    } else {
	$cat_name = $fields[1];
	# RA
	($ra_hours, $ra_minutes, $ra_seconds) = split ' ', $fields[3];
	# Declination in degrees, minutes
	($dec_degrees, $dec_minutes, $dec_seconds) = split ' ', $fields[4];
	$dec_minutes += $dec_seconds / 60;
	# Cartesian coordinates
	if ($fields[11] =~ /^\s+$/ || $fields[11] == 0) {
	    # If the parallax is 0, just skip the star
	    next;
	}
	# Skip stars farther than the specified maximum distance
	if (1/($fields[11]/1000) >= $MAX_DISTANCE) {
	    next;
	}
	($x, $y, $z) = coordinates ($ra_hours, $ra_minutes, $ra_seconds, 
				    $dec_degrees, $dec_minutes, 
				    $fields[11]/1000);
	# Names
	$proper_name = $proper_names{$fields[1]};
	$bayer_name = $bayer_names{$fields[1]};
	if ($proper_name) {
	    $short_name = substr( $proper_name, 0, 15 );
	} elsif ($bayer_name) {
	    $short_name = $bayer_short_names{$fields[1]};
	} else {
	    $short_name = dm_number (@fields);
	}
	# The components
	if ($multiples{$fields[1]}) {
	    # Primary star of a multiple system.
	    @components = @{$multiples{$fields[1]}};
	} else {
	    # Single star.
	    @components = ([' ', # Component
			    dm_number (@fields), # DM
			    $fields[76], # Spectrum
			    $fields[5] # Visual magnitude
			    ]);
	}
    }

    printf( "%04d,HIP%6s,%-15s,%8.3f,%8.3f,%8.3f," . 
	    "%8.3f,%8.3f,%8.3f,%8.3f,%8.3f\n", 
	    $i++, $cat_name, $short_name, 
	    $x, $y, $z, $ra_hours, 
	    $ra_minutes, $ra_seconds, 
	    $dec_degrees, $dec_minutes );
    my $comp;
    foreach $comp (@components) {
	my @comp = @{$comp};
	# Fix the spectral class
	my $spectrum = " " . $comp[2] unless $comp[2] =~ /^D/;
	$spectrum = "" unless $spectrum;
	# Change to absolute magnitude, then to luminosity
	my $lum = 0;
	if ($comp[3]) {
	    my $abs_mag = $comp[3] + 5 - 5 * log10 (1000/$fields[11]);
	    $lum = 10 ** (0.4 * (4.85 - $abs_mag));
	}
	my $comp_proper_name = $proper_name;
	my $comp_bayer_name = $bayer_name;
	if ($comp[4]) {
	    $comp_proper_name = $proper_names{$comp[4]} ? 
		$proper_names{$comp[4]} : $proper_name;
	    $comp_bayer_name = $bayer_names{$comp[4]} ? 
		$bayer_names{$comp[4]} : $bayer_name, 
	}
	printf( "%1s,%-14s,%-39s,%-39s,%-14s,%8.4f\n", 
		$comp[0], $comp[1], 
		$comp_proper_name ? $comp_proper_name : "",
		$comp_bayer_name ? $comp_bayer_name : "",
		$spectrum, $lum );
    }
    print "%\n";    
}

sub read_bayer_flamsteed 
{
    open (BAYER, "bayer_flamsteed_names") || die;
    while (<BAYER>) {
	chomp;
	my ($hip, $name, $short_name) = split /\|/;
	$bayer_names{$hip} = $name;
	$bayer_short_names{$hip} = $short_name;
    }
    close BAYER;
}

sub read_proper_names
{
    open (PROPER_NAMES, "proper_names") || die;
    while (<PROPER_NAMES>) {
	chomp;
	my ($hip, $name) = split /\|/;
	$proper_names{$hip} = $name;
    }
    close PROPER_NAMES;
}

sub read_secondaries
{
    open (SECONDARIES, "secondaries") || die;
    while (<SECONDARIES>) {
	chomp;
	$secondaries{$_} = 1;
    }
    close SECONDARIES;
}

sub read_multiples
{
    open (MULTIPLES, "systems") || die;
    my $hip;
    while (<MULTIPLES>) {
	chomp;
	if (/^\%/) {
	    ($hip) = /^%(\d+)$/;
	    $multiples{$hip} = [];
	} else {
	    my @fields = split /\|/;
	    push @{$multiples{$hip}}, \@fields;
	}
    }
    close MULTIPLES;
}

sub dm_number
{
    my @fields = @_;
    my $dm = "";

    if ($fields[72] !~ /^\s+$/) {
	$dm = $fields[72];
	$dm =~ s/^B/BD/;
    } elsif ($fields[73] !~ /^\s+$/) {
	$dm = $fields[73];
	$dm =~ s/^C/CD/;
    } elsif ($fields[74] !~ /^\s+$/) {
	$dm = $fields[74];
	$dm =~ s/^P/CP/;
    }

    return $dm;
}

sub coordinates
{
    my ($ra_hours, $ra_minutes, $ra_seconds, $dec_degrees, $dec_minutes,
	$parallax) = @_;
    my ( $rvect, $phi, $theta, $rho, $ra, $declination );
    my $DEG_TO_RAD = 0.0174532925;    
    my ($x, $y, $z);
    
    if( $parallax ) {
	$rho = 1 / $parallax;
    } else {
	$rho = 0;
    }
    $ra = $ra_hours * 15 + $ra_minutes * 0.25 + $ra_seconds * 0.0041666666;
    $declination = abs($dec_degrees) + ($dec_minutes / 60.0);
    if( $dec_degrees < 0 ) {
	$declination = -$declination;
    }
    $phi = $ra * $DEG_TO_RAD;
    $theta = $declination * $DEG_TO_RAD;
    $rvect = $rho * cos($theta);
    
    $x = $rvect * cos($phi);
    $y = $rvect * sin($phi);
    $z = $rho * sin($theta);

    return ($x, $y, $z);
}

sub log10
{
    my $n = shift;
    return log($n)/log(10);
}

sub acknowledgements
{
    my $date = `LANG=C date`;
    chomp($date);
    
    print
"Data File generated on " . $date . " by hip2spacechart.sh\n" .
"from The Hipparcos Catalogue (I/239, ESA 1997), and the \n" .
"Catalogue of the Components of Double and Multiple Stars (CCDM)\n" .
"(I/269A, Dommanget+ 2002) obtained from CDS, Strasbourg\n" .
"(http://cdsweb.u-strasbg.fr/).\n\n" .
"See the file data/COPYING for distribution conditions.\n%\n";
}
