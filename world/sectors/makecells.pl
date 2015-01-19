#!/usr/bin/perl


$zTop = 12;
$zBot = -7;
$xLeft = -9;
$xRight = 4;

for ($x = $xLeft; $x <= $xRight; $x++) {

	for ($z = $zTop; $z >= $zBot; $z--) {

		print "cell {\n";		
		print "\tloc=$x,0,$z;\n";
		print "\tmaterial=NULL;\n";
		print "}\n";

	}

}