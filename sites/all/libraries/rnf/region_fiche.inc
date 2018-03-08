<?php
require_once('functions.inc');

function _displayMeddelogo()
{
  $max_width = 160;
  $logo = 'logo-medde.png';
  $logopath = 'sites/default/files/images/'.$logo;
  if (is_file($logopath))
    {
      echo "\n<a target=\"_blank\" title=\"Ministère du Développement durable\" href=\"http://www.developpement-durable.gouv.fr/\">";
      echo "<img alt=\"\" src=\"$logopath\"/></a><br/><br/>\n";
    }
}

function _displayRegionlogo($name)
{
  $max_width = 160;
  if (strstr($name, "("))
    {
       $tab = explode("(", $name);
       $url = _makeURL(trim($tab[0], ' '));
    }
  else
    {
      $url = _makeURL($name);
    }	
  $logo = $url.'.png';
  $logo = str_replace('%27', '-', $logo);
  $logopath = 'sites/default/files/logos/region_'.$logo;
  if (is_file($logopath))
    {
      $sizetab = getimagesize($logopath);
      $width = $sizetab[0];
      if ($width > $max_width)
	{
	  //$jsm_path = '../sites/all/libraries/magnifier/jquery.magnifier.js';
	  //echo "<script type=\"text/javascript\" src=\"$jsm_path\"></script>";
	  echo "\n<img class=\"magnify\" alt=\"\" margin: 0;\" width=\"$max_width\" src=\"$logopath\"/><br/><br/>\n";
	}
      else
	echo "\n<img alt=\"\" src=\"$logopath\"/><br/><br/>\n";
    }
  //else
  //echo "Cannot find $logopath<br/>";
}

function _displayRegionSite($node)
{
  if (isset($node->field_website[$node->language][0]['value']))
    $regweb = trim($node->field_website[$node->language][0]['value']);
  else 
    $regweb = ''; 
  if ($regweb != '')
    {
      echo "<b>SITE INTERNET</b><br/><br/><i>";
      if (strstr($regweb, 'http'))
	$regweb = str_replace('http://', '', $regweb);
      $regweb2 = rtrim($regweb, '/');
      echo "<a target=\"_blank\" href=\"http://$regweb\">$regweb2</a></i>";
      echo "<br /><br /><br />";
    }
}

function _displayFicheRegion()
{
  //echo "Fiche de la region";
  $isproto = true;
  $nid = arg(1);
   if ($nid != '')
     {
       $node = node_load($nid);
       if (isset($node->field_coderegion[$node->language][0]))
	 $mylang = $node->language;
       else
	 $mylang = 'und';
       if (isset($node->field_coderegion[$mylang][0]['value']))
         {
            $rgcode = $node->field_coderegion[$mylang][0]['value'];
	    $rgname = mb_convert_encoding($node->title, 'ISO-8859-1', 'UTF-8');
	    $regrn = node_load_multiple(_getRNnidbyRegion($nid));

	    $rnnlinks = '';
	    $rnrlinks = '';
	    $nbrnr = 0;
	    $nbrnn = 0;
	    $nbrnc = 0;
	    foreach ($regrn as $rn)
	      {
		$type  = $rn->field_typern[$mylang][0]['value'];
		$rncode  = $rn->field_code[$mylang][0]['value'];
		if ($type == 'RNN')
		  $nbrnn++;
		else if ($type == 'RNR')
		  $nbrnr++;
		else
		   $nbrnc++;
		$name  = mb_convert_encoding($rn->title, 'ISO-8859-1', 'UTF-8');

		$url = _makeURL($name); 
		if ($rncode == 'RNR191') // cas special RNR Grand-Lieu
		  $url .= "-rnr";
		$link = "<a href=\"$url\">".he($name)."</a><br/><br/>\n";
		if ($type == 'RNN')
		  $rnnlinks .= $link;
		else
		  $rnrlinks .= $link;
	      }
	    //echo "<br/>";

	    //echo "<b>CODE INSEE</b><br/><i>";
	    //echo $rgcode."</i><br/><br/>";

	    if ($rgcode != 94) // Corse
	      {	
		if (!$isproto)
		  _displayMeddelogo();

		echo "<b>R&eacute;serves nationales</b>: <i>";
		echo $nbrnn."</i><br/><br/>";
		echo "<i>".$rnnlinks."</i><br/>";

		if (!$isproto)
		  {
		    _displayRegionlogo($rgname);
		    _displayRegionSite($node);
		  }

		if ($nbrnr != 0)
		  {
		    echo "<b>R&eacute;serves r&eacute;gionales</b>: <i>";
		    echo $nbrnr."</i><br/><br/>";
		    echo "<i>".$rnrlinks."</i><br/><br/>";
		  }
	      }
	    else
	      {
		if (!$isproto)
		  {
		    _displayRegionlogo($rgname);
		    _displayRegionSite($node);
		  }

		echo "<b>R&eacute;serves de Corse</b>: <i>";
		echo $nbrnc."</i><br/><br/>"; 
		echo "<i>".$rnrlinks."</i><br/><br/>";
	      }
         }
     }
}
?>
