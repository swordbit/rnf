<?php
require_once('functions.inc');

// situation de la reserve sur une carte
// cartes obtenues avec planiglobe.com
function _displaySituationRN()
{
  global $base_url;

  $imgvide = 'region_vide.png';
  echo "<script type=\"text/javascript\">\n";
  echo "function swapRegion(zone,nr){\n";
  echo "var reg=document.getElementById('hovered')\n";
  echo "reg.src='/sites/all/themes/theme_rnf/images/region_'+nr+'.png'\n";
  echo "zone.onmouseout=function(){reg.src='/sites/all/themes/theme_rnf/images/$imgvide'}\n";
  echo "}\n";
  echo "</script>\n";

  $nid = arg(1);
  if ($nid != '')
    {
      $node = node_load($nid);
      if (isset($node->field_code))
	{
	  $rncode = _getfieldvalue($node, $node->field_code, '');
	  $lat    = _getfieldvalue($node, $node->field_latitude, '0.0');
	  $long   = _getfieldvalue($node, $node->field_longitude, '0.0');
	  $dpttab = _getfieldvaluetab($node, $node->field_departement, array());

	  $depts = array();
	  foreach($dpttab as $dptval)
	    {
	      if (trim($dptval['value']) != '')
		$depts[] = $dptval['value'];
	    }
	  if (count($depts) == 1)
	    $dpt = $depts[0];
	  else
	    $dpt = "0"; // plusieurs departements : metropole

	  $fondpath = $base_url.'/sites/all/themes/theme_rnf/images/';

	  $imgfond = $fondpath.'fond_regions.png'; 	

	  // image map
	  echo "<div id=\"pic\" style=\"height: 250px; width: 180px; background-image: url('$imgfond'); background-repeat: no-repeat\">\n";
	  
	  $width  = 180;
	  $height = 250;
	  switch($dpt)
	    {
	    case '971': // Guadeloupe 
	      // Longitude
	      // minimum = Iles          -61.1188 => 83
	      // maximum = Gd Connetable -61.0084 => 88
	      // delta : 0.1104                   => 5
	      $xpos = 5 * (($long -(-61.1188))/0.1104) + 83;
	      // Latitude
	      // minimum = Iles          16.173  => 197 //250-197 = 53
	      // maximum = Gd Connetable 16.3391 => 187
	      // delta : 0.1661                   => 10
	      $ypos = $height - (10 * (($lat -16.173)/0.1661) + 53);
	      break;

	    case '972': // Martinique
	      // Longitude
	      // minimum = Presqu'île    -60.8855 => 123
	      // maximum = Ilets         -60.8324 => 127
	      // delta : 0.0531                   => 4
	      $xpos = 4 * (($long -(-60.8855))/0.0531) + 123;
	      // Latitude
	      // minimum = Ilets         14.4142 => 207 //250-207 = 43
	      // maximum = Presqu'île    14.7688 => 186 
	      // delta : 0.3546                  => 21
	      $ypos = $height - (21 * (($lat -14.4142)/0.3546) + 43);
	      break;

	    case '973': // Guyane
	      // Longitude
	      // minimum = Amana           -53.7149 => 63
	      // maximum = Gd Connetable   -51.9439 => 80
	      // delta : 1.771                      => 17
	      $xpos = 17 * (($long -(-53.7149))/1.771) + 63;
	      // Latitude
	      // minimum = Nouragues         4.0736 => 230 //250-230=20
	      // maximum = Amana             5.6558 => 217
	      // delta : 1.5822                    => 13
	      $ypos = $height - (13 * (($lat -4.0736)/1.5822) + 20);
	      break;
		  
	    case '974': // Réunion
	      // Longitude
	      // minimum = Marine        55.2586 => 141
	      // maximum = Etang         55.2923 => 143
	      // delta : 0.0337	           => 2
	      $xpos = 2 * (($long -55.2586)/ 0.0337) + 141;
	      // Latitude
	      // minimum = Marine       -21.1365 => 188
	      // maximum = Etang        -20.9989 => 196
	      // delta : 0.1376                  => 8
	      $ypos = $height - (8 * (($lat -(-21.1365))/0.1376) + 52);
	      break;

	    case '976': // Mayotte
	      $xpos = 121;
	      $ypos = 231;
	      break;		  
	    case '977': // Saint-Barthélemy
	      $xpos = 27;
	      $ypos = 229;
	      break;      
	    case '978': // Saint-Martin
	      $xpos = 39;
	      $ypos = 178;
	      break;      
	    case '984': // TAAF`
	      $xpos = 164;
	      $ypos = 237;
	      break;

	    default: // Metropole
	      // Longitude
	      // minimum = Iroise      -4.975610  => x = 5
	      // maximum = Delta Sauer  8.134090  => x = 160
	      // delta :           13.11          => 155 
	      $xpos = 155 * (($long -(-4.975610))/13.11) + 5;

	      // Latitude:
	      // minimum = Bonifacio      41.363150 => y = 173
	      // maximum = Dune Marchand  51.073280 => y = 6
	      // delta :                   9,710    => 167 
	      $ypos = 180 - (167 * (($lat -41.363150)/9.710) + 6);
	    }

	  $usecanvas = true;
	  $usesvg = false;
	  
	  if ($usecanvas)
	    {
	  //echo "<canvas id=\"situationRN\" width=\"$width\" height=\"$height\" style=\"background : url($imgfond) no-repeat top center;\">";
	  //echo "Fallback content, in case the browser does not support Canvas.";
	  // echo "</canvas>\n";
	  echo "<canvas id=\"situationRN\" width=\"$width\" height=\"$height\" style=\"\">";
	  echo "Fallback content, in case the browser does not support Canvas.";
	  echo "</canvas>\n";

	  echo "<script type=\"text/javascript\">\n";
	  echo "<!--\n";
	  //echo "var fond = new Image();\n";
	  //echo "fond.src = '".$imgfond."'\n";
	  echo "var elem = document.getElementById('situationRN');\n";
	  echo "if (elem && elem.getContext) {\n";
	  echo "var context = elem.getContext('2d');\n";
	  echo "if (context) {\n";
	  echo "context.fillStyle   = '#f00'\n";
	  //echo "context.drawImage(fond, 0, 0, $width, $height);\n";
	  echo "context.beginPath();\n";
	  // Arguments: x, y, radius, start angle, end angle, anticlockwise
	  echo "context.arc($xpos, $ypos, 5, 0, 360, false);\n";
	  echo "context.fill();\n";
	  echo "context.closePath();\n";
	  echo "}\n";
	  echo "}\n";
	  echo "// -->\n";
	  echo "</script>\n"; 
	    }
	  
	  if ($usesvg)
	    {
	      echo "<svg width=\"$width\" height=\"$height\" opacity=\"0\">";
	      echo "<circle cx=\"$xpos\" cy=\"$ypos\" r=\"5\" fill=\"blue\" stroke=\"gray\" stroke-width=\"0\"/>";
	      echo "</svg>";
	    }

	  $imghover = $fondpath.$imgvide;
	  echo "<img id=\"hovered\" src=\"$imghover\" usemap=\"#regionMap\" width=\"180\" height=\"250\" style=\"position: absolute; top: 68px;\"/>";
	  echo "<map name=\"regionMap\">\n"; 
	  include('region_map.inc');
	  include('regions.inc');
	  foreach($regmap as $rcode => $rzone)
	    {
	      if (strstr($rcode, '-'))
		$rcode = substr($rcode, 0, 1);
	      $name = $regions[$rcode];
	      $alt  = htmlentities($name);
	      $url = _makeURL($name);
	      echo "<area href=\"$url\" alt=\"$alt\" title=\"$alt\" onmouseover=\"swapRegion(this,$rcode)\" shape=\"polygon\" coords=\"$rzone\">\n";
	    }
	  echo "</map>";	
	  echo "</div>";

	  //echo "Lat  : $lat / $ypos<br>";  
	  //echo "Long : $long / $xpos<br>"; 
	}
    }
}
?>
