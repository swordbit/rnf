<?php
 // affiche une carte du monde avec la position de chaque RN
 //

require_once('functions.inc');

function _displayCarteRN()
{
  echo "<script type=\"text/javascript\">\n";
  echo "<!--\n";
  $rntab = _getRNtable();
  _createMapItems($rntab);
  echo "var   latCenter  = 10.0;\n";
  echo "var   longCenter = 2.0;\n";
  echo "var   zoom = 2;\n";
  echo "var limit = \"\"\n";
  echo "// -->\n";
  echo "</script>\n";

  global $base_url;	
  $leafpath  = $base_url."/sites/all/libraries/leaflet";
  echo "<link rel=\"stylesheet\" href=\"".$leafpath."/leaflet.css\" type=\"text/css\"/>\n";
  echo "<!--[if lte IE 8]><link rel=\"stylesheet\" href=\"".$leafpath."/leaflet.ie.css\" /><![endif]-->\n";

  $imgsize = 15;
  echo "<img alt=\"\" title=\"\" width=\"$imgsize\" height=\"$imgsize\" src=\"".$leafpath."/rnn.png\"/>&nbsp;R&eacute;serve naturelle nationale ou de Corse / ";
  echo "<img alt=\"\" title=\"\" width=\"$imgsize\" height=\"$imgsize\" src=\"".$leafpath."/rnr.png\"/>&nbsp;R&eacute;serve naturelle r&eacute;gionale / ";
  //echo "<img alt=\"\" title=\"\" width=\"$imgsize\" height=\"$imgsize\" src=\"".$leafpath."/rnc.png\"/>&nbsp;R&eacute;serve naturelle de Corse";
  if (php_uname('n') == 'horus')
    echo "<div id=\"divleaf\" style=\"margin-left: -190px; width: 910px; height: 650px;\"></div>\n";
  else
    echo "<div id=\"divleaf\" style=\"width: 720px; height: 650px;\"></div>\n";
  
  echo "<script type=\"text/javascript\" src=\"".$leafpath."/leaflet.js\"></script>\n";
  echo "<script type=\"text/javascript\" src=\"".$leafpath."/rn_map.js\"></script>\n";
}
?>
