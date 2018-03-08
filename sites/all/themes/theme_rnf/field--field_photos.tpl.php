<?php
  $nid = arg(1);
  if ($nid != '')
    $node_url = basename(drupal_lookup_path('alias',"node/".$nid));
  else
    $node_url = '';
  $nofigure = false;
?>
<div class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if (!$label_hidden) : ?>
    <h2 class="field-label"<?php print $title_attributes; ?>><?php print $label ?>:&nbsp;</h2>
  <?php endif; ?>
  <?php foreach ($items as $delta => $item): ?>
  <?php if (!$nofigure) { ?>
    <figure class="field-item <?php print $delta % 2 ? 'odd' : 'even'; ?>"<?php print $item_attributes[$delta]; ?>>
<?php
    }
if ($item['#item']['title'] && !$nofigure)
  {
    //echo "<pre>";
    //print_r($item['#item']);
    //echo "</pre>";
    $mytitle = $item['#item']['title'];
    //$item['#item']['alt'] = '';
    $item['#item']['title'] = '';
    print render($item);
    echo "<figcaption>$mytitle</figcaption>";
  }
else
  {
     print render($item);
  }
?>
  <?php if (!$nofigure) { ?>
    </figure>
  <?php } ?>
  <?php endforeach; ?>
</div>
