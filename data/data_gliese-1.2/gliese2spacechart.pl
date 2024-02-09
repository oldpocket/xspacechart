#!/usr/bin/perl -w

########################################################################
# gliese2spacechart.pl
# Copyright (C) 1999, 2000 Miguel Coca <e970095@zipi.fi.upm.es>
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
#   gliese2spacechart catalog >gliese.dat # catalog is the Gliese catalogue
#
########################################################################

use strict;

# Data for extracting names from the remarks field

# Greek letters and constellation names are hardcoded since they are not 
# likely to change.
my %greek_letters = ( "ALP",   "Alpha", "NU",       "Nu", "BET",   "Beta", 
		      "XI",       "Xi", "GAM",   "Gamma", "OMI", "Omicron", 
		      "DEL",   "Delta", "PI",       "Pi", "EPS", "Epsilon", 
		      "RHO",     "Rho", "ZET",    "Zeta", "SIG",   "Sigma", 
		      "ETA",     "Eta", "TAU",     "Tau", "THE",   "Theta", 
		      "UPS", "Upsilon", "IOT",    "Iota", "PHI",     "Phi", 
		      "KAP",  "Kapppa", "CHI",     "Chi", "LAM",  "Lambda", 
		      "PSI",     "Psi", "MU",       "Mu", "OME", "  Omega", 
		      "ALF",   "Alpha" );

my %constellations = ( 
"AND",          "Andromedae", "ANT",             "Antliae",
"APS",              "Apodis", "AQR",             "Aquarii",
"AQL",             "Aquilae", "ARA",                "Arae",
"AUR",             "Aurigae", "ARI",             "Arietis",
"BOO",              "Bootis", "CAE",               "Caeli",
"CAM",      "Camelopardalis", "CNC",              "Cancri",
"CVN",   "Canum Venaticorum", "CMA",       "Canis Majoris",
"CMI",       "Canis Minoris", "CAP",          "Capricorni",
"CAR",             "Carinae", "CAS",         "Cassiopeiae",
"CEP",              "Cephei", "CEN",            "Centauri",
"CET",                "Ceti", "CHA",       "Chamaeleontis",
"CIR",             "Circini", "COL",            "Columbae",
"COM",     "Comae Berenices", "CRA",   "Coronae Australis", 
"CRB",    "Coronae Borealis", "CRV",               "Corvi",
"CRT",            "Crateris", "CRU",              "Crucis",
"CYG",               "Cygni", "DEL",            "Delphini",
"DOR",             "Doradus", "DRA",            "Draconis",
"EQU",             "Equulei", "ERI",             "Eridani",
"FOR",            "Fornacis", "GEM",           "Geminorum",
"GRU",               "Gruis", "HER",            "Herculis",
"HOR",           "Horologii", "HYA",              "Hydrae",
"HYI",               "Hydri", "IND",                "Indi",
"LAC",            "Lacertae", "LEO",              "Leonis",
"LMI",      "Leonis Minoris", "LEP",             "Leporis",
"LIB",              "Librae", "LUP",                "Lupi",
"LYN",              "Lyncis", "LYR",               "Lyrae",
"MEN",              "Mensae", "MIC",         "Microscopii",
"MON",         "Monocerotis", "MUS",              "Muscae",
"NOR",              "Normae", "OCT",            "Octantis",
"OPH",            "Ophiuchi", "ORI",             "Orionis",
"PAV",             "Pavonis", "PEG",              "Pegasi",
"PER",              "Persei", "PHE",           "Phoenicis",
"PIC",            "Pictoris", "PSC",             "Piscium",
"PSA",     "Piscis Austrini", "PUP",              "Puppis",
"PYX",             "Pyxidis", "RET",            "Reticuli",
"SGE",            "Sagittae", "SGR",          "Sagittarii",
"SCO",             "Scorpii", "SCL",          "Sculptoris",
"SCT",               "Scuti", "SER",           "Serpentis",
"SEX",           "Sextantis", "TAU",               "Tauri",
"TEL",          "Telescopii", "TRI",           "Trianguli",
"TRA", "Trianguli Australis", "TUC",             "Tucanae",
"UMA",       "Ursae Majoris", "UMI",       "Ursae Minoris", 
"VEL",             "Velorum", "VIR",            "Virginis",
"VOL",            "Volantis", "VUL",          "Vulpeculae" );

my %star_name_by_catalog = ();

my %star_name_by_designation = ();

my @identifications = ();

# Variables

my $star_names_file = "star_names";
my $is_data = 0;
my $record;
my $greek_letter;
my $constellation;
my $identification;
my ($number, $const);
my $i = 1;
my $current_comp = 0;
# Fields for each star in the catalog.
my ( $ra_hours, $ra_minutes, $ra_seconds, $spectrum, 
     $dec_degrees, $dec_minutes, $abs_magnitude, $parallax, $remarks,
     $lum, $x, $y, $z, $component, $cat_name, $proper_name,
     $bayer_name, $dm_name, $short_name);

# Calculated fields for the star system.
my ( @cat_name, @proper_name, @bayer_name, @dm_name, @short_name, @component, 
     @spectrum, @x, @y, @z, @lum, @ra_hours, @ra_minutes, @ra_seconds, 
     @dec_degrees, @dec_minutes );

read_star_names();

# Creation of a regexp that matches any greek letter abbreviation
$greek_letter = join( "|", keys %greek_letters );
# Creation of a regexp that matches any constellation abbreviation
$constellation = join( "|", keys %constellations );
# Creation of a regexp that matches any additional identification
$identification = join( "|", @identifications );

# Write catalogue info at the beginning of the file

my $date = `LANG=C date`;
chomp($date);

print
"Data File generated on " . $date . " by gliese2spacechart.pl\n" .
"from the Gliese Catalogue of Nearby Stars (V/70A, Gliese+ 1991), obtained\n" .
"from CDS, Strasbourg (http://cdsweb.u-strasbg.fr/).\n\n" .
'##################################################################
    Catalogues available at CDS contain scientific data distributed
for free, for a scientific usage. Only the expenses related to
copying and mailing are charged if relevant.
    Companies including such data in their commercial products cannot
charge their clients for the data. Furthermore, users must be informed
of the origin of the data: this means an explicit reference to the service
provided by the CDS and also to the original author(s) of each catalogue.
##################################################################
%
';
          
while( <> ) {
    chomp;
    parse_record();
    if( $cat_name !~ /^(\s+$)/ ) {
	calculate_data();
	find_names();
	if( !(($i == 1) && ($current_comp == 0)) && is_different() ) {
	    print_star();
	    new_star();
	}
	add_component();
    }
}

sub read_star_names
{
    my $i = 1;
    my $line = "";
    my $line_type = "";

    open( STAR_NAMES, $star_names_file ) || 
	die "Can\'t open $star_names_file\n";
    while( defined($line = <STAR_NAMES>) ) {
	if( ($line !~ /^\w*$/) && ($line !~ /^\#/) ) {
	    if( $line =~ /^\@ ( (?:STAR_NAMES_BY_DESIGNATION)|
				(?:STAR_NAMES_BY_CATALOG)|
				(?:IDENTIFICATIONS) ) /x ) {
		$line_type = $1;
	    }
	    elsif( ($line_type =~ /^STAR_NAMES_BY_DESIGNATION$/) && 
		($line =~ /^\"([^\"]*)\"\s*,\s*\"([^\"]*)\"/) ) {
		$star_name_by_designation{$1} = $2;
	    }
	    elsif( ($line_type =~ /^STAR_NAMES_BY_CATALOG$/) && 
		   ($line =~ /^\"([^\"]*)\"\s*,\s*\"([^\"]*)\"/) ) {
		$star_name_by_catalog{$1} = $2;
	    }
	    elsif( ($line_type =~ /^IDENTIFICATIONS$/) && 
		   ($line =~ /^ \" ( [^\"]* ) \" /x ) ) {
		$identifications[@identifications] = $1;
	    } else {
		die "Syntax Error in $star_names_file, line $i";
	    }
	}
	$i++;
    }
    close STAR_NAMES;
}

sub parse_record {
    $cat_name = substr $_, 0, 8;
    $component = substr $_, 8, 2;
    $spectrum = substr $_, 54, 12;
    $ra_hours = substr $_, 12, 2;
    if( 165 <= length ) {
	$dm_name = substr $_, 153, 12;
    } else {
	$dm_name = "";
    }
    if( $ra_hours =~ /^\s*$/ ) {$ra_hours = 0}; 
    $ra_minutes = substr $_, 15, 2;
    if( $ra_minutes =~ /^\s*$/ ) {$ra_minutes = 0}; 
    $ra_seconds = substr $_, 18, 2;
    if( $ra_seconds =~ /^\s*$/ ) {$ra_seconds = 0}; 
    $dec_degrees = substr $_, 21, 3;
    if( $dec_degrees =~ /^\s*$/ ) {$dec_degrees = 0}; 
    $dec_minutes = substr $_, 25, 4;
    if( $dec_minutes =~ /^\s*$/ ) {$dec_minutes = 0}; 
    $abs_magnitude = substr $_, 121, 5;
    if( $abs_magnitude =~ /^\s*$/ ) {$abs_magnitude = 0}; 
    $parallax = substr $_, 108, 6;
    if( $parallax =~ /^\s*$/ ) {$parallax = 0};
    $parallax = $parallax / 1000;
    if( length > 188 ) {
	$remarks = substr $_, 188;
    } else {
	$remarks = "";
    }
}

sub calculate_data
{
    my $DEG_TO_RAD = 0.0174532925;
    my ( $rvect, $phi, $theta, $rho, $ra, $declination );
    
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

    $lum = 10 ** (0.4 * (4.9 - $abs_magnitude));

    $cat_name =~ s/  +/ /;
    $cat_name =~ s/^\s*//;
    $cat_name =~ s/\s*$//;
    $component = substr( $component, 0, 1 );
    $spectrum =~ s/^\s//;
    $spectrum =~ s/  +/ /;
}

sub find_names
{
    $bayer_name = "";
    $proper_name = "";
    $short_name = "";
    if( ($remarks =~ / \b($greek_letter) \s ($constellation)\b/xi) || 
	($remarks =~ / \b(\d+) \s ($constellation)\b/xi)  ) {
	$number = $1;
	$const = $2;
	$short_name = "$number $const";
	$const =~ s/(\w*)/\U$1\E/;
	if( $greek_letters{$number} ) {
	    $bayer_name = $greek_letters{$number} . " " .  
		$constellations{$const};
	} else {
	    $bayer_name = $number . " " .  $constellations{$const};
	}
    }
    if( $star_name_by_designation{$bayer_name} ) {
	$proper_name = $star_name_by_designation{$bayer_name};
    }
    if( $star_name_by_catalog{$cat_name} ) {
	$proper_name = $star_name_by_catalog{$cat_name};
    }
    if( !$proper_name ) { # If there is no other name try Wolf, Ross, etc
	if( $remarks =~ / ($identification) \s* (\d+)/xi ) {
	    $proper_name = $1 . " " . $2;
	}
    }
    if( $proper_name ) {
	$short_name = substr( $proper_name, 0, 15 ) if $proper_name; 
    } elsif(!$short_name) {
	$short_name = $dm_name;
	$proper_name = $bayer_name;
    }
}

sub print_star
{
    my ( $system_record, @component_records );
    my ( $short_name, $cat_name );
    my ( $comp );

    $cat_name = $cat_name[0];
    $short_name = $short_name[0];

    $system_record = sprintf( "%04d,%-9s,%-15s,%8.3f,%8.3f,%8.3f," . 
			      "%8.3f,%8.3f,%8.3f,%8.3f,%8.3f\n", 
			      $i, $cat_name, $short_name, 
			      $x[0], $y[0], $z[0], $ra_hours[0], 
			      $ra_minutes[0], $ra_seconds[0], 
			      $dec_degrees[0], $dec_minutes[0] );

    for( $comp = 0; $comp < $current_comp; $comp++ ) {
	$component_records[$comp] = sprintf( "%1s,%-14s,%-39s,%-39s," . 
					     "%-14s,%8.4f\n", 
					     $component[$comp], 
					     $dm_name[$comp],
					     $proper_name[$comp], 
					     $bayer_name[$comp], 
					     $spectrum[$comp],
					     $lum[$comp] );
    }

    print $system_record;
    foreach $comp ( @component_records ) {
	print $comp;
    }
    print "%\n";
}

sub is_different
{
    my $next_comp;

    if( $current_comp == 0 ) {
	return 0;
    } else {
	$next_comp = $component[$current_comp-1];
	$next_comp++;
	return ($cat_name !~ /^$cat_name[0]$/) || ($component !~ /^$next_comp$/);
    }
}

sub new_star
{
    $current_comp = 0;
    $i++;
    @cat_name = ();
    @proper_name = ();
    @bayer_name = ();
    @dm_name = ();
    @short_name = ();
    @component = ();
    @spectrum = ();
    @x = ();
    @y = ();
    @z = ();
    @lum = ();
}

sub add_component
{
    $cat_name[$current_comp] = $cat_name;
    $proper_name[$current_comp] = $proper_name;
    $bayer_name[$current_comp] = $bayer_name;
    $dm_name[$current_comp] = $dm_name;
    $short_name[$current_comp] = $short_name;
    $component[$current_comp] = $component;
    $spectrum[$current_comp] = $spectrum;
    $x[$current_comp] = $x;
    $y[$current_comp] = $y;
    $z[$current_comp] = $z;
    $ra_hours[$current_comp] = $ra_hours;
    $ra_minutes[$current_comp] = $ra_minutes;
    $ra_seconds[$current_comp] = $ra_seconds;
    $dec_degrees[$current_comp] = $dec_degrees;
    $dec_minutes[$current_comp] = $dec_minutes;
    $lum[$current_comp] = $lum;
    $current_comp++;
}
