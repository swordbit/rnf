<?php
require_once('functions.inc');

function _displayStatRN()
{
  return; 
  $nbbandeau   = array();
  $nblogos     = array();
  $nbphotos    = array();
  $nbvideos    = array();
  $nbdocuments = array();
  $alltypes    = array();
  $nbpages     = array();

  // les types de pages que l'on retient
  $typetab = array('reserve' => 'R&eacute;serve',
		   'page' => 'Page de base',
		   'region' => 'R&eacute;gion',
		   'gestionnaire' => 'Gestionnaire',
		   'recrutement' => 'Recrutement',
		   'actualites' => 'Actualit&eacute;s',
		   'librairie' => 'Publications',
		   //'partenaire' => 'Partenaire',
		   //'billet' => 'Blog',
		   //'forum' => 'Forum',
		   'page_privee' => 'Page priv&eacute;e'
		   );

  foreach($typetab as $t => $tname)
    {
      // on passe tous les noeuds de ce type en revue
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'node')
	->entityCondition('bundle', $t)
	->addMetaData('account', user_load(1)); // run the query as user 1
      $result = $query->execute();
      
      if (isset($result['node'])) 
	{
	  $nids = array_keys($result['node']); 
	  $nodes = node_load_multiple($nids);

	  foreach ($nodes as $node)
	    {
	      $type = $node->type;
	      //if (!in_array($type, $alltypes))
	      //$alltypes[] = $type;
	  
	      if (isset($node->field_part_logo))
		{
		  $logo = _getfieldvalue($node, $node->field_part_logo, '', 'fid');
		  if ($logo != '')
		    if (isset($nblogos[$type]))
		      $nblogos[$type]++;
		    else
		      $nblogos[$type] = 1;
		} 
	      if (isset($node->field_og_logo))
		{
		  $logo = _getfieldvalue($node, $node->field_og_logo, '', 'fid');
		  if ($logo != '')
		    if (isset($nblogos[$type]))
		      $nblogos[$type]++;
		    else
		      $nblogos[$type] = 1;
		}
	      if (isset($node->field_rn_logo))
		{
		  $logo = _getfieldvalue($node, $node->field_rn_logo, '', 'fid');
		  if ($logo != '')
		    if (isset($nblogos[$type]))
		      $nblogos[$type]++;
		    else
		      $nblogos[$type] = 1;
		}
	      if (isset($node->field_bandeau))
		{
		  $bandeau = _getfieldvalue($node, $node->field_bandeau, '', 'fid');
		  if ($bandeau != '')
		    if (isset($nbbandeau[$type]))
		      $nbbandeau[$type]++;
		    else
		      $nbbandeau[$type] = 1;
		}

	      if (isset($node->field_photos))
		{
		  $photos  = _getfieldvaluetab($node, $node->field_photos, array());
		  if (isset($nbphotos[$type]))
		    $nbphotos[$type] += count($photos);
		  else
		    $nbphotos[$type] = count($photos);
		}
	      if (isset($node->field_image))
		{
		  $photos  = _getfieldvaluetab($node, $node->field_image, array());
		  if (isset($nbphotos[$type]))
		    $nbphotos[$type] += count($photos);
		  else
		    $nbphotos[$type] = count($photos);
		}
	      if (isset($node->field_videos))
		{
		  $videos  =  _getfieldvalue($node, $node->field_videos, '');
		  if (trim($videos) != '')
		    if (isset($nbvideos[$type]))
		      $nbvideos[$type]++;
		    else
		      $nbvideos[$type] = 1;
		}
	      if (isset($node->field_fichier_rn))
		{
		  $fichier = _getfieldvaluetab($node, $node->field_fichier_rn, array());
		  if (isset($nbdocuments[$type]))
		    $nbdocuments[$type] += count($fichier);
		  else
		    $nbdocuments[$type] = count($fichier);
		}
	      if (isset($node->field_fichier_base))
		{
		  $fichier = _getfieldvaluetab($node, $node->field_fichier_base, array());
		  if (isset($nbdocuments[$type]))
		    $nbdocuments[$type] += count($fichier);
		  else
		    $nbdocuments[$type] = count($fichier);
		}
	      if (isset($node->field_fichier_librairie))
		{
		  $fichier = _getfieldvaluetab($node, $node->field_fichier_librairie, array());
		  if (isset($nbdocuments[$type]))
		    $nbdocuments[$type] += count($fichier);
		  else
		    $nbdocuments[$type] = count($fichier);
		}
	      if (isset($node->field_fichiers_recrut))
		{
		  $fichier = _getfieldvaluetab($node, $node->field_fichiers_recrut, array());
		  if (isset($nbdocuments[$type]))
		    $nbdocuments[$type] += count($fichier);
		  else
		    $nbdocuments[$type] = count($fichier);
		}
	      if (isset($node->field_pdf_inline))
		{
		  $fichier = _getfieldvaluetab($node, $node->field_pdf_inline, array());
		  if (isset($nbdocuments[$type]))
		    $nbdocuments[$type] += count($fichier);
		  else
		    $nbdocuments[$type] = count($fichier);
		}
	    }
	}
      $nbpages[$type] = count($nodes);
      unset($result);
    }

  $tot_pages = 0;
  $tot_bandeau = 0;
  $tot_logos = 0;
  $tot_photos = 0;
  $tot_videos = 0;
  $tot_doc = 0;

  echo "<b>Actuellement sur le site :</b>";

  echo "<table style=\"margin: 10px;\"><tr><td><b>Type de page</b></td>";
  echo "<td align=\"right\"><b>Pages</b></td>";
  echo "<td align=\"right\"><b>Bandeaux</b></td>";
  echo "<td align=\"right\"><b>Logos</b></td>";
  echo "<td align=\"right\"><b>Photos</b></td>";
  echo "<td align=\"right\"><b>Vid&eacute;os</b></td>";
  echo "<td align=\"right\"><b>Documents</b></td></tr>";

  foreach($typetab as $t => $tname)
    {
      echo "<tr>";
      echo "<td>$tname</td>";
      if (isset($nbpages[$t]))
	{
	  $tot_pages += $nbpages[$t];
	  echo "<td align=\"right\">".$nbpages[$t]."</td>";
	}
      else
	{
	  echo "<td>&nbsp;</td>";
	}
      if (isset($nbbandeau[$t]))
	{
	  $tot_bandeau += $nbbandeau[$t];
	  echo "<td align=\"right\">".$nbbandeau[$t]."</td>";
	}
      else
	{
	  echo "<td>&nbsp;</td>";
	}
      if (isset($nblogos[$t]))
	{
	  $tot_logos += $nblogos[$t];
	  echo "<td align=\"right\">".$nblogos[$t]."</td>";
	}
      else
	{
	  echo "<td>&nbsp;</td>";
	}
      if (isset($nbphotos[$t]))
	{
	  $tot_photos += $nbphotos[$t];
	  echo "<td align=\"right\">".$nbphotos[$t]."</td>";
	}
      else
	{
	  echo "<td>&nbsp;</td>";
	}
      if (isset($nbvideos[$t]))
	{
	  $tot_videos += $nbvideos[$t];
	  echo "<td align=\"right\">".$nbvideos[$t]."</td>";
	}
      else
	{
	  echo "<td>&nbsp;</td>";
	}
      if (isset($nbdocuments[$t]))
	{
	  $tot_doc += $nbdocuments[$t];
	  echo "<td align=\"right\">".$nbdocuments[$t]."</td>";
	}
      else
	{
	  echo "<td>&nbsp;</td>";
	}
      echo "</tr>";
    }
      echo "<tr><td><b>Total</b></td>";
      echo "<td align=\"right\"><b>$tot_pages</b></td>";
      echo "<td align=\"right\"><b>$tot_bandeau</b></td>";
      echo "<td align=\"right\"><b>$tot_logos</b></td>";
      echo "<td align=\"right\"><b>$tot_photos</b></td>";
      echo "<td align=\"right\"><b>$tot_videos</b></td>"; 
      echo "<td align=\"right\"><b>$tot_doc</b></td>";
      echo "</tr>";

  echo "</table>";

  //print_r($alltypes);
}
?>
