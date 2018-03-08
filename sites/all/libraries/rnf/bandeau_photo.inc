<?php
  require_once('functions.inc'); 

// on cherche un bandeau dans les champs du node
// on met a jour $caption au passage
function _getBandeauByField($node, $photopath, &$caption)
{
  $fids     = array();
  $captions = array();
  if (isset($node->field_bandeau))
    {	
      if ($node->type == 'region')
	{
	  // prise en compte des bandeaux multiples dans les regions
	  $bandeaux = _getfieldvaluetab($node, $node->field_bandeau, array());
	  foreach($bandeaux as $ban)
	    {
	      $fids[] = $ban['fid'];
	      $captions[] = $ban['alt'];
	    }
	  if (count($fids) != 0)
	    {
	      // on tire une image au hasard
	      $fidrank = rand(0, count($fids)-1);
	      $b = $fids[$fidrank];
	      $caption = $captions[$fidrank];
	    }
	}
      
      if (count($fids) == 0)
	{
	  $b = _getfieldvalue($node, $node->field_bandeau, '', 'fid');
	  $caption = _getfieldvalue($node, $node->field_bandeau, '', 'alt');
	}
      //echo "Alt : $caption<br>";
      if ($b != '')
	{
	  $res = db_query("SELECT filename from drupal_file_managed where fid = $b");
	  $uritab = $res->fetchAssoc();
	  if (isset($uritab['filename']))
	    return $photopath.$uritab['filename'];
	}
    }
  return '';
}

function _getBandeauByName($node, $imgpath)
{
 $node_url = basename(drupal_lookup_path('alias',"node/".$node->nid));
 $node_url =  mb_convert_encoding($node_url, 'ISO-8859-1', 'UTF-8');
 $node_url = no_accents($node_url);
 $bandeau = $imgpath.'bandeau-'.$node_url.'.jpg';

 return $bandeau;
}

  global $base_url;
  $imgpath = 'sites/default/files/images/';
  $photopath = 'sites/default/files/photos/';
  $caption = '';

  // on prepare un bandeau par defaut au hasard
  $num = rand(1, 9);
  $srcbandeau = $base_url.'/'.$imgpath.'bandeau-'.$num.'.jpg'; // pour src uniquement
  $nid = arg(1);
  if ($nid != '' && is_numeric($nid))
     {
       $node = node_load($nid);
       // premier essai avec le champs bandeau du node
       $bandeau = _getBandeauByField($node, $photopath, $caption);

       // second essai avec le titre
       if (!is_file($bandeau))
         $bandeau = _getBandeauByName($node, $imgpath);

       if (is_file($bandeau))
         {
	    // on a trouve un bandeau specifique pour la page
	    $srcbandeau = $base_url.'/'.$bandeau;
	 }
       else
         {
	    //echo "Cannot find $bandeau<br>";
	    if (isset($node->field_typern))
	      {
	        // s'il s'agit d'une page RN, on met un bandeau gris
		$srcbandeau = $base_url.'/'.$imgpath.'bandeau-gris.jpg';
	      }
	 }
     }

  if ($caption == '')
    {
      $caption = '';
      include('bandeau_captions.inc');
      $im = str_replace('bandeau-', '', basename($srcbandeau, '.jpg'));
      if (isset($captions["$im"]))
        {
	   $caption = $captions["$im"];
	   $caption = mb_convert_encoding($caption, 'UTF-8', 'ISO-8859-1');
        }
    } 

  echo "<div style=\"width: 720px; height: 289px; padding-top: 5px;\">";
  if ($caption != '')
    echo "<figure tabindex=\"0\">";
  echo "<img alt=\"\" title=\"\" src=\"$srcbandeau\" width=\"720\" height=\"289\"/>";
  if ($caption != '')
    echo "<figcaption class=\"cap-bot\">$caption</figcaption>";
  if ($caption != '')
    echo "</figure>";
  echo "</div>";
?> 
