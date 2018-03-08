<?php
require_once('functions.inc');

// situation de la region sur une carte
// cartes obtenues avec planiglobe.com
function _displaySituationRegion()
{ 
  include('region_map.inc');

  $rgcode = '';
  $nid = arg(1);
  if ($nid != '')
     {
       $node = node_load($nid);
       if (isset($node->field_coderegion))
	 {
	   // cas d'une region
	   if (isset($node->field_coderegion[$node->language][0]['value']))
	     {
	   // code de la region
	       $rgcode = $node->field_coderegion[$node->language][0]['value'];
	     }
	 }

       if (isset($node->field_regionrn))
	 {
	   // cas des partenaires
	   $regtab  = _getfieldvaluetab($node, $node->field_regionrn, array());
	   foreach($regtab as $reg)
	    {
	      $rgnid = $reg['nid']; 
	      $rgnode = node_load($rgnid);
	      $rgcode = _getfieldvalue($rgnode, $rgnode->field_coderegion, '');
	      break;
	    }
	 }
     }

  if ($rgcode == '')
    $imgvide = 'region_vide.png';
  else
    $imgvide = "region_$rgcode.png";

  include('regions.inc');
	   
  echo "<script type=\"text/javascript\">\n";
  echo "function swapRegion(zone,nr){\n";
  echo "var reg=document.getElementById('hovered')\n";
  echo "reg.src='/sites/all/themes/theme_rnf/images/region_'+nr+'.png'\n";
  echo "zone.onmouseout=function(){reg.src='/sites/all/themes/theme_rnf/images/$imgvide'}\n";
  echo "}\n";
  echo "</script>\n";

  global $base_url;
  $fondpath = $base_url.'/sites/all/themes/theme_rnf/images/';
  $imgfond = $fondpath.'fond_regions.png'; 
  $imghover = $fondpath.$imgvide; 

  echo "<div id=\"pic\" style=\"height: 250px; width: 180px; background-image: url('$imgfond'); background-repeat: no-repeat\">\n";
  echo "<img id=\"hovered\" src=\"$imghover\" usemap=\"#regionMap\" width=\"180\" height=\"250\" />";
  echo "<map name=\"regionMap\">\n";
  foreach($regmap as $rcode => $rzone)
    {
      if (strstr($rcode, '-'))
        $rcode = substr($rcode, 0, 1);
      if ($rgcode != $rcode)
	{
	  $name = $regions[$rcode];
	  $alt  = htmlentities($name);
	  $url = _makeURL($name);
	  echo "<area href=\"$url\" alt=\"$alt\" title=\"$alt\" onmouseover=\"swapRegion(this,$rcode)\" shape=\"polygon\" coords=\"$rzone\">\n";
	}
    }
  echo "</map>";
  echo "</div>";
}
?>
