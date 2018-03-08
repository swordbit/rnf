<?php
require_once('functions.inc');

function _displayAnniv($nodes, $typecl)
{
  $curyear = date('Y');
  if (count($nodes) != 0)
    {
      foreach($nodes as $n)
	{
	  $myname2 = mb_convert_encoding($n->title, 'ISO-8859-1', 'UTF-8');
	  $myname = $n->title;
	  $url = _makeURL($myname2);
	  if ($typecl == 'classement')
	    {
	      if (_getfieldvalue($n, $n->field_datere, '') != '')
		continue;
	      $datecly = substr(_getfieldvalue($n, $n->field_datecl, ''), 0, 4);
	    }
	  else
	    $datecly = substr(_getfieldvalue($n, $n->field_datere, ''), 0, 4);
	  $diff = $curyear - $datecly;
	  $fulllink = "<a href=\"../$url\">$myname</a>";
	  if ($diff > 1)
	    echo "Il y a $diff ans, $typecl de $fulllink<br>";
	  else
	    echo "Il y a $diff an, $typecl de $fulllink<br>";
	}
    }
}

function _displayAnniversaire()
{
  $showtitle = false;
  $curdate = '____-'.date('m-d').' %';
  //$curdate = '____-02-10 %';
  //echo $curdate;

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'reserve')
    ->fieldCondition('field_publie', 'value', 1, '=') 
    ->fieldCondition('field_datecl', 'value', $curdate, 'like')
    ->addMetaData('account', user_load(1)); // run the query as user 1
  $result = $query->execute();
  if (isset($result['node'])) 
    {
      echo "<b>Anniversaire de classement</b><br>";
      $showtitle = true;
      $nids = array_keys($result['node']); 
      $nodes = node_load_multiple($nids);
      _displayAnniv($nodes, 'classement');
    }

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'reserve')
    ->fieldCondition('field_publie', 'value', 1, '=') 
    ->fieldCondition('field_datere', 'value', $curdate, 'like')
    ->addMetaData('account', user_load(1)); // run the query as user 1
  $result = $query->execute();
  if (isset($result['node'])) 
    {
      if (!$showtitle)
	echo "<b>Anniversaire de classement</b><br>";
      $nids = array_keys($result['node']); 
      $nodes = node_load_multiple($nids);
      _displayAnniv($nodes, 'reclassement');
    }
}
?>