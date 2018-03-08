// traitement du span readmore
// le span permet de deplier le div qui suit immediatement
// <span class="readmore">(lire la suite de l'article)</span>
// <div>
// </div>

jQuery(document).ready(function($){
    $(".readmore").click(function(){
      $(this).next().slideToggle("slow");
      $(this).hide();
      return false;
    }).next().hide();
   });
