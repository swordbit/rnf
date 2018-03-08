<?php
require_once('functions.inc');

function _displayUsersOld($users)
{
  $roles = array();
  $nbuser   = 0;
  $nbactive = 0;
  $nbblocke = 0;
  //$nbphoto  = 0;

  foreach ($users as $u)
    {
      if ($u->status == 0)
	$nbblocke++;
      else
	$nbactive++;
      //if ($u->picture != '')
      //$nbphoto++;

      $nbuser++;

      $ur = $u->roles;
      foreach($ur as $rkey => $rname)
	{
	  if (isset($roles[$rname]))
	    $roles[$rname]++;
	  else
	    $roles[$rname] = 1;
	}

    }

  arsort($roles);

  echo "<b>$nbuser utilisateurs enregistr&eacute;s</b>&nbsp;:<br/>";
  echo "- $nbactive activ&eacute;s<br/>- $nbblocke bloqu&eacute;s<br/>";
  //echo "Photos : $nbphoto<br/>";
  echo "<b>R&ocirc;les</b>&nbsp;:<br/>";
  $exclude = array('administrator', 
		   'adm librairie',
		   'adm actualités',
		   'adm recrutement',
		   'adm réserves', 
		   'adm utilisateurs',
		   'anonymous user', 
		   'authenticated user'
		   );
  foreach($roles as $rname => $rcount)
    {
      if (!in_array($rname, $exclude))
	echo "$rname : $rcount<br/>";
    }

  // print_r($roles);

  //echo "<pre>";
  //  print_r($u);
  //  echo "</pre>";
}

// version plus propre mais avec pb de perf sur user_load_multiple
function _displayStatUsersOld()
{
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'user')
	->addMetaData('account', user_load(1)); // run the query as user 1
      $result = $query->execute();
      if (isset($result['user'])) 
	{
	  $uids = array_keys($result['user']); 
	  $users = user_load_multiple($uids);
	  _displayUsersOld($users);
	}
}

function _displayStatUsers()
{
  $nbuser   = 0;
  $nbactive = 0;
  $nbblocke = 0;

  $uids = array();
  $res = db_query("SELECT uid, status FROM {users}");
  if ($res)
    {
      while ($row = $res->fetchAssoc()) {
	if ($row['status'] == 0)
	  $nbblocke++;
	else
	  $nbactive++;
	$uids[] = $row['uid'];
	$nbuser++;
      }
    }
  $roles = array();
  $res = db_query("SELECT rid, name FROM {role}");
  if ($res)
    {
      while ($row = $res->fetchAssoc()) {
	$rid = $row['rid'];
	$roles[$rid] = $row['name'];
      }
    }

  $rolescount = array();
  $res = db_query("SELECT uid, rid FROM {users_roles}");
  if ($res)
    {
      while ($row = $res->fetchAssoc()) {
	$uid = $row['uid'];
	$rid = $row['rid'];
	$rname = $roles[$rid];

	if (isset($rolescount[$rname]))
	  $rolescount[$rname]++;
	else
	  $rolescount[$rname] = 1;
      }
    }

  arsort($rolescount);

  echo "<b>$nbuser utilisateurs enregistr&eacute;s</b>&nbsp;:<br/>";
  echo "- $nbactive activ&eacute;s<br/>- $nbblocke bloqu&eacute;s<br/>";
  //echo "Photos : $nbphoto<br/>";
  echo "<b>R&ocirc;les</b>&nbsp;:<br/>";
  $exclude = array('administrator', 
		   'adm librairie',
		   'adm actualités',
		   'adm recrutement',
		   'adm réserves', 
		   'adm utilisateurs',
		   'anonymous user', 
		   'authenticated user'
		   );
  foreach($rolescount as $rname => $rcount)
    {
      if (!in_array($rname, $exclude))
	echo "$rname : $rcount<br/>";
    }
}
?>
