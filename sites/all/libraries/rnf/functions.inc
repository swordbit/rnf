<?php

// suppression des accents
function no_accents($var)
{
  // TODO : AE OE ae oe
  return strtr($var,
   'àáâãäåçèéêëìíîïñòóôõöùúûüýÿÀÁÂÃÄÅÇÈÉÊËÌÍÎÏÑÒÓÔÕÖÙÚÛÜÝ',
   'aaaaaaceeeeiiiinooooouuuuyyAAAAAACEEEEIIIINOOOOOUUUUY');
}

// une fonction de tri specifique qui compare les chaines sans accents
function myStrColl($a, $b)
{
  return strcoll(no_accents($a), no_accents($b));
}

// appel de htmlentities
function he($string)
{
  if (defined('ENT_HTML401'))
    return htmlentities($string, ENT_COMPAT | ENT_HTML401, 'ISO8859-1');
  else
   return htmlentities($string, ENT_COMPAT, 'ISO8859-1');
}

// Converts DMS ( Degrees / minutes / seconds ) 
// to decimal format longitude / latitude
function DMStoDEC($deg, $min, $sec)
{
  return $deg+((($min*60)+($sec))/3600);
} 

// Converts decimal longitude / latitude to DMS
// ( Degrees / minutes / seconds ) 
function DECtoDMS($dec)
{
  // This is the piece of code which may appear to 
  // be inefficient, but to avoid issues with floating
  // point math we extract the integer part and the float
  // part by using a string function.
  
  $vars = explode(".",$dec);
  $deg = $vars[0];
  if (isset($vars[1]))
    $tempma = "0.".$vars[1];
  else
    $tempma = "0.0";

  $tempma = $tempma * 3600;
  $min = floor($tempma / 60);
  $sec = $tempma - ($min*60);

  return array("deg"=>$deg,"min"=>$min,"sec"=>$sec);
}

// pour l'affichage du nombre de RN dans les pages RNN/RNR/RNC
function GetCurMonth()
{
  $mtab = array('01' => 'janvier',   '02' => 'f&eacute;vrier',
		'03' => 'mars',      '04' => 'avril',
		'05' => 'mai',       '06' => 'juin',
		'07' => 'juillet',   '08' => 'ao&ucirc;t',
		'09' => 'septembre', '10' => 'octobre',
		'11' => 'novembre',  '12' => 'd&eacute;cembre');
  $m = date('m');
  return $mtab[$m]." ".date('Y');
}

// fabrication des URL pour les RN
function _makeURL($name)
{
  $namenoa = strtolower(no_accents($name));
  $deltab = array("\xab", "\xa0", "\xbb");
  $namenoa = str_replace($deltab, '', $namenoa);
  $namenosp = strtr($namenoa, " ,('/", "-----");
  $namenosp = str_replace(")", "", $namenosp);
  $namenosp = str_replace('"', "", $namenosp);
  $namenosp = str_replace("*", "-", $namenosp);
  $namenosp = preg_replace("/--*/", "-", $namenosp); 
  
  return (urlencode($namenosp));
}

function _displayLine($title, $value)
{
  echo "<b>$title</b><br/><i>$value</i><br/><br/>\n";
}

// retourne le chemin du dossier files
function _getFilePath()
{
  $hostname = php_uname('n');
  if ($hostname == 'horus')
    $file = '../../sites/default/files/';
  else
    $file = 'sites/default/files/';

  return $file;
}

// affiche les dates
function _displayDate($mysqldate)
{
  $dtab = explode('-', substr($mysqldate, 0, 10));
  return $dtab[2]."/".$dtab[1]."/".$dtab[0];
}

// affiche les departements
function _displayDepartements($dpttab)
{
  $depts = array();
  foreach($dpttab as $dptval)
    {
      if (trim($dptval['value']) != '')
        $depts[] = $dptval['value'];
    }

  if (count($depts) > 0)
    {
      include('departements.inc');
      if (count($depts) > 1)
	echo '<b>D&Eacute;PARTEMENTS</b><br/><i>';
      else
	echo '<b>D&Eacute;PARTEMENT</b><br/><i>';

      $dpttab2 = array();
      foreach ($depts as $dptcode)
	if (isset($departements[$dptcode]))
	  $dpttab2[] = $departements[$dptcode];

      sort($dpttab2);
      echo he(implode(' / ', $dpttab2));
      echo "</i>";
    }
}

function _displayCommunes($comtab)
{
  $coms = array();
  foreach ($comtab as $comval)
    if (trim($comval['value']) != '')
      $coms[] = mb_convert_encoding($comval['value'], 'ISO-8859-1', 'UTF-8');
  
  uasort($coms, 'myStrColl');

  if (count($coms) > 0)
    {
      if (count($coms) > 1)
	echo '<b>COMMUNES</b><br/><i>';
      else
	echo '<b>COMMUNE</b><br/><i>';
      echo he(implode(', ', $coms)).'</i>';
    }
}

// utilise dans import et import_region
// retourne le nid d'une region en fonction de son code INSEE
function _getRegionbycode($rgcode)
{
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->fieldCondition('field_coderegion', 'value', $rgcode, '=')
    ->addMetaData('account', user_load(1)); // run the query as user 1

  $result = $query->execute();
  if (isset($result['node'])) 
    {
      $nids = array_keys($result['node']);
      if (isset($nids[0]))
	return $nids[0];
    }
  return "";
}

function _getRegioninfo($nid)
{
  $result = array();
  if ($nid != '')
    {
      $node = node_load($nid);
      if (isset($node->field_coderegion))
	{
	  $result['code'] = _getfieldvalue($node, $node->field_coderegion, 0);
	  $result['website'] = _getfieldvalue($node, $node->field_website, '');
	  $result['name'] =  mb_convert_encoding($node->title, 'ISO-8859-1', 'UTF-8');
	}
    }
  return $result;

}

// retourne le nid d'un OG en fonction de son nom
function _getOGbyname($ogname)
{
  $ogname =  mb_convert_encoding($ogname, 'UTF-8', 'ISO-8859-1');
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'gestionnaire')
    ->propertyCondition('title', $ogname)
    ->addMetaData('account', user_load(1)); // run the query as user 1

  $result = $query->execute();
  if (isset($result['node'])) 
    {
      $nids = array_keys($result['node']);
      if (isset($nids[0]))
	return $nids[0];
    }
  return "";
}

// retourne le nid d'une reserve en fonction de son code DREAL (RNNxxx)
function _getRNbycode($rncode)
{
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'reserve')
    ->fieldCondition('field_code', 'value', $rncode, '=')
    ->addMetaData('account', user_load(1)); // run the query as user 1

  $result = $query->execute();
  if (isset($result['node'])) 
    {
      $nids = array_keys($result['node']);
      if (isset($nids[0]))
	return $nids[0];
    }
  return "";
}

// retourne tous les nid de noeux de type RN
function _getRNnid($ignorepubli = false)
{
  $query = new EntityFieldQuery();
  if ($ignorepubli)
    {
      $query->entityCondition('entity_type', 'node')
            ->entityCondition('bundle', 'reserve') 
	    ->propertyOrderBy('title', 'asc')
    	    ->addMetaData('account', user_load(1)); // run the query as user 1
    }
  else
    {
      $query->entityCondition('entity_type', 'node')
            ->entityCondition('bundle', 'reserve')
	    ->fieldCondition('field_publie', 'value', 1, '=') 
	    ->propertyOrderBy('title', 'asc')
    	    ->addMetaData('account', user_load(1)); // run the query as user 1
    }

  $result = $query->execute();
  if (isset($result['node'])) 
    {
      $nids = array_keys($result['node']);
      return $nids;
    }
  return array();
}

// retourne tous les nid de noeux de type RN d'une region
function _getRNnidbyRegion($rgnid, $ignorepubli = false)
{
  $query = new EntityFieldQuery();
  if ($ignorepubli)
    {
      $query->entityCondition('entity_type', 'node')
            ->entityCondition('bundle', 'reserve')
    	    ->fieldCondition('field_regionrn', 'nid', $rgnid, '=')
	    ->propertyOrderBy('title', 'asc')
    	    ->addMetaData('account', user_load(1)); // run the query as user 1
    }
  else
    {
      $query->entityCondition('entity_type', 'node')
            ->entityCondition('bundle', 'reserve')
	    ->fieldCondition('field_regionrn', 'nid', $rgnid, '=')
    	    ->fieldCondition('field_publie', 'value', 1, '=')
	    ->propertyOrderBy('title', 'asc')
    	    ->addMetaData('account', user_load(1)); // run the query as user 1
    }

  $result = $query->execute();
  if (isset($result['node'])) 
    {
      $nids = array_keys($result['node']);
      return $nids;
    }
  return array();
}

function _getfieldvalue($node, $field, $defaut, $key = 'value')
{
  if (isset($node->language))
    {
      if (isset($field[$node->language][0][$key]))
        return $field[$node->language][0][$key];
    }
  if (isset($field['und'][0][$key]))
    return $field['und'][0][$key];
 
  return $defaut;
}

function _getfieldnid($node, $field, $defaut)
{
  if (isset($node->language))
    {
      if (isset($field[$node->language][0]['nid']))
        return $field[$node->language][0]['nid'];
    }
  if (isset($field['und'][0]['nid']))
    return $field['und'][0]['nid'];
 
  return $defaut;
}

function _getfieldvaluetab($node, $field, $defaut)
{
  if (isset($node->language))
    {
      if (isset($field[$node->language]))
        return $field[$node->language];
    }
  if (isset($field['und']))
    return $field['und'];
 
  return $defaut;
}

// recherche des infos sur une RN
function _getRNinfo($nid)
{
  $rninfo = array();
  if ($nid != '')
    {
      $node = node_load($nid);
      if (isset($node->field_code))
	{
	  $rninfo['code'] = _getfieldvalue($node, $node->field_code, 0);
	  $rninfo['inpn'] = _getfieldvalue($node, $node->field_inpn, '');
	  $rninfo['wdpa'] = _getfieldvalue($node, $node->field_code_wdpa, '');
	  $rninfo['name'] =  mb_convert_encoding($node->title, 'ISO-8859-1', 'UTF-8');
	  $rninfo['longname'] = $node->title; 
	  $rninfo['type'] = _getfieldvalue($node, $node->field_typern, ''); 
	  $rninfo['surface'] = _getfieldvalue($node, $node->field_surface, 0);
	  $rninfo['website'] = _getfieldvalue($node, $node->field_website, '');

	  $datecl = _getfieldvalue($node, $node->field_datecl, '');
	  $rninfo['datecl'] = _displayDate($datecl);
      
	  $rninfo['datere'] = '';
	  if (isset($node->field_datere[$node->language][0]['value']))
	    {
	      $datere = $node->field_datere[$node->language][0]['value'];
	      if (strstr($datere, '/'))
		$rninfo['datere'] = _displayDate($node->field_datere[$node->language][0]['value']);
	    }    
	  $rninfo['long'] =  _getfieldvalue($node, $node->field_longitude, '0.0');
	  $rninfo['lat']  = _getfieldvalue($node, $node->field_latitude, '0.0');
	
	  $rninfo['conserv'] = _getfieldvalue($node, $node->field_conservateur, '');
	  $rninfo['publie'] = _getfieldvalue($node, $node->field_publie, 'Non');
	  $rninfo['depts'] = _getfieldvaluetab($node, $node->field_departement, array());
	  $rninfo['region'] = _getfieldvaluetab($node, $node->field_regionrn, array());
	  $rninfo['communes'] = _getfieldvaluetab($node, $node->field_communes, array());
	  $rninfo['og1'] = _getfieldnid($node, $node->field_og1, '');
	  $rninfo['og2'] = _getfieldnid($node, $node->field_og2, '');
	  $rninfo['og3'] = _getfieldnid($node, $node->field_og3, '');
	}
    }
  return $rninfo;
}

function YesNo($value)
{
  if ($value != 0)
    return 'Oui';
  else
    return 'Non';
}

//  on a besoin de 
// - type / code / name / lat / long / outrem / surface / publie / datecl

function _getRNtable($ignorepubli = false)
{
  $rntab = array();

  $tabnid = _getRNnid($ignorepubli);
  $nodes = node_load_multiple($tabnid); 
  
  if (count($nodes) != 0)
    {
      foreach($nodes as $n)
	{
	  $datecl = _getfieldvalue($n, $n->field_datecl, '');
	  

	  $rntab[] = array(
		'inpn'    => _getfieldvalue($n, $n->field_inpn, ''),
		'code'    => _getfieldvalue($n, $n->field_code, ''),
		'type'    => _getfieldvalue($n, $n->field_typern, ''),
		'name'    => mb_convert_encoding($n->title, 'ISO-8859-1', 'UTF-8'),
		'surface' => _getfieldvalue($n, $n->field_surface, '0.0'),
		'datecl'  => _displayDate($datecl),
		'long'    => _getfieldvalue($n, $n->field_longitude, 0),
		'lat'     => _getfieldvalue($n, $n->field_latitude, 0),
		'outrem'  => YesNo(_getfieldvalue($n, $n->field_outremer, 0)),
		'publie'  => YesNo(_getfieldvalue($n, $n->field_publie, 0))
	   );
	}
    }

  return $rntab;
}


// statistiques sur les RN, utilise dans les pages RNN/RNR/RNC
function _getRNstat($rntype)
{ 
  $nb_rn  = 0;
  $surf = 0.0;

  $rnlist = _getRNtable();
  switch($rntype)
    {
    case 'RNN':
    case 'RNR':
    case 'RNC':
      foreach ($rnlist as $rn)
	{
	  if ($rn['publie'] == 'Oui' && $rn['type'] == $rntype)
	    {
	      $nb_rn++;
	      $surf += $rn['surface'];
	    }
	}
      break;
    default:
      foreach ($rnlist as $rn)
	{
	  if ($rn['publie'] == 'Oui')
	    {
	      $nb_rn++;
	      $surf += $rn['surface'];
	    }
	}
    }
  return array('nb' => $nb_rn, 'surf' => $surf);
}

function _createRNLimitsForRegion($rntab)
{
  $path = 'sites/default/files/';

  if (count($rntab) != 0)
    {   
      echo "var rn_limit = [\n";
      foreach ($rntab as $rn)
	{
	  $rncode = _getfieldvalue($rn, $rn->field_code, 0);  
	  $limpath = $path.'limites/'.strtolower($rncode).'.json';
	  if (is_file($limpath))
	    {
	      $json = file_get_contents($limpath);
	      echo "$json\n";
	      echo ",\n";
	    }
	}
      echo "];\n";
    }
}

// creation des items JS pour leaflet
// utilise un tableau de nodes
// sert a l'affichage de chaque region
function _createMapItemsForRegion($rntab)
{
  if (count($rntab) != 0)
    {
      echo "var rn_itemList = [\n";
      $zoom = 2;
      foreach ($rntab as $rn)
	{
	  $type   = _getfieldvalue($rn, $rn->field_typern, '');
	  $rncode = _getfieldvalue($rn, $rn->field_code, 0);
	  $name   = mb_convert_encoding($rn->title, 'ISO-8859-1', 'UTF-8');
	  $lat    = _getfieldvalue($rn, $rn->field_latitude, 0);
	  $long   = _getfieldvalue($rn, $rn->field_longitude, 0);
	  $url = _makeURL($name);
	  if ($rncode == 'RNR191') // cas special Grand-Lieu
	    $url .= '-rnr';
          if ($type != '' && $rncode != '' && $lat != 0 && $long != 0)
	    {
	      echo '{latit:"'.$lat.'", longi:"'.$long.'", title:"'.he($name);
	      echo '", url:"'.$url.'", type:"'.$type.'", code:"'.$rncode;
	      echo '", zoom:"'.$zoom.'"},'."\n"; 
            }
	}
      echo "];\n";
   }
}

// creation des items JS pour leaflet
// utilise un tableau de tableau
// sert a l'affichage de la carte generale (rn_map.inc)
function _createMapItems($rntab)
{
  if (count($rntab) != 0)
    {
      echo "var rn_itemList = [\n";
      foreach ($rntab as $rn)
	{
	  $type   = $rn['type'];
	  $rncode = $rn['code'];
	  $name   = $rn['name'];
	  $lat    = $rn['lat'];
	  $long   = $rn['long'];
	  $om     = $rn['outrem'];
	  $surf   = $rn['surface'];
	  $zoom   = 5;
	  if ($surf > 500 || $om == 'Oui')
	     $zoom   = 4;
	  if ($surf > 2000 || $om == 'Oui')
	     $zoom   = 3;
	  if ($surf > 6000 || $om == 'Oui')
	     $zoom   = 2;
	  $url = _makeURL($name);
	  if ($rncode == 'RNR191') // cas special Grand-Lieu
	    $url .= '-rnr';
	  echo '{latit:"'.$lat.'", longi:"'.$long.'", title:"'.he($name);
	  echo '", url:"'.$url.'", type:"'.$type.'", code:"'.$rncode;
	  echo '", zoom:"'.$zoom.'"},'."\n"; 
	}
      echo "];\n";
   }
}

// distance entre deux RN : formules de trigo spherique
function _DistToRN($rnlat, $rnlong, $lat, $long)
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

// recherche des RN les plus proches
// fait une recherche dans les node de type reserve
function _findNearest($rncode, $lat, $lon)
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
	       if ($rncode != _getfieldvalue($n, $n->field_code, ''))
	         {
		   $myname = mb_convert_encoding($n->title, 'ISO-8859-1', 'UTF-8');
		   $mylon = _getfieldvalue($n, $n->field_longitude, 0.0);
		   $mylat = _getfieldvalue($n, $n->field_latitude, 0.0);
		   $dist = _DistToRN($latrad, $lonrad, $mylat, $mylon);
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
	  $url = _makeURL($name);
	  if ($j == $max - 1)
	    $result.= "<a href=\"$url\">".he($name)."</a><br/>";
	  else
	    $result.= "<a href=\"$url\">".he($name)."</a>, ";
	}
      $result .= "</div><br/>\n";
      return $result;
    }

  return "";
}

// affichage de la liste des region sous forme de tableau
function _displayRegions()
{
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'region') 
    ->propertyOrderBy('title', 'asc')
    ->addMetaData('account', user_load(1)); // run the query as user 1

  $result = $query->execute();
  if (isset($result['node'])) 
    {
      $rgnodes = node_load_multiple(array_keys($result['node']));
      $rgtab = array();
      foreach($rgnodes as $rg)
	$rgtab[] = mb_convert_encoding($rg->title, 'ISO-8859-1', 'UTF-8');

      // tri alphabetique sans accents
      setlocale(LC_ALL, "fr_CH.utf8");
      usort($rgtab, 'myStrColl');


      // affichage du tableau
      $nbcol = 2; // nb de colonnes

      $nbreg = count($rgtab); 	// nb de regions
      // combien de lignes ?
      if ($nbreg % $nbcol == 0)
	$nbline = $nbreg / $nbcol;
      else
	$nbline = 1 + floor($nbreg / $nbcol);
      
      $wid = floor(100/$nbcol);	// largeur de colonne

      //echo "Lines : $nbline<br>";
      echo '<div style="vertical-align: top; width: '.$wid.'%; display: inline-block;">';
      $i = 1;
      foreach ($rgtab as $rgname)
        {
	   $id = str_replace(' ', '-', strtolower(no_accents(trim($rgname))));
     	   $urlr = _makeURL($rgname); 
           echo "<a href=\"$urlr\">".he(trim($rgname))."</a><br/>\n";
	   if ($i % $nbline == 0)
	     echo '</div><div style="vertical-align: top; width: '.$wid.'%; display: inline-block;">';
	   $i++;
        }
      echo "</div>";
     }
}

// creation de polygones en JSON
function _createLimit($name, $rncode = '')
{
  $path = 'sites/default/files/';
  $limpath = $path.'limites/'.strtolower($rncode).'.json';
  if (is_file($limpath))
    {
      // on a trouve la limite
      $json = file_get_contents($limpath);
      echo "var limit = $json;\n";
      return true;
    }
 else
    {
      echo "var limit = \"\";\n";
    }
  return false;
}

// lecture des infos complementaires sur la RN
function _readOtherinfoRN($rncode)
{
  $info = '';

  $infofile = _getFilePath().'descriptifs/'.strtolower($rncode).".txt";
  if (is_file($infofile))
    $info = file_get_contents($infofile);
  $info = mb_convert_encoding($info, 'UTF-8', 'ISO-8859-1');

  return nl2br($info);
}

// affichage de la carte d'une RN 
function _displayRNMap()
{
 // on recupere les infos de la RN	
  $nid = arg(1);
  
  $rncode = 0;
  $name = '';
  $publie = false;
  $surf = 0.0;
  $type = 'RNN';
  $lat = 0.0;
  $long = 0.0;

  if ($nid != '')
    {
      $node = node_load($nid);
      if (isset($node->field_code))
	{
	  $rncode   = _getfieldvalue($node, $node->field_code, 0);
	  $name     =  mb_convert_encoding($node->title, 'ISO-8859-1', 'UTF-8');
	  $longname = $node->title; 
	  $type     = _getfieldvalue($node, $node->field_typern, ''); 
	  $surf     = _getfieldvalue($node, $node->field_surface, 0);
	  $long     =  _getfieldvalue($node, $node->field_longitude, '0.0');
	  $lat      = _getfieldvalue($node, $node->field_latitude, '0.0');
	  $publie   = _getfieldvalue($node, $node->field_publie, 'Non');
	  
	  if ($publie == 'Non' || $publie == '0')
	    echo "<span style=\"color: red;\">Avertissement : suite &agrave; la loi \"D&eacute;mocratie de proximit&eacute;\", cette r&eacute;serve n'est actuellement plus class&eacute;e.</span>";

	  //echo '<h3>Localisation</h3>';
	  if ($surf < 5)
	    $zoom = 16;    // moins de 5
	  else if ($surf < 10)
	    $zoom = 15;    // entre 5 et 10
	  else if ($surf < 300)
	    $zoom = 14;    // entre 10 et 300
	  else if ($surf < 1000)
	    $zoom = 13;    // entre 300 et 1000
	  else if ($surf < 3000)
	    $zoom = 12;    // entre 1000 et 3000
	  else if ($surf < 10000)
	    $zoom = 11;   // entre 3000 et 10000
	  else if ($surf < 1000000)
	    $zoom = 10;   // entre 10000 et 1000000 (nouragues)
	  else
	    $zoom = 7; // max = 2270000
           
	  // corrections specifiques de zoom pour certaines RN
	  // trop eclatees ou etalees
	  if ($rncode == 'RNN73')  // geologique-de-haute-provence
	    $zoom = 10;
	  if ($rncode == 'RNN90')  // geologique-du luberon
	    $zoom = 10;
	  if ($rncode == 'RNR270')  // crozon
	    $zoom = 11;
	  if ($rncode == 'RNN170') // coteaux-de-la-seine
	    $zoom = 12;
	  if ($rncode == 'RNR195') // landes-de-lan-bern-et-magoar-pen-vern
	    $zoom = 12;
	  if ($rncode == 'RNR231') // marais-de-la-taute
	    $zoom = 12;
	  if ($rncode == 'RNN145') // pointe de givet
	    $zoom = 12;
	  if ($rncode == 'RNN167') // grottes et pelouses
	    $zoom = 13;
	  if ($rncode == 'RNN108') // iroise
	    $zoom = 12;
	  if ($rncode == 'RNR210') // plateau des landes
	    $zoom = 12;  
	  if ($rncode == 'RNN96') // sites geol de l'essone
	    $zoom = 11;  
	  if ($rncode == 'RNR196') // cragou et vergam
	    $zoom = 12;
	  if ($rncode == 'RNC315') // iles du cap corse
	    $zoom = 11; 
	  if ($rncode == 'RNN5') // banc d'arguin
	    $zoom++; 
	  if ($rncode == 'RNR258') // cailleries
	    $zoom++;
	  if ($rncode == 'RNN137') // estuaire de la seine
	    $zoom--;
	  if ($rncode == 'RNN63') // ile-de-groix
	    $zoom--; 
	  if ($rncode == 'RNN62') // saucats-la-brede
	    $zoom--;
	  if ($rncode == 'RNN11') // haute-vallee-severaise
	    $zoom--; 
	  if ($rncode == 'RNR78') // iles-du-haut-rhone
	    $zoom--;
	  if ($rncode == 'RNR75') // jasseries de colleigne
	    $zoom--;
	  if ($rncode == 'RNN155') // la bassee
	    $zoom--; 
	  if ($rncode == 'RNR191') // lac de gd-lieu-rnr
	    $zoom--;
	  if ($rncode == 'RNN1') // lac luitel
	    $zoom++; 
	  if ($rncode == 'RNR11') // larris et tourbiere
	    $zoom--;
	  if ($rncode == 'RNR257') // marais de briere
	    $zoom--;
	  if ($rncode == 'RNN176') // massif forestier strasbourg
	    $zoom--;
	  if ($rncode == 'RNN89') // ramieres du val de drome
	    $zoom--;
	  if ($rncode == 'RNN141') // rochers et tourbieres bitche
	    $zoom--; 
	  if ($rncode == 'RNN26') // saint-mesmin
	    $zoom--;
	  if ($rncode == 'RNN32') // sept-iles
	    $zoom--; 
	  if ($rncode == 'RNN119') // val-d-allier
	    $zoom--;
	  if ($rncode == 'RNR134') // vallee de la renaudie
	    $zoom--;
	  if ($rncode == 'RNN37') // gd pierre et vitain
	    $zoom--;
	  if ($rncode == 'RNN180') // interet geologique du lot
	    $zoom = 10; 
	  if ($rncode == 'RNR305') // coteaux chemin des dames
	    $zoom = 11;
	  
	  $latoffset = 0.0;
	  $longoffset = 0.0;
	  // corrections de position pour certaines RN
	  if ($rncode == 'RNN167') // grottes et pelouses
	    {
	      $latoffset = 0.02;
	      $longoffset = -0.02;
	    }
	  if ($rncode == 'RNN174') // belle henriette
	    {
	      $longoffset = -0.015;
	    }
	  if ($rncode == 'RNN62') // saucats-la-brede
	    {
	      $longoffset = -0.015;
	    }
	  if ($rncode == 'RNN63') // ile-de-groix
	    { 
	      $latoffset = 0.018;
	      $longoffset = -0.035;
	    } 
	  if ($rncode == 'RNR256') // grand-voyeux
	    {  
	      $latoffset = 0.005;
	      $longoffset = -0.005;
	    }
	  if ($rncode == 'RNN108') // iroise
	    {
	      $latoffset = -0.015;
	      $longoffset = 0.01;
	    } 
	  if ($rncode == 'RNN23') // sagnes de la godivelle
	    {
	      $latoffset = 0.005;
	    }
	  if ($rncode == 'RNN96') // sites geol de l'essone
	    {
	      $latoffset = -0.05;
	    }
	  if ($rncode == 'RNC315') // iles du cap corse
	    {
	      $longoffset = -0.05;
	    }
	  global $base_url;
	  $leafpath  = $base_url."/sites/all/libraries/leaflet";
	  echo "<script type=\"text/javascript\">\n";
	  echo "<!--\n";
	  echo 'function MT(name, domain) { document.write("<a href=\"mailto:" + name + "\u0040" + domain + "\">" + name + "\u0040" + domain + "</a>"); } ';
	  echo "var rn_itemList = [\n";
	  echo "{latit:\"$lat\", longi:\"$long\", ";
	  echo "title:\"".he($name)."\", ";
	  echo "type:\"$type\", code:\"$rncode\"}\n";
	  echo "];\n";
	  $rnlat  = $lat + $latoffset;
	  $rnlong = $long + $longoffset;
	  echo "var rn_lat = $rnlat;\n";
	  echo "var rn_long = $rnlong;\n";
	  $haslimit = _createLimit($name, $rncode); 
	  echo "  var   zoom = $zoom;\n";
	  echo "// -->\n";
	  echo "</script>\n"; 
	  echo "<link rel=\"stylesheet\" href=\"".$leafpath."/leaflet.css\" type=\"text/css\"/>\n";
	  echo "<!--[if lte IE 8]><link rel=\"stylesheet\" href=\"".$leafpath."/leaflet.ie.css\" /><![endif]-->\n";

	  // emplacement de la carte
	  echo "<div id=\"divleaf\" style=\"width: 420px; height: 450px;\"></div>\n";

	  echo "<script type=\"text/javascript\" src=\"".$leafpath."/leaflet.js\"></script>\n";
	  echo "<script type=\"text/javascript\" src=\"".$leafpath."/rn_simple_map.js\"></script>\n";
	}
    }
}

// affichage d'une carte de region
function _displayRegionMap()
{
  $isproto = true;
  $nid = arg(1);
  $rgcode  = 0;
  $website = '';
  $rgname  = '';
  if ($nid != '')
    {
      $node = node_load($nid);
      if (isset($node->field_coderegion))
	{
	  $rgcode  = _getfieldvalue($node, $node->field_coderegion, 0);
	  $website = _getfieldvalue($node, $node->field_website, '');
	  $rgname  =  mb_convert_encoding($node->title, 'ISO-8859-1', 'UTF-8');
	}

      if ($rgcode != 0)
	{
	  // recherche des RNs de la region
	  $regrn = node_load_multiple(_getRNnidbyRegion($nid));

	  // recherche des points extremes pour positionner la carte
	  $minlat = 90.0;
	  $maxlat = -90.0;
	  $minlong = 180.0;
	  $maxlong = -180.0;
	  $sumlong = 0.0;
	  echo "<br/>";
	  foreach ($regrn as $rn)
	    {
	      $lat = _getfieldvalue($rn, $rn->field_latitude, 0.0);
	      if ($lat != 0.0)
		{
		  if ($lat > $maxlat)
		    $maxlat = $lat;
		  if ($lat < $minlat)
		    $minlat = $lat;
		}
	      $long = _getfieldvalue($rn, $rn->field_longitude, 0.0);
	      if ($long != 0.0)
		{
		  if ($long > $maxlong)
		    $maxlong = $long;
		  if ($long < $minlong)
		    $minlong = $long;
		}
	    }

	  $midlat  = ($maxlat + $minlat)/2;
	  $midlong = ($maxlong + $minlong)/2;

	  global $base_url;
	  $leafpath  = $base_url."/sites/all/libraries/leaflet";
	  echo "<script type=\"text/javascript\">\n";
	  echo "<!--\n"; 
	  _createMapItemsForRegion($regrn); 
	  //_createRNLimitsForRegion($regrn);
	  echo "  var   latCenter  = $midlat;\n";
	  echo "  var   longCenter = $midlong;\n";
	  if ($rgcode == 75 || $rgcode == 84 || $rgcode == 76 || $rgcode == 27 || $rgcode == 44)
	    $zoom = 7;
	  else
	    $zoom = 8;
	  echo "  var   zoom = $zoom;\n";
	  echo "var limit = \"\"\n";
	  echo "// -->\n";
	  echo "</script>\n";

	  $leafpath  = $base_url."/sites/all/libraries/leaflet";
	  echo "<link rel=\"stylesheet\" href=\"".$leafpath."/leaflet.css\" type=\"text/css\"/>\n";
	  echo "<!--[if lte IE 8]><link rel=\"stylesheet\" href=\"".$leafpath."/leaflet.ie.css\" /><![endif]-->\n";

	  $imgsize = 15;
	  if ($rgcode == 94)
	    {
	      // Cas de la Corse
	      echo "<img alt=\"\" title=\"\" width=\"$imgsize\" height=\"$imgsize\" src=\"".$leafpath."/rnc.png\"/>&nbsp;R&eacute;serve naturelle de Corse";
	    }
	  else
	    {
	      echo "<img alt=\"\" title=\"\" width=\"$imgsize\" height=\"$imgsize\" src=\"".$leafpath."/rnc.png\"/>&nbsp;R&eacute;serve naturelle nationale";
	      echo " / <img alt=\"\" title=\"\" width=\"$imgsize\" height=\"$imgsize\" src=\"".$leafpath."/rnr.png\"/>&nbsp;R&eacute;serve naturelle r&eacute;gionale";
	    }	    

	  echo "<div id=\"divleaf\" style=\"width: 720px; height: 620px;\"></div>\n";
	  echo "<script type=\"text/javascript\" src=\"".$leafpath."/leaflet.js\"></script>\n";
	  echo "<script type=\"text/javascript\" src=\"".$leafpath."/rn_map.js\"></script>\n";
	}
    }

  if ($isproto)
    { 
      $collectivite = '';
      $representant = ''; 
      $reseau = '';
      $nbcolumn = 0;		// combien de colonnes
      if (isset($node->field_collectivite))
	{ 
	  $nid = _getfieldnid($node, $node->field_collectivite, 0);
	  if ($nid != 0)
	    {
	      $colnode = node_load($nid);
	      $name = $colnode->title;
	      $url = _makeURL(mb_convert_encoding($name, 'ISO-8859-1', 'UTF-8'));
	      $collectivite = "<a href=\"$url\">$name</a>";
	      $nbcolumn++;
	    }
	}
      if (isset($node->field_representant_etat))
	{ 
	  $nid = _getfieldnid($node, $node->field_representant_etat, 0);
	  if ($nid != 0)
	    {
	      $repnode = node_load($nid);
	      $name = $repnode->title;
	      $url = _makeURL(mb_convert_encoding($name, 'ISO-8859-1', 'UTF-8'));
	      $representant = "<a href=\"$url\">$name</a>";
	      $nbcolumn++;
	    }
	}
      if (isset($node->field_reseau_rn))
	{ 
	  $nid = _getfieldnid($node, $node->field_reseau_rn, 0);
	  if ($nid != 0)
	    {
	      $resnode = node_load($nid);
	      $name = $resnode->title;
	      $url = _makeURL(mb_convert_encoding($name, 'ISO-8859-1', 'UTF-8'));
	      $reseau = "<a href=\"$url\">$name</a>";
	      $nbcolumn++;
	    }
	}

      echo "<table width=\"100%\" border=\"0\"><tr>";
      if ($nbcolumn == 1)
	$colwidth = '';
      else if ($nbcolumn == 2)
	$colwidth = ' width="50%"';
      else
	$colwidth = ' width="33%"';

      if ($collectivite != '')
	{
	  echo "<td $colwidth align=\"center\" valign=\"top\" style=\"padding-right: 10px;\">";
	  echo "$collectivite<br>";
	  echo "</td>";
	}

      if ($representant != '')
	{
	  echo "<td $colwidth align=\"center\" valign=\"top\" style=\"padding-right: 10px;\">";
	  echo "$representant<br>";
	  echo "</td>";
	} 
      if ($reseau != '')
	{
	  echo "<td $colwidth align=\"center\" valign=\"top\" style=\"padding-right: 10px;\">";
	  echo "$reseau<br>";
	  echo "</td>";
	}
      echo "</tr>";
      echo "</table>";
    }


  if (false)
    {
      echo "<table width=\"100%\" border=\"0\"><tr>";
      echo "<td width=\"33%\" valign=\"top\" style=\"padding-right: 10px;\">";
      echo "<h3>Conseil r&eacute;gional</h3>";

      $logo = _makeURL($rgname).'.png';
      $logo = str_replace('%27', '-', $logo);
      $logopath = 'sites/default/files/logos/region_'.$logo;
      echo "\n<img alt=\"\" src=\"$logopath\"/><br/>\n";

      if ($website != '')
	{
	  $regweb = $website;
	  echo "<b>SITE INTERNET</b><br/><i>";
	   if (strstr($regweb, 'http'))
	     $regweb = str_replace('http://', '', $regweb);
	   $regweb2 = rtrim($regweb, '/');
	   echo "<a target=\"_blank\" href=\"http://$regweb\">$regweb2</a></i><br/><br/>";
	}
      echo "Ici, un texte de pr&eacute;sentation du Conseil r&eacute;gional.";
      
      echo "</td>";

      echo "<td width=\"33%\" valign=\"top\" style=\"padding: 0 10px;\">";
      echo "<h3>Dreal</h3>";
      echo "Ici, un texte de pr&eacute;sentation de la Dreal.";
      echo "</td>";

      echo "<td width=\"33%\" valign=\"top\" style=\"padding-left: 10px;\">";
      echo "<h3>R&eacute;seau</h3>";
      echo "Ici, un texte de pr&eacute;sentation du r&eacute;seau des RNs.";
      echo "</td>";
      echo "</tr>";
      echo "</table>";
    }
}

function _displayActualitesRN()
{
  $rnnid = arg(1);
  if ($rnnid != 'add')
    {
      $curdate = date("Y-m-d");
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'node')
	->entityCondition('bundle', 'actualites')
	->fieldCondition('field_reserve', 'nid', $rnnid, '=')
	->fieldCondition('field_date_retrait', 'value', $curdate, '>=')
	/* ->propertyOrderBy('field_date_mel', 'desc') */
	->addMetaData('account', user_load(1));
      $result = $query->execute();
      if (isset($result['node'])) 
	{
	  $nids = array_keys($result['node']);
	  if (count($nids) > 0)
	    {
	      $firstnid = $nids[0];
	      $act_url = drupal_lookup_path('alias', "node/".$firstnid);

	      echo "<a href=\"../$act_url\">Actualit&eacute;s</a>";
	    }
	}
    }
}

function _displayLinkOutil()
{
  $linktab = array(
	'malles-pedagogiques' => '<a href="les-malles-pedagogiques">malles p&eacute;dagogiques</a>',
	'fiches' => '<a href="les-fiches">fiches</a>',
	'maquettes' => '<a href="les-maquettes">maquettes</a>',
	'jeux' => '<a href="les-jeux">jeux</a>',
	'autres' => '<a href="les-autres-outils">autres outils</a>'
		   );
  $nid = arg(1); 
  if ($nid != '')
    {
      $node = node_load($nid);
      if (isset($node->field_type_outil))
	{
	  $type = _getfieldvalue($node, $node->field_type_outil, 0);
	  $url = _makeURL(mb_convert_encoding($type, 'ISO-8859-1', 'UTF-8'));
	  if (isset($linktab[$url]))
	    echo "Cet outil fait partie des ".$linktab[$url].".<br/><br/>\n";
	}
    }
}
  //require_once ("sites/all/libraries/rnf/functions.inc");
  //_displayLinkOutil();
?>
