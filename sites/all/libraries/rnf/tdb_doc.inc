<?php


function _getFileName($node, $fid)
{
   $res = db_query("SELECT filename from drupal_file_managed where fid = $fid");
   $uritab = $res->fetchAssoc();
   if (isset($uritab['filename']))
     return $uritab['filename'];
}

function _showTdBDocument()
{
  global $base_url;

  $nbrn = 0;
  $alldesc = array();
  $udesc = array();
  
  $misdec = 0;
  $misplq = 0;

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', 'reserve')
	->fieldCondition('field_publie', 'value', 1, '=') 
	->propertyOrderBy('title', 'asc')
    	->addMetaData('account', user_load(1)); // run the query as user 1
  $result = $query->execute();
  if (isset($result['node'])) 
    {
      echo "<table width=\"100%\">";
      echo "<tr>";
      echo "<td align=\"center\" width=\"25%\"><b>Nom de la r&eacute;serve</b></td>";
      echo "<td align=\"center\"><b>Num</b></td>";
      echo "<td align=\"center\"><b>Bandeau</b></td>";
      echo "<td align=\"center\"><b>Nom img</b></td>";
      echo "<td align=\"center\"><b>Desc</b></td>";
      echo "<td align=\"center\"><b>Nom fich</b></td>";
      echo "<td align=\"center\"><b>Décret</b></td>";
      echo "<td align=\"center\"><b>Plaquette</b></td>";
      echo "</tr>";

      $nids = array_keys($result['node']);
      $regrn = node_load_multiple($nids);
      foreach ($regrn as $rn)
        { 
	  $rncode  = _getfieldvalue($rn, $rn->field_code, '');
	  $body    = _getfieldvalue($rn, $rn->body, '');
	  $type    = _getfieldvalue($rn, $rn->field_typern, '');
	  $inpn    = _getfieldvalue($rn, $rn->field_inpn, '');
	  $bandeau = _getfieldvalue($rn, $rn->field_bandeau, '', 'fid');
	  $fichier = _getfieldvaluetab($rn, $rn->field_fichier_rn, array());
	  $photos  = _getfieldvaluetab($rn, $rn->field_photos, array());

	  $name   = mb_convert_encoding($rn->title, 'ISO-8859-1', 'UTF-8');
	  $url    = _makeURL($name);
	  if ($rncode == 'RNR191') // cas special Grand-Lieu
	    $url .= '-rnr';
	  $publie = 'Non';
	  $link = '../'.$url;
	  $name = mb_convert_encoding($name, 'UTF-8', 'ISO-8859-1');	
	  
	  //if ($rncode == 'RNN18')
	  //print_r($photos);

	  if ($type == 'RNN' || $type == 'RNC' || true)
	    {
	      if ($publie == 'Oui')
		echo "<tr>";
	      else
		echo "<tr style=\"background: #ccc;\">";
	      echo "<td><a href=\"$link\">$name</a></td>";
	      if ($inpn != '' 
		  && (($type == 'RNR' && strstr($inpn, 'FR93'))
		      || ($type == 'RNN' && strstr($inpn, 'FR36'))
		      || ($type == 'RNC' && strstr($inpn, 'FR36'))
		      ))
		echo "<td style=\"background: #cfc;\" align=\"center\">$rncode</td>";
	      else
		echo "<td style=\"background: #fcc;\" align=\"center\">$rncode</td>";
	      if ($bandeau != '')
		{  
		  $bandfile = _getFileName($rn, $bandeau);
		  $codelow = strtolower($rncode);
		  if (strstr($bandfile, 'bandeau') && strstr($bandfile, $codelow))
		    {
		      echo "<td align=\"center\" style=\"background: #cfc;\">OK</td>";
		    }
		  else
		    {
		      echo "<td align=\"center\" style=\"background: #fcc;\">Non</td>"; 
		    }
		}
	      else
		{
		  echo "<td align=\"center\">&nbsp;</td>"; 
		}  

	      // controle des photos
	      if (count($photos) != 0)
		{
		  $goodnames = true;  
		  $gooddesc  = true;
		  foreach($photos as $filedesc)
		    {
		      $fname = $filedesc['filename'];
		      if (!strstr($fname, strtolower($rncode)))
			$goodnames = false;
		      $fdesc = $filedesc['alt'];
		      if (!strstr($fdesc, ' - © '))
			$gooddesc = false;
		    }
		  if ($goodnames)
		    echo "<td align=\"center\" style=\"background: #cfc;\">OK</td>";
		  else
		    echo "<td align=\"center\" style=\"background: #fcc;\">Non</td>";
		  if ($gooddesc)
		    echo "<td align=\"center\" style=\"background: #cfc;\">OK</td>";
		  else
		    echo "<td align=\"center\" style=\"background: #fcc;\">Non</td>"; 
		}
	      else
		{
		  echo "<td align=\"center\">&nbsp;</td>"; 
		  echo "<td align=\"center\">&nbsp;</td>"; 
		}

	      // controle des fichiers attaches
	      if (count($fichier) != 0)
		{
		  $founddecret = false;
		  $foundplaquette = false;
		  $goodnames = true;

		  foreach($fichier as $filedesc)
		    {
		      $fname = $filedesc['filename']; 
		      $fdesc = $filedesc['description'];
		      $alldesc[] = "$fdesc - <a href=\"$link\">$rncode</a>";
		      if (!in_array($fdesc, $udesc))
			$udesc[] = $fdesc;
		      if (!strstr($fname, strtolower($rncode)))
			$goodnames = false;
		      if ($type == 'RNN' 
			  && (strstr($fname, 'decret') || strstr($fname, 'arret')))
			$founddecret = true;
		      if ($type == 'RNC' && strstr($fname, 'decret'))
			$founddecret = true;
		      if ($type == 'RNR' && strstr($fname, 'delib'))
			$founddecret = true;
		      if (strstr($fdesc, 'Plaquette') || strstr($fdesc, 'Fiche'))
			$foundplaquette = true;
		    }
		  if ($goodnames)
		    echo "<td align=\"center\" style=\"background: #cfc;\">OK</td>";
		  else
		    echo "<td align=\"center\" style=\"background: #fcc;\">Non</td>"; 

		  if ($founddecret)
		    echo "<td align=\"center\" style=\"background: #cfc;\">OK</td>";
		  else
		    {
		      echo "<td align=\"center\" style=\"background: #fcc;\">Non</td>";
		      $misdec++;
		    }
		  if ($foundplaquette)
		    echo "<td align=\"center\" style=\"background: #cfc;\">OK</td>";
		  else
		    {
		      echo "<td align=\"center\" style=\"background: #fcc;\">Non</td>"; 
		      $misplq++;
		    }
		}
	      else
		{
		  $misdec++;
		  $misplq++;
		  echo "<td align=\"center\">&nbsp;</td>";
		  echo "<td align=\"center\">&nbsp;</td>";
		  echo "<td align=\"center\">&nbsp;</td>"; 
		} 

	      echo "</tr>\n";
	      $nbrn++;
	    }
        }
      echo "<tr><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td>";
      echo "<td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td>";
      echo "<td align=\"center\">$misdec</td>";
      echo "<td align=\"center\">$misplq</td>";
      echo "</tr>";

      echo "</table>";
      
      sort($alldesc);
      foreach ($alldesc as $desc)
	echo "$desc<br>";
      echo "<hr>";
      sort($udesc);
      foreach ($udesc as $desc)
	echo "$desc<br>";
    }
  if (isset($z))
    {
      echo "<pre>";
      print_r($z);
      echo "</pre>";
    }
}


?>