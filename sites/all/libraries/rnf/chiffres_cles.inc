<?php
// module pour afficher les chiffres cles
// infos necessaire pour toutes les RN:
// code/type/publie/surface/outrem/datecl

require_once('functions.inc');

function _pc($num)
{
  return number_format($num, 1, ',', ' ');
}

function _ha($num)
{
  $tmp = number_format(ceil($num), 0, ',', ' ');
  return str_replace(' ', '&nbsp;', $tmp);
}

function _displayChiffrescles()
{
  $showyears = false;
  $dogoogle = false;

  $rnlist = _getRNtable();

  $nb_rn  = 0;
  $nb_rnn = 0;
  $nb_rnr = 0;
  $nb_rnc = 0;
  $nb_rnn_me = 0;
  $nb_rnr_me = 0;
  $nb_rnn_om = 0;
  $nb_rnr_om = 0;
  $nb_metrop = 0;
  $nb_outrem = 0;

  $surf = 0.0;
  $surf_rnn = 0.0;
  $surf_rnr = 0.0;
  $surf_rnc = 0.0;
  $surf_rnn_me = 0.0;
  $surf_rnr_me = 0.0;
  $surf_rnn_om = 0.0;
  $surf_rnr_om = 0.0;
  $surf_outrem = 0.0;
  $surf_metrop = 0.0;

  $yeartab = array();

  foreach ($rnlist as $rn)
    {
      $rncode  = $rn['code']; 
      $type    = $rn['type']; 
      $publie  = $rn['publie']; 
      $surface = $rn['surface'];
      $outrem  = $rn['outrem'];
      if ($publie == 'Oui')
        {
	  if ($type == 'RNR')
	    {
	      $nb_rnr++;
	      $surf_rnr += $surface;
	      if ($outrem == 'Oui')
		{
		  $nb_rnr_om++;
		  $surf_rnr_om += $surface;
		}
	      else
		{
		  $nb_rnr_me++;
		  $surf_rnr_me += $surface;
		}
	    }
	  else if ($type == 'RNN')
	    {
	      $nb_rnn++;
	      $surf_rnn += $surface;
	      if ($outrem == 'Oui')
		{
		  $nb_rnn_om++;
		  $surf_rnn_om += $surface;
		}
	      else
		{
		  $nb_rnn_me++;
		  $surf_rnn_me += $surface;
		}
	    }
	  else 
	    {
	      $nb_rnc++;
	      $surf_rnc += $surface;
	    }
	
	  if ($rn['outrem'] == 'Oui')
	    {
	      $nb_outrem++;
	      $surf_outrem += $surface;
	    }
	  else
	    {
	      $nb_metrop++;
	      $surf_metrop += $surface;
	    }

	  $nb_rn++;
	  $surf += $surface;

	  $datecl = $rn['datecl'];
	  $y = substr($datecl, 6, 4);
	  if ($y == 0)
	    echo "$rncode<br>";
	  if (isset($yeartab[$y]))
	    $yeartab[$y]++;
	  else
	    $yeartab[$y] = 1;
	}
    }

  $nb_rnn_pc = _pc(100*$nb_rnn/$nb_rn);
  $nb_rnr_pc = _pc(100*$nb_rnr/$nb_rn);
  $nb_rnc_pc = _pc(100*$nb_rnc/$nb_rn);

  $surf_rnn_pc = _pc(100*$surf_rnn/$surf);
  $surf_rnr_pc = _pc(100*$surf_rnr/$surf);
  $surf_rnc_pc = _pc(100*$surf_rnc/$surf);

  $nb_metrop_pc = _pc(100*$nb_metrop/$nb_rn);
  $nb_outrem_pc = _pc(100*$nb_outrem/$nb_rn);

  // le camembert pour le compte des RNs en float right
  echo "<div id=\"chart_rn1\" style=\"float: right; width: 360px; height: 200px; text-align: right;\">";
  if (!$dogoogle)
    echo 'Part des RNN, RNR, RNC en nombre';
  echo "</div>";

  // le paragraphe du haut
  echo "Aujourd'hui, le r&eacute;seau des r&eacute;serves naturelles compte ";
  echo "$nb_rn r&eacute;serves naturelles class&eacute;es dont :<br>";
  echo "<ul style=\"margin: 0;\">";
  echo "<li>$nb_rnn <a href=\"../fonctionnement/reserves-naturelles-nationales\">r&eacute;serves naturelles nationales</a> ($nb_rnn_pc&thinsp;%)";
  echo " pour "._ha($surf_rnn)." hectares ($surf_rnn_pc&thinsp;%)&nbsp;;</li>";
  echo "<li>$nb_rnr <a href=\"../fonctionnement/reserves-naturelles-regionales\">r&eacute;serves naturelles r&eacute;gionales</a> ($nb_rnr_pc&thinsp;%)";
  echo " pour "._ha($surf_rnr)." hectares ($surf_rnr_pc&thinsp;%)&nbsp;;</li>";
  echo "<li>$nb_rnc <a href=\"../fonctionnement/reserves-naturelles-de-corse\">r&eacute;serves naturelles de Corse</a> ($nb_rnc_pc&thinsp;%)";
  echo " pour "._ha($surf_rnc)." hectares ($surf_rnc_pc&thinsp;%).</li>";
  echo "</ul>";

  // le tableau des chiffres
?>
    <table width="100%" style="border-collapse: collapse;">
      <tr>
        <td>&nbsp;</td>
        <td style="border: 1px solid black;" colspan="2" bgcolor="#cccccc" align="center"><b>RNN</b></td>
        <td style="border: 1px solid black;" colspan="2" bgcolor="#cccccc" align="center"><b>RNR</b></td>
        <td style="border: 1px solid black;" colspan="2" bgcolor="#cccccc" align="center"><b>RNC</b></td>
        <td style="border: 1px solid black;" colspan="2" bgcolor="#cccccc" align="center"><b>TOTAL</b></td>
      </tr>
      <tr>
        <td>&nbsp;</td>
        <td style="border: 1px solid black;" bgcolor="#cccccc" align="center"><b>Nb</b></td>
        <td style="border: 1px solid black;" bgcolor="#cccccc" align="center"><b>Superficie</b></td>
        <td style="border: 1px solid black;" bgcolor="#cccccc" align="center"><b>Nb</b></td>
        <td style="border: 1px solid black;" bgcolor="#cccccc" align="center"><b>Superficie</b></td>
        <td style="border: 1px solid black;" bgcolor="#cccccc" align="center"><b>Nb</b></td>
        <td style="border: 1px solid black;" bgcolor="#cccccc" align="center"><b>Superficie</b></td>
        <td style="border: 1px solid black;" bgcolor="#cccccc" align="center"><b>Nb</b></td>
        <td style="border: 1px solid black;" bgcolor="#cccccc" align="center"><b>Superficie</b></td>
      </tr>
      <tr>
	<td style="border: 1px solid black;" align="center" bgcolor="#cccccc">M&eacute;tropole</td>
	<td style="padding-right: 1em; border: 1px solid black;" align="right"><?php echo $nb_rnn_me;?></td>
        <td style="padding-right: 1em; border: 1px solid black;" align="right"><?php echo _ha($surf_rnn_me);?></td>
 	<td style="padding-right: 1em; border: 1px solid black;" align="right"><?php echo $nb_rnr_me;?></td>
        <td style="padding-right: 1em; border: 1px solid black;" align="right"><?php echo _ha($surf_rnr_me);?></td>
        <td style="padding-right: 1em; border: 1px solid black;" align="right"><?php echo $nb_rnc;?></td>
        <td style="padding-right: 1em; border: 1px solid black;" align="right"><?php echo _ha($surf_rnc);?></td>
        <td style="padding-right: 1em; border: 1px solid black;" align="right"><b><?php echo $nb_metrop;?></b></td>
        <td style="padding-right: 1em; border: 1px solid black;" align="right"><?php echo _ha($surf_metrop);?></td>
      </tr>
      <tr>
	<td style="border: 1px solid black;" align="center" bgcolor="#cccccc">Outre-Mer</td>
	<td style="padding-right: 1em; border: 1px solid black;" align="right"><?php echo $nb_rnn_om;?></td>
        <td style="padding-right: 1em; border: 1px solid black;" align="right"><?php echo _ha($surf_rnn_om);?></td>
 	<td style="padding-right: 1em; border: 1px solid black;" align="right"><?php echo $nb_rnr_om;?></td>
        <td style="padding-right: 1em; border: 1px solid black;" align="right"><?php echo _ha($surf_rnr_om);?></td>
	<td style="padding-right: 1em; border: 1px solid black;" align="right">&nbsp;</td>
        <td style="padding-right: 1em; border: 1px solid black;" align="right">&nbsp;</td>
        <td style="padding-right: 1em; border: 1px solid black;" align="right"><b><?php echo $nb_outrem;?></b></td>
        <td style="padding-right: 1em; border: 1px solid black;" align="right"><?php echo _ha($surf_outrem);?></td>
      </tr>
      <tr>
	<td style="border: 1px solid black;" align="right">Total</td>
	<td style="padding-right: 1em; border: 1px solid black;" align="right"><b><?php echo $nb_rnn;?></b></td>
        <td style="padding-right: 1em; border: 1px solid black;" align="right"><?php echo _ha($surf_rnn);?></td>
 	<td style="padding-right: 1em; border: 1px solid black;" align="right"><b><?php echo $nb_rnr;?></b></td>
        <td style="padding-right: 1em; border: 1px solid black;" align="right"><?php echo _ha($surf_rnr);?></td>
	<td style="padding-right: 1em; border: 1px solid black;" align="right"><b><?php echo $nb_rnc;?></b></td>
        <td style="padding-right: 1em; border: 1px solid black;" align="right"><?php echo _ha($surf_rnc);?></td>
        <td style="padding-right: 1em; border: 1px solid black;" align="right"><b><?php echo $nb_rn;?></b></td>
        <td style="padding-right: 1em; border: 1px solid black;" align="right"><?php echo _ha($surf);?></td>
      </tr>
    </table>
<?php
  // le camembert pour la surface des RNs
  echo "<div id=\"chart_rn2\" style=\"float: right; width: 350px; height: 200px; text-align: right;\">";
  if (!$dogoogle)
    echo 'Part des RNN, RNR, RNC en surface, hors RN TAAF';
  echo "</div>";
?>
	<strong>Les plus grandes :</strong><br/>
	<ul style="margin: 0;">
	<li>La <a href="../terres-australes-francaises">r&eacute;serve naturelle des Terres australes fran&ccedil;aises</a> qui couvre 67,2 millions d'hectares&nbsp;;</li>
  <li>La <a href="../archipel-des-glorieuses">r&eacute;serve naturelle de l'archipel des Glorieuses</a> dans l'oc&eacute;an Indien&nbsp;(4&nbsp;379&nbsp;000&nbsp;ha)&nbsp;;</li>
	<li>La <a href="../nouragues">r&eacute;serve naturelle des Nouragues</a> en Guyane qui prot&egrave;ge 100&nbsp;000&nbsp;ha de for&ecirc;t tropicale&nbsp;;</li>
	<li>Les plus grandes en M&eacute;tropole&nbsp;: les <a href="../bouches-de-bonifacio">Bouches de Bonifacio</a> (79&nbsp;460&nbsp;ha), les <a href="../sept-iles">Sept-&Icirc;les</a> (19&nbsp;700&nbsp;ha), les <a href="../hauts-plateaux-du-vercors">hauts plateaux du Vercors</a> (17&nbsp;000 ha), la <a href="../camargue">Camargue</a> (13&nbsp;000 ha) et la <a href="../haute-chaine-du-jura">haute cha&icirc;ne du Jura</a> (10&nbsp;900 ha).</li></ul>
	<strong>Les plus petites :</strong><br/>
	<ul style="margin: 0;">
	<li>La <a href="../mine-du-verdy">r&eacute;serve naturelle de la mine du Verdy</a> (0,05&nbsp;ha)&nbsp;;</li>
	<li>La <a href="../recif-fossile-de-marchon-christian-gourrat">r&eacute;serve naturelle du récif fossile de Marchon - Christian Gourrat</a> (0,1&nbsp;ha)&nbsp;;</li>
	<li>La <a href="../geologique-de-normandie-maine">r&eacute;serve naturelle g&eacute;ologique de Normandie-Maine</a> (0,37&nbsp;ha).</li></ul>
<?php
  if ($showyears)
    echo "<div id=\"chart_rn3\" style=\"width: 720px; height: 200px;\"></div>";

  // on soustrait la RN des TAAF : 67200000 ha
  $surf_rnn -= 67200000;
  
  if ($dogoogle)
    {
      echo "<script type=\"text/javascript\" src=\"https://www.google.com/jsapi\"></script>\n";
      echo "<script type=\"text/javascript\">\n";
      echo 'google.load("visualization", "1", {packages:["corechart"]});'."\n";
      echo 'google.setOnLoadCallback(drawChart);'."\n";
      echo 'function drawChart() {'."\n";
      echo 'var data1 = google.visualization.arrayToDataTable(['."\n";
      echo "  ['Reserve', 'Nombre'],\n";
      echo "  ['RNN',  $nb_rnn],\n";
      echo "  ['RNR',  $nb_rnr],\n";
      echo "  ['RNC',  $nb_rnc]\n";
      echo " ]);\n";
      echo "var options1 = { title: 'Part des RNN, RNR, RNC en nombre' };\n";
      echo "\n";
      echo "var chart1 = new google.visualization.PieChart(document.getElementById('chart_rn1'));\n";
      echo "chart1.draw(data1, options1);\n";

      echo 'var data2 = google.visualization.arrayToDataTable(['."\n";
      echo "  ['Reserve', 'Surface'],\n";
      echo "  ['RNN',  ".round($surf_rnn)."],\n";
      echo "  ['RNR',  ".round($surf_rnr)."],\n";
      echo "  ['RNC',  ".round($surf_rnc)."]\n";
      echo " ]);\n";
      echo "var options2 = { title: 'Part des RNN, RNR, RNC en surface, hors RN TAAF' };\n";
      echo "\n";
      echo "var chart2 = new google.visualization.PieChart(document.getElementById('chart_rn2'));\n";
      echo "chart2.draw(data2, options2);\n";

      if ($showyears)
        {
          ksort($yeartab);
          echo 'var data3 = google.visualization.arrayToDataTable(['."\n";
          echo "  ['Annee', 'Nombre'],\n";
          $nby = count($yeartab);
          $i = 0;
          foreach($yeartab as $year => $nbrn)
            {
	      if ($i != $nby - 1)
	        echo "  ['".$year."',$nbrn],\n";
	      else
	        echo "  ['".$year."',$nbrn]\n";
	      $i++;
	    } 
          echo " ]);\n";
          echo "var options3 = { title: 'Annee de classement' };\n";
          echo "var chart3 = new google.visualization.ColumnChart(document.getElementById('chart_rn3'));\n";
          echo "chart3.draw(data3, options3);\n";
        }

      echo "}\n";
      echo "</script>\n";
   }
  else
   {
     // use D3 instead of google
     $chart1 = array(
       'id' => 'chart_rn1',
        'type' => 'PieChart',
        'width' => '300',
        'height' => '180',
        'rows' => array( 
            array('RNN', $nb_rnn),
            array('RNR', $nb_rnr),
            array('RNC', $nb_rnc),
        ),
      );
     d3_draw($chart1);

     $chart2 = array(
       'id' => 'chart_rn2',
        'type' => 'PieChart',
        'width' => '300',
        'height' => '180',
        'rows' => array( 
            array('RNN', round($surf_rnn)),
            array('RNR', round($surf_rnr)),
            array('RNC', round($surf_rnc)),
        ),
      );
     d3_draw($chart2);

     if ($showyears)
        {
           ksort($yeartab);
           $tab = array();
           foreach($yeartab as $year => $nbrn)
             $tab[] = array($year, $nbrn);
           $chart3 = array(
              'id' => 'chart_rn3',
              'type' => 'ColumnChart',
              'width' => '800',
              'height' => '200',
              'legend' => array('Nb RN'),
              'rows' => $tab);
           d3_draw($chart3);
        }
   }
}
?>
 