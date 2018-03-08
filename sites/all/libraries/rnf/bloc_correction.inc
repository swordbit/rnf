<?php
  global $user;
   $myuid = (bool) $user->uid;
   if ($myuid)
     {
        // utilisateur connecte 
	$nid = arg(1);
	if ($nid != '' && $nid != 'add') 
	  {
	    $node = node_load($nid); 
	    if (isset($node->title))
	      {
	         $subject = "Proposition de correction pour la RN ".$node->title;
		 $message = "Proposition de correction pour la RN ".$node->title;
	         $url = "contact?category=Proposition+de+correction";
		 $url .= "&subject=".urlencode($subject);
		 $url .= "&message=".urlencode($message);
	         echo "<a href=\"$url\">Proposer une correction</a>";
              }
          }
     }

?>
