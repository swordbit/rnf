/* Small sample of Leaflet usage */
var map = new L.Map('divleaf'); // reference au div du PHP
var Url = 'http://tile.openstreetmap.org/{z}/{x}/{y}.png';

 // ajout des limites eventuelles
 if (limit != '')
   L.geoJson(limit, { style: { color: '#ff0000', weight: 3, opacity: 1 } }).addTo(map);
    
var myAttribution = "Map data &copy; 2012 <a href='http://www.openstreetmap.org/'>OpenStreetMap</a> contributors";
var myLayer = new L.TileLayer(Url, {maxZoom: 18, attribution: myAttribution});
var center = new L.LatLng(rn_lat, rn_long);
map.setView(center, zoom).addLayer(myLayer);

var scale = new L.Control.Scale({ position: 'bottomleft', metric: true, imperial: false } );
scale.addTo(map);

var rnnIcon = L.icon({
	iconUrl: 'sites/all/libraries/leaflet/rnn.png',
	iconSize: new L.Point(22, 25),
	iconAnchor: new L.Point(9, 10),
	popupAnchor: new L.Point(-3, -12),
	//shadowUrl: 'sites/all/libraries/leaflet/rnn-shadow.png',
	//shadowSize: new L.Point(22, 25),
	//shadowAnchor: new L.Point(-3, -12)
});

var rnrIcon = L.icon({
	iconUrl: 'sites/all/libraries/leaflet/rnr.png',
	iconSize: new L.Point(22, 25),
	iconAnchor: new L.Point(9, 10),
	popupAnchor: new L.Point(-3, -12),
	//shadowUrl: 'sites/all/libraries/leaflet/rnn-shadow.png',
	//shadowSize: new L.Point(22, 25),
	//shadowAnchor: new L.Point(-3, -12)
});

var rncIcon = L.icon({
	iconUrl: 'sites/all/libraries/leaflet/rnc.png',
	iconSize: new L.Point(22, 25),
	iconAnchor: new L.Point(9, 10),
	popupAnchor: new L.Point(-3, -12),
	//shadowUrl: 'sites/all/libraries/leaflet/rnn-shadow.png',
	//shadowSize: new L.Point(22, 25),
	//shadowAnchor: new L.Point(-3, -12)
});

    // creation des markers pour chaque RN
    len = rn_itemList.length;
    for (i = 0; i<len; i++)
      {
	if (rn_itemList[i])
  	  {
	    rnlat  = parseFloat(rn_itemList[i].latit);
	    rnlong = parseFloat(rn_itemList[i].longi);
	    type   = rn_itemList[i].type;
	    code   = rn_itemList[i].code;
	    
	    var markerLocation = new L.LatLng(rnlat, rnlong);
	    if (type == "RNN")
	      var marker = new L.Marker(markerLocation, {icon: rnnIcon});
	    else if (type == "RNR")
	      var marker = new L.Marker(markerLocation, {icon: rnrIcon});
            else
              var marker = new L.Marker(markerLocation, {icon: rncIcon});
	    
	    map.addLayer(marker);
	    rnname = "<b>" + rn_itemList[i].title + "</b><br><center>" + code + "<center/>";
	    marker.bindPopup(rnname);
	  }
      }

function zoomTo(z) { map.setZoom(z); }
function zoomIn()  { map.zoomIn();   }
function zoomOut() { map.zoomOut(); }
