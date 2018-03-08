/* Small sample of Leaflet usage */
var map = new L.Map('divleaf'); // reference au div du PHP

 // ajout des limites des RN
 //if (rn_limit) {
 //var nbrn = rn_limit.length;
 //for (i=0;i<nbrn;i++) { L.geoJson(rn_limit[i], { style: { color: '#ff0000', weight: 3, opacity: 1 } }).addTo(map); }
 // }

// Tiles by MapQuest are no more available
//var Url = 'http://{s}.mqcdn.com/tiles/1.0.0/osm/{z}/{x}/{y}.png';
//var myAttribution = "Data, imagery and map information provided by <a href=\"http://open.mapquest.co.uk\" target=\"_blank\">MapQuest</a>, <a href=\"http://www.openstreetmap.org/\" target=\"_blank\">OpenStreetMap</a> and contributors.";
//var subDomains = ['otile1','otile2','otile3','otile4'];
//var myLayer = new L.TileLayer(Url, {maxZoom: 18, attribution: myAttribution, subdomains: subDomains});

// Tiles by OpenStreetmap are ok
var Url = 'http://tile.openstreetmap.org/{z}/{x}/{y}.png';
var myAttribution = "Map data &copy; 2012 <a href='http://www.openstreetmap.org/'>OpenStreetMap</a> contributors";
var myLayer = new L.TileLayer(Url, {maxZoom: 18, attribution: myAttribution});

var center = new L.LatLng(latCenter, longCenter);
map.setView(center, zoom).addLayer(myLayer);

var scale = new L.Control.Scale({ position: 'bottomleft', metric: true, imperial: false } );
scale.addTo(map);

var rnnIcon = L.icon({
	iconUrl: 'sites/all/libraries/leaflet/rnn.png',
	iconSize: new L.Point(22, 25),
	iconAnchor: new L.Point(9, 10),
	popupAnchor: new L.Point(-3, -12),
});

var rnrIcon = L.icon({
	iconUrl: 'sites/all/libraries/leaflet/rnr.png',
	iconSize: new L.Point(22, 25),
	iconAnchor: new L.Point(9, 10),
	popupAnchor: new L.Point(-3, -12),
});

var rncIcon = L.icon({
	iconUrl: 'sites/all/libraries/leaflet/rnc.png',
	iconSize: new L.Point(22, 25),
	iconAnchor: new L.Point(9, 10),
	popupAnchor: new L.Point(-3, -12),
});

    var myMarkers = new Array();
    var mapzoom = map.getZoom();

    // ajout des limites eventuelles
    if (limit != '')
      L.geoJson(limit, { style: { color: '#ff0000', weight: 3, opacity: 1 } }).addTo(map);

    // creation des markers pour chaque RN
    len = rn_itemList.length;
    j = 0;
    for (i = 0; i<len; i++)
      {
	if (rn_itemList[i])
  	  {
	    rnlat  = parseFloat(rn_itemList[i].latit);
	    rnlong = parseFloat(rn_itemList[i].longi);
	    zoom   = rn_itemList[i].zoom;
	    url    = rn_itemList[i].url;
	    type   = rn_itemList[i].type;
	    code   = rn_itemList[i].code;
	    
	    if (mapzoom >= zoom)
	      {
		 var markerLocation = new L.LatLng(rnlat, rnlong);
		 if (type == "RNN")
		   var marker = new L.Marker(markerLocation, {icon: rnnIcon});
	         else if (type == "RNR")
	           var marker = new L.Marker(markerLocation, {icon: rnrIcon});
                 else
                   var marker = new L.Marker(markerLocation, {icon: rncIcon});

	         rnname = "<a href=\""+url+"\"><b>" + rn_itemList[i].title + "</b></a><br><center>" + code + "<center/>";
	         marker.bindPopup(rnname);
	         map.addLayer(marker);
		 myMarkers[j] = marker;
		 j++;
             }
	  }
      }

 // quand on zoom avant, on retire les markers
 map.on( "zoomstart", function( e ) {
 zoom = map.getZoom( );
 len = myMarkers.length;
 for (i = 0; i<len; i++)
     map.removeLayer(myMarkers[i]);
  });
  
  // a la fin du zoom, on recalcule les markers a mettre
  map.on( "zoomend", function( e ) {
    mapzoom = map.getZoom( );
    len = rn_itemList.length;
    j = 0;
    for (i = 0; i<len; i++)
      {
	if (rn_itemList[i])
  	  {
	    rnlat  = parseFloat(rn_itemList[i].latit);
	    rnlong = parseFloat(rn_itemList[i].longi);
	    zoom   = rn_itemList[i].zoom;
	    url    = rn_itemList[i].url;
	    type   = rn_itemList[i].type;
	    code   = rn_itemList[i].code;
	    
	    if (mapzoom >= zoom)
	      {
		 var markerLocation = new L.LatLng(rnlat, rnlong);
		 if (type == "RNN")
		   var marker = new L.Marker(markerLocation, {icon: rnnIcon});
	         else if (type == "RNR")
	           var marker = new L.Marker(markerLocation, {icon: rnrIcon});
                 else
                   var marker = new L.Marker(markerLocation, {icon: rncIcon});

	         rnname = "<a href=\""+url+"\"><b>" + rn_itemList[i].title + "</b></a><br><center>" + code + "<center/>";
	         marker.bindPopup(rnname);
	         map.addLayer(marker);
		 myMarkers[j] = marker;
		 j++;
             }
	  }
      }
  });

function zoomTo(z) { map.setZoom(z); }
function zoomIn()  { map.zoomIn();   }
function zoomOut() { map.zoomOut(); }
