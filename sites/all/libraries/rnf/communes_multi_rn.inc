<?php
require_once ("sites/all/libraries/rnf/functions.inc");

function _showMultiRegion($multireg)
{
  echo "<br/><strong>R&eacute;serves naturelles sur plusieurs r&eacute;gions</strong><br/>";
  foreach ($multireg as $rnlink)
    echo "$rnlink<br/>";
}

function _showMultiDepartement($multidep)
{
  echo "<br/><strong>R&eacute;serves naturelles sur plusieurs d&eacute;partements</strong><br/>";
  foreach ($multidep as $rnlink)
    echo "$rnlink<br/>";
}

function _showDepWithoutRN($depart)
{
  require ("sites/all/libraries/rnf/departements.inc");

  echo "<br/><strong>D&eacute;partements ou territoires sans r&eacute;serve naturelle class&eacute;e</strong><br/>";

  foreach($departements as $num => $name)
    { 
      $name = mb_convert_encoding($name, 'UTF-8', 'ISO-8859-1');	
      //echo "$num<br/>";
      if (strlen($num) > 1)
	if (!in_array($num, $depart))
	  echo "$name ($num)<br/>";
    }
}

function _showRegionWithoutRN($region)
{
  require ("sites/all/libraries/rnf/regions.inc");

  echo "<br/><strong>R&eacute;gions sans r&eacute;serve naturelle</strong><br/>";

  foreach($regions as $num => $name)
    { 
      $name = mb_convert_encoding($name, 'UTF-8', 'ISO-8859-1');	
      //echo "$num<br/>";
      if (!in_array($name, $region))
	  echo "$name<br/>";
    }
}


function _showCommunes()
{
  $communes  = array();
  $multireg  = array();
  $multidep  = array();
  $depart    = array();
  $region    = array();

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', 'reserve')
	->fieldCondition('field_publie', 'value', 1, '=') 
	->propertyOrderBy('title', 'asc')
    	->addMetaData('account', user_load(1)); // run the query as user 1
  $result = $query->execute();
  if (isset($result['node'])) 
    {
      $nids = array_keys($result['node']);
      $allrn = node_load_multiple($nids);
      foreach ($allrn as $rn)
        {  
	  $rncode  = _getfieldvalue($rn, $rn->field_code, '');
	  $body    = _getfieldvalue($rn, $rn->body, '');
	  $type    = _getfieldvalue($rn, $rn->field_typern, '');
	  $comtab  = _getfieldvaluetab($rn, $rn->field_communes, array());
	  $regtab  = _getfieldvaluetab($rn, $rn->field_regionrn, array());
	  $deptab  = _getfieldvaluetab($rn, $rn->field_departement, array());
	  //$lati    = _getfieldvalue($rn, $rn->field_latitude, 0.0);
	  //$long    = _getfieldvalue($rn, $rn->field_longitude, '0.0');
	  $rnname    = mb_convert_encoding($rn->title, 'ISO-8859-1', 'UTF-8');
	  $url     = _makeURL($rnname);
	  if ($rncode == 'RNR191') // cas special Grand-Lieu
	    $url .= '-rnr';
	  $rnname = mb_convert_encoding($rnname, 'UTF-8', 'ISO-8859-1');	
	  $rnlink = "<a href=\"../$url\">$rnname</a>";

	  // on essaie de differencier les communes par region
	  $nbreg = count($regtab);
	  if ($nbreg == 1)
	    {
	      // une seule region, on ajoute le nom de la region aux communes
	      $rgnid = $regtab[0]['nid'];
	      $rgnode = node_load($rgnid);
	      $rgname = trim($rgnode->title);
	      $suffix = "_".$rgname; 
	      if (!in_array($rgname, $region))
		$region[] = $rgname; // pour controle des regions sans RN
	    }
	  else
	    {
	      // RN sur plusieurs regions : voir plus bas
	      $suffix = '';
	      //echo $name." - ".$rncode."<br/>";
	      $multireg[] = $rnlink." (".$nbreg.")";
	    }

	  // plusieurs departements ?
	  if (count($deptab) != 1)
	    {
	      // test contre les valeurs vides
	      $ismult = 1;
	      $hasdep = false;
	      foreach($deptab as $dep)
		if ($dep['value'] != '')
		  {
		    if ($hasdep)
		      $ismult++;
		    else
		      $hasdep = true;
		  }
		
	      if ($ismult > 1)
		$multidep[] = $rnlink." (".$ismult.")";
	    }
	  
	  foreach($deptab as $dep)
	    {
	      if ($dep['value'] != '')
		{
		  $depnum = $dep['value'];
		  if (!in_array($depnum, $depart))
		    $depart[] = $depnum; // pour controle des dep sans RN
		}
	    }

	  // pour chaque commune de la RN
	  foreach ($comtab as $comval)
	    {
	      if (trim($comval['value']) != '')
		{	
		  $comname = trim($comval['value']);
		  if ($suffix != '')
		    {
		      $comname .= $suffix;
		    }
		  else
		    {
		      // cas des RN multi regions, il faut preciser
		      // la region pour chaque commune de la RN
		      switch($rncode)
			{
			case 'RNN169': // astrobleme
			  if ($comname == 'Chassenon' 
			      || $comname == 'Pressignac')
			    $comname .= "_Limousin";
			  else
			     $comname .= "_Poitou-Charentes";
			  break;
			case 'RNN137': // estuaire de la seine
			  if ($comname == 'Saint-Samson-de-la-Roque')
			    $comname .= "_Haute-Normandie";
			  else
			    $comname .= "_Basse-Normandie";
			  break;
			case 'RNR194': // pont de fer
			  if ($comname == 'Assérac')
			    $comname .= "_Pays-de-la-Loire";
			  else
			    $comname .= "_Bretagne";
			  break;
			case 'RNN41': // gorges ardeche
			  if ($comname == 'Aiguèze'
			      || $comname == 'Garn')
			    $comname .= "_Languedoc-Roussillon";
			  else
			    $comname .= "_Rhône-Alpes";
			  break;
			case 'RNN95': // massif du ventron
			  if ($comname == 'Cornimont' || $comname == 'Ventron')
			    $comname .= "_Lorraine";
			  else
			    $comname .= "_Alsace";
			  break;
			case 'RNN127': // val de loire
			  if ($comname == 'Couargues'
			      || $comname == 'Herry'
			      || $comname == 'La Chapelle-Montlinard')
			    $comname .= "_Centre";
			  else
			    $comname .= "_Bourgogne";
			  break;
			}
		    }

		  if (isset($communes[$comname]))
		    $communes[$comname][$rnname] = $rnlink;
		  else
		    $communes[$comname] = array($rnname => $rnlink);
		}
	    }
	}
    }

  // repartition dans un tableau different selon le nombre de RN
  // pour pouvoir trier chaque tableau ensuite...
  $restab = array();
  foreach($communes as $comname => $comrn)
    {
      $nbrn = count($comrn);
      if ($nbrn != 1)
	{
	  if (isset($restab[$nbrn]))
	    $restab[$nbrn][$comname] = $comrn;
	  else
	    $restab[$nbrn] = array($comname => $comrn);
	}
    }

  echo "<br/><strong>Communes ayant plusieurs r&eacute;serves naturelles</strong><br/>";
  asort($restab);

  foreach($restab as $nbr => $coms)
    {
      ksort($coms);
      //echo "<strong>$nbr RNs</strong><br/>";
      foreach($coms as $comname => $comrn)
	{
	  // on extrait le vrai nom de commune (sans la region)
	  $namet = explode("_", $comname);
	  $name = $namet[0];
	  echo "<b>".$name."</b> (".$nbr.")&nbsp;:&nbsp;";
	  $isfirst = true;
	  asort($comrn);
	  foreach($comrn as $rn)
	    {
	    if ($isfirst)
	      echo "$rn";
	    else
	      echo ", ".$rn;
	    $isfirst = false;
	    }
	  echo "<br/>";
	}
    }

  _showMultiRegion($multireg); 
  _showMultiDepartement($multidep);
  _showDepWithoutRN($depart); 
  //_showRegionWithoutRN($region);
}


?>