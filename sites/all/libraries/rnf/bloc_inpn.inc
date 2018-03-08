<?php
   global $user;
   $path = 'sites/default/files/';
   $myuid = (bool) $user->uid;
   if ($myuid || true)
     {
       // utilisateur connecte 
       $nid = arg(1);
       if ($nid != '' && $nid != 'add') 
	 {  
	   $node = node_load($nid); 
	   if (isset($node->field_inpn))
	     {
	       $inpn = _getfieldvalue($node, $node->field_inpn, '');
	       $rncode  = _getfieldvalue($node, $node->field_code, '');
	       $inpnpath = $path.'inpn/'.strtolower($rncode).'.js';
	       if (is_file($inpnpath))
		 {
		   global $base_url;	
		   $d3path  = $base_url."/sites/all/libraries/d3";
		   echo "<script type=\"text/javascript\" src=\"".$d3path."/d3.min.js\"></script>\n";
		   echo '<style type="text/css" media="screen">
                #sb_hierarchy { width:420px; height: 400px; }
                svg { cursor:pointer; }
                #sb_hierarchy_legend { width:420px; height:30px; }
                   </style>';
		   echo "<p><b>Espèces mentionnées dans le périmètre de la réserve</b><br><small>(listes non exhaustives)</small></p>";
		   echo '<div id="sb_hierarchy">&nbsp;</div>';
		   echo '<div id="sb_hierarchy_legend">&nbsp;</div>';
		   echo "<p style=\"width: 420px; text-align: right;\"><small>Sources des données : Observatoire des Réserves naturelles / INPN</small></p>";
		   echo "<script type=\"text/javascript\" src=\"$inpnpath\"></script>";
		   $jspath =  $base_url."/sites/all/libraries/rnf";
		   echo "<script type=\"text/javascript\" src=\"".$jspath."/d3_hierarchical_pie.js\"></script>\n";
		 }
	     }
	 }
     }
?>
