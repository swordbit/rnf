<?php
require_once ("sites/all/libraries/rnf/functions.inc");

function _showRNWithDblCode($rnlist)
{
  if (count($rnlist) != 0)
    {
      echo "<br/><strong>R&eacute;serves naturelles avec code en double</strong><br/>";
      foreach($rnlist as $rnlink)
	echo "$rnlink<br/>";
    }
}
function _showRNWithoutOG($rnlist)
{
  if (count($rnlist) != 0)
    {
      echo "<br/><strong>R&eacute;serves naturelles sans gestionnaire</strong><br/>";
      foreach($rnlist as $rnlink)
	echo "$rnlink<br/>";
    }
}

function _showRNWithoutINPN($rnlist)
{
  if (count($rnlist) != 0)
    {
      echo "<br/><strong>R&eacute;serves naturelles sans num&eacute;ro INPN</strong><br/>";
      foreach($rnlist as $rnlink)
	echo "$rnlink<br/>";
    }
}

function _showRNWithoutWDPA($rnlist)
{
  if (count($rnlist) != 0)
    {
      echo "<br/><strong>R&eacute;serves naturelles sans code WDPA</strong><br/>";
      foreach($rnlist as $rnlink)
	echo "$rnlink<br/>";
    }
}

function _showRNWithoutMilieux($rnlist)
{
  if (count($rnlist) != 0)
    {
      echo "<br/><strong>R&eacute;serves naturelles sans indication de milieux</strong><br/>";
      foreach($rnlist as $rnlink)
	echo "$rnlink<br/>";
    }
}

function _showRNWithoutLimite($rnlist)
{
  if (count($rnlist) != 0)
    {
      echo "<br/><strong>R&eacute;serves naturelles sans indication de limites</strong><br/>";
      foreach($rnlist as $rnlink)
	echo "$rnlink<br/>";
    }
}

function _showRNWeb($rnlist)
{
  if (count($rnlist) != 0)
    {
      echo "<br/><strong>Sites web</strong><br/>";
      foreach($rnlist as $rnlink)
	echo "$rnlink<br/>";
    }
}

function _controlRN()
{
  $noog      = array();
  $noinpn    = array();
  $nowdpa    = array();
  $nomil     = array();
  $nolim     = array(); 
  $allcodes  = array();
  $dblcode   = array();
  $websites  = array();

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', 'reserve')
    //->fieldCondition('field_publie', 'value', 1, '=') 
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
	  $inpn    = _getfieldvalue($rn, $rn->field_inpn, '');
	  $wdpa    = _getfieldvalue($rn, $rn->field_code_wdpa, '');
	  $milvalues = _getfieldvaluetab($rn, $rn->field_milieux, array());
	  $publie  = _getfieldvalue($rn, $rn->field_publie, '');
	  $limite  = _getFilePath().'limites/'.strtolower($rncode).'.json';
	  $body    = _getfieldvalue($rn, $rn->body, '');
	  $type    = _getfieldvalue($rn, $rn->field_typern, ''); 
	  $rnweb   = _getfieldvalue($rn, $rn->field_website, '');
	  $og1     = _getfieldnid($rn, $rn->field_og1, '');
	  $og2     = _getfieldnid($rn, $rn->field_og2, '');
	  $og3     = _getfieldnid($rn, $rn->field_og3, '');
	  $rnname    = mb_convert_encoding($rn->title, 'ISO-8859-1', 'UTF-8');
	  $url     = _makeURL($rnname);
	  if ($rncode == 'RNR191') // cas special Grand-Lieu
	    $url .= '-rnr';
	  $rnname = mb_convert_encoding($rnname, 'UTF-8', 'ISO-8859-1');	
	  $rnlink = "<a href=\"../$url\">$rnname</a>";

	  if (!in_array($rnweb, $websites))
	    $websites[] = $rnweb;

	  // recherche des codes en double
	  if (isset($allcodes[$rncode]))
	    $dblcode[] = $rnlink." / ".$allcodes[$rncode];
	  else
	    $allcodes[$rncode] = $rnlink;

	  if ($publie)
	    {
	      // recherche des RN sans OG
	      if ($og1 == '' && $og2 == '' && $og3 == '')
		$noog[] = $rnlink;
	      // recherche des RN sans code INPN
	      if ($inpn == '')
		$noinpn[] = $rnlink;
	      // recherche des RN sans code WDPA
	      if ($wdpa == '')
		$nowdpa[] = $rnlink;
	      // recherche des RN sans indication de milieux
	      $nbm = 0;
	      foreach ($milvalues as $mil)
		{
		  if (trim($mil['value']) != '')
		    $nbm++;
		}
	      if ($nbm == 0)
		$nomil[] = $rnlink;
	      // recherche des RN sans limites
	      if (!is_file($limite))
		$nolim[] = $rnlink;
	    }
	}
    }

  _showRNWithDblCode($dblcode); 
  _showRNWithoutOG($noog); 
  _showRNWithoutINPN($noinpn); 
  _showRNWithoutWDPA($nowdpa); 
  _showRNWithoutMilieux($nomil);
  _showRNWithoutLimite($nolim); 
  _showRNWeb($websites);
}


?>