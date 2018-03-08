<?php
require_once('functions.inc');
 
$tabrn = array();
$ognid = arg(1);
if ($ognid != 'add')
  {
    // on prend toutes les RNs
    $nidtab = _getRNnid(true);
    $reserves = node_load_multiple($nidtab);
    foreach($reserves as $rn)
      {
	$rnname = mb_convert_encoding($rn->title, 'ISO-8859-1', 'UTF-8');
	$nameiso = $rn->title;
	$url = _makeURL($rnname);
	if (isset($rn->field_og1))
	  {	
	    $onid = _getfieldnid($rn, $rn->field_og1, 0);
	    if ($ognid == $onid)
	      $tabrn[] = "<a href=\"../".$url."\">".$nameiso."</a>";
	  }
	if (isset($rn->field_og2))
	  {
	    $onid = _getfieldnid($rn, $rn->field_og2, 0);
	    if ($ognid == $onid) 
	      $tabrn[] = "<a href=\"../".$url."\">".$nameiso."</a>";
	  }
	if (isset($rn->field_og3))
	  {
	    $onid = _getfieldnid($rn, $rn->field_og3, 0);
	    if ($ognid == $onid)
	       $tabrn[] = "<a href=\"../".$url."\">".$nameiso."</a>";
	  }
      }
  }
 $nb = count($tabrn);
 if ($nb != 0)
   {
     if ($nb == 1)
       echo "<b>R&eacute;serve g&eacute;r&eacute;e :</b><br/>";
     else
       echo "<b>R&eacute;serves g&eacute;r&eacute;es :</b><br/>";
      foreach($tabrn as $rnlink)
	echo "$rnlink<br/>";
    }
?>