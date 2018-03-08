<?php
require_once ("sites/all/libraries/rnf/functions.inc");

function _showRNWithNoBandeau($rnlist)
{
  $nb = count($rnlist);
  if ($nb != 0)
    {
      echo "<br/><strong>Pages de r&eacute;serves naturelles sans bandeau : $nb</strong><br/>";
      foreach($rnlist as $rnlink)
	echo "$rnlink<br/>";
    }
}

function _showRNWithOneImg($rnlist)
{
  $nb = count($rnlist);
  if ($nb != 0)
    {
      echo "<br/><strong>Pages de r&eacute;serves naturelles avec seulement une photo : $nb</strong><br/>";
      foreach($rnlist as $rnlink)
	echo "$rnlink<br/>";
    }
}

function _showRNWithTwoImg($rnlist)
{
  $nb = count($rnlist);
  if ($nb != 0)
    {
      echo "<br/><strong>Pages de r&eacute;serves naturelles avec seulement deux photos : $nb</strong><br/>";
      foreach($rnlist as $rnlink)
	echo "$rnlink<br/>";
    }
}

function _showRNWithThreeImg($rnlist)
{
  $nb = count($rnlist);
  if ($nb != 0)
    {
      echo "<br/><strong>Pages de r&eacute;serves naturelles avec seulement trois photos : $nb</strong><br/>";
      foreach($rnlist as $rnlink)
	echo "$rnlink<br/>";
    }
}

function _showNoImg()
{
  $nobandeau = array();
  $oneimg    = array();
  $twoimg    = array();
  $threeimg  = array();

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
	  $bandeau = _getfieldvalue($rn, $rn->field_bandeau, '', 'fid');
	  $photos  = _getfieldvaluetab($rn, $rn->field_photos, array());
	  $photocount = count($photos);
	  $type    = _getfieldvalue($rn, $rn->field_typern, '');
	  $rnname    = mb_convert_encoding($rn->title, 'ISO-8859-1', 'UTF-8');
	  $url     = _makeURL($rnname);
	  if ($rncode == 'RNR191') // cas special Grand-Lieu
	    $url .= '-rnr';
	  $rnname = mb_convert_encoding($rnname, 'UTF-8', 'ISO-8859-1');	
	  $rnlink = "<a href=\"../$url\">$rnname</a> ($type)";

	  if ($bandeau == '')
	    $nobandeau[] = $rnlink;

	  if ($photocount <= 1)
	    $oneimg[] = $rnlink; 
	  if ($photocount == 2)
	    $twoimg[] = $rnlink; 
	  if ($photocount == 3)
	    $threeimg[] = $rnlink;
	}
    }

  _showRNWithNoBandeau($nobandeau); 
  _showRNWithOneImg($oneimg);
  _showRNWithTwoImg($twoimg); 
  //_showRNWithThreeImg($threeimg); 
}


?>