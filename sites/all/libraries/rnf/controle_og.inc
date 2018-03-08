<?php
require_once ("sites/all/libraries/rnf/functions.inc");

function _showOGWithBadAdress($badadr)
{
  if (count($badadr) > 0)
    {
      echo "<br/><strong>Adresse avec virgule</strong><br/>";
      foreach($badadr as $oglink)
	echo "$oglink<br/>";
    }
}


function _showOGWithBadCP($badcp)
{
  if (count($badcp) > 0)
    {
      echo "<br/><strong>Code postal incorrect</strong><br/>";
      foreach($badcp as $oglink)
	echo "$oglink<br/>";
    }
}

function _showOGWithBadCity($badcity)
{
  if (count($badcity) > 0)
    {
      echo "<br/><strong>Localit&eacute; incorrecte</strong><br/>";
      foreach($badcity as $oglink)
	echo "$oglink<br/>";
    }
}

function _showOGWithBadTel($badtel)
{
  if (count($badtel) > 0)
    {
      echo "<br/><strong>T&eacute;l&eacute;phone avec un .</strong><br/>";
      foreach($badtel as $oglink)
	echo "$oglink<br/>";
    }
}

function _showOGWithBadFax($badfax)
{ 
  if (count($badfax) > 0)
    {
      echo "<br/><strong>Fax avec un .</strong><br/>";
      foreach($badfax as $oglink)
	echo "$oglink<br/>";
    }
}

function _showOGWithBadMail($badmail)
{
  if (count($badmail) > 0)
    {
      echo "<br/><strong>Courriel sans @</strong><br/>";
      foreach($badmail as $oglink)
	echo "$oglink<br/>";
    }
}


function _showOGWithBadWeb($badweb)
{
  if (count($badweb) > 0)
    {
      echo "<br/><strong>Web avec http</strong><br/>";
      foreach($badweb as $oglink)
	echo "$oglink<br/>";
    }
}


function _showOGWithoutWeb($noweb)
{
  if (count($noweb) > 0)
    {
      echo "<br/><strong>Gestionnaires sans site web</strong><br/>";
      foreach($noweb as $oglink)
	echo "$oglink<br/>";
    }
}


function _showOGWithoutRN($norn)
{
  if (count($norn) > 0)
    {
      echo "<br/><strong>Gestionnaires sans r&eacute;serve naturelle</strong><br/>";
      foreach($norn as $oglink)
	echo "$oglink<br/>";
    }
}


function _showOG($ogtab)
{
  $norn    = array();
  $badadr  = array();
  $badcp   = array();
  $badcity = array();
  $badtel  = array();
  $badfax  = array();
  $badmail = array();
  $badweb  = array();
  $noweb  = array();

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', 'gestionnaire')
	->propertyOrderBy('title', 'asc')
    	->addMetaData('account', user_load(1)); // run the query as user 1
  $result = $query->execute();
  if (isset($result['node'])) 
    {
      $nids = array_keys($result['node']);
      $allog = node_load_multiple($nids);
      foreach($allog as $og)
	{
	  $nid = $og->nid; 
	  $ogname = $og->title;
	  $ogname_u = mb_convert_encoding($og->title, 'ISO-8859-1', 'UTF-8');
	  $url = _makeURL($ogname_u);
	  $url = 'node/'.$nid;
	  $oglink = "<a href=\"../$url\">$ogname</a>";
	  $ogadr1  = _getfieldvalue($og, $og->field_og_adresse1, '');
	  $ogadr2  = _getfieldvalue($og, $og->field_og_adresse2, '');
	  $ogcopos = _getfieldvalue($og, $og->field_code_postal, '');
	  $ogville = _getfieldvalue($og, $og->field_ville, '');
	  $ogtel   = _getfieldvalue($og, $og->field_telephone, '');
	  $ogfax   = _getfieldvalue($og, $og->field_fax, '');
	  $ogemail = _getfieldvalue($og, $og->field_email, '');
	  $ogweb   = _getfieldvalue($og, $og->field_site_web, '');

	  // virgule dans l'adresse
	  if (strstr($ogadr1, ',') || strstr($ogadr2, ','))
	    $badadr[] = $oglink;
	  // code postal a 5 caracteres
	  if (strlen(trim($ogcopos)) != 5)
	    $badcp[] = $oglink;
	  // ville en majuscules
	  if (strtoupper(trim($ogville)) == trim($ogville))
	    $badcity[] = $oglink;
	  // point dans les numeros de tel
	  if (strstr($ogtel, '.'))
	    $badtel[] = $oglink;
	  // point dans les numeros de fax
	  if (strstr($ogfax, '.'))
	    $badfax[] = $oglink; 
	  // email incorrect
	  if ($ogemail != '' 
	      && (!strstr($ogemail, '@') || strstr($ogemail, '@@')))
	    $badmail[] = $oglink;
	  // web avec http
	  if (strstr($ogweb, 'http'))
	    $badweb[] = $oglink;
	  if ($ogweb == '')
	    $noweb[] = $oglink;
	  if (!in_array($nid, $ogtab))
	    $norn[] = $oglink;
	}
    }
  _showOGWithBadAdress($badadr); 
  _showOGWithBadCP($badcp); 
  _showOGWithBadCity($badcity); 
  _showOGWithBadTel($badtel); 
  _showOGWithBadFax($badfax);
  _showOGWithBadMail($badmail);
  _showOGWithBadWeb($badweb);
  //_showOGWithoutWeb($noweb);
  _showOGWithoutRN($norn);
}

function _controlOG()
{
  $ogtab     = array();

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
	  $publie  = _getfieldvalue($rn, $rn->field_publie, '');
	  $body    = _getfieldvalue($rn, $rn->body, '');
	  $type    = _getfieldvalue($rn, $rn->field_typern, '');
	  $og1     = _getfieldnid($rn, $rn->field_og1, '');
	  $og2     = _getfieldnid($rn, $rn->field_og2, '');
	  $og3     = _getfieldnid($rn, $rn->field_og3, '');
	  $rnname    = mb_convert_encoding($rn->title, 'ISO-8859-1', 'UTF-8');
	  $url     = _makeURL($rnname);
	  if ($rncode == 'RNR191') // cas special Grand-Lieu
	    $url .= '-rnr';
	  $rnname = mb_convert_encoding($rnname, 'UTF-8', 'ISO-8859-1');	
	  $rnlink = "<a href=\"../$url\">$rnname</a>";

	  
	  if ($og1 != '' && !in_array($og1, $ogtab))
	    $ogtab[] = $og1; 
	  if ($og2 != '' && !in_array($og2, $ogtab))
	    $ogtab[] = $og2;
	  if ($og3 != '' && !in_array($og3, $ogtab))
	    $ogtab[] = $og3;
	}
    }

  _showOG($ogtab);
}


?>