<?php
// affichage de la fiche d'une region

require_once('functions.inc');

function _displayRNlogo($node, $rncode)
{
  $rnlogo  = _getfieldvalue($node, $node->field_rn_logo, '', 'filename');
  if ($rnlogo != '')
    {
      //  $logo = 'logo-'.strtolower($rncode).'.jpg';	// valeur par defaut
      $logo = $rnlogo;

      $max_width = 160;
      $logopath = 'sites/default/files/logos/'.$logo;

      if (is_file($logopath))
	{
	  $sizetab = getimagesize($logopath);
	  $width = $sizetab[0];
	  if ($width > $max_width)
	    {
	      //$jsm_path = '../sites/all/libraries/magnifier/jquery.magnifier.js';
	      //echo "<script type=\"text/javascript\" src=\"$jsm_path\"></script>";
	      echo "\n<img class=\"magnify\" alt=\"\" margin: 0;\" width=\"$max_width\" src=\"$logopath\"/><br/>\n";
	    }
	  else
	    echo "\n<img alt=\"\" style=\"float:right; margin: 0;\"  src=\"../$logopath\"/><br/>\n";
	}
    }
  else
    {
      echo "<br/>";
    }
}

function _displayRNGPS($lat, $long)
{
  $latdec = DECtoDMS($lat);
  if (isset($latdec['deg']))
    {
      echo "<b>GPS</b><br/><i>";
      echo $latdec['deg']."&deg; ";
      echo $latdec['min']."&apos; ";
      echo floor($latdec['sec'])."&quot;"; 
      if ($latdec['deg'] > 0)
	echo " N ";
      else
	echo " S ";
    }
  $longdec = DECtoDMS($long);
  if (isset($longdec['deg']))
    {
      echo ", ".$longdec['deg']."&deg; ";
      echo $longdec['min']."&apos; ";
      echo floor($longdec['sec'])."&quot;";
      if ($longdec['deg'] < 0)
	echo " W";
      else
	echo " E";
      echo "</i><br/><br/>\n";
    }
}

function _displayRNLocalisation($regtab, $dpttab, $comtab)
{
  // code pour la region 
  $nidtab = array();
  foreach ($regtab as $rg)
    $nidtab[] = $rg['nid'];

  if (count($nidtab) > 0)
    {
      $regtab2 = array();
      foreach($nidtab as $nid)
        {
	  $rgnode = node_load($nid);
	  $rgname = trim($rgnode->title);
	  $rgname = mb_convert_encoding($rgname, 'ISO-8859-1', 'UTF-8');
	  $regurl = str_replace(' ', '-', strtolower(no_accents($rgname)));
	  $regurl = str_replace("'", '-', $regurl);
	  $regtab2[] = "<a href=\"$regurl\">".he($rgname)."</a>";
        }

      if (count($regtab2) > 1)
	echo '<b>R&Eacute;GIONS</b><br/><i>';
      else
	echo '<b>R&Eacute;GION</b><br/><i>';
      echo implode(' / ', $regtab2);
      echo "</i><br /><br />\n";
    }


  // code pour le departement
  _displayDepartements($dpttab);
  echo "<br /><br />\n";


  // affichage des communes
  _displayCommunes($comtab);
  echo "<br /><br />\n";
}


// affichage d'un organisme de gestion
function _displayOGnode($ognid)
{
  $og = node_load($ognid);
  $ogtitle = mb_convert_encoding($og->title,  'ISO-8859-1', 'UTF-8');
  $oglogo  = _getfieldvalue($og, $og->field_og_logo, '', 'filename');
  $ogadr1  = mb_convert_encoding(_getfieldvalue($og, $og->field_og_adresse1, ''),  'ISO-8859-1', 'UTF-8');
  $ogadr2  = mb_convert_encoding(_getfieldvalue($og, $og->field_og_adresse2, ''),  'ISO-8859-1', 'UTF-8');
  $ogcopos = _getfieldvalue($og, $og->field_code_postal, '');
  $ogville = mb_convert_encoding(_getfieldvalue($og, $og->field_ville, ''),  'ISO-8859-1', 'UTF-8');
  $ogtel = _getfieldvalue($og, $og->field_telephone, '');
  $ogfax = _getfieldvalue($og, $og->field_fax, '');
  $ogweb = _getfieldvalue($og, $og->field_site_web, '');
  if (strstr($ogweb, 'http:'))
    $ogweb = str_replace('http://', '', $ogweb);
  if (strstr($ogweb, 'https:'))
    $ogweb = str_replace('https://', '', $ogweb);
  
  $ogemail = _getfieldvalue($og, $og->field_email, '');
	
  if ($oglogo != '')		 
    {
      global $base_url;
      $logopath = $base_url."/sites/default/files/logos/".$oglogo;	
      if ($ogweb != '')
      	echo "<a target=\"_blank\" href=\"http://$ogweb\">";
      echo "<img src=\"$logopath\" width=\"190\">";
      if ($ogweb != '')
        echo "</a>";
    }

  echo "<strong>".he($ogtitle)."</strong> <br />\n";
  if ($ogadr1 != '')
    echo he($ogadr1)."<br />\n";
  if ($ogadr2 != '')
    echo he($ogadr2)."<br />\n";
  if ($ogcopos != '' && $ogville != '')
    echo he($ogcopos)." ".he($ogville)."<br />\n";
  if ($ogtel != '')
    echo "T&eacute;l.&nbsp; ". he($ogtel)."<br />\n";
  if ($ogfax != '')
    echo "Fax&nbsp; ". he($ogfax)."<br />\n";
  echo "<br/>";
  if ($ogemail != '')
    {
      $mail = explode('@', $ogemail);
      echo "<script type=\"text/javascript\">MT('".$mail[0]."', '".$mail[1]."')</script><br/>\n";
    }
  if ($ogweb != '')
    {
      $ogweb2 = rtrim($ogweb, '/');
      echo "<a target=\"_blank\" href=\"http://$ogweb\">".$ogweb2."</a>\n";
    }
}

function _displayFicheRN()
{
  //echo "Fiche de la reserve";
  $nid = arg(1);
  if ($nid != '')
    {
      $node = node_load($nid);
      if (isset($node->field_code))
        {
	  $rncode = _getfieldvalue($node, $node->field_code, '');
	  $name = $node->title;
	  _displayRNlogo($node, $rncode);

	  $latitude  = _getfieldvalue($node, $node->field_latitude, '0.0');
	  $longitude = _getfieldvalue($node, $node->field_longitude, '0.0');

	  echo "<b>TYPE</b><br/><i>";
	  $typetab = array('RNN' => 'R&eacute;serve naturelle nationale',
			   'RNR' => 'R&eacute;serve naturelle r&eacute;gionale',
			   'RNC' => 'R&eacute;serve naturelle de Corse');

	  $type = _getfieldvalue($node, $node->field_typern, '');
	  echo $typetab[$type]." ($type)</i><br/><br/>\n";

	  $inpn = _getfieldvalue($node, $node->field_inpn, '');
	  $wdpa = _getfieldvalue($node, $node->field_code_wdpa, '');
	  echo "<b>CODE</b><br/><i>";
	  echo $rncode."</i>";
	  $inpnurl = 'http://inpn.mnhn.fr/espace/protege/'.$inpn;
	  if ($inpn != '')
	    echo " / <a target=\"_blank\" href=\"$inpnurl\">".$inpn."</a>";
	  $wdpaurl = 'http://protectedplanet.net/sites/'.$wdpa;
	  if ($wdpa != '')
	    echo "<br/><br/><b>Id WDPA</b><br/><a target=\"_blank\" href=\"$wdpaurl\">".$wdpa."</a>";
	  echo "<br/><br/>\n";

	  $rnweb = _getfieldvalue($node, $node->field_website, '');
	  if ($rnweb != '')
	    {
	      echo "<b>SITE INTERNET</b><br/><i>";
        if (strstr($rnweb, 'https:'))
          $rnweb = str_replace('https://', '', $rnweb);
	      if (strstr($rnweb, 'http:'))
          $rnweb = str_replace('http://', '', $rnweb);
        $rnweb2 = rtrim($rnweb, '/');
	      if (strlen($rnweb2) > 60)
	        {
		  $prefix = substr($rnweb2, 0, 30);
		  $suffix = substr($rnweb2, -30);
		  $rnweb2 = $prefix."...".$suffix;
		}
	      echo "<a target=\"_blank\" href=\"http://$rnweb\">$rnweb2</a></i><br /><br />\n";
	    }

	  // lien google pour la recherche d'info
	  //$hostname = php_uname('n');
	  //if ($hostname == 'horus')
	  // {
	  //   $q = urlencode('"reserve naturelle" '.$name);
	  //   $u = "http://www.google.fr/?hl=fr&source=hp&q=$q";
	  //  echo "<a href=\"$u\">Google</a><br><br>";
	  //}

	  _displayRNGPS($latitude, $longitude);

	  $surface = _getfieldvalue($node, $node->field_surface, 0);
	  if ($surface != 0)
	    {
	      echo "<b>SUPERFICIE CLASS&Eacute;E</b><br/><i>";
	      $surf = number_format($surface,2,',',' ');
	      echo str_replace(' ', "&thinsp;", $surf)." hectares</i><br/><br/>\n";
	    }

	  echo "<b>CLASSEMENT</b><br/><i>";
	  $datere = _getfieldvalue($node, $node->field_datere, '');
	  $datecl = _getfieldvalue($node, $node->field_datecl, '');
	  if ($datere != '')
	    echo _displayDate($datere)."</i><br/><br/>\n";
	  else
	    echo _displayDate($datecl)."</i><br/><br/>\n";
	    
	  $conserv = _getfieldvalue($node, $node->field_conservateur, '');
	  if ($conserv != '')	
	    { 
	      $conserv = mb_convert_encoding($conserv, 'ISO-8859-1', 'UTF-8');
	      echo "<b>CONSERVATEUR</b><br/><i>";
	      echo he($conserv)."</i><br /><br />\n";
	    }

	  $regtab = _getfieldvaluetab($node, $node->field_regionrn, array());
	  $dpttab = _getfieldvaluetab($node, $node->field_departement, array());
	  $comtab = _getfieldvaluetab($node, $node->field_communes, array());

	  // affichage region, departement, communes
	  _displayRNLocalisation($regtab, $dpttab, $comtab);

	  // milieux rencontres
	  $milieux = array();
	  $milvalues = _getfieldvaluetab($node, $node->field_milieux, array());
	  foreach ($milvalues as $mil)
	    {
	      if (trim($mil['value']) != '')
		$milieux[] = $mil['value'];
	    }
	  if (count($milieux) > 0)
	    {
	      include('corine.inc');
	      echo '<b>MILIEUX</b><br/><i>';
	      foreach($milieux as $codemi)
		$miltab[] = trim($codecorine[$codemi]);
	      $milieux = implode(', ', $miltab);
	      echo he($milieux)."</i><br /><br />\n";
	    }
	    

	  // patrimoine geologique
	  $patgeol = _getfieldvalue($node, $node->field_patgeol, 0);
	  if ($patgeol == 1)
	    echo '<b>PATRIMOINE<br>G&Eacute;OLOGIQUE</b><br/><i>Oui</i><br /><br />';

	  if ($rncode != 'RNN161') // TAFF
	    echo _findNearest($rncode, $latitude, $longitude);

	  // organismes de gestion
	  $og1 = _getfieldnid($node, $node->field_og1, '');
	  if ($og1 != '')
	    {
	      echo '<h3>G&eacute;r&eacute;e par</h3>';
	      _displayOGnode($og1);
	    }
	  $og2 = _getfieldnid($node, $node->field_og2, '');
	  if ($og2 != '')
	    {
	      echo "<br/><br/>";
	      _displayOGnode($og2);
	    }
	  $og3 = _getfieldnid($node, $node->field_og3, '');
	  if ($og3 != '')
	    {
	      echo "<br/><br/>";
	      _displayOGnode($og3);
	    }
	}
    }
}
?>
