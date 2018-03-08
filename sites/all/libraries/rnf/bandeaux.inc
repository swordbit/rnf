<?php 
  global $base_url;
  require_once ("sites/all/libraries/rnf/functions.inc");
  require_once ("sites/all/libraries/rnf/bandeau_captions.inc");
  $imgpath = 'sites/default/files/images/';
  $fondpath = $base_url.$imgpath;
  if (isset($captions))
  {
  foreach($captions as $name => $caption)
    {
      $bandeau = $imgpath.'bandeau-'.$name.'.jpg';
      if (is_file($bandeau))
	{
	  $wrongsize = false;
	  $caption = htmlentities($caption);
	  $srcbandeau = $base_url.'/'.$bandeau;
	  $sizetab = getimagesize($bandeau);
	  $width = $sizetab[0];
	  $height = $sizetab[1];
	  if ($width != 720 || $height != 289)
	    $wrongsize = true;

	  if ($wrongsize)
	    echo "<b style=\"color: red;\">$name $width x $height</b><br/>";
	  else
	    echo "<b>$name</b><br/>";

	  if (strstr($caption, 'Aubertel') || true)
	    {
	  echo "<figure style=\"margin-bottom: 5px;\">";
	  echo "<img alt=\"\" title=\"\" src=\"$srcbandeau\"/>\n";
	  echo "<figcaption class=\"cap-bot\">$caption</figcaption>";
	  echo "</figure>\n";
	    }
	}
      else
	{
	  //echo "Cannot find $bandeau<br/>";
	}
    }
}
?>
