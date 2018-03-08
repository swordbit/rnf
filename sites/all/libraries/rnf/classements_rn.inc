<?php
require_once ("sites/all/libraries/rnf/functions.inc");

function Abbrev($name)
{
  switch ($name)
    {
    case 'Aquitaine-Limousin-Poitou-Charentes':
      return 'Aqu-Lim-Poit-Ch';
    case 'Auvergne-Rhône-Alpes':
      return 'Auv-Rh-Alpes';
    case 'Bourgogne-Franche-Comté':
      return 'Bourgogne-FrC';
    case 'Centre-Val de Loire':
      return 'Centre-VdL';
    case 'Charente-Maritime':
      return 'Char.-Maritime';
    case 'Languedoc-Roussillon-Midi-Pyrénées':
      return 'Lang-Rous-Midi-P';
    case 'Nord-Pas-de-Calais-Picardie':
      return 'Hauts-de-France';
    case 'Provence-Alpes-Côte d\'Azur':
      return 'PACA'; 
    case 'Languedoc-Roussillon':
      return 'Lang.-Rouss.'; 
    case 'Pyrénées-Orientales':
      return 'Pyr.-Orient.';
    case 'Hautes-Pyrénées':
      return 'Hautes-Pyr.';
    case 'Bouches-du-Rhône':
      return 'B.-du-Rhône';
    default:
      return $name;
    }
  
}

function _showClassementDep($depart)
{
  require ("sites/all/libraries/rnf/departements.inc");

  echo "<br/><strong>Classement des territoire ou d&eacute;partement</strong><br/>";

  arsort($depart);
  //print_r($depart);

  foreach($depart as $num => $nb)
    { 
      //if ($nb > 0)
      if (true)
	{
	  $name = $departements[$num];
	  $name = mb_convert_encoding($name, 'UTF-8', 'ISO-8859-1');	
	  echo "$name ($nb)<br/>";
	}
    }
}

function _showClassementRegion($region)
{
  require ("sites/all/libraries/rnf/regions.inc");

  echo "<br/><strong>Classement des r&eacute;gions</strong><br/>";

  arsort($region);
  //print_r($region);

  foreach($region as $num => $nb)
    { 
      //if ($nb > 0)
      if (true)
	{
	  $name = $regions[$num];
	  $name = mb_convert_encoding($name, 'UTF-8', 'ISO-8859-1');	
	  echo "$name ($nb)<br/>";
	}
    }
}


function _ShowColumnChart($chartname, $rnn, $type, $max)
{
  $data = array();
  foreach($rnn as $name => $tab)
    {
      $value = $tab[$type];
      $data[$name] = $value;
    }
  arsort($data);

  $rows = array();  
  $i = 0;
  foreach($data as $name => $value)
    {
      $rows[] = array($name, $value);
      $i++;
      if ($i == $max)
	break;
    }

  $chart = array(
		 'id' => str_replace(' ', '_', $chartname.$type),
		 'type' => 'ColumnChart',
		 'fill' => '#ccc',
		 'legend' => array($type),
		 'rows' => $rows
		 );

  //echo "<pre>";
  //print_r($chart);
  //echo "</pre>";

  echo d3_draw($chart);
}

function _showClassement()
{ 
  define('NB_RNN', 'Nb RNN');
  define('NB_RNR', 'Nb RNR-RNC');
  define('SU_RNN', 'Surf RNN');
  define('SU_RNR', 'Surf RNR-RNC');


  $depart_tab    = array();
  $region_tab    = array();
  $depart_rn = array();
  
  require ("sites/all/libraries/rnf/regions.inc");
  require ("sites/all/libraries/rnf/departements.inc");

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
	  //$body    = _getfieldvalue($rn, $rn->body, '');
	  $type    = _getfieldvalue($rn, $rn->field_typern, '');
	  $regtab  = _getfieldvaluetab($rn, $rn->field_regionrn, array());
	  $deptab  = _getfieldvaluetab($rn, $rn->field_departement, array());
	  //$lati    = _getfieldvalue($rn, $rn->field_latitude, 0.0);
	  //$long    = _getfieldvalue($rn, $rn->field_longitude, '0.0');
	  $surface = _getfieldvalue($rn, $rn->field_surface, 0);

	  $surface = $surface / 100; // conversion en km2

	  //if ($rncode == 'RNN46')
	  //echo $type."<br.";
	  //print_r($regtab);

	  foreach($regtab as $reg)
	    {
	      $rgnid = $reg['nid'];
	      $rgnode = node_load($rgnid);
	      $rgcode = _getfieldvalue($rgnode, $rgnode->field_coderegion, '');
	      $rgname = $regions[$rgcode];
	      $rgname = mb_convert_encoding($rgname, 'UTF-8', 'ISO-8859-1');
	      $rgname = Abbrev($rgname);

	      if (!isset($region_tab[$rgname]))
		$region_tab[$rgname] = array(NB_RNN => 0, NB_RNR => 0, 
					     SU_RNN => 0, SU_RNR => 0 );

	      $rgtab = $region_tab[$rgname];
	      if ($type == 'RNN')
		{
		  $rgtab[NB_RNN]++; 
		  if ($rncode != 'RNN161' && $rgcode != '3') // TAAF et Guyane
		  $rgtab[SU_RNN] += $surface;
		}
	      else
		{
		  $rgtab[NB_RNR]++;
		  if ($rncode != 'RNC147') // bonifacio
		    $rgtab[SU_RNR] += $surface;
		}
	      $region_tab[$rgname] = $rgtab;
	    }
	  
	  foreach($deptab as $dep)
	    {
	      if ($dep['value'] != '')
		{
		  if ($type == 'RNR')
		    {
		  $depnum = $dep['value'];
		  if (isset($depart_rn[$depnum]))
		    $depart_rn[$depnum]++;
		  else
		    $depart_rn[$depnum] = 1;
		    }
		}
	    }

	  foreach($deptab as $dep)
	    {
	      if ($dep['value'] != '')
		{
		  $depnum = $dep['value'];
		  $depname = $departements[$depnum];
		  $depname = mb_convert_encoding($depname, 'UTF-8', 'ISO-8859-1');	
		  $depname = Abbrev($depname);
		  if (!isset($depart_tab[$depname]))
		    $depart_tab[$depname] = array(NB_RNN => 0, NB_RNR => 0,
						  SU_RNN => 0, SU_RNR => 0 );

		  $deptab = $depart_tab[$depname];
		  if ($type == 'RNN')
		    {
		      $deptab[NB_RNN]++;
		      if ($rncode != 'RNN161' && $depnum != '973') // TAAF et Guyane
			$deptab[SU_RNN] += $surface;

		    }
		  else
		    {
			$deptab[NB_RNR]++; 
			if ($rncode != 'RNC147') // bonifacio
			  $deptab[SU_RNR] += $surface;
		    }
		  $depart_tab[$depname] = $deptab;
		}
	    }
	}
    }

  //echo "<pre>";
  //print_r($depart_tab);
  //echo "</pre>";

  $max = 16;
  echo "<br><b>Top $max des r&eacute;gions en nombre de RNN</b>";
  _ShowColumnChart('region', $region_tab, NB_RNN, $max);

  $max = 15;
  echo "<br><b>Top $max des r&eacute;gions en nombre de RNR/RNC</b>";
  _ShowColumnChart('region', $region_tab, NB_RNR, $max);

  $max = 15;
  echo "<br><b>Top $max des r&eacute;gions en surface (km&sup2;) de RNN (hors TAAF et Guyane)</b>";
  _ShowColumnChart('region', $region_tab, SU_RNN, $max);

  $max = 15;
  echo "<br><b>Top $max des r&eacute;gions en surface (km&sup2;) de RNR/RNC (hors B. de Bonifacio)</b>";
  _ShowColumnChart('region', $region_tab, SU_RNR, $max);

  echo "<hr>";

  $max = 30;

  echo "<br><b>Top $max des d&eacute;partements en nombre de RNN</b>";
  _ShowColumnChart('depart', $depart_tab, NB_RNN, $max);

  echo "<br><b>Top $max des d&eacute;partements en nombre de RNR/RNC</b>";
  _ShowColumnChart('depart', $depart_tab, NB_RNR, $max);

  echo "<br><b>Top $max des d&eacute;partements en surface (km&sup2;) de RNN (hors TAAF et Guyane)</b>";
  _ShowColumnChart('depart', $depart_tab, SU_RNN, $max);

  echo "<br><b>Top $max des d&eacute;partements en surface (km&sup2;) de RNR/RNC (hors B. de Bonifacio)</b>";
  _ShowColumnChart('depart', $depart_tab, SU_RNR, $max);


  //_showClassementRegion($rnn);
  //_showClassementDep($depart_rn);

}
?>