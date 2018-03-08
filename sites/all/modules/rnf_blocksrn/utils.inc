<?php
function clean_accents($var)
{
  return strtr($var,
   'àáâãäçèéêëìíîïñòóôõöùúûüýÿÀÁÂÃÄÇÈÉÊËÌÍÎÏÑÒÓÔÕÖÙÚÛÜÝ',
   'aaaaaceeeeiiiinooooouuuuyyAAAAACEEEEIIIINOOOOOUUUUY');
}

function my_str_coll($a, $b)
{
  return strcoll(clean_accents($a), clean_accents($b));
}

function gfv($node, $fieldname, $defaut = '', $key = 'value')
{
  $tab = field_get_items('node', $node, $fieldname);
  if ($tab)
    if (isset($tab[0]))
      if (isset($tab[0][$key]))
	return $tab[0][$key];
  return $defaut;
}

function gfvtab($node, $fieldname, $defaut)
{
  $tab = field_get_items('node', $node, $fieldname);
  if ($tab)
    return $tab;

  return $defaut;
}

function gfvnid($node, $fieldname, $defaut)
{
  $tab = field_get_items('node', $node, $fieldname);
  if ($tab)
    if (isset($tab[0]))
      if (isset($tab[0]['nid']))
	return $tab[0]['nid'];
 
  return $defaut;
}

function hent($string)
{
  if (defined('ENT_HTML401'))
    return htmlentities($string, ENT_COMPAT | ENT_HTML401, 'ISO8859-1');
  else
   return htmlentities($string, ENT_COMPAT, 'ISO8859-1');
}

// Converts decimal longitude / latitude to DMS
// ( Degrees / minutes / seconds ) 
function dec_to_dms($dec)
{
  // This is the piece of code which may appear to 
  // be inefficient, but to avoid issues with floating
  // point math we extract the integer part and the float
  // part by using a string function.
  
  $vars = explode(".",$dec);
  $deg = $vars[0];
  if (isset($vars[1]))
    $tempma = "0.".$vars[1];
  else
    $tempma = "0.0";

  $tempma = $tempma * 3600;
  $min = floor($tempma / 60);
  $sec = $tempma - ($min*60);

  return array("deg"=>$deg,"min"=>$min,"sec"=>$sec);
}

function disp_date($mysqldate)
{
  $dtab = explode('-', substr($mysqldate, 0, 10));
  return $dtab[2]."/".$dtab[1]."/".$dtab[0];
}

function create_limit($name, $rncode)
{
  $limit = '';
  $path = 'sites/default/files/';
  $limpath = $path.'limites/'.strtolower($rncode).'.json';
  if (is_file($limpath))
    {
      // on a trouve la limite
      $json = file_get_contents($limpath);
      $limit .= "var limit = $json;\n";
    }
 else
    {
      $limit .= "var limit = \"\";\n";
    }
  return $limit;
}

function make_url($name)
{
  $namenoa = strtolower(clean_accents($name));
  $deltab = array("\xab", "\xa0", "\xbb");
  $namenoa = str_replace($deltab, '', $namenoa);
  $namenosp = strtr($namenoa, " ,('/", "-----");
  $namenosp = str_replace(")", "", $namenosp);
  $namenosp = str_replace('"', "", $namenosp);
  $namenosp = str_replace("*", "-", $namenosp);
  $namenosp = preg_replace("/--*/", "-", $namenosp); 
  
  return (urlencode($namenosp));
}
