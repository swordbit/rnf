function swapReg(zone,nr){
    var reg=document.getElementById('hovered');
    reg.src='/sites/all/themes/theme_rnf/images/region_'+nr+'.png';
    zone.onmouseout=function(){reg.src='/sites/all/themes/theme_rnf/images/region_vide.png'};
}
