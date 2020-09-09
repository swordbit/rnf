<?php

function _showTdB()
{
  global $base_url;

  $nbinpn = 0;
  $nbwdpa = 0;
  $nbband = 0; 
  $nblogo = 0; 
  $nblege = 0;
  $nbtext = 0;
  $nbtotalphot = 0; 
  $nbphot = 0;
  $nbmili = 0;
  $nbcomm = 0;
  $nblimi = 0;

  $totalnote = 0;
  $notes = array();
  for ($i = 0; $i < 11; $i++)
    $notes[$i] = 0;

  $totaltxt = 0;

  $misinpn = 0;
  $miswdpa = 0; 
  $misband = 0;
  $mislege = 0;
  $mislogo = 0;
  $mistext = 0;
  $misphot = 0;
  $mismili = 0;
  $miscomm = 0;
  $mislimi = 0; 
  $misgood = 0;

  $nbgood  = 0;
  $nbrn = 0;

  $showcomm = false;
  $showleg = false;

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
      echo "<td align=\"center\"><b>Logo</b></td>";
      if ($showleg)
      echo "<td align=\"center\"><b>Leg.</b></td>";
      echo "<td align=\"center\"><b>Texte</b></td>";
      echo "<td align=\"center\"><b>Photos</b></td>";
      echo "<td align=\"center\"><b>Milieux</b></td>";
      if ($showcomm)
	echo "<td align=\"center\"><b>Comm.</b></td>";
      echo "<td align=\"center\"><b>Limites</b></td>";
      echo "<td align=\"center\"><b>&nbsp;</b></td>";
      echo "<td align=\"center\"><b>Note</b></td>"; 
      echo "<td align=\"center\">&nbsp;</td>";
      echo "</tr>";

      $vert = '#cfc;'; 
      $bleu = '#ccf;'; 
      $rose = '#fcc;';
      $gris = '#ccc;';
      $orange = '#ffa200;';
      $rouge = '#f88;';

      $nids = array_keys($result['node']);
      $regrn = node_load_multiple($nids);
      foreach ($regrn as $rn)
        { 
	  $note    = 0;
	  $nid     = $rn->nid;
	  $rncode  = _getfieldvalue($rn, $rn->field_code, '');
	  $body    = _getfieldvalue($rn, $rn->body, '');
	  $type    = _getfieldvalue($rn, $rn->field_typern, '');
	  $inpn    = _getfieldvalue($rn, $rn->field_inpn, '');
	  $wdpa    = _getfieldvalue($rn, $rn->field_code_wdpa, '');
	  $bandeau = _getfieldvalue($rn, $rn->field_bandeau, '', 'fid');
	  $legende = _getfieldvalue($rn, $rn->field_bandeau, '', 'alt');
	  $rnlogo  = _getfieldvalue($rn, $rn->field_rn_logo, '', 'filename');
	  $photos  = _getfieldvaluetab($rn, $rn->field_photos, array());
	  $photocount = count($photos);

	  $name   = mb_convert_encoding($rn->title, 'ISO-8859-1', 'UTF-8');
	  $url    = _makeURL($name);
	  if ($rncode == 'RNR191') // cas special Grand-Lieu
	    $url .= '-rnr';
	  $publie = 'Non';
	  $limite  = _getFilePath().'limites/'.strtolower($rncode).'.json';
	  $link = '../'.$url;
	  $name = mb_convert_encoding($name, 'UTF-8', 'ISO-8859-1');	
	  
	  //if ($type == 'RNN' || $type == 'RNC' || true)
	    {
	      $isgood = true;
	      $neargood = true;
	      if ($publie == 'Oui')
		echo "<tr>";
	      else
		echo "<tr style=\"background: $gris\">";
	      echo "<td><a href=\"$link\">$name</a></td>";
	      if ($inpn != '' 
		  && (($type == 'RNR' && strstr($inpn, 'FR93'))
		      || ($type == 'RNN' && strstr($inpn, 'FR36'))
		      || ($type == 'RNC' && (strstr($inpn, 'FR36') || strstr($inpn, 'FR92')))))
		{
		  $nbinpn++;
		  $note++;	// INPN rempli : + 1 point
		  if ($wdpa != '')
		    {  
		      $nbwdpa++;
		      if ($type == 'RNR' && !preg_match('/^555/', $wdpa))
			echo "<td style=\"background: $orange\" align=\"center\">$rncode</td>";		
		      else
			echo "<td style=\"background: $vert\" align=\"center\">$rncode</td>";
		    }
		  else
		    {
		      $miswdpa++;
		      echo "<td title=\"Aucun WDPA\" style=\"background: $rose\" align=\"center\">$rncode</td>";
		    }
		}
	      else
		{
		  $misinpn++;
		  echo "<td title=\"Aucun INPN\" style=\"background: $rouge\" align=\"center\">$rncode</td>";
		}
	      if ($bandeau != '')
		{  
		  $note++; // bandeau rempli : + 1 point
		  $nbband++;
		  echo "<td align=\"center\" style=\"background: $vert\">OK</td>";
		}
	      else
		{
		  $isgood = false; 
		  $neargood = false;
		  $misband++;
		  echo "<td title=\"Aucun bandeau\" align=\"center\" style=\"background: $rose\">Non</td>"; 
		}  
	      if ($rnlogo != '')
		{  
		  $nblogo++;
		  echo "<td align=\"center\" style=\"background: $vert\">OK</td>";
		}
	      else 
		{
		  $mislogo++;
		  echo "<td title=\"Aucun logo\" align=\"center\" style=\"background: $rose\">Non</td>"; 
		}  

	      if ($showleg)
		{
		  if ($legende != '')
		    { 
		      $nblege++;
		      echo "<td align=\"center\" style=\"background: $vert\">OK</td>";
		    }	
		  else
		    { 
		      $isgood = false;
		      $neargood = false;
		      $mislege++;
		      echo "<td align=\"center\" style=\"background: $rose\">Non</td>"; 
		    }
		}
	      if ($body != '')
		{  
		  $nbtext++;
		  if (strstr($body, "readmore"))
		    $hasreadmore = true;
		  else
		    $hasreadmore = false;
		  $nbchar = strlen(trim($body));
		  $nd = "";
		  if ($hasreadmore)
		    $nbchar -= 75;
		  else
		    $nd = "nd";
		  
		  if ($nbchar > 1600)
		    $note += 3;  // texte de +de 1600 car : + 3 point
		  else if ($nbchar > 1200)
		    $note += 2;	 // texte de +de 1200 car : + 2 point
		  else
		    $note++;     // texte de -de 1200 car : + 1 point

		  $totaltxt += $nbchar;

		  if ($nbchar < 800)
		    echo "<td title=\"< 800\" align=\"center\" style=\"background: $rouge\">$nbchar $nd</td>"; 
		  else if ($nbchar < 1200)
		    echo "<td title=\"< 1200\" align=\"center\" style=\"background: $rose\">$nbchar $nd</td>"; 
		  else if ($nbchar < 1600)
		      echo "<td title=\"< 1600\" align=\"center\" style=\"background: $bleu\">$nbchar $nd</td>";
		  else
		    echo "<td align=\"center\" style=\"background: $vert\">$nbchar $nd</td>";
		}
	      else
		{ 
		  $isgood = false;
		  $mistext++;
		  echo "<td title=\"Aucun texte\" align=\"center\" style=\"background: $rose\">Non</td>"; 
		}
	      if ($photocount != 0)
		{
		  $nbtotalphot += $photocount;
		  if ($photocount >= 3)
		    {
		      $note += 3;  // plus de 3 photos : + 3 point
		      $nbphot += 3;
		    }
		  else
		    {
		      $note += $photocount; // autant de point que de photo
		      $nbphot += $photocount;
		      if ($photocount == 1)
			$misphot += 2;	// minimum 3 photos par RN
		      else if ($photocount == 2)
			$misphot += 1;	// minimum 3 photos par RN
		    }
		  echo "<td align=\"center\" style=\"background: $vert\">$photocount</td>";
		  if ($photocount < 2)
		    $neargood = false; // au moins deux photos
		}
	      else
		{  
		  $isgood = false;
		  $neargood = false;
		  $misphot +=3;	// minimum 3 photos par RN
		  echo "<td title=\"Moins de 3 photos\" align=\"center\" style=\"background: $rouge\">$photocount</td>";
		}

	      $nbm = 0;
	      $milvalues = _getfieldvaluetab($rn, $rn->field_milieux, array());
	      foreach ($milvalues as $mil)
		{
		  if (trim($mil['value']) != '')
		    $nbm++;
		}

	      if ($nbm > 0)
		{
		  $nbmili++;
		  $note++; // milieux indique : + 1 point
		  echo "<td align=\"center\" style=\"background: $vert\">OK</td>";
		}
	      else
		{
		  $isgood = false;
		  //$neargood = false;
		  $mismili++;
		  echo "<td title=\"Aucun milieu\" align=\"center\" style=\"background: $rose\">Non</td>"; 
		}

	      if ($showcomm)
		{
		  $nbc = 0;
		  $comtab = _getfieldvaluetab($rn, $rn->field_communes, array());
		  foreach ($comtab as $comval)
		    if (trim($comval['value']) != '')
		      $nbc++;

		  if ($nbc > 0)
		    {
		      $nbcomm++;
		      echo "<td align=\"center\" style=\"background: $vert\">OK</td>";
		    }
		  else
		    {
		      $isgood = false;
		      $neargood = false;
		      $miscomm++;
		      echo "<td title=\"Aucune commune\" align=\"center\" style=\"background: $rose\">Non</td>"; 
		    }
		}

	      if (is_file($limite))
		{
		  $note++; // limite indiquee : +1 point
		  $nblimi++;
		  echo "<td align=\"center\" style=\"background: $vert\">OK</td>";
		}
	      else
		{ 
		  $isgood = false;
		  $neargood = false;
		  $mislimi++;
		  echo "<td title=\"Aucune limite\" align=\"center\" style=\"background: $rouge\">Non</td>"; 
		}

	      if ($isgood)
		{
		  $nbgood++;
		  echo "<td align=\"center\" style=\"background: $vert\">OK</td>";
		}
	      else if ($neargood)
	      	{
		  $misgood++;
		echo "<td align=\"center\" style=\"background: $bleu\">Non</td>"; 
                }
	      else if ($note > 7)
		{
		  $misgood++;
		  echo "<td align=\"center\" style=\"background: $rose\">Non</td>"; 
		}
	      else
		{ 
		  $misgood++;
		  echo "<td align=\"center\" style=\"background: $rouge\">Non</td>"; 
		}

	      if ($note > 7)
		echo "<td title=\"Note > 7\" align=\"center\" style=\"background: $vert\">$note</td>";
	      else if ($note > 4)
		echo "<td title=\"Note > 4\" align=\"center\" style=\"background: $bleu\">$note</td>"; 
	      else
		echo "<td align=\"center\" style=\"background: $rose\">$note</td>"; 
	      $edit = "../node/$nid/edit";
	      // URL: trunk/node/277/edit
	      echo "<td align=\"center\"> <a target=\"_blank\" href=\"$edit\">Mod</a> </td>";
	      echo "</tr>\n";
	      $nbrn++;
	      $totalnote += $note;
	      if (!isset($notes[$note]))
		$notes[$note] = 1;
	      else
		$notes[$note]++;
	    }
        }


      $moyenne = number_format($totalnote / $nbrn, 2);
      echo "<tr>";
      echo "<td><b>Actuellement : $nbrn RN</b></td>";
      echo "<td align=\"center\"><b>".$nbinpn." / ".$nbwdpa."</b></td>";
      echo "<td align=\"center\"><b>$nbband band.</b></td>";
     echo "<td align=\"center\"><b>$nblogo logos</b></td>"; 
      if ($showleg)
	echo "<td align=\"center\"><b>$nblege leg.</b></td>";
      echo "<td align=\"center\"><b>$nbtext textes</b></td>";
      echo "<td align=\"center\"><b>$nbtotalphot im.</b></td>";
      echo "<td align=\"center\"><b>$nbmili mil.</b></td>"; 
      if ($showcomm)
	echo "<td align=\"center\"><b>$nbcomm com.</b></td>"; 
      echo "<td align=\"center\"><b>$nblimi lim.</b></td>"; 
      echo "<td align=\"center\"><b>$nbgood OK</b></td>"; 
      echo "<td align=\"center\"><b>$moyenne</b></td>";
      echo "</tr>";
      echo "<tr>";

      echo "<td colspan=\"2\" align=\"left\"><b>Pourcentage qui sont OK :</b></td>";
      $percent = floor(100 * $nbband / $nbrn);
      echo "<td align=\"center\"><b>$percent %</b></td>"; 
      $percent = floor(100 * $nblogo / $nbrn);
      echo "<td align=\"center\"><b>$percent %</b></td>"; 
      if ($showleg)
	{
	  $percent = floor(100 * $nblege / $nbrn);
	  echo "<td align=\"center\"><b>$percent %</b></td>";
	}
      $percent = floor(100 * $nbtext / $nbrn);
      echo "<td align=\"center\"><b>$percent %</b></td>";
      $percent = floor(100 * $nbphot / (3 * $nbrn));
      echo "<td align=\"center\"><b>$percent %</b></td>";
      $percent = floor(100 * $nbmili / $nbrn);
      echo "<td align=\"center\"><b>$percent %</b></td>";
      if ($showcomm)
	{
	  $percent = floor(100 * $nbcomm / $nbrn);
	  echo "<td align=\"center\"><b>$percent %</b></td>";
	}
      $percent = floor(100 * $nblimi / $nbrn);
      echo "<td align=\"center\"><b>$percent %</b></td>";
      $percent = floor(100 * $nbgood / $nbrn);
      echo "<td align=\"center\"><b>$percent %</b></td>";
      echo "</tr>";

      echo "<tr>";
      echo "<td align=\"right\"><b>Manque :</b></td>";
      echo "<td align=\"center\"><b>".$misinpn." / ".$miswdpa."</b></td>";
      echo "<td align=\"center\"><b>$misband</b></td>";
      echo "<td align=\"center\"><b>$mislogo</b></td>";
      if ($showleg)
	echo "<td align=\"center\"><b>$mislege</b></td>";
      echo "<td align=\"center\"><b>$mistext</b></td>";
      echo "<td align=\"center\"><b>$misphot</b></td>";
      echo "<td align=\"center\"><b>$mismili</b></td>";
      if ($showcomm)
	echo "<td align=\"center\"><b>$miscomm</b></td>";
      echo "<td align=\"center\"><b>$mislimi</b></td>";
      echo "<td align=\"center\"><b>$misgood</b></td>";
      echo "</tr>";
  
      echo "</table>";
    }

  echo "Moyenne des textes : ".ceil($totaltxt / $nbrn)." caractères";
  echo " / Moyenne des images : ".number_format($nbtotalphot / $nbrn, 2)." images<br>";

  echo "<table width=\"100%\" border=\"1\"><tr>";
  for ($i = 1; $i < 11; $i++)
    echo "<td width=\"10%\" align=\"center\">note $i</td>";
  echo "</tr><tr>";
  for ($i = 1; $i < 11; $i++)
    echo "<td align=\"center\">$notes[$i]</td>";
  echo "</tr><tr>";
  for ($i = 1; $i < 11; $i++)
    if ($notes[$i] != 0)
      echo "<td align=\"center\">".round(100 * $notes[$i] / $nbrn)." %</td>";
    else
      echo "<td align=\"center\">&nbsp;</td>";
   echo "</tr></table>";

   $showgooglechart = false;
   if ($showgooglechart)
     {
       echo "<div id=\"chart_notes\" style=\"width: 720px; height: 200px;\"></div>";
       echo "<script type=\"text/javascript\" src=\"https://www.google.com/jsapi\"></script>\n";
       echo "<script type=\"text/javascript\">\n";
       echo 'google.load("visualization", "1", {packages:["corechart"]});'."\n";
       echo 'google.setOnLoadCallback(drawChart);'."\n";
       echo 'function drawChart() {'."\n";
       echo 'var data4 = google.visualization.arrayToDataTable(['."\n";
       echo "  ['Note', 'Nombre de fiches'],\n";
       $nbnote = count($notes);
       $i = 0;
       foreach($notes as $note => $nbrn)
	 {
	   if ($i != $nbnote - 1)
	     echo "  ['".$note."',$nbrn],\n";
	   else
	     echo "  ['".$note."',$nbrn]\n";
	   $i++;
	 } 
       echo " ]);\n";
       echo "var options4 = { title: 'Note de la fiche' };\n";
       echo "var chart4 = new google.visualization.ColumnChart(document.getElementById('chart_notes'));\n";
       echo "chart4.draw(data4, options4);\n";
       echo "}\n";
       echo "</script>\n";
     }

   $showd3chart = false;
   if ($showd3chart)
     {
       echo "<div id=\"chart_notes\" style=\"width: 720px; height: 200px;\"></div>";
       $data = array();
       foreach($notes as $note => $nbrn)
	 $data[$note] = $nbrn;
       //arsort($data);

       $rows = array();  
       $i = 0;
       foreach($data as $name => $value)
	 {
	   $rows[] = array($name, $value);
	   $i++;
	 }
       $chart = array(
		      'id' => 'chart_notes',
		      'type' => 'ColumnChart',
		      'fill' => '#ccc',
		      'legend' => array('Nbre de RN'),
		      'rows' => $rows
		 );
       d3_draw($chart);
     }
}


?>