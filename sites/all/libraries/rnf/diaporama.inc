<?php
function CreateDiaporama($prefix, $nbinslide, $w, $h)
{
  $inum = 1;
 
      echo "var mygallery_1=new fadeSlideShow({\n";
      echo "	wrapperid: \"sl_rn\",\n";
      echo "	dimensions: [$w, $h],\n";
      echo "	imagearray: [\n";
      for ($j = 1; $j <= $nbinslide; $j++)
	{
	  $iname = sprintf("%03s", $inum); 
	  $ipath = base_path(). path_to_theme()."/photos/diaporama/".$prefix.$iname.".jpg";

	  if ($j != $nbinslide)
	    echo "	[\"".$ipath."\", \"\", \"\", \"\"],\n";
	  else
	    echo "	[\"".$ipath."\", \"\", \"\", \"\"]\n";
	  $inum++;
	}
      echo "	],\n";
      echo "	displaymode: {type:'auto', pause:3000, cycles:0, delay:2000, wraparound:false},\n";
      echo " 	persist:false, fadeduration:800, ignoremouse:true, togglerid:\"diapoctrl\"";
      echo "})\n";
    }

 $prefix = 'diaporn';
 $nbs    =  178;
 $width  = 720;
 $height = 527;
 $nid = arg(1);
 if ($nid != '')
   {
     $node = node_load($nid);
     $name = $node->title;
     if ($name != 'Patrimoine')
       {
	 $prefix = 'patribio';
	 $nbs = 60;
	 $height = 540;
       }
   }



 echo '<script type="text/javascript" src="'.base_path() . path_to_theme().'/js/fadeslideshow.js"></script>';
 echo '<script type="text/javascript">'."\n";
 CreateDiaporama($prefix, $nbs, $width, $height);
 echo '</script>'."\n";

 echo "<div id=\"sl_rn\"></div>";
 echo "<center>";
 echo "<div id=\"diapoctrl\">";
 $left = base_path() . path_to_theme().'/js/left.png';
 $right = base_path() . path_to_theme().'/js/right.png';
 echo "<a href=\"#\" class=\"prev\"><b>&lt;</b></a> <span class=\"status\" style=\"margin:0 50px; font-weight:bold\"></span> <a href=\"#\" class=\"next\"><b>&gt;</b></a>";
//echo "<a href=\"#\" class=\"prev\"><img src=\"$left\"></a> <span class=\"status\" style=\"margin:0 50px; font-weight:bold\"></span> <a href=\"#\" class=\"next\"><img src=\"$right\"></a>";
 echo "</div>";
 echo "</center>\n";

?>