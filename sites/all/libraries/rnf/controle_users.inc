<?php
require_once('functions.inc');

// Champs utilisateurs :
// uid
// name = login
// pass
// mail = adresse courriel
// theme
// signature
// signature_format : filtered_html
// created = date de creation
// access = date dernier access
// login
// status = bloque (0) ou actif (1)
// picture = photos
// init
// data
// roles (array)
// field_nom (array)
// field_prenom (array)
// field_groupes (array)
// field structure (array)
// field_reserve (array)
// realname

// url : ../../user/<uid>/edit

// TODO
// - structure en double ?
// - RN sans representants

function _showUsersWithDblReal($ulist)
{
  if (count($ulist) != 0)
    {
      echo "<br/><strong>Utilisateurs avec nom en double</strong><br/>";
      foreach($ulist as $ulink)
	echo "$ulink<br/>";
    }
}

function _showUsersWithDblMail($ulist)
{
  if (count($ulist) != 0)
    {
      echo "<br/><strong>Utilisateurs avec adresse courriel en double</strong><br/>";
      foreach($ulist as $ulink)
	echo "$ulink<br/>";
    }
}

function _showRNRWithoutUser($reserves, $represented)
{
  $isfirst = true;
  $i = 0;
  foreach ($reserves as $nid => $rnname)
    {
      if (!in_array($nid, $represented))
	{
	  if ($isfirst)
	    { 
	      echo "<br/><strong>RNR sans repr&eacute;sentant</strong><br/>";
	      $isfirst = false;
	    }
	  echo "$rnname<br>";
	  $i++;
	}
    }
  echo "$i RNR<br>";
}

function _showRNNWithoutUser($reserves, $represented)
{
  $isfirst = true;
  $i = 0;
  foreach ($reserves as $nid => $rnname)
    {
      if (!in_array($nid, $represented))
	{
	  if ($isfirst)
	    { 
	      echo "<br/><strong>RNN sans repr&eacute;sentant</strong><br/>";
	      $isfirst = false;
	    }
	  echo "$rnname<br>";
	  $i++;
	}
    }
  echo "$i RNN<br>";
}


function _controlUsers()
{
  $nbuser   = 0;
  $nbactive = 0;
  $nbblocke = 0;
  $nbphoto  = 0;
  $realnames = array();
  $courriel  = array();
  $doublereal = array();
  $doublemail = array();
  $allrnn     = array();
  $allrnr     = array();
  $represented = array();

  // recherche de toutes les RN publiees pour savoir lesquelles
  // sont representees
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
	  $nid = $rn->nid;
	  $type    = _getfieldvalue($rn, $rn->field_typern, '');
	  if ($type == 'RNN')
	    $allrnn[$nid] = $rn->title;
	  else
	    $allrnr[$nid] = $rn->title;
	}
    }

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'user')
    ->addMetaData('account', user_load(1)); // run the query as user 1
  $result = $query->execute();
  if (isset($result['user'])) 
    {
      $uids = array_keys($result['user']); 
      $users = user_load_multiple($uids);
      $i = 0;
      foreach ($users as $u)
	{ 
	  $uid = $u->uid;
	  if ($uid == -1)
	    {
	      // debug
	      echo "<pre>";
	      print_r($u);
	      echo "</pre>";
	      break;
	    }
	  $name = $u->name;
	  $realname = $u->realname;
	  if ($realname != '')
	    {
	      if (isset($realnames[$realname]))
		{
		  $name1 = $realnames[$realname];
		  $doublereal[] = "$realname (\"$name\" et \"$name1\")";
		}
	      else
		$realnames[$realname] = $name;
	    }
	  //echo "$uid - $realname<br>";
	    
	  $mail = $u->mail;
	  if (isset($courriel[$mail]))
	    {
	      $name1 = $courriel[$mail];
	      $doublemail[] = "$mail (\"$name\" et \"$name1\")";
	    }
	  else
	    $courriel[$mail] = $name;

	  $restab = _getfieldvaluetab($u, $u->field_reserve, array());
	  if (count($restab) != 0)
	    {
	      foreach($restab as $r)
		{
		  $nid = $r['nid'];
		  if (!in_array($nid, $represented))
		    $represented[] = $nid;
		}
	    }

	  $nbuser++;
	  $i++;
	}
    }								       

  echo "$nbuser utilisateurs control&eacute;s<br>";

  _showUsersWithDblReal($doublereal);
  _showUsersWithDblMail($doublemail);
  _showRNNWithoutUser($allrnn, $represented);
  _showRNRWithoutUser($allrnr, $represented);
}

?>