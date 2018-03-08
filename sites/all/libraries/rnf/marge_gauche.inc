<?php
  global $base_url;
  $margintop = '';
  $nid = arg(1);
  if ($nid != '')
    {
      $node_url = drupal_lookup_path('alias', "node/".$nid);
      $node_url = mb_convert_encoding($node_url, 'ISO-8859-1', 'UTF-8');
      $node_url = no_accents($node_url);
      $node_path = dirname($node_url);
      $node_name = basename($node_url);
    }
  include('motifs.inc');
  $fondpath = $base_url.'/sites/all/themes/theme_rnf/images/';

  // les motifs possibles pour actualites / recrutement
$actu_tab = array('motif_vanneau.png', // 324
		  'motif_azure.png',
		  'motif_daphne.png', // 268 x 230
		  'motif_ophrys.png', // 347
		  'motif_oeillet.png', // 178
		  'motif_tabac.png', // 254
		  'motif_solitaire.png' // 305
		    );

switch ($node_path) 
  {
  case 'actualites':
  case 'recrutement':
    $inum = rand(0, count($actu_tab)-1);
    $imgname = $actu_tab[$inum];
    // pas de bandeau, donc marge haute plus reduite
    $margintop = ' style="margin-top: 60px;"';
    break;
  default:
    $imgname = 'motif_martagon.png'; // 410
    $node = node_load($nid);
    // si il y a un PDF integre, pas d'image en marge
    if (isset($node->field_pdf_inline))
      $imgname = '';
    $margintop = ' style="margin-top: 60px;"';
  }

// sensibiliser : trop 3 lignes : dolomede (183)

// Pas utilises
//$imgname = 'motif_feuille.png'; // 189
//$imgname = 'motif_nenuphar.png'; // 212
//$imgname = 'motif_bergeronnette.png'; // 232
//$imgname = 'motif_epilobe.png'; // 224 x 547

switch ($node_name)
  { 
  case 'import_rn':
  case 'patrimoine':
  case 'tdb_rn':
  case 'bandeaux':
  case 'espace-adherent':
    $imgname = '';		// no image
    break;
  case 'missions':		  // OK
    $imgname = 'motif_milan.png'; // 258 x 137
    break;
  case 'reserves-naturelles-de-corse': // OK
    $imgname = 'motif_buccin.png'; // 174
    break;
case 'frequence-grenouille':	    // OK
     $imgname = 'motif_triton.png'; // 177
     break;
  case 'proteger': 
  case 'espace-membre':
    $imgname = 'motif_oeillet.png'; // 178
    break;
  case 'sensibiliser': 
  case 'commission-education-a-l-environnement':
    $imgname = 'motif_dolomede.png'; // 250 x 183
    break;
  case 'chiffres-cles':
  case 'devenir-mecene':    
    $imgname = 'motif_foubassan.png'; // 335 x 203
    break;
  case 'gerer':		
  case 'gestion':
    $imgname = 'motif_koniks.png'; // 375x220
    break;
  case 'autorites-de-classement':
  case 'mecenes':
    $imgname = 'motif_grues.png'; // 222
    break;
  case 'recrutement':
    $imgname = 'motif_lepture.png'; // 230
    break;
  case 'partenaires':		       // motif trop grand
  case 'reseau-rnf':
  case 'sciences-participatives':
    $imgname = 'motif_piegrieche.png'; // 231
    break;
  case 'evenements-annuels':
    $imgname = 'motif_tarier.png'; // 238
    break;
  case 'jour-de-la-nuit':
  case 'un-ailleurs-en-france-forets-naturelles':
    $imgname = 'motif_chevechette.png'; // 248
    break;
   case 'commission-communication':
     $imgname = 'motif_azure.png'; // 248
     break;
  case 'dons':
    $imgname = 'motif_cigogne.png'; // 250
    break;
  case 'rnf':
    $imgname = 'motif_orchis.png'; // 262
    break;
  case 'cahiers-scientifiques':
    $imgname = 'motif_hibou.png'; // 152x263
    break;
  case 'mentions-legales':
    $imgname = 'motif_araignee.png'; // 264
    break;
  case 'outre-mer-nature-et-savoir-faire':	
    $imgname = 'motif_grenouille.png'; // 267
    break;
  case 'fonctionnement':
  case 'commission-personnel':
  case 'communication':
    $imgname = 'motif_primevere.png'; // 295
    break;
  case 'connaissances':
    $imgname = 'motif_binoculaire.png'; // 300x206
    break;
  case 'reserves-naturelles':
  case 'commission-territoires-et-developpement-durable':
  case 'expositions':
  case 'la-france-sauvage-photographie-des-reserves-naturelles':
    $imgname = 'motif_calopteryx.png'; // 304
    break;
  case 'coeurs-de-nature':
  case 'sensibilisation':
  case 'actions-education-a-l-environnement':
    $imgname = 'motif_solitaire.png'; // 305
    break;
  case 'reserves-naturelles-nationales':
  case 'commission-scientifique':
    $imgname = 'motif_apollon.png'; // 308
    break;
  case 'patrimoine-geologique':
  case 'commission-patrimoine-geologique':
  case 'cahier-de-geologie':
    $imgname = 'motif_ammonite.png'; // 311
    break;
 case 'commission-outremer':
    $imgname = 'motif_albatros.png'; // 322
    break;
  case 'reserves-naturelles-regionales':
    $imgname = 'motif_buse.png'; // 325
    break; 
  case 'commissions':
  case 'congres':
    $imgname = 'motif_rousserolle.png'; // 331
    break;
  case 'equipe-salariee':
  case 'carnets-des-reserves-naturelles': 
    $imgname = 'motif_tabac.png'; // 300 x 387
    break;
  case 'definition':
  case 'ressources':
    $imgname = 'motif_cartegeo.png'; // 468 x 531
    break;
  case 'guide-methodologique-des-plans-de-gestion':
    $imgname = 'motif_balisage.png'; // 345 x 108
    break;
  case 'membres':
  case 'fete-de-la-nature':
    $imgname = 'motif_decticelle.png'; // 352
    break;
  case 'patrimoine-biologique': 
  case 'actualites':
    $imgname = 'motif_nacre.png'; // 391
    break;
  case 'bureau-et-ca':
    $imgname = 'motif_citron.png'; // 542 x 387
    break;
  case 'telechargement':
    $imgname = 'motif_renouee.png'; // 413
    break; 
  case 'commission-rnr-et-rnc':
    $imgname = 'motif_soldanelle.png'; // 429
    break; 
  case 'memoire-de-la-terre':
    $imgname = 'motif_lezard.png'; // 494
    break;
  case 'partenaires-techniques':
    $imgname = 'motif_aconit.png'; // 558
    break;
  default:
    break;
   }

  //$imgnum = rand(0, count($motiftab)-1);
  //$imgnum  = 0;

  // pour mettre des motifs plus larges que 200px a gauche
  // sur figure : margin-left : 200 - largeur_image
  // sur figcapion : margin-left : largeur_image - 200
   
  if ($imgname != '')
    {
      $imgpath = $fondpath.$imgname;
      $sizetab = getimagesize($imgpath);
      $width = $sizetab[0];
      $deltafig = 200 - $width;
      $deltafig .= "px";

      $leg = $motiftab[$imgname];
      $leg = mb_convert_encoding($leg, 'UTF-8', 'ISO-8859-1');

      echo "<div class=\"leftmargin\" $margintop>";
      echo "<figure style=\"margin-left:$deltafig;\">";
      echo "<img alt=\" \" title=\"\" src=\"$imgpath\"/>\n";
      echo "<figcaption>$leg</figcaption>";
      echo "</figure>";
      echo "</div>";
    }
?>
