<?php

function CreateSlideshow()
{
  $w = 200;
  $h = 166;
  $delaytab = array(1 => 800, 2 => 2400, 3 => 1600, 4 => 0, 5 => 3200);

  $legende = array(
	 array('001', 'RN du Lac de Grand-Lieu'),
	 array('002', 'Crabe porcelaine, Mayotte'),
	 array('003', 'Oed&eacute;m&egrave;re noble, RN de l\'estuaire de la Seine'), // fleur jaune
	 array('004', 'Parc National de la Guadeloupe'),
	 array('005', 'Papillon Flamb&eacute;, RN des Gorges de l\'Ard&egrave;che'),
	 array('006', 'RN du Marais d\'Orx'),
	 array('007', 'Sternes caugek, RN du Banc d\'Arguin'),
	 array('008', 'Ilet Hardy, Martinique'),
	 array('009', 'Archipel de Riou, Bouches du Rhone'),
	 array('010', 'RN des Gorges du Gardon, Ard&egrave;che'),
	 array('011', 'Grand Mars changeant, RN de Ch&eacute;rine'),
	 array('012', 'La Soufri&egrave;re, Guadeloupe'),
	 array('013', 'RN g&eacute;ologique de Haute-Provence'),
	 array('014', 'Accouplement d\'agrions &eacute;l&eacute;gants'),
	 array('015', 'Araign&eacute;e de mer, Archipel des Gl&eacute;nan'),
	 array('016', 'RN des Gorges du Gardon, Ard&egrave;che'),
	 array('017', 'Gorges de l\'Ard&egrave;che'),
	 array('018', '&Icirc;les Lavezzi, Bonifacio, Corse'),
	 array('019', 'Mangrove Saint-Louis, Guadeloupe'),
	 array('020', '&Icirc;lot Mbouzi, Mayotte'),
	 array('021', 'Suivi de la cistude d\'Europe, Marais d\'Orx'),
	 //array('022', 'Rainette arboricole, RN du Pinail'),
	 array('022', 'Observatoire, St Quentin en Yvelines'),
	 array('023', 'R&eacute;serve naturelle du Pinail'),
	 array('024', 'Grand Macabou, Martinique'),
	 array('025', 'RN des Sept-Iles, Bretagne'),
	 array('026', 'RN de Saint-Nicolas-des-Gl&eacute;nan, Bretagne'),
	 array('027', 'Chevreuil, For&ecirc;t de Tron&ccedil;ais'),
	 array('028', 'R&eacute;serve naturelle du Pinail'),
	 array('029', 'R&eacute;serve Naturelle de Scandola, Corse'),
	 array('030', 'Sporophile rouge-gorge, Guadeloupe'), 
	 array('031', 'Inventaire des col&eacute;opt&egrave;res, Hettange-Grande'),
	 array('032', 'Bourrache officinale (Borago officinalis)'),
	 array('033', 'Ascalaphe soufr&eacute; (Libelloides coccajus)'),
	 array('034', 'R&eacute;serve Naturelle de Montenach, Moselle'),
	 array('035', 'Decticelle slendide (Eupholidoptera chabrieri)'),
	 array('036', 'Suivi &agrave; la longue-vue, RN Lileau des Niges'),
	 array('037', 'Bousier, RN de Combe Lavaux-jean Roland'),
	 array('038', 'Ammonite, RN de Hettange Grande'),
	 array('039', 'Vaches Highland, Estuaire de la Seine'),
	 array('040', 'Jeune pousse, RBI du Kertoff'),
	 array('041', 'Champignons, RN du Tanet-Gazon du Faing'),
	 array('042', 'Agrions, Petite Camargue Alsacienne'),
	 array('043', 'Orthopt&egrave;re, RN des Gorges du Gardon'),
	 array('044', 'L&eacute;zard vert, Martinique'),
	 array('045', 'Poissons fossiles, RN G&eacute;ologique du Luberon'),
	 array('046', 'M&eacute;duse, Archipel des Gl&eacute;nan'),
	 array('047', 'Nudibranche, Archipel des Gl&eacute;nan'),
	 array('048', 'Concr&eacute;tions, Grotte du T.M. 71'),
	 array('049', 'Aigrette, Martinique'),
);

  shuffle($legende); 		// on melange les images
  $nb_in_slide = 7;		// nombre d'image dans chaque slidshow
  $inum = 0;
  for ($i = 1; $i < 6; $i++)
    {
      echo "var mygallery".$i."=new fadeSlideShow({\n";
      echo "	wrapperid: \"sl".$i."\",\n";
      echo "	dimensions: [$w, $h],\n";
      echo "	imagearray: [\n";
      for ($j = 0; $j < $nb_in_slide; $j++)
	{
	  $iname = $legende[$inum][0];
	  $leg   = $legende[$inum][1];
	  $ipath = base_path(). path_to_theme()."/photos/rnf-".$iname.".jpg";

	  if ($j != $nb_in_slide - 1)
	    echo "	[\"".$ipath."\", \"\", \"\", \"".$leg."\"],\n";
	  else
	    echo "	[\"".$ipath."\", \"\", \"\", \"".$leg."\"]\n";
	  $inum++;
	}
      echo "	],\n";
      echo "	displaymode: {type:'auto', pause:4000, cycles:0, delay:".$delaytab[$i].", wraparound:false},\n";
      echo " 	persist: false, fadeduration: 800, descreveal: \"ondemand\", togglerid: \"\"";
      echo "})\n";
    }
}


// affiche les slideshows de la page d'accueil
function DisplaySlideshow()
{
  for ($i = 1; $i < 6; $i++) 
    echo "<span id=\"sl".$i."\"></span>";
}


 echo '<link href="'.base_path().path_to_theme().'/css/frontpage.css" rel="stylesheet" type="text/css" />';
 echo '<script type="text/javascript" src="'.base_path() . path_to_theme().'/js/fadeslideshow.js"></script>';
 echo '<script type="text/javascript">'."\n";
   CreateSlideshow();
 echo '</script>'."\n";

 echo '<div class="container">'."\n";

if (true) {
  // version 40 ans
  
echo ' <div class="anniv40ans">'."\n";
echo '  <img width="143" alt="" title="" src="'.base_path().path_to_theme().'/images/logo_40ans.png"/>';
echo ' <a target="_blank" href="https://naturadapt.com/"><img class="life" width="150" alt="" title="" src="'.base_path().path_to_theme().'/images/logo_lifenaturadapt.jpg"/></a>';
echo '	</div>'."\n";

 // le titre "RESERVES NATURELLES DE FRANCE en police Eurostile-ExtendedTwo
 echo '	<div class="rnftitle">'."\n";
 echo ' <img width="1000" height="30" alt="" title="" src="'.base_path().path_to_theme().'/images/rnf_title.png"/>';
 echo '	</div>'."\n";

 echo ' <div class="imgbandeau"> '."\n";
 DisplaySlideshow();
 echo '	</div>'."\n";

 // le slogan "Aujourd'hui la terre de demain"
 echo '	<div class="aujourdhui">'."\n";
 echo ' <img width="512" height="53" alt="" title="" src="'.base_path().path_to_theme().'/images/aujourdhui_black.png"/>';
 echo '	</div>'."\n";
  
} else {
  
  // version classique
 echo ' <div class="lifenatureadapt">'."\n";
 echo ' <a target="_blank" href="https://naturadapt.com/"><img width="150" alt="" title="" src="'.base_path().path_to_theme().'/images/logo_lifenaturadapt.jpg"/></a>';
 echo '	</div>'."\n";

 // le titre "RESERVES NATURELLES DE FRANCE en police Eurostile-ExtendedTwo
 echo '	<div class="rnftitle">'."\n";
 echo ' <img width="1000" height="30" alt="" title="" src="'.base_path().path_to_theme().'/images/rnf_title.png"/>';
 echo '	</div>'."\n";

 echo ' <div class="imgbandeau"> '."\n";
 DisplaySlideshow();
 echo '	</div>'."\n";

 // le slogan "Aujourd'hui la terre de demain"
 echo '	<div class="aujourdhui">'."\n";
 echo ' <img width="512" height="53" alt="" title="" src="'.base_path().path_to_theme().'/images/aujourdhui.png"/>';
 echo '	</div>'."\n";
}

 echo '</div>'."\n";
?>
