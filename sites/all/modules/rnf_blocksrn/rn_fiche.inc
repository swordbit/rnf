<?php

module_load_include('inc', 'rnf_blocksrn', 'utils');
module_load_include('inc', 'rnf_blocksrn', 'departements');

function get_og($ognid)
{
  $result = '';

  $og = node_load($ognid);
  $ogtitle = mb_convert_encoding($og->title,  'ISO-8859-1', 'UTF-8');
  $oglogo  = gfv($og, 'field_og_logo', '', 'filename');
  $ogadr1  = mb_convert_encoding(gfv($og, 'field_og_adresse1', ''),  'ISO-8859-1', 'UTF-8');
  $ogadr2  = mb_convert_encoding(gfv($og, 'field_og_adresse2', ''),  'ISO-8859-1', 'UTF-8');
  $ogcopos = gfv($og, 'field_code_postal', '');
  $ogville = mb_convert_encoding(gfv($og, 'field_ville', ''),  'ISO-8859-1', 'UTF-8');
  $ogtel = gfv($og, 'field_telephone', '');
  $ogfax = gfv($og, 'field_fax', '');
  $ogweb = gfv($og, 'field_site_web', '');
  if (strstr($ogweb, 'http'))
    $ogweb = str_replace('http://', '', $ogweb);
  $ogemail = gfv($og, 'field_email', '');
	
  if ($oglogo != '')		 
    {
      global $base_url;
      $logopath = $base_url."/sites/default/files/logos/".$oglogo;	
      if ($ogweb != '')
      	$result .= "<a target=\"_blank\" href=\"http://$ogweb\">";
      $result .= "<img src=\"$logopath\" width=\"190\">";
      if ($ogweb != '')
        $result .= "</a>";
    }

  $result .= "<strong>".hent($ogtitle)."</strong> <br />\n";
  if ($ogadr1 != '')
    $result .= hent($ogadr1)."<br />\n";
  if ($ogadr2 != '')
    $result .= hent($ogadr2)."<br />\n";
  if ($ogcopos != '' && $ogville != '')
    $result .= hent($ogcopos)." ".hent($ogville)."<br />\n";
  if ($ogtel != '')
    $result .= "T&eacute;l.&nbsp; ". hent($ogtel)."<br />\n";
  if ($ogfax != '')
    $result .= "Fax&nbsp; ". hent($ogfax)."<br />\n";
  $result .= "<br/>";
  if ($ogemail != '')
    {
      $mail = explode('@', $ogemail);
      $result .= "<script type=\"text/javascript\">MT('".$mail[0]."', '".$mail[1]."')</script><br/>\n";
    }
  if ($ogweb != '')
    {
      $ogweb2 = rtrim($ogweb, '/');
      $result .= "<a target=\"_blank\" href=\"http://$ogweb\">".$ogweb2."</a>\n";
    }
  return $result;
}

function get_rn_logo($node, $rncode)
{
  $result = '';
  $rnlogo  = gfv($node, 'field_rn_logo', '', 'filename');
  if ($rnlogo != '')
    {
      //  $logo = 'logo-'.strtolower($rncode).'.jpg';	// valeur par defaut
      $max_width = 160;
      $logopath = 'sites/default/files/logos/'.$rnlogo;
      if (is_file($logopath))
	{
	  $sizetab = getimagesize($logopath);
	  $width = $sizetab[0];
	  if ($width > $max_width)
	    {
	      $result = "\n<img class=\"magnify\" alt=\"\" margin: 0;\" width=\"$max_width\" src=\"$logopath\"/><br/>\n";
	    }
	  else
	    $result = "\n<img alt=\"\" style=\"float:right; margin: 0;\"  src=\"../$logopath\"/><br/>\n";
	}
    }
  else
    {
      $result = "<br/>";
    }
  return $result;
}

function get_rn_gps($lat, $long)
{
  $result = '';

  $latdec = dec_to_dms($lat);
  if (isset($latdec['deg']))
    {
      $result .= "<b>GPS</b><br/><i>";
      $result .= $latdec['deg']."&deg; ";
      $result .= $latdec['min']."&apos; ";
      $result .= floor($latdec['sec'])."&quot;"; 
      if ($latdec['deg'] > 0)
	$result .= " N ";
      else
	$result .= " S ";
    }
  $longdec = dec_to_dms($long);
  if (isset($longdec['deg']))
    {
      $result .= ", ".$longdec['deg']."&deg; ";
      $result .= $longdec['min']."&apos; ";
      $result .= floor($longdec['sec'])."&quot;";
      if ($longdec['deg'] < 0)
	$result .= " W";
      else
	$result .= " E";
      $result .= "</i><br/><br/>\n";
    }
  return $result;
}

function get_rn_departements($dpttab)
{
  $result = '';
  $depts = array();
  foreach($dpttab as $dptval)
    {
      if (trim($dptval['value']) != '')
        $depts[] = $dptval['value'];
    }

  if (count($depts) > 0)
    {
      if (count($depts) > 1)
	$result .= '<b>D&Eacute;PARTEMENTS</b><br/><i>';
      else
	$result .= '<b>D&Eacute;PARTEMENT</b><br/><i>';

      $dpttab2 = array();
      include('departements.inc');
      foreach ($depts as $dptcode)
	if (isset($departements[$dptcode]))
	  $dpttab2[] = $departements[$dptcode];

      sort($dpttab2);
      $result .= hent(implode(' / ', $dpttab2));
      $result .= "</i>";
    }
  return $result;
}

function get_rn_communes($comtab)
{
  $result = '';
  $coms = array();
  foreach ($comtab as $comval)
    if (trim($comval['value']) != '')
      $coms[] = mb_convert_encoding($comval['value'], 'ISO-8859-1', 'UTF-8');
  
  uasort($coms, 'my_str_coll');

  if (count($coms) > 0)
    {
      if (count($coms) > 1)
	$result .= '<b>COMMUNES</b><br/><i>';
      else
	$result .= '<b>COMMUNE</b><br/><i>';
      $result .= hent(implode(', ', $coms)).'</i>';
    }
  return $result;
}

function get_rn_localisation($regtab, $dpttab, $comtab)
{
  $result = '';

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
	  $regurl = str_replace(' ', '-', strtolower(clean_accents($rgname)));
	  $regurl = str_replace("'", '-', $regurl);
	  $regtab2[] = "<a href=\"$regurl\">".hent($rgname)."</a>";
        }

      if (count($regtab2) > 1)
	$result .= '<b>R&Eacute;GIONS</b><br/><i>';
      else
	$result .= '<b>R&Eacute;GION</b><br/><i>';
      $result .= implode(' / ', $regtab2);
      $result .= "</i><br /><br />\n";
    }

  // code pour le departement
  $result .= get_rn_departements($dpttab);
  $result .= "<br /><br />\n";


  // affichage des communes
  $result .= get_rn_communes($comtab);
  $result .= "<br /><br />\n";

  return $result;
}

function dist_to_rn($rnlat, $rnlong, $lat, $long)
{
  $r = 6378;                    // rayon terrestre
  $lat = deg2rad($lat);
  $long = deg2rad($long);
  
  $dlong = $rnlong - $long;

  $prod1 = cos($rnlat)*cos($lat)*cos($dlong);
  $prod2 = sin($rnlat)*sin($lat);

  $dist = $r * acos($prod1 + $prod2);

  return ceil($dist);
}

function find_nearest($rncode, $lat, $lon)
{ 
  $disttab = array();
  $offset_lat = 1.58;
  $offset_lon = 1.72;
  $latrad = deg2rad($lat);
  $lonrad = deg2rad($lon);
  $latmin = $lat - $offset_lat;
  $latmax = $lat + $offset_lat;
  $lonmin = $lon - $offset_lon;
  $lonmax = $lon + $offset_lon;
  
  // recherche des RN publiee 
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', 'reserve')
	->fieldCondition('field_publie', 'value', 1, '=')
    	->fieldCondition('field_latitude',  'value', $latmin, '>')
    	->fieldCondition('field_latitude',  'value', $latmax, '<')
    	->fieldCondition('field_longitude', 'value', $lonmin, '>')
    	->fieldCondition('field_longitude', 'value', $lonmax, '<')
    	->addMetaData('account', user_load(1)); // run the query as user 1

  $result = $query->execute();
  if (isset($result['node'])) 
    {
      $nids = array_keys($result['node']);
      $nodes = node_load_multiple($nids);

      if (count($nodes) != 0)
        {
	  foreach($nodes as $n)
	    {
	       if ($rncode != gfv($n, 'field_code', ''))
	         {
		   $myname = mb_convert_encoding($n->title, 'ISO-8859-1', 'UTF-8');
		   $mylon = gfv($n, 'field_longitude', 0.0);
		   $mylat = gfv($n, 'field_latitude', 0.0);
		   $dist = dist_to_rn($latrad, $lonrad, $mylat, $mylon);
		   $disttab[$myname] = $dist;
                 }
            }
        }
    }

  if (count($disttab) > 0)
    {
      // tri des distances
      asort($disttab);
      $max = 6; 		// $max RN affichees
      if (count($disttab) < $max)
	$max = count($disttab);
      if ($rncode == 'RNN125' || $rncode == 'RNN29' || $rncode == 'RNR291') // Martinique
       	$max = 2;
      if ($rncode == 'RNN142' || $rncode == 'RNN173') // Guadeloupe
       	$max = 1;
      $rnlist = array_keys($disttab);
      $result = "<b>AUTRES R&Eacute;SERVES<br>NATURELLES<br>&Agrave; PROXIMIT&Eacute;</b><br/>";
      $result .= "<div style=\"font-style: italic;\">";
      for($j = 0; $j < $max; $j++)
	{
	  $name = $rnlist[$j];
	  $url = make_url($name);
	  if ($j == $max - 1)
	    $result.= "<a href=\"$url\">".hent($name)."</a><br/>";
	  else
	    $result.= "<a href=\"$url\">".hent($name)."</a>, ";
	}
      $result .= "</div><br/>\n";
      return $result;
    }

  return "";
}

function get_fiche_rn()
{  
  $fiche = '';
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $rnnid = arg(1);
    if ($rnnid != 'add')
      { 
	$node = node_load($rnnid);
	if (isset($node->field_code))
	  {
	    $rncode = gfv($node, 'field_code');
	    $name = $node->title;
	    $fiche .= get_rn_logo($node, $rncode);

	    $latitude  = gfv($node, 'field_latitude', '0.0');
	    $longitude = gfv($node, 'field_longitude', '0.0');

	    $fiche .= "<b>TYPE</b><br/><i>";
	    $typetab = array('RNN' => 'R&eacute;serve naturelle nationale',
			     'RNR' => 'R&eacute;serve naturelle r&eacute;gionale',
			     'RNC' => 'R&eacute;serve naturelle de Corse');

	    $type = gfv($node, 'field_typern', '');
	    $fiche .= $typetab[$type]." ($type)</i><br/><br/>\n";

	    $inpn = gfv($node, 'field_inpn', '');
	    $wdpa = gfv($node, 'field_code_wdpa', '');
	    $fiche .= "<b>CODE</b><br/><i>$rncode</i>";

	    $inpnurl = 'http://inpn.mnhn.fr/espace/protege/'.$inpn;
	    if ($inpn != '')
	      $fiche .= " / <a target=\"_blank\" href=\"$inpnurl\">".$inpn."</a>";

	    $wdpaurl = 'http://protectedplanet.net/'.$wdpa;
	    if ($wdpa != '')
	      $fiche .= "<br/><br/><b>Id WDPA / EUNIS</b><br/><a target=\"_blank\" href=\"$wdpaurl\">".$wdpa."</a>";
      $eunisurl = 'https://eunis.eea.europa.eu/sites/'.$wdpa;
	      $fiche .= " / <a target=\"_blank\" href=\"$eunisurl\">".$wdpa."</a>";
	    $fiche .= "<br/><br/>\n";

	    $rnweb = gfv($node, 'field_website', '');
	    if ($rnweb != '')
	      {
		$fiche .= "<b>SITE INTERNET</b><br/><i>";
		if (strstr($rnweb, 'http'))
		  $rnweb = str_replace('http://', '', $rnweb);
		$rnweb2 = rtrim($rnweb, '/');
		if (strlen($rnweb2) > 60)
		  {
		    $prefix = substr($rnweb2, 0, 30);
		    $suffix = substr($rnweb2, -30);
		    $rnweb2 = $prefix."...".$suffix;
		  }
		$fiche .= "<a target=\"_blank\" href=\"http://$rnweb\">$rnweb2</a></i><br /><br />\n";
	      }

	    // lien google pour la recherche d'info
	    //$hostname = php_uname('n');
	    //if ($hostname == 'horus')
	    // {
	    //   $q = urlencode('"reserve naturelle" '.$name);
	    //   $u = "http://www.google.fr/?hl=fr&source=hp&q=$q";
	    //  echo "<a href=\"$u\">Google</a><br><br>";
	    //}

	    $fiche .= get_rn_gps($latitude, $longitude);

	    $surface = gfv($node, 'field_surface', 0);
	    if ($surface != 0)
	      {
		$fiche .= "<b>SUPERFICIE CLASS&Eacute;E</b><br/><i>";
		$surf = number_format($surface,2,',',' ');
		$fiche .= str_replace(' ', "&thinsp;", $surf)." hectares</i><br/><br/>\n";
	      }

	    $fiche .= "<b>CLASSEMENT</b><br/><i>";
	    $datecl = gfv($node, 'field_datecl', '');
	    $fiche .= disp_date($datecl)."</i><br/><br/>\n";

	    $datere = gfv($node, 'field_datere', '');
	    if ($datere != '')
	      {
		$fiche .= "<b>RECLASSEMENT</b><br/><i>";
		$fiche .= disp_date($datere)."</i><br/><br/>\n";
	      }

	    $conserv = gfv($node, 'field_conservateur', '');
	    if ($conserv != '')	
	      { 
		$conserv = mb_convert_encoding($conserv, 'ISO-8859-1', 'UTF-8');
		$fiche .= "<b>CONSERVATEUR</b><br/><i>";
		$fiche .= hent($conserv)."</i><br /><br />\n";
	      }

	    $regtab = gfvtab($node, 'field_regionrn', array());
	    $dpttab = gfvtab($node, 'field_departement', array());
	    $comtab = gfvtab($node, 'field_communes', array());

	    // affichage region, departement, communes
	    $fiche .= get_rn_localisation($regtab, $dpttab, $comtab);

	    // milieux rencontres
	    $milieux = array();
	    $milvalues = gfvtab($node, 'field_milieux', array());
	    foreach ($milvalues as $mil)
	      {
		if (trim($mil['value']) != '')
		  $milieux[] = $mil['value'];
	      }
	    if (count($milieux) > 0)
	      {
		$fiche .= '<b>MILIEUX</b><br/><i>';
		include('corine.inc');
		//module_load_include('inc', 'rnf_blocksrn', 'corine');
		foreach($milieux as $codemi)
		  $miltab[] = trim($codecorine[$codemi]);
		$milieux = implode(', ', $miltab);
		$fiche .= hent($milieux)."</i><br /><br />\n";
	      }
	    
	    // patrimoine geologique
	    $patgeol = gfv($node, 'field_patgeol', 0);
	    if ($patgeol == 1)
	      $fiche .= '<b>PATRIMOINE<br>G&Eacute;OLOGIQUE</b><br/><i>Oui</i><br /><br />';

	    if ($rncode != 'RNN161') // TAFF
	      $fiche .= find_nearest($rncode, $latitude, $longitude);

	    // organismes de gestion
	    $og1 = gfvnid($node, 'field_og1', '');
	    if ($og1 != '')
	      {
		$fiche .= '<h3>G&eacute;r&eacute;e par</h3>';
		$fiche .= get_og($og1);
	      }
	    $og2 = gfvnid($node, 'field_og2', '');
	    if ($og2 != '')
	      {
		$fiche .= "<br/><br/>";
		$fiche .= get_og($og2);
	      }
	    $og3 = gfvnid($node, 'field_og3', '');
	    if ($og3 != '')
	      {
		$fiche .= "<br/><br/>";
		$fiche .= get_og($og3);
	      }
	  }
      }
  }
  return $fiche;
}
