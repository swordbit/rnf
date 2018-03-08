<?php
require_once('functions.inc');

function _displayUser($u)
{
  $imgroot = '../sites/default/files/styles/thumbnail/public/pictures/';
  if ($u->picture != '')
    {
      $filename = $u->picture->filename;
      $src = $imgroot.$filename;
      echo "<img src=\"$src\"><br>";
    }
  else
    {
      $src = '../sites/default/files/images/empty.jpg';
      echo "<img width=\"82\" height=\"100\" src=\"$src\"><br>";
    }

  $realname = $u->realname;
  echo "<b>$realname</b><br>";

  $struct = _getfieldvalue($u, $u->field_structure, '');
  echo "$struct<br>";

  $commission = _getfieldvalue($u, $u->field_groupes, '');
  echo "<i>$commission</i><br>";

  $excluded = array('authenticated user');

  //$roles = $u->roles;
  //foreach($roles as $rkey => $rname)
  //if (!in_array($rname, $excluded))
  //echo "$rname<br>";

  if (false)
    {
      if ($u->realname == 'Pierre-Marie Aubertel')
	{
	  echo "<pre>";
	  ///echo print_r($u->field_nom);
	  echo print_r($u);
	  echo "</pre>";
	}
    }
}

function _displayUsers($users)
{
  $nbcol = 3; 			// nombre de colonnes
  $nbuser   = 0;
  $colw = floor(100 / $nbcol); 	// largeur des colonnes du tableau
  
  // utilisateurs exclus du trombinoscope
  $excluded = array('Admin Quod', 'Test Test', 
		    'test Test redac',
		    'Steph Essai Essai Steph');

  echo "<table border=\"1\">";
  foreach ($users as $u)
    {
      if (isset($u->realname) && trim($u->realname) != '')
	{
	  $realname = $u->realname;
	  if (!in_array($realname, $excluded))
	    {
	      if ($nbuser % $nbcol == 0)
		echo "<tr>";

	      echo "<td align=\"center\" width=\"$colw %\">";
	      _displayUser($u);
	      echo "</td>\n";

	      if ($nbuser % $nbcol == $nbcol - 1)
		echo "</tr>\n";

	      $nbuser++;
	    }
	}
    }

  // cellules manquantes du tableau
  $nbmis = $nbcol - ($nbuser % $nbcol);
  if ($nbmis != 0)
    {
      for ($i = 0; $i < $nbmis; $i++)
	echo "<td>&nbsp;</td>";
      echo "</tr>\n";
    }
  echo "</table>";
}

function _displayTrombi()
{
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'user')
    ->fieldOrderBy('field_nom', 'value', 'asc')
    ->propertyCondition('status', 1)
    ->addMetaData('account', user_load(1)); // run the query as user 1
  $result = $query->execute();
  if (isset($result['user'])) 
    {
      $uids = array_keys($result['user']); 
      $users = user_load_multiple($uids);
      _displayUsers($users);
    }
}
?>